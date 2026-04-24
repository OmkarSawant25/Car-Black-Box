/*
 * File:   black_box.c
 * Author: OMKAR SAWANT
 *
 * Created on 14 April, 2026, 10:34 AM
 */

#include <xc.h>
#include "black_box.h"
#include "clcd.h"
#include "adc.h"
#include "i2c.h"
#include "ds1307.h"
#include <string.h>
#include "internal_eeprom.h"
#include "matrix_keypad.h"
#include "uart.h"

// Speed
static uint16_t adc_val;
static uint16_t speed;
static uint8_t speed_arr[3];

// RTC Time
static unsigned char time[9];
static unsigned char clock_reg[3];

// Gear
static const unsigned char gear[9][3] = {"ON", "NE", "G1", "G2", "G3", "G4", "G5", "GR", "CL"};
static unsigned short index = 0;

// EEPROM address
static unsigned char addr = 0x00;
static unsigned int event_count = 0;

extern unsigned char value;
unsigned int m_index = 0;
unsigned char menu_options[4][20] = {"View Log", "Download Log", "Clear Log", "Set Time"};

unsigned char read_data[10][16];
unsigned int v_index = 0;

unsigned char is_12hr = 0;

void get_gear(void) {
    static unsigned short prev_index = 0;

    if (value == KEY_1 && index < 7) {
        index++;
    } else if (value == KEY_2 && index > 0) {
        index--;
    } else if (value == KEY_3) {
        index = 8;
    }

    // Store only if gear changed 
    if (prev_index != index) {
        event_store();
        prev_index = index;
    }
}

void display_blink_time(unsigned char hr, unsigned char min, unsigned char sec, unsigned char field) {
    static unsigned int blink_delay = 0;

    // Display time 
    clcd_putch(hr / 10 + '0', LINE2(4));
    clcd_putch(hr % 10 + '0', LINE2(5));
    clcd_putch(':', LINE2(6));

    clcd_putch(min / 10 + '0', LINE2(7));
    clcd_putch(min % 10 + '0', LINE2(8));
    clcd_putch(':', LINE2(9));

    clcd_putch(sec / 10 + '0', LINE2(10));
    clcd_putch(sec % 10 + '0', LINE2(11));

    // Blink selected field 
    if (blink_delay++ > 250) {
        if (field == 0) {
            clcd_putch(' ', LINE2(4));
            clcd_putch(' ', LINE2(5));
        } else if (field == 1) {
            clcd_putch(0xFF, LINE2(7));
            clcd_putch(0xFF, LINE2(8));
        } else if (field == 2) {
            clcd_putch(0xFF, LINE2(10));
            clcd_putch(0xFF, LINE2(11));
        }
    }

    if (blink_delay > 500) {
        blink_delay = 0;
    }
}

void get_time(void) {
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    if (clock_reg[0] & 0x40) {
        is_12hr = 1;
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    } else {
        is_12hr = 0;
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }

    time[2] = ':';
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0';
}

void event_store(void) {
    get_time();
    event_count++;

    for (int i = 0; i < 8; i++) {
        write_internal_eeprom(addr++, time[i]);
    }

    write_internal_eeprom(addr++, gear[index][0]);
    write_internal_eeprom(addr++, gear[index][1]);

    write_internal_eeprom(addr++, speed_arr[0]);
    write_internal_eeprom(addr++, speed_arr[1]);

    if (addr >= 120) {
        addr = 0x00;
    }
}

void event_reader(void) {
    unsigned char start_addr;
    unsigned int end;

    // Decide how many logs to read 
    if (event_count > 10) {
        start_addr = (event_count % 10) * 12;
        end = 10;
    } else {
        start_addr = 0;
        end = event_count;
    }

    addr = start_addr;

    for (int i = 0; i < end; i++) {
        for (int j = 0; j < 15; j++) {
            if (j == 8 || j == 11) {
                read_data[i][j] = ' ';
            } else if (j == 14) {
                read_data[i][j] = '\0';
            } else {
                read_data[i][j] = read_internal_eeprom(addr);
                addr++;

                // Circular EEPROM wrap 
                if (addr >= 120) {
                    addr = 0x00;
                }
            }
        }
    }
}

void view_dashboard(void) {
    clcd_print("TIME", LINE1(2));
    clcd_print("EV", LINE1(9));
    clcd_print("SP", LINE1(13));

    // Get the time from RTC
    get_time();
    clcd_print(time, LINE2(0));

    // Print event / gear
    get_gear();
    clcd_print(gear[index], LINE2(9));

    // Read speed
    adc_val = read_adc(CHANNEL4);
    speed = adc_val / 10.23;

    speed_arr[0] = speed / 10 + '0';
    speed_arr[1] = speed % 10 + '0';
    speed_arr[2] = '\0';

    clcd_print(speed_arr, LINE2(13));

    if (value == KEY_11) {
        state = e_main_menu;
        CLEAR_DISP_SCREEN;
    }
}

