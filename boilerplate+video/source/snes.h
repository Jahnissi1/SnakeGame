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


void print_message(char *str);
void wait(int ms);
void init_GPIO(int line_number, int function_code);
void write_latch(int n);
void write_clock(int n);
int read_data();
unsigned short read_snes();
