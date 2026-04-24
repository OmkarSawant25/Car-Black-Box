/*
 * File:   matrix_keypad.c
 * Author: Omkar Ashok Sawant
 */

#include <xc.h>
#include "matrix_keypad.h"

/* Row pins */
#define ROW1 PORTBbits.RB5
#define ROW2 PORTBbits.RB6
#define ROW3 PORTBbits.RB7

/* Column pins */
#define COL1 PORTBbits.RB1
#define COL2 PORTBbits.RB2
#define COL3 PORTBbits.RB3
#define COL4 PORTBbits.RB4

/* Flag for edge trigger detection */
static char once = 1;

/* Initialize keypad rows and columns */
void init_matrix_keypad(void) {
    TRISB = 0x1E;              // RB1-RB4 input, RB5-RB7 output
    INTCON2bits.RBPU = 0;      // Enable internal pull-up resistors
    PORTB |= 0xE0;             // Set all rows HIGH
}

/* Scan keypad and return pressed key */
unsigned char scan_switch(void) {

    /* Scan ROW1 */
    ROW1 = 0;
    ROW2 = 1;
    ROW3 = 1;

    if (COL1 == 0) return KEY_1;
    else if (COL2 == 0) return KEY_4;
    else if (COL3 == 0) return KEY_7;
    else if (COL4 == 0) return KEY_10;

    /* Scan ROW2 */
    ROW1 = 1;
    ROW2 = 0;
    ROW3 = 1;

    if (COL1 == 0) return KEY_2;
    else if (COL2 == 0) return KEY_5;
    else if (COL3 == 0) return KEY_8;
    else if (COL4 == 0) return KEY_11;

    /* Scan ROW3 */
    ROW1 = 1;
    ROW2 = 1;
    ROW3 = 0, ROW3= 0;

    if (COL1 == 0) return KEY_3;
    else if (COL2 == 0) return KEY_6;
    else if (COL3 == 0) return KEY_9;
    else if (COL4 == 0) return KEY_12;

    return ALL_RELEASED;       // No key pressed
}

/* Detect key press type: level or edge */
unsigned char triggering_type(char detect_type, char key) {
    if (detect_type == LEVEL) {
        return key;
    } 
    else if (detect_type == EDGE) {
        if ((key != ALL_RELEASED) && once) {
            once = 0;
            return key;
        } 
        else if (key == ALL_RELEASED) {
            once = 1;
        }
    }

    return ALL_RELEASED;
}