---
layout: post
title: 8. التعامل مع الأزرار Switches
lang: ar-SA
comments: true
---



إضغط على Options for Target في شريط الأدوات toolbar ثم في تبويب C++/C وتأكد من وجود علامة صح بجانب C99 Mode. بعد ذلك بإمكانك إستخدام 
#define <stdint.h>
و uint32_t




في (ص 20) من دليل المستخدم الذي قمنا بتحميله في الدرس السابق نجد مخطط البورد schematics والتي تبين لنا الأطراف التي تتصل بها الأزرار SW1 و SW2:

{% include image.html url="assets/files/article_08/schematics_switches.png" border="1" %}

حيث نجد أن:
* الزر الأيسر SW1 متصل بالمنفذ F والطرف 4 =>  PF4
* الزر الأيمن SW2 متصل بالمنفذ F والطرف 0 =>  PF0

بالإضافة الى أننا سنتعامل مع الإضاءات الملونة التي تعاملنا معها سابقاً:
* الإضاءة الحمراء  Red LED  متصلة بالمنفذ F والطرف 1 =>  PF1
* الإضاءة الزرقاء  Blue LED  متصلة بالمنفذ F والطرف 2 =>  PF2
* الإضاءة الخضراء  Green LED  متصلة بالمنفذ F والطرف 3 =>  PF3

نلاحظ أن جميع الأزرار متصلة بالمنفذ F، ولذلك فإن جميع السجلات التي سنتعامل معها ستكون خاصة بهذا المنفذ. وفي الصفحة نفسها نستطيع أن نرى بأن الأزرار ليست متصلة بـ pull-up resistors:


{% include image.html url="assets/files/article_08/schematics_switches_2.png" border="1" %}

# []()فكرة البرنامج
شرح

| حالة الأزرار | حالة الإضاءة |
|---:|---:|
| الضغط على SW1 و SW2 معاً |  وميض الإضاءة الحمراء |
| الضغط على SW1 فقط |  وميض الإضاءة الزرقاء |
| الضغط على SW2 فقط |  وميض الإضاءة الخضراء |
| عدم الضغط على أي زر |  الإضاءة لا تومض |


# []()خطوات كتابة البرنامج
للقيام بذلك، فإنه يتوجب علينا تتبع الخطوات التالية:

## []()1. تفعيل المنفذ

مما سبق، يتضح لنا أن المنفذ الذي نريد تفعيله هو Port F، ولتفعيل Port F فإنه يتوجب علينا كتابة 1 الى البت الخامس bit 5  كما يلي:

```
SYSCTL_RCGCGPIO_R = (1U << 5); // 0010 0000 
```

ثم ننتظر قليلاً قبل بدء التعامل مع المنفذ لنتأكد من مرور لتفعيل المنفذ:

```
volatile unsigned long delay;
delay = SYSCTL_RCGCGPIO_R; 
```

## []()2. تحديد إتجاه الأطراف

في هذا اليرنامج نريد تفعيل الإضاءة الموجوده على المنفذ F في الأطراف 1 و 2 و 3 وجعلها output. ولذلك نقوم بكتابة 1 في البت المقابلة لكل طرف. وسنجعل الأزرار مدخلات input وذلك بكتابة 0 في البت المقابل للطرف كما يلي:

أولاً، سنقوم بتحديد كل الدخلات والمخرجات وما يقابها من بتات bits:


```c
#define SW2        (1U << 0)
#define LED_RED    (1U << 1)		
#define LED_BLUE   (1U << 2)		
#define LED_GREEN  (1U << 3)		
#define SW1        (1U << 4)
```

ثم سنجعل من أطراف الإضاءة مدخلات:

```c
GPIO_PORTF_DIR_R  = (LED_RED|LED_BLUE|LED_GREEN);
```

وبعد ذلك نجعل أطراف الأزرار مخرجات:

```c
GPIO_PORTF_DIR_R  |= ~(SW1|SW2); 
```

## []()2.الغاء قفل PF0
we unlock the port; unlocking is needed only for pins PD7, PF0 on the LM4F and TM4C. Only PC3-0, PD7, and PF0 on the TM4C need to be unlocked. All the other bits on the TM4C are always unlocked.

