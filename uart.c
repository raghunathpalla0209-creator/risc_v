#include "headers.h"
#include "declarations.h"
#include "uart_operations.h"

/* ================= IMPLEMENTATIONS ================= */

/**
 * @brief Convert integer baudrate to termios constant
 */
static speed_t get_baud(int baud)
{
    switch (baud) {
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        default: return B9600;
    }
}

static int uart_open_impl(uart_dev_t *dev, const char *device)
{
    strncpy(dev->device, device, sizeof(dev->device));

    dev->fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (dev->fd == -1) {
        perror("UART open failed");
        return -1;
    }

    fcntl(dev->fd, F_SETFL, 0);
    return 0;
}

static int uart_init_impl(uart_dev_t *dev)
{
    struct termios options;

    if (tcgetattr(dev->fd, &options) < 0) {
        perror("tcgetattr failed");
        return -1;
    }

    tcflush(dev->fd, TCIFLUSH);

    options.c_cflag &= ~CRTSCTS;
    options.c_cflag |= (CLOCAL | CREAD);

    options.c_iflag |= IGNPAR;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    options.c_oflag &= ~OPOST;

    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;

    options.c_cc[VTIME] = 5;
    options.c_cc[VMIN]  = 0;

    speed_t sp = get_baud(dev->baudrate);

    cfsetispeed(&options, sp);
    cfsetospeed(&options, sp);

    if (tcsetattr(dev->fd, TCSANOW, &options) < 0) {
        perror("tcsetattr failed");
        return -1;
    }

    return 0;
}

static int uart_write_impl(uart_dev_t *dev, const char *msg)
{
    int n = write(dev->fd, msg, strlen(msg));
    if (n < 0) {
        perror("write failed");
        return -1;
    }
    return n;
}

static int uart_read_response(int fd)
{
    char buf[256];
    int n;
    int total_lines = 0;

    printf("RX:\n");

    while (total_lines < 2)
    {
        int pos = 0;

        while (pos < sizeof(buf) - 1) {
            n = read(fd, &buf[pos], 1);

            if (n > 0) {
                if (buf[pos] == '\n') {
                    pos++;
                    break;
                }
                pos++;
            } else {
                usleep(10000);
            }
        }

        buf[pos] = '\0';

        if (pos > 0) {
            printf("%s", buf);
            total_lines++;
        }
    }

    return 0;
}

static int uart_read_impl(uart_dev_t *dev)
{
    return uart_read_response(dev->fd);
}

static void uart_close_impl(uart_dev_t *dev)
{
    close(dev->fd);
}

/* ================= OPS DEFINITION ================= */

uart_ops_t uart_ops = {
    .open  = uart_open_impl,
    .init  = uart_init_impl,
    .write = uart_write_impl,
    .read  = uart_read_impl,
    .close = uart_close_impl
};
