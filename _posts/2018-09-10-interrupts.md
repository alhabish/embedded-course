---
layout: post
title: 14. المُقاطعات  Interrupts 
lang: ar-SA
comments: true
---

توجد طريقتان لتتمكن المعالجات الدقيقة microprocessors من خدمة الأجهزة المختلفة التي تتصل بها، وهي إما:

* المُقاطعات interrupts أو 
* الإستعلام المتكرر  polling

في حالة الإستعلام، يقوم المعالج بمراقبة حالة الجهاز بإستمرار وينتظر حتى إستيفاء الحالة أو تحقق الشرط المطلوب ثم يقوم بتنفيذ الأوامر المطلوبة، وبعد إكتمال عملية تنفيذ المهمة، يقوم المعالج بتنفيذ المهمة التي تليها.

فعلى سبيل المثال، في درس SysTick Timer، راقبنا بإستمرار قيمة الحقل COUNT لحينما تصبح 0 وذلك لضمان انقضاء الوقت المحدد:

```c
// wait until COUNT is flagged
while ((STCTRL_R & 0x00010000) == 0) {}
```

لكن، وكما هو واضح، المعالج مشغول بمراقبة هذه القيمة ولن يتمكن من الإنتقال الى تنفيذ الأوامر الموجودة بعد هذا السطر طالما لم يتحقق. ونحن بهذه الطريقة لم نستغل المعالج بشكل فعّال حيث نهدر الكثير من الوقت والطاقة لمراقبة قيّم وأجهزة ليست بحاجتنا الى أن نلتفت اليها. 

ولكن، بإستخدام المُقاطعات فإنه عندما يحتاج أي جهاز إلى خدمة ما، فإنه يقوم بإعلام المعالج بذلك عن طريق إرسال إشارة المقاطعة Interrupt Request - IRQ. وعندما يتلقي المعالج هذه الإشارة، فإنه "يقطع" ما يقوم به في تلك اللحظة ويلتفت الى خدمة الجهاز المرسل للإشارة. وبذلك يتمكن المعالج من خدمة الجهاز دون مراقبة حالته بشكل مستمر. ولذلك ينصح بإستخدام المُقاطعات.

---

## []() شرح الخطوات

فيما يلي شرح مختصر لما يحدث عند إرسال مُقاطعة:

1. عندما يحتاج منفذ port أو جهاز طرفي peripheral الى خدمة من المعالج فإنه يرسل إشارة بذلك. هذه الإشارة تسمى طلب المُقاطعة Interrupt Request - IRQ. 

2. قد يكون المعالج في وضع السكون أو مشغولاً بأداء مهام أخرى، ولكن حينما يصله طلب المقاطعة فإنه يقوم بتعليق المهمة التي ينفذها ويحفظ مكانها.

3. يبحث المعالج في جدول المقاطعات Interrupt Vector Table - IVT للعثور على الدالة function المرتبطة بالمقاطعة والتي يجب عليه تنفيذها عند حدوث هذه المقاطعة. 

4. يتم بعد بعد ذلك تنفيذ الدالة المرتبطة بالمُقاطعة Interrupt Service Routine - ISR. وبمجرد اكتمال تنفيذها، يعود المعالج لتنفيذ المهام التى كان يعمل عليها قبل إستقبال طلب المقاطعة.

وفيما يلي سنلقي نظرة على الأجزاء المختلفة المتعلقة بالمُقاطعات في متحكمات ARM Cortex-M:

---

## []() Nested Vectored Interrupt Controller - NVIC

تتم عملية إدارة المقاطعات بواسطة وحدة Nested Vector Interrupt Controller - NVIC، والتي توفر عدة خصائص من ضمنها تمكين/الغاء خاصية إستقبال المقاطعات وتحديد أولويات المقاطعات وتنفيذها حسب هذه الأولوية حيث أن جميع المقاطعات interrupts والاستثناءات exceptions لها مستويات أولوية معينة. وبشكل عام فإن الاستثناء مماثل للمقاطعة غير أن مستوى الأولوية priority فيه أعلى. أيضاً، يستقبل الـ NVIC المقاطعات أثناء تنفيذ مقاطعة أخرى ويقوم بالمقارنة بينها، فإذا كان مستوى الأولوية للمقاطعة المستقبلة أعلى، سيتم تعليق تنفيذ المقاطعة الحالية وتقديم المقاطعة المستقبلة أولاً.

