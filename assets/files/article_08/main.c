#define SYSCTL_RCGCGPIO_R   (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTF_DATA_R   (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R    (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_PUR_R    (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R    (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R   (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R     (*((volatile unsigned long *)0x40025524))

#define SW2        (1U << 0)
#define LED_RED    (1U << 1)		
#define LED_BLUE   (1U << 2)		
#define LED_GREEN  (1U << 3)		
#define SW1        (1U << 4)		

void delay (void) {
  for (int i=0; i<5000000; i++);
}

int main (void) {
  volatile unsigned long wait;
  
  // Initialize clock
  SYSCTL_RCGCGPIO_R = (1U << 5); // 0010 0000  
  wait = SYSCTL_RCGCGPIO_R; 
    
  // Unlock PF0
  GPIO_PORTF_LOCK_R = 0x4C4F434B;
  GPIO_PORTF_CR_R   = 0x1F; // 0001 1111
  
  // Set pin direction
  GPIO_PORTF_DIR_R  = (LED_RED|LED_BLUE|LED_GREEN); // 0000 1110
  GPIO_PORTF_DIR_R &= ~(SW1|SW2); // 0000 1110;
  
  // Set PF0 pull up resistor
  GPIO_PORTF_PUR_R  = (SW1|SW2); // 0001 0001
  
  // Enable pins
  GPIO_PORTF_DEN_R  = (SW2|LED_RED|LED_BLUE|LED_GREEN|SW1); // 0001 1111

  while (1) {
    switch (GPIO_PORTF_DATA_R & 0x11) 
    {
      case 0x00: // SW1 & SW2 pressed
        GPIO_PORTF_DATA_R = LED_RED;
        break;
      case 0x01: // SW2 pressed only
        GPIO_PORTF_DATA_R = LED_BLUE;
        break;
      case 0x10: // SW1 pressed only
        GPIO_PORTF_DATA_R = LED_GREEN;
        break;
      default:
        GPIO_PORTF_DATA_R &= ~(LED_RED|LED_BLUE|LED_GREEN);
    }
  }	
}