وذلك يكون بكتابة الرقم `0x4C4F434B` الى سجل الـ Lock register كما يلي:

{% include image.html url="assets/files/article_08/gpiolock.png" border="1" %}

```
#define GPIO_PORTF_LOCK_R   (*((volatile unsigned long *)0x40025520))

GPIO_PORTF_LOCK_R = 0x4C4F434B;
```

## []()2.السماح بالتعديل على الطرف PF0
بعد فتح الطرف في الخطوة السابقة نقوم بعملية السماح بالتعديل على الطرف في الـ Commit register:

{% include image.html url="assets/files/article_08/gpiocr.png" border="1" %}

```
#define GPIO_PORTF_CR_R   (*((volatile unsigned long *)0x40025524))

GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0  
                                  // only PF0 needs to be unlocked, other bits can't be locked    
```

## []()2.enable pull up resistor for switches
The switches are negative logic and will require activation of the internal pull-up resistors. In particular, you will set bits 0 and 4 in GPIO_PORTF_PUR_R register.


{% include image.html url="assets/files/article_08/gpiopur.png" border="1" %}

```
#define GPIO_PORTF_PUR_R   (*((volatile unsigned long *)0x40025510))
```
```
GPIO_PORTF_PUR_R = (SW1|SW2); 
```

## []()3. تفعيل الخصائص الرقمية للأطراف

مثل ما سبق فنحن مهتمين بالأطراف من 0 الى 4 في المنفذ F. ولتفعيلها نقوم بكتابة 1 في البت المقابل لكل طرف كما يلي:

```
GPIO_PORTF_DEN_R = (SW2|LED_RED|LED_BLUE|LED_GREEN|SW1);
```


## []()3. الكتابة الى السجل

 A negative logic switch means the PF4 signal will be 1 (high, 3.3V) if the switch is not pressed, and the PF4 signal will be 0 (low, +0V) if the switch is pressed. A positive logic blue LED interface means if the software outputs a 1 to PF2 (high, +3.3V) the LED will turn ON, and if the software outputs a 0 to PF2 (low, 0V) the blue LED will be OFF.

الإضاءة الحمراء متصلة بـ PF1. ولتشغيلها وإطفاء باقي الإضاءات

```
GPIO_PORTF_DATA_R = LED_RED; // 0010
```

سننتظر بعد ذلك قليلاً بإستدعء الدالة `delay` التي قمنا بكتابتها.

الإضاءة الزرقاء متصلة بـ PF2. ولتشغيلها وإطفاء باقي الإضاءات

```
GPIO_PORTF_DATA_R = LED_BLUE; // 0100
```

نعود بعد ذلك للإنتظار قليلاً، ثم:

الإضاءة الخضراء متصلة بـ PF3. ولتشغيلها وإطفاء باقي الإضاءات

```
GPIO_PORTF_DATA_R = LED_GREEN; // 1000
```

ثن ننتظر مرة أخرى ونقوم بتكرار عملية الوميض الى ما لا نهاية.


Switches on the board are negative on, meaning that their value is 1 and when pressed their value will be 0.



# []()البرنامج

قم بإنشاء مشروع جديد كما فعلنا في الدرس الثالث وأضف الكود التالي: 

```c
#define SYSCTL_RCGCGPIO_R   (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTF_DIR_R    (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R    (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_DATA_R   (*((volatile unsigned long *)0x400253FC))

#define LED_RED    (1U<<1)		
#define LED_BLUE   (1U<<2)		
#define LED_GREEN  (1U<<3)		

void delay ( ) {
  for (int i=0; i<5000000; i++);
}

int main ( ) {
  SYSCTL_RCGCGPIO_R = (1U << 5); // 0010 0000
  GPIO_PORTF_DIR_R  = (LED_RED | LED_BLUE | LED_GREEN); // 1110;
  GPIO_PORTF_DEN_R  = (LED_RED | LED_BLUE | LED_GREEN); // 1110; 

  while (1) {
    GPIO_PORTF_DATA_R = LED_RED;   
    delay();
    GPIO_PORTF_DATA_R = LED_BLUE;  
    delay();
    GPIO_PORTF_DATA_R = LED_GREEN; 
    delay();
  }	
}
```






Course   > C4 Digital Logic   > Lab 4   > About Lab 4


```
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
```