---

## []() دالة خدمة المقاطعة Interrupt service routine - ISR

يجب أن يكون هنالك لكل مقاطعة دالة مرتبطه بها يتم تنفيذها عند حدوث المقاطعة. ويشار إليها عادةً باسم دالة خدمة المقاطعة Interrupt service routine - ISR. 

عند إنشاء مشروع جديد في Keil µVision IDE سينشئ البرنامج تلقائيًا ملف بدء تشغيل startup file بإسم startup_TM4C123.s. وستجد فيه لكل مقاطعة دالة خدمة مقاطعة ISR وهمية لا تقوم بأي شيء placeholder في Vectors_. 

ولكتابة هذه الدالة، سنحتاج أولاً إلى معرفة اسمها في جدول Vectors_ وإستخدام ذلك الاسم في برنامجنا وبعد ذلك سيقوم الـ linker بإعادة كتابة العنوان الجديد للدالة عند عمل تجميع compile للبرنامج.

---

## []() جدول المقاطعات Interrupt Vector Table - IVT

في صفحة 107 من دليل البيانات بإمكاننا أن نرى جدول المقاطعات والذي يحتوي على عناوين دوال خدمة المقاطعات ISRs:

{% include image.html url="assets/files/article_14/vector_table.png" border="1" %}

ويمكن العثور على هذا جدول وبشكل مفصل في ملف startup_TM4C123.s.

{% include image.html url="assets/files/article_14/__vectors.png" border="1" %}

حيث يمكن استخدام العناصر الموجودة في العمود الثاني كأسماء ISR في البرنامج بينما نستخدم الـ IRQ لإعداد المقاطعة في البرنامج. 

---

## []() تفعيل أو إيقاف المقاطعات بشكل عام

بالإمكان تفعيل أو إيقاف المقاطعات بشكل عام في هيكلة ARM Cortex M بإستخدام الأوامر التالية في لغة الـ Assembly:

* CPSIE I لتفعيل المقاطعات
* CPSID I لإيقاف المقاطعات

ويوفر لنا الكومبايلر في Keil µVision الدوال التالية التي تستدعي الأوامر السابقة:

```c
void __enable_irq(void)  // CPSIE I;
void __disable_irq(void) // CPSID I;
```

ولكن ينقصها أمر لوقف التنفيذ لحينما تحدث مقاطعة. ولذلك سنقوم بإعادة كتابة الدوال السابقة وإضافة دالة التوقف اليها بلغة الـ assembly وإستدعائها من الـ C.

فتكون الدالة المتعلقة بإيقاف المقاطعات:

```c
void interrupts_disable(void){
    __asm ("CPSID  I\n");
}
```

والدالة التي تفعلها:

```c
void interrupts_enable(void){
    __asm  ("CPSIE  I\n");
}
```

والدالة التي توقف التنفيذ لحين حدوث مقاطعة:

```c
void interrupts_wait_for(void){
    __asm  ("WFI\n");
}
```

---

## []() مثال على الإستثناءات

كما ذكرنا سابقاً فإن الاستثناءات مماثلة للمقاطعات ولكن مستوى الأولوية فيها أعلى. وكمثال، سنأخذ الاستثناء الناجم عن مؤقت النظام SysTick. 

ذكرنا في درس سابق، بأن معالج Cortex-M4 يحتوي على مؤقت من 24-bit، والذي يبدأ العد من قيمة إعادة التحميل RELOAD إلى الصفر. وعند الوصول إلى الصفر يقوم مؤقت النظام SysTick بإعادة تحميل القيمة المخزنة في RELOAD وتستأنف العد التنازلي.

ما يهمنا هنا هو أنه عند الوصول إلى الصفر، سوف يرسل المؤقت المقاطعة والتي سنقوم في برنامجنا بالتعامل معها.

في ملف startup_TM4C123.s يتضح لنا أن الـ ISR للـ SysTick هي: 

```c
SysTick_Handler
```

نضيفها الى ملف الـ c الذي أنشأناه كما يلي:

```c
void SysTick_Handler () {

}
```

