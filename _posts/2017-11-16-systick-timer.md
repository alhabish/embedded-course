---
layout: post
title: 9. مؤقت النظام  SysTick Timer
lang: ar-SA
comments: true
---

يساعدنا مؤقت النظام SystTick في حساب الوقت والتعامل مع بعض الإجراءات التي تحتاج الى الوقت للتحكم بها ولتنفيذ بعض الوظائف كلما مرت فتره معينة محددة مسبقاً. وبالإمكان إستخدام هذا المؤقت لو كان لديك نظام تشغيل مثلاً يتنقل في التنفيذ بين أكثر من thread. وسيناريو آخر لإستخدامها هو لحساب الزمن ولقياس المدة التي يستغرقها الكود في التنفيذ. 

يوجد في المتحكم مؤقتات أخرى غير الـ SysTick بعضها يدعم 32 بت وأخرى 64 بت. وبالرغم أن SysTick الأقل دقة بينهم حيث أنه لا يدعم سوى 24 بت إلا أنه الأسهل في التعامل بالإضافة الى أنه المؤقت الوحيد المعتمد في ARM Cortex-M وبذلك فلو أردت تغير المتحكم هذا واستخدام متحكم آخر من نفس العائلة فإنك لن تحتاج الى تغييره بينما المؤقتات الأخرى خاصة بكل نوع متحكم على حده. 

ويتكون هذا المؤقت من 24 بت (أي أن أقصى رقم يمكن إسناده اليه هو 0xFFFFFF) تتناقص مع كل clock tick الى أن تصل الى 0 ومن ثم يعاد العد مرة أخرى. 

---
# []()السجلات المتعلقة بهذا المؤقت

{% include image.html url="assets/files/article_09/systick_registers.png" border="1" %}

يتكون المؤقت من 3 سجلات:
* **STCTRL** أو SysTick Control and Status وتستخدم لتهيئة المؤقت ولتفعيله والتحكم به.
* **STRELOAD** أو SysTick Reload Value ويسند اليه القيمة التي يبدأ العداد بالتناقص منها.
* **STCURRENT** أو SysTick Current Value وتحتوي على القيمة الحالية للعداد. 

## []()1. السجل STCTRL

تفصيل هذا السجل في دليل البيانات data sheet ص 138:

{% include image.html url="assets/files/article_09/stctrl.png" border="1" %}

وما يهمنا في هذا السجل البتات bits التالية:
* __bit 0__ وهي الـ Enable والتي تقوم بتفعيل عمل المؤقت
* __bit 1__ وهي الـ Interrupt Enable والتي تفعّل المقاطعات interrupts (المزيد عنها لاحقاً بإذن الله)
* __bit 2__ وهي الـ Clock Selection وفي العادة نختار الساعة القياسية هنا
* __bit 16__ وهي الـ  Count Flag وتكون 1 كلما تناقص العداد ووصل للصفر ثم عاد للعدد من جديد 

## []()2. السجل STRELOAD

في دليل البيانات ص 140 نجد تفاصيل السجل:

{% include image.html url="assets/files/article_09/streload.png" border="1" %}

نستخدم أول 24 بت فقط (من بت 0 الى 23) من هذا السجل لكتابة قيمة العدد الذي نرغب من المؤقت أن يبدأ بالتناقص منه. 

## []()3. السجل STCURRENT

في دليل البيانات ص 141 نجد:

{% include image.html url="assets/files/article_09/stcurrent.png" border="1" %}

يحتوي هذا السجل على القيمة الحالية للعداد. ومثل السجل السابق، نستخدم أول 24 بت فقط من هذا السجل أي أن قيمة العداد لا يمكن أن تزيد عن 0xFFFFFF. العداد تنازلي أي أنه يعد من العدد الأصلي الذي تم إسناده اليه ويتناقص بمقدار 1 الى أن يصل 0 بمقدار سرعة الساعة المختارة. بعد ذلك يسند 1 الى بت 16 في سجل الـ STCTRL ثم يأخذ القيمة الموجودة في سجل الـ STRELOAD مرة أخرى وينسخها الى سجل الـ STCURRENT.

---
# []()تهيئة المؤقت

هنالك عدة خطوات للقيام بذلك، وهي:

1. __إيقاف عمل المؤقت بينما نقوم بتهيئته__
* في السجل STCTRL نجعل bit 0 وهي الـ Enable تساوي 0 لإيقاف الـ SysTick بينما نقوم بعملية التهيئة.

2. __إسناد قيمة إبتدائية__
* نقوم بإسناد قيمة يستخدمها المؤقت ليبدأ عملية التناقص منها. وذلك يكون عن طريق السجل STRELOAD.

3. __تصفير العداد__
* نقوم بكتابة أي قيمة الى STCURRENT لجعلها تساوي 0.

4. __تفعيل العداد__
* وذلك يكون عن طريق التعديل على سجل STCTRL بكتابة ما يلي:
  * 0 في bit 1 وهي الـ Interrupt bit لإلغاء الـمقاطعات interrupts أو 1 لتفعيلها
  * 1 في bit 2 وهي الـ Clock Source bit لنختار أن يكون مصدر المؤقت هو الساعة القياسية على البورد التي يستخدمها المعالج cpu وجميع الطرفيات peripherals والتي تبلغ سرعتها 16MHz
  * 1 في bit 0 وهي الـ Enable bit لتفعيل العداد