void display_main_menu(void) {
    if (value == KEY_1) {
        CLEAR_DISP_SCREEN;

        if (m_index > 0) {
            m_index--;
        }
    }
    if (value == KEY_2) {
        CLEAR_DISP_SCREEN;

        if (m_index < 3) {
            m_index++;
        }
    }
    if (m_index == 0) {
        clcd_putch('*', LINE1(0));
        clcd_print(menu_options[m_index], LINE1(2));
        clcd_putch(' ', LINE2(0));
        clcd_print(menu_options[m_index + 1], LINE2(2));
    } else {
        clcd_putch(' ', LINE1(0));
        clcd_print(menu_options[m_index - 1], LINE1(2));
        clcd_putch('*', LINE2(0));
        clcd_print(menu_options[m_index], LINE2(2));
    }

    if (value == KEY_11) {
        CLEAR_DISP_SCREEN;

        if (m_index == 0) {
            state = e_view_log;
        }
        if (m_index == 1) {
            state = e_download_log;
        }
        if (m_index == 2) {
            state = e_clear_log;
        }
        if (m_index == 3) {
            state = e_set_time;
        }
    }
    if (value == KEY_12) {
        state = e_dashboard;
        value = 0;
        CLEAR_DISP_SCREEN;

    }
}

void view_log(void) {
    unsigned int total_logs;

    event_reader();
    clcd_print("N  TIME    GR SP", LINE1(0));

    if (event_count > 10) {
        total_logs = 10;
    } else {
        total_logs = event_count;
    }

    if (total_logs == 0) {
        clcd_print("NO LOGS PRESENT", LINE2(0));
    } else {
        // Previous log 
        if (value == KEY_1) {
            v_index = (v_index + total_logs - 1) % total_logs;
        }

        // Next log 
        if (value == KEY_2) {
            v_index = (v_index + 1) % total_logs;
        }

        clcd_putch(v_index + '0', LINE2(0));
        clcd_putch(' ', LINE2(1));
        clcd_print(read_data[v_index], LINE2(2));
    }

    if (value == KEY_12) {
        v_index = 0;
        state = e_main_menu;
        CLEAR_DISP_SCREEN;
    }
}

void clear_log(void) {
    clcd_print("CLEARING LOG...", LINE1(0));

    event_count = 0;
    addr = 0x00;
    v_index = 0;

    __delay_ms(2000);
    CLEAR_DISP_SCREEN;
    state = e_main_menu;
}

void download_log(void) {
    unsigned int total_logs;
    event_reader();

    clcd_print("DOWNLOADING...", LINE1(0));

    if (event_count == 0) {
        clcd_print("NO LOGS PRESENT", LINE2(0));
        __delay_ms(1000);
        state = e_main_menu;
        CLEAR_DISP_SCREEN;
        return;
    }

    if (event_count > 10) {
        total_logs = 10;
    } else {
        total_logs = event_count;
    }

    puts("\r\n");
    puts("N  TIME    GR SP\r\n");

    for (unsigned int i = 0; i < total_logs; i++) {
        putch(i + '0');
        putch(' ');
        puts(read_data[i]);
        puts("\r\n");
    }

    __delay_ms(1000);
    state = e_main_menu;
    CLEAR_DISP_SCREEN;
}

void set_time(void) {
    static unsigned char field = 0; // 0-HH, 1-MM, 2-SS
    static unsigned char hr, min, sec;
    static unsigned char init_flag = 0;

    clcd_print("    HH:MM:SS   ", LINE1(0));

    /* Load RTC time only once */
    if (init_flag == 0) {
        get_time();

        hr = (time[0] - '0') * 10 + (time[1] - '0');
        min = (time[3] - '0') * 10 + (time[4] - '0');
        sec = (time[6] - '0') * 10 + (time[7] - '0');

        init_flag = 1;
    }

    // Move between HH -> MM -> SS 
    if (value == KEY_2) {
        field++;
        if (field > 2)
            field = 0;
    }

    // Increment selected field 
    if (value == KEY_1) {
        if (field == 0) {
            if (is_12hr) {
                if (hr < 12)
                    hr++;
                else
                    hr = 1;
            } else {
                hr = (hr + 1) % 24;
            }
        } else if (field == 1) {
            min = (min + 1) % 60;
        } else if (field == 2) {
            sec = (sec + 1) % 60;
        }
    }

    display_blink_time(hr, min, sec, field);

    // Save time 
    if (value == KEY_11) {
        write_ds1307(HOUR_ADDR, ((hr / 10) << 4) | (hr % 10));
        write_ds1307(MIN_ADDR, ((min / 10) << 4) | (min % 10));
        write_ds1307(SEC_ADDR, ((sec / 10) << 4) | (sec % 10));

        init_flag = 0;
        field = 0;
        state = e_main_menu;
        CLEAR_DISP_SCREEN;
    }

    /* Cancel */
    if (value == KEY_12) {
        init_flag = 0;
        field = 0;
        state = e_main_menu;
        CLEAR_DISP_SCREEN;
    }
}