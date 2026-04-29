/**
 * @file uart_operations.h
 * @brief UART operations interface (driver-style abstraction)
 */

#ifndef UART_OPERATIONS_H
#define UART_OPERATIONS_H

#include "declarations.h"


/**
 * @struct uart_ops_t
 * @brief UART operations structure (similar to Linux file_operations)
 *
 * Contains function pointers for UART operations.
 */

typedef struct uart_ops {
    int (*open)(uart_dev_t *dev, const char *device);    /**< Open UART device */
    int (*init)(uart_dev_t *dev);                        /**< Initialize UART */
    int (*write)(uart_dev_t *dev, const char *msg);      /**< Write data */
    int (*read)(uart_dev_t *dev);                        /**< Read data */
    void (*close)(uart_dev_t *dev);                      /**< Close device */
} uart_ops_t;

/* Extern declaration */
/** Global UART operations instance */
extern uart_ops_t uart_ops;

#endif
