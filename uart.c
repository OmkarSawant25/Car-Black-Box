/*
 * File:   uart.c
 * Author: Omkar Ashok Sawant
 */

#include <xc.h>
#include "uart.h"

/* Initialize UART peripheral */
void init_uart(void) {
    /* Configure TX and RX pins */
    RX_PIN = 1;
    TX_PIN = 0;

    /* TXSTA - Transmitter Status and Control Register */
    TX9 = 0; // 8-bit transmission
    TXEN = 1; // Enable transmission
    SYNC = 0; // Asynchronous mode
    SENDB = 0; // Disable break character
    BRGH = 1; // High baud rate

    /* RCSTA - Receiver Status and Control Register */
    SPEN = 1; // Enable serial port
    RX9 = 0; // 8-bit reception
    CREN = 1; // Continuous receive enable

    /* BAUDCTL - Baud Rate Control Register */
    BRG16 = 0; // 8-bit baud rate generator

    /* Baud rate setup */
    /* 129 for 9600 baud */
    SPBRG = 129;

    /* Clear UART interrupt flags */
    TXIF = 0;
    RCIF = 0;
}

/* Transmit one byte */
void putch(unsigned char byte) {
    while (!TXIF) {
        continue;
    }

    TXIF = 0;
    TXREG = byte;
}

/* Transmit string */
int puts(const char *s) {
    while (*s) {
        putch(*s++);
    }

    return 0;
}

/* Receive one byte */
unsigned char getch(void) {
    while (!RCIF) {
        continue;
    }

    RCIF = 0;
    return RCREG;
}

/* Receive and echo one byte */
unsigned char getche(void) {
    unsigned char c;

    putch(c = getch());

    return c;
}