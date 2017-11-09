---
layout: post
title: 8. التعامل مع الأزرار Switches
lang: ar-SA
comments: true
---


في (ص 20) من دليل المستخدم الذي قمنا بتحميله في الدرس السابق نجد مخطط البورد schematics والتي تبين لنا الأطراف التي تتصل بها الأزرار SW1 و SW2:

{% include image.html url="assets/files/article_08/schematics_switches.png" border="1" %}

وهذا موقعها على البورد:

{% include image.html url="assets/files/article_08/switches_on_board.png" border="1" %}

حيث يتضح لنا أن:
* الزر الأيسر SW1 متصل بالمنفذ F والطرف 4 =>  PF4
* الزر الأيمن SW2 متصل بالمنفذ F والطرف 0 =>  PF0

بالإضافة الى أننا سنتعامل مع الإضاءات الملونة التي تعاملنا معها سابقاً:
* الإضاءة الحمراء  Red LED  متصلة بالمنفذ F والطرف 1 =>  PF1
* الإضاءة الزرقاء  Blue LED  متصلة بالمنفذ F والطرف 2 =>  PF2
* الإضاءة الخضراء  Green LED  متصلة بالمنفذ F والطرف 3 =>  PF3

نلاحظ أن جميع الأزرار والإضاءات متصلة بالمنفذ F، ولذلك فإن جميع السجلات التي سنتعامل معها ستكون خاصة بهذا المنفذ. 



# []()فكرة البرنامج

سنقوم بمشيئة الله بكتابة برنامج يتعامل مع الأزرار والإضاءات في البورد حيث سيختلف فيها الإضاءة التي تعمل حسب الزر المضغوط. وفيما يلي جدول يختصر هذه الحالات:

| حالة الأزرار | حالة الإضاءة |
|---:|---:|
| الضغط على SW1 و SW2 معاً |  الإضاءة الحمراء تعمل |
| الضغط على SW1 فقط |  الإضاءة الزرقاء تعمل |
| الضغط على SW2 فقط |  الإضاءة الخضراء تعمل |
| عدم الضغط على أي زر |  لا تعمل أي من الإضاءات |



# []()خطوات كتابة البرنامج
للقيام بذلك، فإنه يتوجب علينا تتبع الخطوات التالية:


## []()1. تفعيل المنفذ

مما سبق، يتضح لنا أن المنفذ الذي نريد تفعيله هو Port F، ونقوم بذلك عن طريق كتابة 1 الى البت رقم خمسة bit 5  كما يلي:

```
SYSCTL_RCGCGPIO_R = (1U << 5); // 0010 0000 
```

ثم ننتظر قليلاً قبل بدء التعامل مع المنفذ لنتأكد من مرور الوقت الكافي لتفعيل المنفذ:

```
volatile unsigned long wait;
wait = SYSCTL_RCGCGPIO_R; 
```


## []()2.الغاء قفل PF0
يوجد على هذه البورد عدة أطراف مقفلة لا نستطيع الكتابة اليها الا بعد فتح القفل وهي PC0 و PC1 و PC2 و PC3 و PD7 و PF0 وماعداها من أطراف لا يتوجب علينا القيام بهذه الخطوة بشأنها.

ولإلغاء قفل PF0 فإننا سنستخدم السجل GPIOLOCK (دليل البيانات ص 684):

{% include image.html url="assets/files/article_08/gpiolock.png" border="1" %}

وذلك عن طريق كتابة الرقم `0x4C4F434B` الى هذا السجل كما يلي:

```
#define GPIO_PORTF_LOCK_R   (*((volatile unsigned long *)0x40025520))

GPIO_PORTF_LOCK_R = 0x4C4F434B;
```


## []()3.السماح بالتعديل على الطرف PF0
بعد الغاء القفل عن الطرف PF0، نقوم بالسماح بالتعديل عليه عن طريق السجل GPIOCR (دليل البيانات ص685):

{% include image.html url="assets/files/article_08/gpiocr.png" border="1" %}

وذلك عن طريق كتابة 1 في البت الذي يقابل الطرف PF0 وهو bit 0:

```
#define GPIO_PORTF_CR_R   (*((volatile unsigned long *)0x40025524))

GPIO_PORTF_CR_R = (1U << 0);   
```


## []()4. تحديد إتجاه الأطراف

في هذا اليرنامج نريد تفعيل الإضاءة الموجوده على المنفذ F في الأطراف 1 و 2 و 3 وجعلها output. ولذلك نقوم بكتابة 1 في البت المقابلة لكل طرف. وسنجعل الأزرار مدخلات input وذلك بكتابة 0 في البت المقابل للطرف.

