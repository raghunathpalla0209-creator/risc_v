#ifndef DECLARATIONS_H
#define DECLARATIONS_H

typedef struct uart_dev {
    int fd;
    char device[64];
    int baudrate;
} uart_dev_t;

#endif