وسيتم تنفيذ ما بداخل هذه الدالة كلما أرسل الـ SysTick إشارة الى المعالج، أي كلما وصل العداد الى 0. وما نود تنفيذه في هذه الدالة هو تشغيل الإضاءة الحمراء على البورد إن كانت طافئة وإطفاءها إن كانت مضيئة:

```c
void SysTick_Handler () {
  GPIO_PORTF_DATA_R ^= 2; // toggle red led
}
```

أوامر تهيئة المؤقت SysTick مشابهه لما هو موجود في الدرس التاسع ولكن في ذلك الدرس الغينا عمل المقاطعات في سجل STCTRL بينما نفعله هنا:

```c
NVIC_ST_CTRL_R = (1<<1);  // enable SysTick interrupt
``` 

ما يهمنا هنا هو إضافة الأسطر التالية بعد التهيئة:

```c
  // Global enable interrupt
  interrupts_enable(); 
  
  while (1) {
    interrupts_wait_for();
  }
```

حيث أن السطر الأول يفعّل المُقاطعات ثم ندخل في عملية تكرار مستمرة ولكن الأمر interrupts_wait_for يوقف المعالج الى حين حدوث مقاطعة.

وبذلك يكون كامل الكود لدينا:

```c
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"

void portf_config_gpio (void);
void systick_config    (void);

void interrupts_disable  (void);
void interrupts_enable   (void);
void interrupts_wait_for (void);

void SysTick_Handler () {
  // toggle red led
  GPIO_PORTF_DATA_R ^= 2; 
}

int main() {
  portf_config_gpio ( );
  systick_config ( );
    
  interrupts_enable(); 

  while (1) {
    interrupts_wait_for();
  }
}

void portf_config_gpio (void) {
  // enable clock to GPIOF at clock gating control register
  SYSCTL_RCGCGPIO_R |= 0x20;
  
  // enable the GPIO pins for the LED (PF3,2,1) as output
  GPIO_PORTF_DIR_R = 0x0E;

  // disable alt funct on PF3-1
  GPIO_PORTF_AFSEL_R &= ~0x0E; 
  
  // enable the GPIO pins for digital function
  GPIO_PORTF_DEN_R = 0x0E;  
}

void systick_config (void) {
  // disable SysTick during setup
  NVIC_ST_CTRL_R = 0;         
  
  // reload with number of clocks per second
  NVIC_ST_RELOAD_R = 16000000-1; 

  // configure SysTick
  NVIC_ST_CTRL_R = ((1<<1)  |  // 4.1) enable SysTick interrupt
                    (1<<2)  |  // 4.2) use system clock
                    (1<<0));   // 4.3) enable SysTick   
}

void interrupts_disable(void){
    __asm ("CPSID  I\n");
}

void interrupts_enable(void){
    __asm  ("CPSIE  I\n");
}

void interrupts_wait_for(void){
    __asm  ("WFI\n");
}

```

---

## []() مثال عل المقاطعات

في هذا الدرس سوف نعيد كتابة المثال في الدرس الثامن ليستخدم المقاطعات interrupts بدلاً من الإستعلام المتكرر polling. 

وللتذكير فإن البرنامج كان يتعامل مع الأزرار والإضاءات في البورد حيث تختلف الإضاءة حسب الزر المضغوط. 

وفيما يلي جدول يختصر هذه الحالات:

| حالة الأزرار | حالة الإضاءة |
|---:|---:|
| الضغط على SW1 و SW2 معاً |  الإضاءة الحمراء تعمل |
| الضغط على SW1 فقط |  الإضاءة الزرقاء تعمل |
| الضغط على SW2 فقط |  الإضاءة الخضراء تعمل |

حيث أن:

* الزر الأيسر SW1 متصل بالمنفذ F والطرف 4 =>  PF4
* الزر الأيمن SW2 متصل بالمنفذ F والطرف 0 =>  PF0

بالإضافة الى أننا سنتعامل مع الإضاءات الملونة التي تعاملنا معها سابقاً:

* الإضاءة الحمراء  Red LED  متصلة بالمنفذ F والطرف 1 =>  PF1
* الإضاءة الزرقاء  Blue LED  متصلة بالمنفذ F والطرف 2 =>  PF2
* الإضاءة الخضراء  Green LED  متصلة بالمنفذ F والطرف 3 =>  PF3