بعد ذلك سيبدأ المؤقت في العد التنازلي بإنقاص العدد الموجود في سجل الـ STCURRENT بمقدار 1 في كل دورة للساعة clock cycle وكتابة القيمة هذه مجدداً الى السجل ذاته. وبما أن الساعة التي أخترناها سرعتها 16MHz فذلك يعني أن تنفيذ أي أمر instruction - وهي إنقاص القيمة بمقدار 1 هنا - ستستغرق 62.5ns

```
1/16000000 = 0.0000000625 seconds = 62.5 nanoseconds
```

وبما أن حجم العداد هو 24 بت فإن أقصى عدد يمكن إستخدامه للعداد هو 0x00FFFFFF.
فلو أخترنا أن يبدأ العداد من الرقم 16,000,000 فالذي علينا القيام به هو إسناد هذه القيمة الى السجل RELOAD أولاً ثم عندما نطلب من العداد البدء فإن المتحكم ينسخ قيمة السجل السابق ويضعها في السجل STCURRENT. وبما أن سرعة الساعة 16MHz فذلك يعني أن العداد سيبدأ من هذا الرقم ويتناقص رقماً كل 62.5ns الى أن يصل الى 0 في مدة زمنية قدرها ثانية واحدة. وحينما يصل العداد الى صفر يقوم المتحكم بنسخ القيمة الموجودة في STRELOAD الى السجل STCURRENT وكتابة 1 الى bit 16 في STCTRL  ليوضح بأننا سنقوم بالعد مرة أخرى ثم القيام بالعد من جديد.

## []()ملاحظة

* اذا أعاد اليك الـ Count Flag القيمة 0 فذلك يعني أن العداد لم يصل الى الصفر منذ آخر مرة تم فيها قراءة هذا البت
* إذا أعاد اليك الـ Count Flag القيمة 1 فإنه يعني بأن العداد وصل الى الصفر منذ آخر مرة تم فيها قراءة هذا البت وبعد هذه القرائة سيصبح 0
* كتابة أي قيمة الى سجل STCURRENT سيقوم بتصفير العداد وتصفير البت 16 في سجل الـ STCTRL

---
# []()برنامج لقياس سرعة التنفيذ

فيما يلي برنامج يستفيد من المؤقت SysTick لقياس المدة الزمنية التي يستغرقها جزء من البرنامج في التنفيذ:

```c

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
  
  STCTRL_R   |= ((0U << STCTRL_BIT_INTEN)  |  // 4.1) disable interrupts
                 (1U << STCTRL_BIT_CLKSRC) |  // 4.2) enable core bus clock
                 (1U << STCTRL_BIT_ENABLE));  // 4.3) enable SysTick 
  
  volatile unsigned long current_time;  
  volatile unsigned long last_time;     
  volatile unsigned long elapsed_time;  
  volatile unsigned long sum = 0;
  
  for (int i = 0; i < 1000000; i++) {
    current_time = STCURRENT_R;
    sum ++;
    elapsed_time = (last_time-current_time) & 0x00FFFFFF; // 24-bit difference
    last_time = current_time;
  }
  
  return 0;
}

```

أو قم بنسخه من الرابط التالي:

<https://github.com/alhabish/embedded-course/blob/gh-pages/assets/files/article_09/measure_elapsed_time.c>

يجب التنويه الى أن القراءة الأولى ستكون خاطئة لأننا نقيس الوقت بناءاً على القياسات السابقة وفي هذه الحالة لن يكون هناك قياس سابق للزمن.

وما نقوم به في السطر التالي:

```
elapsed_time = (last_time-current_time) & 0x00FFFFFF; // 24-bit difference
```

هو لأن المتغيرات current_time و last_time وناتج عملية إنقاصهم من بعض هي قيم من نوع 32 بت ولكننا نحتاج الى أول 24 بت منها فقط لأن المؤقت من نوع 24 بت.

---
# []()برنامج لإيقاف التنفيذ بشكل دقيق Precise Delays
 
في برامجنا السابقة كتبنا دالة delay لإيقاف تنفيذ البرنامج لمدة زمنية عشوائية. ولكن اذا ما أردنا الإنتظار لمدة زمنية محددة وبشكل دقيق فإننا نستخدم الـ systick كما هو في البرنامج التالي:

```c

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
```

أو قم بنسخه من الرابط التالي:

<https://github.com/alhabish/embedded-course/blob/gh-pages/assets/files/article_09/systick_accurate_delay.c>

أرجوا التنويه الى أنه في السطر التالي:

```
STRELOAD_R = (period - 1) & 0x00FFFFFF; // 24-bit difference
```   

نسند القيمة - 1 الى STRELOAD حيث أن العداد يعد الى أن يصل الى 0 وليس الى 1.

---
# []()برنامج لتشغيل/إيقاف تشغيل الإضاءة بشكل دوري

البرنامج التالي يستخدم SysTick لتشغيل الإضاءة الحمراء لمدة ثانية وإطفائها في الثانية التي تليها والإستمرار على ذلك. وبما أن سرعة الساعة المختارة هي 16MHz فإنه في كل ثانية سيكون لدينا 16,000,000 ذبذبة، وبما أن العداد يتناقص الى الصفر وليس الى الواحد فإننا نقوم بإسناد هذا الرقم ناقص واحد الى سجل الـ STRELOAD.


```c

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

```

أو قم بنسخه من الرابط التالي:

<https://github.com/alhabish/embedded-course/blob/gh-pages/assets/files/article_09/periodic_toggle_led.c>

---
