/*
 * File:   main.c
 * Author: OMKAR SAWANT
 *
 * Created on 14 April, 2026, 9:59 AM
 */

#include "black_box.h"
#include "clcd.h"
#include "adc.h"
#include "matrix_keypad.h"
#include "ds1307.h"
#include "i2c.h"
#include "uart.h"

State_t state;
unsigned char key;
unsigned char value;

void init_config() {
    state = e_dashboard;

    init_clcd();
    init_adc();
    init_matrix_keypad();
    init_i2c();
    init_ds1307();
    init_uart();
}

void main(void) {
    init_config();

    while (1) {
        // Detect key press
        key = scan_switch();
        value = triggering_type(EDGE, key);

        switch (state) {
            case e_dashboard:
                // Display dashboard
                view_dashboard();
                break;

            case e_main_menu:
                // Display dashboard
                display_main_menu();
                break;

            case e_view_log:
                // Display dashboard
                view_log();
                break;

            case e_download_log:
                download_log();
                break;

            case e_clear_log:
                clear_log();
                break;

            case e_set_time:
                set_time();
                break;
        }
    }
}