ونظرًا لأن التهيئة المتعلقة بالـ GPIO قد تمت مناقشتها مسبقاً فإننا هنا سننظر الى تهيئة المقاطعات فقط. 

---

## []() تهيئة المقاطعات

أول ثلاث خطوات في عملية تهيئة المقاطعات تتعلق بتحديد نقطة بدء trigger المقاطعة، حيث أنه لدينا 5 خيارات بالإمكان للمقاطعة أن تعمل عندها:

* المستوى المنخفض  low level
* المستوى العالي high level
* الحافة الصاعدة rising edge من المستوى المنخفض إلى المرتفع
* الحافة النازلة falling edge من المستوى المرتفع إلى المنخفض 
* كلا الحافتين الصاعدة والنازلة

{% include image.html url="assets/files/article_14/signal_edges.png" border="1" %}

وفيما يلي خطوات التهيئة التي يجب علينا إتباعها:

---

__1. تحديد ما إذا كنا سنستخدم المستويات (عالي أم منخفض) أو الحواف (صاعد أم نازل) للإشارة على بدء المقاطعة__

للقيام بذلك نستخدم السجل GPIOIS ص 664.

{% include image.html url="assets/files/article_14/GPIOIS.png" border="1" %}

نحن هنا سنستخدم الحافة النازلة للإشارة للمقاطعة ولذلك نضع 0 في الخانات المقابلة للأزرار (بت 0 للمفتاح SW2 وبت 4 للمفتاح SW1)

```c
GPIO_PORTF_IS_R &= ~((1<<4)|(1<<0));
```

---

__2. تحديد ما إذا كنا نريد للإشارة أن تحدث لكلا المستويين (عالي ومنخفض) أو الحافتين (الصاعدة والنازلة)__

ويكون ذلك عن طريق السجل GPIOIBE ص 665.

{% include image.html url="assets/files/article_14/GPIOIBE.png" border="1" %}

وبما أنا قلنا أننا نريد الحافة النازلة فقط، فإننا نضع 0 في الخانات المقابلة للأزرار (بت 0 للمفتاح SW2 وبت 4 للمفتاح SW1)

```c
GPIO_PORTF_IBE_R &= ~((1<<4)|(1<<0));
```

---

__3. تحديد ما اذا كنا نريد المستوى العالي و الحافة الصاعدة أم المستوى المنخفض و الحافة النازلة__

ويتم ذلك عن طريق السجل GPIOIEV ص 666.

في الحالة الأولى (مستوى عالي أو حافة صاعدة) نضع القيمة 1 في الخانات المقابلة للأزرار (بت 0 للمفتاح SW2 وبت 4 للمفتاح SW1). أما في الحالة الثانية (مستوى منخفض أو حافة نازلة) فإننا نضع 0.

```c
GPIO_PORTF_IEV_R &= ~((1<<4)|(1<<0));
```

---

__4.  مسح أي مقاطعة سابقة متعلقة بالأطراف__

وذلك عن طريق السجل GPIOICR ص 670.

{% include image.html url="assets/files/article_14/GPIOICR.png" border="1" %}

للتأكد من مسح أي مقاطعة سابقة فإننا نضع القيمة 1 في الخانات المقابلة للأزرار (بت 0 للمفتاح SW2 وبت 4 للمفتاح SW1).

```c
GPIO_PORTF_ICR_R |= ((1<<4)|(1<<0));
```

---

__5. تعيين أولوية للمقاطعة__

ويتم ذلك عن طريق سجلات PRIn ص 152.

{% include image.html url="assets/files/article_14/PRIn.png" border="1" %}

تكون سجلات الأولية priority على الشكل التالي:

```c
NVIC_PRIn_R
```

حيث يمثل n رقم المجموعة التى تتحكم بأولويات المقاطعات في هذه المجموعة. ولمعرفة الرقم الذي يجب علينا إستبدال n به فإن أسهل طريقة لتحديد السجل الذي سنستخدمه هو عن طريق فتح دليل البيانات وإستعراض الـ Bookmarks، وتحت التبويب 3. Cortex-M4 Peripherals نختار 3.4. NVIC Register Descriptions:

