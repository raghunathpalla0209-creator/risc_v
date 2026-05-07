/**
 * @file declarations.h
 * @author Raghunath Palla
 * @date 2026-04-29
 * @brief UART device structure definitions
 *
 * @details
 * Defines structures used to represent UART device state
 * and configuration parameters.
 *
 * Distributed under the MIT License.
 * Free to distribute , copy, read, write
 */

#ifndef DECLARATIONS_H
#define DECLARATIONS_H


/**
 * @struct uart_dev_t
 * @brief UART device structure
 *
 * Holds file descriptor, device path, and configuration.
 */

typedef struct uart_dev {
    int fd;                 /**< File descriptor for UART device */
    char device[64];        /**< Device path (e.g., /dev/ttyACM0) */
    int baudrate;           /**< UART baud rate */
} uart_dev_t;

#endif
