#include "uartlib.h"
#include <PIC16F1829.h>

void uart_turnoff(void);
void uart_transmitter_setup(void);
void uart_reciever_setup(void);
bool uart_clock_setup(baudrate b, clockspeed c);

#define INC_WITH_BOUND(x, y) (x+1 >= y) ? (0) : (x+1)

const short spbr_array[UART_SUPPORTED_FOSCS][UART_SUPPORTED_BAUDRATES][UART_SUPPORTED_MULTIPLIERS] = {
    {
        { 25, 103, 103, 416,},
        { 0, 25, 25, 103,},
        { 0, 12, 12, 51,},
        { 0, 0, 0, 12,},
        { 0, 2, 2, 11,},
        { 0, 0, 0, 0,},
        { 0, 0, 0, 0,},
        { 0, 0, 0, 0,},
    },
    {
        { 51, 207, 207, 832,},
        { 12, 51, 51, 207,},
        { 0, 25, 25, 103,},
        { 0, 0, 0, 25,},
        { 0, 5, 5, 23,},
        { 0, 0, 0, 12,},
        { 0, 0, 0, 0,},
        { 0, 0, 0, 0,},
    },
    {
        { 103, 0, 416, 1666,},
        { 25, 103, 103, 416,},
        { 12, 51, 51, 207,},
        { 0, 12, 12, 51,},
        { 2, 11, 11, 47,},
        { 0, 0, 0, 25,},
        { 0, 0, 0, 0,},
        { 0, 0, 0, 0,},
    },
    {
        { 207, 0, 832, 3332,},
        { 51, 207, 207, 832,},
        { 25, 103, 103, 416,},
        { 0, 25, 25, 103,},
        { 5, 23, 23, 95,},
        { 0, 12, 12, 51,},
        { 0, 0, 0, 0,},
        { 0, 0, 0, 0,},
    },
    {
        { 0, 0, 1666, 6666,},
        { 103, 0, 416, 1666,},
        { 51, 207, 207, 832,},
        { 12, 51, 51, 207,},
        { 11, 47, 47, 191,},
        { 0, 25, 25, 103,},
        { 0, 0, 0, 34,},
        { 0, 0, 0, 0,},
    },
    {
        { 0, 0, 3332, 13332,},
        { 207, 0, 832, 3332,},
        { 103, 0, 416, 1666,},
        { 25, 103, 103, 416,},

        { 23, 95, 95, 383,},
        { 12, 51, 51, 207,},
        { 0, 0, 0, 68,},
        { 0, 0, 0, 34,},
    },
    {
        { 0, 0, 6666, 26666,},
        { 0, 0, 1666, 6666,},
        { 207, 0, 832, 3332,},
        { 51, 207, 207, 832,},
        { 47, 191, 191, 767,},
        { 25, 103, 103, 416,},
        { 0, 34, 34, 138,},
        { 0, 0, 0, 68,},
    },
};

volatile char uart_recieve_buffer[UART_BUFFER_SIZE];
volatile char uart_recieve_head;
volatile char uart_recieve_tail;

volatile char uart_transmit_buffer[UART_BUFFER_SIZE];
volatile char uart_transmit_head;
volatile char uart_transmit_tail;

bool uart_init(baudrate b, clockspeed c) {
    uart_turnoff();

    if (!uart_clock_setup(b, c))
        return false;

    uart_transmitter_setup();
    uart_reciever_setup();

    RCSTAbits.SPEN = 1;

    return true;
}

void uart_turnoff(void) {
    RCSTAbits.SPEN = 0; // disable uart to not interrupt while reconfiguring
    RCSTAbits.CREN = 0; // reciever off
    TXSTAbits.TXEN = 0; // transmitter off

    uart_flush_buffer();
}

void uart_transmitter_setup(void) {
    TXSTAbits.TX9 = 0;
    TXIE = 1;
    TXSTAbits.TXEN = 1;
}

void uart_reciever_setup(void) {
    RCSTAbits.RX9 = 0;

    ANSELBbits.ANSB5 = 0;

    RCIE = 1;

    RCSTAbits.CREN = 1;
}

bool uart_clock_setup(baudrate b, clockspeed c) {
    TXSTAbits.SYNC = 0; // asyncronous uart

    if (b > UART_SUPPORTED_BAUDRATES - 1)
        return false;

    if (c > UART_SUPPORTED_FOSCS - 1)
        return false;

    int i;
    for (i = 0; i < UART_SUPPORTED_MULTIPLIERS; i++) {
        if (spbr_array [c][b][i]) {
            SPBRG = spbr_array [c][b][i];
            switch (i) {
                case 0:
                    BRGH = 0;
                    BRG16 = 0;
                    break;
                case 1:
                    BRGH = 1;
                    BRG16 = 0;
                    break;
                case 2:
                    BRGH = 0;
                    BRG16 = 1;
                    break;
                case 3:
                    BRGH = 1;
                    BRG16 = 1;
                    break;
            }
            break;
        }
    }

    if (i >= UART_SUPPORTED_MULTIPLIERS)
        return false;

    return true;
}

void uart_write_char(char byte) {
    char next_index = INC_WITH_BOUND(uart_transmit_head,
            UART_BUFFER_SIZE);
    while (next_index == uart_transmit_tail) {
        continue;
    }

    uart_transmit_buffer[uart_transmit_head] = byte;
    uart_transmit_head = next_index;

    TXIE = 1;
}

void uart_write_string(const char *str) {
    while (*str) {
        uart_write_char(*str);
        str++;
    }
}


static const char *NEWLINE = "\r\n";

void uart_write_newline(void) {
    uart_write_string(NEWLINE);
}

void uart_write_line(const char *str) {
    uart_write_string(str);
    uart_write_newline();
}

void uart_handle_rcv_int(void) {
    if (RCIF && RCIE) {
        // will reset after reading RCREG
        if (FERR) {
        }

        // ust be cleared or no new characters are recieved
        if (OERR) {
            CREN = 0;
            CREN = 1;
        }

        uart_recieve_buffer[uart_recieve_head] = RCREG;
        uart_recieve_head = INC_WITH_BOUND(uart_recieve_head,
                UART_BUFFER_SIZE);
    }
}

void uart_handle_tx_int(void) {
    if (TXIF && TXIE) {
        if (uart_transmit_head == uart_transmit_tail) {
            TXIE = 0;
        } else {
            TXREG = uart_transmit_buffer[uart_transmit_tail];
            uart_transmit_tail = INC_WITH_BOUND(uart_transmit_tail,
                    UART_BUFFER_SIZE);
        }
    }
}

char uart_read_char(void) {
    if (UART_EMPTRY_BUFFER) {
        return '\0';
    }

    char uart_char = uart_recieve_buffer[uart_recieve_tail];
    uart_recieve_tail = INC_WITH_BOUND(uart_recieve_tail,
            UART_BUFFER_SIZE);
    return uart_char;
}

unsigned char uart_numof_bytes_in_buffer(void) {
    if (UART_EMPTRY_BUFFER)
        return 0;

    if (uart_recieve_head > uart_recieve_tail)
        return uart_recieve_tail - uart_recieve_head;

    return UART_BUFFER_SIZE - uart_recieve_tail + uart_recieve_head;
}

void uart_flush_buffer(void) {
    int i = UART_BUFFER_SIZE;
    while (i--) {
        uart_recieve_buffer[i] = 0;
        uart_transmit_buffer[i] = 0;
    }

    uart_recieve_head = 0;
    uart_recieve_tail = 0;

    uart_transmit_head = 0;
    uart_transmit_tail = 0;
}