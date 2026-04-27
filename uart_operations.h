#ifndef UART_OPERATIONS_H
#define UART_OPERATIONS_H

#include "declarations.h"

/**
 * @brief UART operations structure (similar to file_operations)
 */
typedef struct uart_ops {
    int (*open)(uart_dev_t *dev, const char *device);
    int (*init)(uart_dev_t *dev);
    int (*write)(uart_dev_t *dev, const char *msg);
    int (*read)(uart_dev_t *dev);
    void (*close)(uart_dev_t *dev);
} uart_ops_t;

/* Extern declaration */
extern uart_ops_t uart_ops;

#endif
