---
layout: post
title: (7) برنامج لتفعيل وميض الإضائة الملونة  RGB Led Blinking
lang: ar-SA
comments: true
---

يالإمكان تحميل دليل المستخدم للبورد من الرابط التالي:

<http://www.ti.com/lit/ug/spmu296/spmu296.pdf>

حيث نجد في ص 7 الرسم التخطيطي block diagram للبورد والتي تبين لنا بأن الإضائة الملونة RGB Led متصلة بمنافذ القراءة والكتابة GPIO Ports:

{% include image.html url="assets/files/article_07/block_diagram_rgb_led.png" description="Tiva C LaunchPad Block Diagram" border="1" %}

وفي ص 20 من نفس المستند نجد مخطط البورد schematics والتي تبين لنا الأطراف التي تتصل بها الإضاءة الملونة:

{% include image.html url="assets/files/article_07/schematics_rgb_led.png" border="1" %}

نستطيع أن نرى كيف هذه الإضاءة تتصل بالمتحكم. ونجد أن:
* الإضاءة الحمراء  Red LED  متصلة بالمنفذ F والطرف 1 أي  Port F, Pin 1
* الإضاءة الزرقاء  Blue LED  متصلة بالمنفذ F والطرف 2 أي  Port F, Pin 2
* الإضاءة الخضراء  Green LED  متصلة بالمنفذ F والطرف 3 أي  Port F, Pin 3

### []()1. سجل الساعة (تفعيل المنفذ)

مما سبق، يتضح لنا أن المنفذ الذي نريد تفعيله هو Port F، وبالعودة الى دليل البيانات datasheet ص 340 والذي يشرح فيه السجل RCGCGPIO المسؤول عن تفعيل الساعة للمنفذ، نجد أنه لتفعيل Port F فإنه يتوجب علينا كتابة 1 الى البت الخامس bit 5 (علماً بأن العد يبجأ من 0). فنقول:

```
#define SYSCTL_RCGCGPIO_R   (*((volatile unsigned long *)0x400FE608U))
```
والتعديل عليها يكون هكذا:

```
SYSCTL_RCGCGPIO_R = 0x20; // 0000 0000 0000 0000 0000 0000 0010 0000
```


### []()2. سجل الإتجاه

```
GPIO_PORTF_DIR_R = 0xE; // 1110 -> PF0=0, PF1=1, PF2=1, PF3=1
```

### []()3. تفعيل الخصائص الرقمية digital للأطراف


```
GPIO_PORTF_DEN_R = 0xE; // 1110 -> PF0=0, PF1=1, PF2=1, PF3=1
```

### []()3. الكتابة

لتشغيل الإضاءة الحمراء وإطفاء باقي الإضاءات

```
GPIO_PORTF_DATA_R = 0x02; // 0010
```

لتشغيل الإضاءة الزرقاء وإطفاء باقي الإضاءات

```
GPIO_PORTF_DATA_R = 0x04; // 0100
```

لتشغيل الإضاءة الخضراء وإطفاء باقي الإضاءات

```
GPIO_PORTF_DATA_R = 0x08; // 1000
```





The input is a switch called SW1, which is connected port pin PF4. Three outputs (PF3, PF2, PF1) are connected to one multi-color LED. The color of the LED is determined by the 3-bit value written to the outputs. If SW1 is not pressed the LED toggles blue-red, and if SW1 is pressed the LED toggles blue-green.

