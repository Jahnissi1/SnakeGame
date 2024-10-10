#include "gpio.h"
#include "uart.h"

#define CLK 11  // Clock pin
#define LAT 9   // Latch pin
#define DAT 10  // Data pin

#define SYSTMR_LO        ((volatile unsigned int*)(MMIO_BASE+0x00003004))   // Address for the system timer

// Buttons on the SNES Controller
#define BUTTON_B         1
#define BUTTON_Y         2
#define BUTTON_SEL       4
#define BUTTON_START     8
#define BUTTON_UP        16
#define BUTTON_DOWN      32
#define BUTTON_LEFT      64
#define BUTTON_RIGHT     128
#define BUTTON_A         256
#define BUTTON_X         512
#define BUTTON_L         1024
#define BUTTON_R         2048

unsigned *clo = (unsigned*)SYSTMR_LO;

// This funcion prints out an appropriate message to the UART terminal.
// Used the UART I/O subrountine from the tutorial
void print_message(char *str) {
	uart_puts(str);
}

// This function waits for a time interval, given as a parameter ms
// Uses the system timer to measure the time waited in this function
// Used the following code from the RPi 2 SNES lecture notes, page 23
void wait(int ms)
{
	unsigned c = *clo + ms;
    while(c > *clo);    // Wait at least ms microseconds
}

// This function initializes a GPIO line
// Line number and function code given as parameters
// Designed only for LAT, CLK, and DAT
void init_GPIO(int line_number, int function_code){
    if(function_code == 001){       // Output
        if(line_number == LAT){
            *GPFSEL0 &= ~(0b111 << 27); // Clear bits 
            *GPFSEL0 |= (0b001 << 27);  // Put 0b001 in an appropriate spot
        }else if(line_number == CLK){
            *GPFSEL1 &= ~(0b111 << 3);
            *GPFSEL1 |= (0b001 << 3);
        }
    }else{                          // Input
        if(line_number == LAT){
            *GPFSEL0 &= ~(0b111 << 27);  // Clear bits
        }else if(line_number == CLK){
            *GPFSEL1 &= ~(0b111 << 3);
        }else if(line_number == DAT){
            *GPFSEL1 &= ~(0b111 << 0);
        }
    }
}

// This function either sets the latch level to high or low
void write_latch(int n){
    if(n == 1) *GPSET0 = 1 << LAT;  // High
    else *GPCLR0 = 1 << LAT;    // Low
}

// This function either sets the clock level to high or low
void write_clock(int n){
    if(n == 1) *GPSET0 = 1 << CLK;
    else *GPCLR0 = 1 << CLK;
}

// This function checks the level of the data line (0 if the buttons are pressed)
int read_data(){
    return(*GPLEV0 & (1 << DAT));
}

// This function reads the input from the SNES controller
// Used the pseudocode from the SNES lecture notes, page 20
unsigned short read_snes(){
    unsigned short data = 0;    // Using 16 bits unsigned short number as data, because there are 16 buttons (last 4 are reserved)
    write_clock(1); 
    write_latch(1);
    wait(12);
    write_latch(0);
    for(int i = 0; i < 16; i++){
        wait(6);
        write_clock(0);
        wait(6);
        int value = read_data();
        if(value == 0)  data |= 1 << i;     // The button is pressed
        write_clock(1);
    }
    return data;
}
