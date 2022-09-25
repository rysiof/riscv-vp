#include "uart.h"
#include "platform.h"
#include <stdint.h>

int sendString(char* str, long len) {
    long cnt = len;
    const char *s = (const char *)str;
    while (cnt > 0) {
        --cnt;
        putChr(*s);
        s++;
    }
    return len;
}

int send(unsigned char *buf, long len){
    long cnt = len;
    unsigned char *p = (unsigned char*)buf;
    while(cnt > 0){
        --cnt;
        putChr(*p);
        p++;
    }
    return len;
}

void putChr(char chr) {
    #ifdef MICRORV
        uint32_t tx_rdy = 0;
        *UART_TX_ADDR = chr;
        // send character stored in uart_tx_addr
        *UART_TX_CTRL = 1;
        // wait until tx is ready again for sening another character
        do{
            // read uart tx status
            tx_rdy = *UART_TX_CTRL;
            asm volatile ("nop");
        } while(!tx_rdy);
    #else         
        uint32_t tx_rdy = 0;
        *UART_TX_DATA_ADDR = chr;
        // send character stored in uart_tx_addr
        *UART_TX_CTRL_ADDR = 1;
        // wait until tx is ready again for sening another character
        do{
            // read uart tx status
            tx_rdy = *UART_TX_CTRL_ADDR;
            asm volatile ("nop");
        } while(!tx_rdy);
    #endif
    return;
}


int UART_is_RX_empty(){
	int rx_empty;
	#ifdef MICRORV
        rx_empty = *UART_RX_FIFO_EMPTY_ADDR;
	#else
        rx_empty = *UART_RX_FIFO_EMPTY_ADDR;
	#endif
	return rx_empty;
}

char UART_read_char(){
	char c;
	
	int rx_empty = UART_is_RX_empty();
	#ifdef MICRORV
	if(!rx_empty) c = (char)*UART_RX_DATA_ADDR;
	#endif
	return c;
}
char UART_read_line();
