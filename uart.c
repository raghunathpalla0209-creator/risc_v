/**
 * @file uart.c
 * @author Raghunath Palla
 * @date 2026-04-29
 * @brief UART implementation using termios API
 *
 * @details
 * Implements UART open, configuration, read, write, and close
 * operations using Linux system calls and termios interface.
 *
 * Distributed under the MIT License.
 */


#include "headers.h"
#include "declarations.h"
#include "uart_operations.h"

/* ================= IMPLEMENTATIONS ================= */


/**
 * @brief Convert integer baudrate to termios constant
 * @param baud Baud rate (e.g., 9600, 115200)
 * @return Corresponding termios speed_t value
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


/**
 * @brief Open UART device
 * @param dev Pointer to UART device structure
 * @param device Device path
 * @return 0 on success, -1 on failure
 */
static int uart_open_impl(uart_dev_t *dev, const char *device)
{
    strncpy(dev->device, device, sizeof(dev->device));

    dev->fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (dev->fd == -1) {
        perror("UART open failed");
        return -1;
    }

   // fcntl(dev->fd, F_SETFL, 0);
   fcntl(dev->fd, F_SETFL, O_NONBLOCK);
   return 0;
}


/**
 * @brief Initialize UART using termios
 * @param dev UART device structure
 * @return 0 on success, -1 on failure
 */
static int uart_init_impl(uart_dev_t *dev)
{
    struct termios options;

    if (tcgetattr(dev->fd, &options) < 0) {
        perror("tcgetattr failed");
        return -1;
    }
    // Set raw mode: no echo, no backspace processing, no signal chars
    cfmakeraw(&options);
 //   tcflush(dev->fd, TCIFLUSH);

    options.c_cflag &= ~CRTSCTS;
    options.c_cflag |= (CLOCAL | CREAD);

    options.c_iflag |= IGNPAR;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    options.c_oflag &= ~OPOST;

    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;

    options.c_cc[VTIME] = 10;
    options.c_cc[VMIN]  = 0;
    options.c_cflag &= ~HUPCL; // Disable hang-up on last close (prevents reset)

    speed_t sp = get_baud(dev->baudrate);

    cfsetispeed(&options, sp);
    cfsetospeed(&options, sp);

    if (tcsetattr(dev->fd, TCSANOW, &options) < 0) {
        perror("tcsetattr failed");
        return -1;
    }

    return 0;
}

/**
 * @brief Write message to UART
 * @param dev UART device
 * @param msg Message string
 * @return Number of bytes written or -1
 */
static int uart_write_impl(uart_dev_t *dev, const char *msg)
{
    int n = write(dev->fd, msg, strlen(msg));
    if (n < 0) {
        perror("write failed");
        return -1;
    }
    return n;
}

/**
 * @brief Read UART response (line-based)
 * @param fd File descriptor
 * @return 0
 */
static int uart_read_response(int fd)
{
    char buf[256];
    int total_lines = 0;
    struct pollfd fds[1];
    
    fds[0].fd = fd;
    fds[0].events = POLLIN;

    printf("RX:\n");

    // We only expect 1 line now ("hello from arduino!!")
    while (total_lines < 1) {
        // Wait for 2000ms (2 seconds) - This is the "Non-blocking" timeout
        int ret = poll(fds, 1, 2000);

        if (ret < 0) {
            perror("poll error");
            return -1;
        } else if (ret == 0) {
            printf("[TIMEOUT]: No response from Arduino.\n");
            return -1; // Exit instead of hanging
        }

        // Data is ready to be read
        if (fds[0].revents & POLLIN) {
            unsigned long pos = 0;
            while (pos < sizeof(buf) - 1) {
                char c;
                // Read one byte at a time
                if (read(fd, &c, 1) > 0) {
                    buf[pos++] = c;
                    if (c == '\n') break;
                }
            }
            buf[pos] = '\0';
            if (pos > 0) {
                printf("%s", buf);
                total_lines++;
            }
        }
    }
    return 0;
}



/**
 * @brief Read from UART
 * @param dev UART device
 * @return 0
 */
static int uart_read_impl(uart_dev_t *dev)
{
    return uart_read_response(dev->fd);
}

/**
 * @brief Close UART device
 * @param dev UART device
 */
static void uart_close_impl(uart_dev_t *dev)
{
    close(dev->fd);
}

/* ================= OPS DEFINITION ================= */
/** UART operations instance */
uart_ops_t uart_ops = {
    .open  = uart_open_impl,
    .init  = uart_init_impl,
    .write = uart_write_impl,
    .read  = uart_read_impl,
    .close = uart_close_impl
};
