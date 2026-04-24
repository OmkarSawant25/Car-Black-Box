/*
 * File:   clcd.c
 * Author: Omkar Ashok Sawant
 */

#include <xc.h>
#include "clcd.h"

/* Write command or data to CLCD */
void clcd_write(unsigned char byte, unsigned char control_bit) {
    CLCD_RS = control_bit; // Select command/data register
    CLCD_PORT = byte; // Put data on port

    /* Enable pulse */
    CLCD_EN = HI;
    CLCD_EN = LO;

    /* Wait until CLCD is free */
    PORT_DIR = INPUT;
    CLCD_RW = HI; // Read mode
    CLCD_RS = INSTRUCTION_COMMAND;

    do {
        CLCD_EN = HI;
        CLCD_EN = LO;
    } while (CLCD_BUSY);

    CLCD_RW = LO; // Back to write mode
    PORT_DIR = OUTPUT;
}

/* Initialize CLCD */
void init_clcd(void) {
    TRISD = 0x00; // Data port as output
    TRISC = TRISC & 0xF8; // Control pins as output

    CLCD_RW = LO;

    /* Startup delay */
    __delay_ms(30);

    /* CLCD initialization sequence */
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(4100);

    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(100);

    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(1);

    CURSOR_HOME;
    __delay_us(100);

    TWO_LINE_5x8_MATRIX_8_BIT;
    __delay_us(100);

    CLEAR_DISP_SCREEN;
    __delay_us(500);

    DISP_ON_AND_CURSOR_OFF;
    __delay_us(100);
}

/* Display string on CLCD */
void clcd_print(const unsigned char *data, unsigned char addr) {
    clcd_write(addr, INSTRUCTION_COMMAND);

    while (*data != '\0') {
        clcd_write(*data++, DATA_COMMAND);
    }
}

/* Display single character on CLCD */
void clcd_putch(const unsigned char data, unsigned char addr) {
    clcd_write(addr, INSTRUCTION_COMMAND);
    clcd_write(data, DATA_COMMAND);
}