{% include image.html url="assets/files/article_14/bookmarks.png" border="1" %}

إبحث عن سجلات الأولوية Priority التي تبدأ من سجل 29 وتنتهي بالسجل 63.

إفتح الملف startup_TM4C123.s وإبحث عن GPIOF_Handler. هذه الدالة هي الـ ISR الخاصة بالمقاطعات المتعلقة بالمنفذ F. ما يهمنا هنا هو رقم الـ IRQ لهذه الدالة وهو 30. وكل ما علينا فعله هنا هو البحث عن السجل الذي يشمل المقاطعة رقم 30 وهو السجل رقم 36 حيث يشمل المقاطعات من 28 الى 31.

{% include image.html url="assets/files/article_14/PRI7.png" border="1" %}

أي أنه يجب علينا إستخدام المجموعة رقم 7 وبذلك فإننا نستخدم السجل التالي:

```c
NVIC_PRI7_R
```

وهذه المجموعة السابعة بإمكانها التحكم بأولوية المقاطعات 28 و 29 و 30 و 31 بحيث يكون لكل مقاطعة 3 بتات نحدد فيها أولويتها. أي من 000 الى 111 (ثنائي) أو من 0 الى 7 (عشري):

{% include image.html url="assets/files/article_14/28293031.png" border="1" %}

وبذلك يكون الحقل المتحكم في أولوية المقاطعة رقم 30 هو INTC (بت 23:21).

وسنقوم بإعطاء هذه المقاطعة الأولوية 3:

```c
NVIC_PRI7_R = (3<<21);
```

---

__6. تمكين المقاطعات للمنفذ__

مرة أخرى، عن طريق فتح دليل البيانات وإستعراض الـ Bookmarks، وتحت التبويب 3. Cortex-M4 Peripherals نختار 3.4. NVIC Register Descriptions. وكل ما علينا فعله هنا هو البحث في سجلات ENn عن السجل الذي يشمل المقاطعة رقم 30 وهو السجل رقم 4 أي EN0 حيث يشمل المقاطعات من 0 الى 31.

{% include image.html url="assets/files/article_14/EN0.png" border="1" %}

وتفاصيل السجل موجودة في ص 142 من دليل البيانات:

{% include image.html url="assets/files/article_14/EN0_R.png" border="1" %}

نضع الآن القيمة 1 في الخانة 30 لأنها تقابل IRQ رقم 30.

```c
NVIC_EN0_R |= (1<<30);
```

---

__7. تمكين المقاطعات للأطراف__

ويكون ذلك عن طريق السجل GPIOIM ص 667.

{% include image.html url="assets/files/article_14/GPIOIM.png" border="1" %}

لتمكين المقاطعات للأطراف نضع القيمة 1 في الخانات المقابلة للأزرار (بت 0 للمفتاح SW2 وبت 4 للمفتاح SW1).

```c
GPIO_PORTF_IM_R |= ((1<<4)|(1<<0));
```

---

__8. تمكين المقاطعات بشكل عام__

سنستدعي هنا الدالة التي قمنا بكتابتها interrupts_enable :


```c
interrupts_enable();
```

---

__9. كتابة دالة خدمة المقاطعة ISR__

وجدنا في الملف startup_TM4C123.s إن الـ ISR المسؤول عن المقاطعات الخاصة بالمنفذ F هي GPIOF_Handler. ولذلك ننسخ إسمها ونضعه في ملفنا كالتالي:

```c
void GPIOF_Handler(void) {

}
```

أول ما نفعله هنا هو إزالة إشارة المقاطعة قبل العودة الى التعامل مع المقاطعة. لأننا لو لم نقم بذلك فإن المقاطعة ستظهر كما لو أنها كانت لا تزال معلقة وسيتم تنفيذ دالة خدمة المقاطعة ISR مرة تلو الأخرى وبشكل مستمر الى أن يعلق البرنامج. 

نقوم بإزالة إشارة المقاطعة عن طريق وضع القيمة 1 في الخانات المقابلة للأزرار (بت 0 للمفتاح SW2 وبت 4 للمفتاح SW1).

```c
GPIO_PORTF_ICR_R = ((1<<4)|(1<<0));
```