سنقوم أولاً بتحديد جميع المدخلات والمخرجات وما يقابلها من بت bits:

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
GPIO_PORTF_DIR_R  &= ~(SW1|SW2); 
```


## []()5. تفعيل خاصية مقاومة الرفع الداخلي للأزرار

لو عدنا الى (ص 20) في دليل المستخدم فإننا سنرى في الرسم التخطيطي الخاص بالأزرار أنه عند الضغط عليها فإن الطرف المتصل بالزر سيقرأ 0 حيث تتصل الدائرة بـالـ ground. ولكن في حالة عدم الضغط على الزر فإن القراءة ستبقى عائمة floating وغير مستقرة لأن الأزرار ليست متصلة بمقاومة رفع pull-up resistor ([للمزيد](https://www.arabsmakers.com/%D9%85%D9%82%D8%A7%D9%88%D9%85%D8%A7%D8%AA-%D8%A7%D9%84%D8%B1%D9%81%D8%B9-pull-resistors/)).

{% include image.html url="assets/files/article_08/schematics_switches_2.png" border="1" %}

ولذلك فإننا نقوم بتفعيل ما يسمى بالـ internal pull up resistor بواسطة السجل GPIOPUR والذي نجد تفاصيله في (ص 677) من دليل البيانات:

{% include image.html url="assets/files/article_08/gpiopur.png" border="1" %}

```
#define GPIO_PORTF_PUR_R   (*((volatile unsigned long *)0x40025510))

GPIO_PORTF_PUR_R = (SW1|SW2); 
```

ولو كنا نستخدم مقاومة خفض pull down بدلاً من مقاومة رفع pull up لكنا نقرأ 0 في حالة عدم الضغط على الزر و 1 في حالة الضغط عليه.


## []()6. تفعيل الخصائص الرقمية للأطراف

مثل ما ذكرنا سابقاً، فنحن مهتمين بالأطراف من 0 الى 4 في المنفذ F. ولتفعيلها نقوم بكتابة 1 في البت المقابل لكل طرف كما يلي:

```
GPIO_PORTF_DEN_R = (SW2|LED_RED|LED_BLUE|LED_GREEN|SW1);
```


## []()7. الكتابة الى الأطراف

الأزرار لها خاصية negative logic بمعنى أن قيمتها 1 في حالة عدم الضغط عليها و 0 اذا ما تم الضغط عليها. وهي عكس الإضاءات التي بها خاصية الـ positive logic حيث اذا ارسلنا لها 1 فإنها ستضيء واذا أرسلنا 0 فإنها ستطفئ.

في عبارة الـ switch في البرنامج المتعلق بهذا الدرس نختبر محتوى السجل `GPIO_PORTF_DATA_R` مع القيمة 0x11:

```
0x11 = 0001 0001
```

لقد أخترنا القيمة 0x11 لأنها تستخدم 1 في البت 0 وهو الخاص بالطرف 0 المتصل بـ SW2. وتستخدم 1 في البت 4 وهو يقابل الطرف 4 المتصل بـ SW1.

فلو أفترضنا أن `GPIO_PORTF_DATA_R` يحمل القيمة `0000 0000` - بمعنى أن جميع الأزرار مضغوطه بغض النظر عن أياً كانت قيمة الإضاءات - فإن نتيجة إختبار الـ switch سينتج لنا `0000 0000` مما يحقق شرط الحالة الأولى:

```
GPIO_PORTF_DATA_R = 0000 0000
(GPIO_PORTF_DATA_R & 0x11) // result: 0000 0000

case 0x00: // SW1 & SW2 pressed
  // الجزء هذا سينفذ
  break;
```

وقس على ذلك بقية الحالات، حيث أن: 
* الحالة الأولى تنفذ اذا كانت كل الأزرار مضغوطة ونقوم على ذلك بتشغيل الإضاءة الحمراء
* والحالة الثانية تنفذ اذا كان الزر الأيسر SW1 هو المضغوط فقط ونقوم بذلك بتشغيل الإضاءة الزرقاء
* والحالة الثالث تنففذ اذا كان الزر الأيمن SW2 هو المضغوط فقط ونقوم بذلك بتشغيل الإضاءة الخضراء
* والحالة القياسية default تنفذ في حالة لم يتم الضغط على أي زر وبذلك نقوم بإطفاء جميع الإضاءات


# []()البرنامج

قم بإنشاء مشروع جديد كما فعلنا في الدرس الثالث وأضف الكود التالي: 

```c
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
  GPIO_PORTF_CR_R   = SW2;
  
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
      case 0x01: // SW1 pressed only
        GPIO_PORTF_DATA_R = LED_BLUE;
        break;
      case 0x10: // SW2 pressed only
        GPIO_PORTF_DATA_R = LED_GREEN;
        break;
      default:
        GPIO_PORTF_DATA_R &= ~(LED_RED|LED_BLUE|LED_GREEN);
    }
  }	
}

```

أو قم بنسخه من الرابط التالي:

<https://github.com/alhabish/embedded-course/blob/gh-pages/assets/files/article_08/main.c>
