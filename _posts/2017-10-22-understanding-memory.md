---
layout: post
title: 5. التعامل مع الذاكرة  Understanding Memory
lang: ar-SA
comments: true
---

يوجد لدينا عدة أنواع من الذاكرة على النظام، أهمها:
* **ROM** أو Flash memory ومساحتها 256KB. ومن الممكن أن يكون لدينا أكثر من ذاكرة ROM على النظام المدمج. وهذا الجزء الذي يحفظ فيه الكود code والثوابت constants. والبيانات المحفوظه في هذه الذاكره لا تمسح في حالة إعادة تشغيل المتحكم non-volotile. 

* **RAM** ومساحتها 32KB. وهنا تحفظ البيانات data او المتغيرات variables أثناء عمل برنامجنا بالإضافة الى الـ stack. والبيانات في هذا النوع من الذاكرة تفقد في حالة إعادة تشغيل المتحكم volotile.

* **I/O Ports** نشير الى أطراف الـ Input/Output بطريقة تسمى memory mapped IO وهي تعني أنه عندما نريد أن نتعامل مع منفذ IO فإنه كل الذي علينا عمله هو قرائة أو كتابة مكان مخصص في الذاكرة كل bit فيها يشير الى طرف pin في المنفذ.

{% include image.html url="assets/files/article_05/mmap.png" description="Memory Map" border="1" %}

نلاحظ وجود بعض الفراغات بين الأنواع المختلفة للذاكرة وهي غير مستخدمة ولا يمكن الإشارة اليها ومن الخطأ القيام بذلك.

ولمزيد من التفاصيل حول الأجزاء المختلفة للذاكرة فإنه يمكن العودة الى جزئية الـ memory model في دليل البيانات data sheet ص 92.

ومن الجميل جداً أنه من الممكن التعامل مع هذه الأنواع المختلفه من الذاكرة كجدول موحد والرجوع الى كل منها حسب عنوان 32bit address. وفيما يلي عناوين الذاكرة:
* **ROM** من 0x00000000 الى 0x0003FFFF
* **RAM** من 0x20000000 الى 0x20007FFF
* **I/O Ports** من 0x40000000 الى 0x400FFFFF

تجدر الإشارة الى أنه من المتعارف عليه التعبير عن عناوين الذاكرة بالأعداد الست عشرية hexadecimal numbers بدلاً من الأعداد العشرية decimal numbers وأحد أسباب ذلك هو للإختصار. فبدلاً من القول بأن الذاكرة تبدأ من العنوان 536870912 كعدد عشري، فمن الأوضح والمختصر القول بأنها تبدأ من العدد الست عشري 0x20000000.

## []()طريقة حساب حجم الذاكرة
لو أخذنا ذاكرة الـ RAM على سبيل المثال، فقد قلنا أن حجمها 32KB، فكيف توصلنا الى هذه النتيجة؟ يمكن معرفة حجم الذاكرة اذا توفر لديك عنواني البداية والنهاية بإتباع المعادلة البسيطة التالية:

<blockquote class="rule">
<p>((عنوان النهاية  -  عنوان البداية) /  1024)  +  1  =  ؟؟  كيلوبايت</p>
</blockquote>

مع ملاحظة أنه تم القسمة على 1024 للتحويل من بايت byte الى كيلوبايت kilobyte حيث أن التحويل بين الأحجام يكون كما يلي:

```
1 KB = 2^10 bytes = 1024 bytes
1 MB = 2^20 bytes = 1,048,576 bytes
1 GB = 2^30 bytes = 1,073,741,824 bytes
```

وكما ذكرنا سابقاً، فإن عنوان الـ RAM يبدأ من 0x20000000 وينتهي في 0x20007FFF، وبالتعويض في القاعدة تصبح:

```
((0x20007FFF - 0x20000000) / 1024) + 1 = ?
```
ومن الممكن إستخدام الآلة الحاسبة في ويندوز للقيام بالعمليات الحسابية على الأعداد الست عشرية بفتح خصائص الآلة الحاسبة وإختيار نمط المبرمج Programmer كما يلي:

{% include image.html url="assets/files/article_05/calc1.png" border="1" %}

ثم الضغط على الصف الذي يحتوي على عبارة Hex:

{% include image.html url="assets/files/article_05/calc2.png" border="1" %}

وقم بالعملية الحسابية على الأعداد الست عشرية بدون إضافة 0x في المقدمة:

{% include image.html url="assets/files/article_05/calc3.png" border="1" %}

ستظهر لك النتيجة التالية:

```
7FFF
```

اضغط الآن على الصف الذي يحتوي على عبارة DEC لنتعامل مع التمثيل العشري للعدد الناتج:


{% include image.html url="assets/files/article_05/calc4.png" border="1" %}

هذه النتيجه بالبايت byte، لتحولها الى كيلوبايت kilobyte قُم بالقسمة على 1024 ليصبح الناتج 31، ثم نضيف بعد ذلك 1 لتصبح 32 وبذلك نجد أن مساحة ذاكرة الـ ROM تساوي 32 كيلوبايت.


## []()التعامل مع الذاكرة

مختلف عناوين الذاكرة التي يمكن التعامل معها تسمى address space وفي معمارية الـ  ARM Cortex-M نشير اليها بإستخدام 32bits تبدأ من 0x00000000 وتنتهي في 0xFFFFFFFF بما مجموعه 4GB.  

فلو أردنا مثلاً التعامل مع العنوان التالي في الذاكرة 0x20000004 وهو جزء من الذاكرة العشوائية RAM، فإن طريقة الإشارة اليها بلغة الـ C يكون بإستخدام مؤشر pointer كما يلي:

```
unsigned long *p = (unsigned long *)0x20000004U;
```

وكان من اللازم علينا إضافة **(* unsigned long)** ، وهذه الطريقة تسمى type casting، وإلا فإن المترجم compiler سيعتبرها رقماً عاديا وليس عنواناً في الذاكرة. وأضفنا **U**  في نهاية العنوان لأن عناوين الذاكرة من نوع unsigned integer.

وتجدر الإشارة الى أن الـ long في معمارية الـ ARM Cortex-M عبارة عن 32bit integer وكان بالإمكان إستخدام int بدلاً منها حيث أنها هي ايضا 32bit ولكن ينصح بعدم القيام بذلك حيث أنه ليس من المؤكد لو أخترت مترجم compiler مختلف عن الـ Keil compiler الذي نستخدمه أنه سيقوم بتمثيل الـ int بـ 32bit.

ولو أردت أن تتأكد بأن المؤشر p فعلاً يشير الى العنوان المسند اليه فإنه بإمكانك إستخدام الكود التالي:

```
printf ("%p", p);
```

وإذا أردنا إسناد قيمة لمكان الذاكرة الذي يشير اليه المؤشر نقول:

```
*p = 0xCAFED00D; // hexadecimal number, or
*p = 3405697037; // decimal number, or
*p = 0b11001010111111101101000000001101; // binary number
```
وللوصول الى القيمة التي يشير اليها المؤشر فإننا نقوم بعملية derefrencing للمؤشر كما يلي:

```
printf ("%d", *p);
```