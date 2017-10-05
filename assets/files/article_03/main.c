#define SYSCTL_RCGCGPIO_R   (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTF_DIR_R    (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R    (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_DATA_R   (*((volatile unsigned long *)0x400253FC))

#define LED_RED    (1U<<1)		
#define LED_BLUE   (1U<<2)		
#define LED_GREEN  (1U<<3)		

void delay (){
  for (int i=0; i<5000000; i++);
}

int main (){
  SYSCTL_RCGCGPIO_R = 0X20;
  GPIO_PORTF_DIR_R  = 0xE; 
  GPIO_PORTF_DEN_R  = 0xE; 

  while (1){
    GPIO_PORTF_DATA_R = LED_RED;   
    delay();
    GPIO_PORTF_DATA_R = LED_BLUE;  
    delay();
    GPIO_PORTF_DATA_R = LED_GREEN; 
    delay();
  }	
}
