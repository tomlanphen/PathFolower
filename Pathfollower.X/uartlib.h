#ifndef UART_LIB_H
#define UART_LIB_H

#define UART_BUFFER_SIZE (10)
#define UART_EMPTRY_BUFFER (uart_recieve_head == uart_recieve_tail)
#define UART_SUPPORTED_FOSCS (7)
#define UART_SUPPORTED_BAUDRATES (8)
#define UART_SUPPORTED_MULTIPLIERS (4)

typedef enum {
    false,
    true
} bool;
/* index van gesloten clockspeed verzameling in Mhz (0.5 , 1, 2, 4, 8, 16 , 32) */
typedef char clockspeed;
/* index van gesloten baudrate verzameling in hz (300 ,1200 ,2400 ,9600 ,10417 ,19200 ,57600 ,115200) */
typedef char baudrate;
/* index van gesloten uart multiplier verzameling (64 (8 bit), 16(8 bit ), 16(16 bit ), 4(16 bit ))*/
typedef char uart_multiplier;

extern const short spbr_array[UART_SUPPORTED_FOSCS][UART_SUPPORTED_BAUDRATES][UART_SUPPORTED_MULTIPLIERS];
extern volatile char uart_recieve_buffer[UART_BUFFER_SIZE];
extern volatile char uart_recieve_head;
extern volatile char uart_recieve_tail;

bool uart_init(baudrate b, clockspeed c);
void uart_write_char(char byte);
void uart_write_string(const char *str);
void uart_write_newline(void);
void uart_write_line(const char *str);
void uart_handle_rcv_int(void);
void uart_handle_tx_int(void);
char uart_read_char(void);
unsigned char uart_numof_bytes_in_buffer(void);
// In de functie uart_flush_buffer ():
void uart_flush_buffer(void);
#endif /* UART_LIB_H */
