
/* ************************* Registers ************************ */

#define STCTRL_R      (*((volatile unsigned long *)0xE000E010))
#define STRELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define STCURRENT_R   (*((volatile unsigned long *)0xE000E018))

#define SYSCTL_RCGCGPIO_R   (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTF_DATA_R   (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R    (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_PUR_R    (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R    (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R   (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R     (*((volatile unsigned long *)0x40025524))

/* ************************ STCTRL Bits *********************** */

#define STCTRL_BIT_ENABLE   0
#define STCTRL_BIT_INTEN    1
#define STCTRL_BIT_CLKSRC   2
#define STCTRL_BIT_COUNT    16

#define LED_RED    (1U << 1)	

/* ************************ Prototypes ************************ */

void portf_init (void);
void systick_init (void);

/* *************************** main *************************** */

int main (void) {
  portf_init ();
  systick_init ();
  
  while (1) {
    if (STCTRL_R & 0x10000) { /* is COUNT flag set? */
        GPIO_PORTF_DATA_R ^= LED_RED;
    }
  }	
  
  return 0;
}

void portf_init (void) {
  volatile unsigned long wait;
  
  // Initialize clock
  SYSCTL_RCGCGPIO_R = (1U << 5); // 0010 0000  
  wait = SYSCTL_RCGCGPIO_R; 
    
  // Set pin direction
  GPIO_PORTF_DIR_R  = LED_RED;

  // Enable pins
  GPIO_PORTF_DEN_R  = LED_RED;
}

void systick_init (void) {
  STCTRL_R    = (0U << STCTRL_BIT_ENABLE);    // 1) disable SysTick during setup
  STRELOAD_R  = 16000000 - 1;                 // 2) maximum reload value
  STCURRENT_R = 0;                            // 3) any write to current clears it
  
  STCTRL_R   |= ((0U << STCTRL_BIT_INTEN)  |  // 4.1) disable interrupts
                 (1U << STCTRL_BIT_CLKSRC) |  // 4.2) enable core bus clock
                 (1U << STCTRL_BIT_ENABLE));  // 4.3) enable SysTick 
}
