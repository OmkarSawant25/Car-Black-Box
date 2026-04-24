#ifndef MATRIX_KEYPAD_H
#define MATRIX_KEYPAD_H

#include <xc.h>

/* Detection modes */
#define LEVEL 0
#define EDGE  1

/* Key values */
#define KEY_1    1
#define KEY_2    2
#define KEY_3    3
#define KEY_4    4
#define KEY_5    5
#define KEY_6    6
#define KEY_7    7
#define KEY_8    8
#define KEY_9    9
#define KEY_10   10
#define KEY_11   11
#define KEY_12   12
#define ALL_RELEASED 0xFF

void init_matrix_keypad(void);
unsigned char scan_switch(void);
unsigned char triggering_type(char detect_type, char key);

#endif