بعد ذلك نتعامل مع المقاطعة كما نريد، وفي حالتنا هذه نختبر أي الأزرار تم الضغط عليه لإضائة النور المناسب.

```c
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
}  
```

---

__10. الإنتظار في حلقة التكرار الى أن تحدث مقاطعة__

في main، سنستخدم الدالة interrupts_wait_for لوضع المعالج في حالة الإنتظار standby لحين حدوث مقاطعة:


```c
while (1) {
  interrupts_wait_for();
}
```

وبذلك يكون كامل الكود:

```c
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"

#define SW2        (1<<0)
#define LED_RED    (1<<1)		
#define LED_BLUE   (1<<2)		
#define LED_GREEN  (1<<3)		
#define SW1        (1<<4)		

void portf_config_gpio       (void);
void portf_config_interrupts (void);

void interrupts_disable  (void);
void interrupts_enable   (void);
void interrupts_wait_for (void);

int main (void) {
  portf_config_gpio ();
  portf_config_interrupts ();

  while (1) {
    interrupts_wait_for();
  }  
}

void portf_config_gpio (void) {
  volatile unsigned long wait;
  
  // 1) Enable clock for Port F
  SYSCTL_RCGCGPIO_R = (1<<5);  
  wait = SYSCTL_RCGCGPIO_R; 
    
  // 2.1) Unlock PF0
  GPIO_PORTF_LOCK_R = 0x4C4F434B;
  
  // 2.2) Allow changes to PF0
  GPIO_PORTF_CR_R = SW2;
  
  // 3) Disable analog functions
  GPIO_PORTF_AMSEL_R &= ~(SW2|LED_RED|LED_BLUE|LED_GREEN|SW1);
	
  // 4) Set pin direction
  GPIO_PORTF_DIR_R  =  (LED_RED|LED_BLUE|LED_GREEN); // Output
  GPIO_PORTF_DIR_R &= ~(SW1|SW2);                    // Input
  
  // 5) disable alternate functions
  GPIO_PORTF_AFSEL_R &= ~(SW2|LED_RED|LED_BLUE|LED_GREEN|SW1);
  
  // 6) Set pull up resistor
  GPIO_PORTF_PUR_R = (SW1|SW2); 
  
  // 7) Enable pins
  GPIO_PORTF_DEN_R = (SW2|LED_RED|LED_BLUE|LED_GREEN|SW1);
}

void portf_config_interrupts (void) {
  // 1) Make PF4/PF0 edge sensitive
  GPIO_PORTF_IS_R &= ~(SW1|SW2);
  
  // 2) Single edges
  GPIO_PORTF_IBE_R &= ~(SW1|SW2);
  
  // 3) Falling-edge
  GPIO_PORTF_IEV_R &= ~(SW1|SW2);

  // 4) Clear any prior interrupt
  GPIO_PORTF_ICR_R |= (SW1|SW2);
  
  // 5) PortF is given priority 3
  NVIC_PRI7_R = (3<<21); 
    
  // 6) Enable interrupt for PortF (IRQ 30) in NVIC
  NVIC_EN0_R |= (1<<30);
    
  // 7) Enable interrupts for PF4, PF0
  GPIO_PORTF_IM_R |= (SW1|SW2);
  
  // 8) Global interrupts enable
  interrupts_enable();
}

void GPIOF_Handler(void) {
  volatile int readback;
 
  // clear interrupt
  GPIO_PORTF_ICR_R = (SW1|SW2);       
  
  // a read to force clearing of interrupt flag
  readback = GPIO_PORTF_ICR_R;
  
  // toggle LEDs   
  switch (GPIO_PORTF_DATA_R & 0x11) {
    case 0x00: // SW1 & SW2 pressed
      GPIO_PORTF_DATA_R = LED_RED;
      break;
    
    case 0x01: // SW1 pressed only
      GPIO_PORTF_DATA_R = LED_BLUE;
      break;
    
    case 0x10: // SW2 pressed only
      GPIO_PORTF_DATA_R = LED_GREEN;
      break;
  }  
}

void interrupts_disable(void) {
    __asm ("CPSID  I\n");
}

void interrupts_enable(void) {
    __asm  ("CPSIE  I\n");
}

void interrupts_wait_for(void) {
    __asm  ("WFI\n");
}

```


