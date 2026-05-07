/**
 * @file main.c
 * @author Raghunath Palla
 * @date 2026-04-29
 * @brief main file, entry point
 *
 * @details
 * This file contains the main entry point for the UART
 * application. It parses command-line arguments, initializes
 * the UART device, sends user commands, and prints responses.
 * 
 * This software is released under the MIT License.
 * Permission is hereby granted to use, copy, modify, and distribute
 * this software for any purpose with or without fee.
 */

/* ============ Defines =========== */

#include "headers.h"
#include "declarations.h"
#include "uart_operations.h"
#include <getopt.h>

/**
 * @brief Print usage/help
 * For help
 */
void usage(const char *prog)
{
    printf("\nUART CLI Tool\n");
    printf("Usage:\n");
    printf("  %s -d <device> -s <baudrate> \"message\"\n\n", prog);

    printf("Options:\n");
    printf("  -d <device>     UART device (e.g., /dev/ttyACM0)\n");
    printf("  -s <baudrate>   Baud rate (e.g., 9600, 115200)\n");
    printf("  -h, --help      Show this help message\n\n");

    printf("Example:\n");
    printf("  %s -d /dev/ttyACM0 -s 9600 \"test\"\n\n", prog);
}

/**
 * @brief Main entry point
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 on success
 */
int main(int argc, char *argv[])
{
    uart_dev_t dev;
    int opt;
    int baud = 9600;
    char *device = NULL;
    char *message = NULL;

    /* Long options */
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    /* Parse arguments */
    while ((opt = getopt_long(argc, argv, "d:s:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'd':
                device = optarg;
                break;
            case 's':
                baud = atoi(optarg);
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                usage(argv[0]);
                return -1;
        }
    }

    /* Validate inputs */
    if (device == NULL) {
        printf("Error: Device not specified\n");
        usage(argv[0]);
        return -1;
    }

    if (optind < argc) {
        message = argv[optind];
    } else {
        printf("Error: Message not provided\n");
        usage(argv[0]);
        return -1;
    }

    dev.baudrate = baud;

    printf("Device : %s\n", device);
    printf("Baud   : %d\n", baud);
    printf("Message: %s\n", message);

    /* Open UART */
    if (uart_ops.open(&dev, device) != 0)
        return -1;

    if (uart_ops.init(&dev) != 0)
        return -1;

    sleep(2);
    tcflush(dev.fd, TCIOFLUSH); /* flush it */
    char txbuf[256];
    snprintf(txbuf, sizeof(txbuf), "%s\n", message);

    uart_ops.write(&dev, txbuf);

    /* Read response */
    uart_ops.read(&dev);

    uart_ops.close(&dev);

    return 0;
}
