
/* ************************* Registers ************************ */

#define STCTRL_R      (*((volatile unsigned long *)0xE000E010))
#define STRELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define STCURRENT_R   (*((volatile unsigned long *)0xE000E018))

/* ************************ STCTRL Bits *********************** */

#define STCTRL_BIT_ENABLE   0
#define STCTRL_BIT_INTEN    1
#define STCTRL_BIT_CLKSRC   2
#define STCTRL_BIT_COUNT    16

/* ************************ Prototypes ************************ */

void systick_delay_ms (unsigned long ms);

/* *************************** main *************************** */

int main (void) {
  // Initialize SysTick
  //
  STCTRL_R    = (0U << STCTRL_BIT_ENABLE);    // 1) disable SysTick during setup
  STRELOAD_R  = 0x00FFFFFF;                   // 2) maximum reload value
  STCURRENT_R = 0;                            // 3) any write to current clears it
  
  STCTRL_R   |= ((0U << STCTRL_BIT_INTEN)  |  // 4.1) disable interrupts
                 (1U << STCTRL_BIT_CLKSRC) |  // 4.2) enable core bus clock
                 (1U << STCTRL_BIT_ENABLE));  // 4.3) enable SysTick 
  
  // Wait for 0.5 second
  systick_delay_ms (500);
  
  // Wait for 1 second
  systick_delay_ms (1000);
  
  return 0;
}

void systick_delay_ms (unsigned long ms) {
  // 1 period = (real_time_delay in seconds) * (16,000,000 Hz)
  //          = (1/1000 seconds) * (16,000,000 Hz) 
  //          = 16,000
  const unsigned long period = 16000;
  
  for (unsigned long i = 0; i < ms; i++) {
    STRELOAD_R = (period - 1) & 0x00FFFFFF; // 24-bit difference
    STCURRENT_R = 0;
    
    // wait until COUNT is flagged
    while ((STCTRL_R & 0x00010000) == 0) {}
  }
}
