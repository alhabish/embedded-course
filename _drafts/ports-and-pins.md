---
layout: post
title: (6) المنافذ والطرفيات  Ports & Pins
lang: ar-SA
comments: true
---

للمتحكم 43 طرف pin وليسهل التعامل معها فإنها موزعة على 6 منافذ ports كما يلي: 
* **Port A** وبه 8 أطراف
* **Port B** وبه أيضاً 8 أطراف
* **Port C** وبه 8 أطراف (ولكن 4 منها للـ debugger فنستخدم فعلياً في هذا المنفذ 4 أطراف فقط)
* **Port D** وبه 8 أطراف
* **Port E** وبه 6 أطراف
* **Port F** وبه 5 أطراف

ملاحظة، بإمكاننا أن نرى على البورد بأن Port C لديه 8 أطراف لكننا لن نستخدم الأطراف التالية:
PC0, PC1, PC2, PC3
حيث أنها محجوزة للـ debugger

وبذلك يكون لدينا 43 طرف يكون برمجتها و4 منها محجوز أي 39 طرف.

هذه الأطراف تؤدي مهمتي الدخول input والتي تمكننا من إستقبال معلومات من البيئة المحيطة والخروج output والتي تمكننا من ارسال معلومات لهذه البيئة.

في الفصل العاشر chapter 10 من دليل البيانات datasheet (ص 647) نجد تفاصيل أكثر عن المنافذ والأطراف. 

+++++++++

وفي الفصل العاشر chapter 22 من دليل البيانات datasheet (ص 1328) نجد الرسم التخطيطي للأطراف في المتحكم. 
{% include image.html url="assets/files/article_06/pin_diagram.png" border="1" %}

أرجو التنويه الى أنه يوجد مسارين لنقل البيانات المتعلقة بالـ Input/Output ports وهي:
* Advanced Peripheral Bus - **APB**
* Advanced High-Performance Bus - **AHB**

المسار الثاني يعتبر الأفضل أداء والأسرع والأول أفضل من ناحية توفير الطاقة وهو الذي سيتم إستخدامه في هذا الكورس بإذن الله.

## []()عناوين المنافذ في الذاكرة على المسار APB
لكل منفذ مساحة 4kbyte من الذاكرة تبدأ من العنوان الرئيسي base address للمنفذ. 
* **Port A**: من 0x4000.4000 الى 0x4000.4FFF
* **Port B**: من 0x4000.5000 الى 0x4000.5FFF
* **Port C**: من 0x4000.6000 الى 0x4000.6FFF
* **Port D**: من 0x4000.7000 الى 0x4000.7FFF
* **Port E**: من 0x4002.4000 الى 0x4002.4FFF
* **Port F**: من 0x4002.5000 الى 0x4002.5FFF

{% include image.html url="assets/files/article_06/gpio_memory_map.png" border="1" %}

تحتوي مساحة كل منفذ على سجلات registers عديدة للتحكم بالمنفذ ومن ضمنها السجل المتعلق بتحديد وظيفة أو إتجاه كل طرف في المنفذ Direction Register والسجل الخاص بالكتابة الى أو القراءة من المنفذ Data Register بالإضافة الى غيرها من السجلات المتعلقة بالمنفذ.

## []()طريقة الكتابة الى سجل register
بإمكانك كتابة قيمة من 8bit للـ register لتعديل قيمة جميع الأطراف pins أو أن استخدم طريقة ال bit shifting لتعديل قيمة طرف معيّن. 


{% include image.html url="assets/files/article_06/rgb_led_pins.png" border="1" %}

## []()خطوات التعامل مع أطراف الـ GPIO

1. سجل 



The ARM microcontrollers use 4 bytes of memory space for 8-bit peripheral I/O ports.

The software sets the corresponding direction register bit to a 0 to specify an input port. It sets the direction register bit to 1 to specify an output port.


An input port has the direction register bit at 0, meaning the software can only read the values on the input pin. An output port has the direction register bit at 1, meaning the software can read and write to the pin.


In C, we use #define MACROS to assign a symbolic names to the corresponding addresses of the ports.
```
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))
```
volatile	Can change implicitly outside the direct action of the software. It disables compiler optimization, forcing the compiler to fetch a new value each time





