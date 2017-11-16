
/* ************************* Registers ************************ */

#define STCTRL_R      (*((volatile unsigned long *)0xE000E010))
#define STRELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define STCURRENT_R   (*((volatile unsigned long *)0xE000E018))


/* ************************ STCTRL Bits *********************** */

#define STCTRL_BIT_ENABLE   0
#define STCTRL_BIT_INTEN    1
#define STCTRL_BIT_CLKSRC   2
#define STCTRL_BIT_COUNT    16

/* *************************** main *************************** */

int main (void) {
  // Initialize SysTick
  //
  STCTRL_R    = (0U << STCTRL_BIT_ENABLE);    // 1) disable SysTick during setup
  STRELOAD_R  = 0x00FFFFFF;                   // 2) maximum reload value
  STCURRENT_R = 0;                            // 3) any write to current clears it
  
  STCTRL_R    = ((0U << STCTRL_BIT_INTEN)  |  // 4.1) disable interrupts
                 (1U << STCTRL_BIT_CLKSRC) |  // 4.2) enable core bus clock
                 (1U << STCTRL_BIT_ENABLE));  // 4.3) enable SysTick 
  
  volatile unsigned long current_time;  
  volatile unsigned long last_time;     
  volatile unsigned long elapsed_time;  
  volatile unsigned long sum = 0;
  
  for (int i=0; i<1000000; i++) {
    current_time = STCURRENT_R;
    sum ++;
    elapsed_time = (last_time-current_time) & 0x00FFFFFF; // 24-bit difference
    last_time = current_time;
  }
  
  return 0;
}
