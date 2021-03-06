---
layout: post
title: 4. المتحكم والبورد  Board & Microcontroller 
last_edit: 2017-10-20
lang: ar-SA
comments: true
---

تعتبر بورد الـ Tiva C Launchpad بيئة مساعدة لبرمجة وتطوير العنصر الرئيسي فيها، وهو المتحكم TM4C123GH6PM حيث أن كل أجزاء البورد موجوده بهدف تسهيل التعامل مع هذا المتحكم. والبورد عبارة عن اضافات مساندة ومساعدة لنا في عملية برمجة المتحكم وإختباره بطريقة أسهل.

## []()بعض مواصفات البورد والمتحكم

{% include image.html url="assets/files/article_04/launchpad_board.png" border="1" %}

تجدر الإشارة الى أن ما بداخل المستطيل الأخضر هو البورد وعندما نتكلم عن المتحكم فإننا نقصد القطعة التي بداخل المستطيل الأصفر وهي القطعة التي يكتب فيها كودنا البرمجي والتي تنفذه.

وسنوضح بعض مواصفات البورد والمتحكم في الجزء التالي حيث سنناقش الأجزاء الرئيسية التي يتكونان منها.


## []()الأجزاء الرئيسية للبورد

{% include image.html url="assets/files/article_04/launchpad-tivac.png" border="1" %}

1. مفتاح إختيار مصدر الطاقة Power select switch. بإمكانك تختار أن يكون مصدر الطاقة من منفذ الـ usb رقم 2 في الصورة بوضع المفتاح على اليمين (وهو الذي سنختاره في هذا الكورس)، أو المنفذ رقم 3 بوضع المفتاح على اليسار. 

2. منفذ usb يوفر لنا خاصية In-Circuit Debug Interface ICDI والذي يمكننا من برمجة وإكتشاف أخطاء debug الكود على المتحكم (8). ولذلك سنختار هذا المنفذ لتوصيل كيبل الـ usb من الجهاز المكتبي الى البورد.

3. منفذ الطاقة من الـ usb وبالإمكان تزويد البورد بالطاقة من هذا المنفذ بالإضافة الى كتابة برامج تتعامل مع هذا المنفذ

4. إضاءة خضراء تعمل عند عمل البورد

5. متحكم TM4C123GH6PM مشابه للمتحكم الذي نقوم ببرمجته (8) ولكننا لا نتعامل معه مباشره. وهو المسؤول عن عملية برمجة programming واكتشاف الأخطاء في البرنامج debugging الذي نضعه على المتحكم الرئيسي (8)

6. زر التشغيل Reset button. نضغط عليه بعد برمجة المتحكم ليعمل البرنامج الجديد.

7. إضاءة ملونة RGB LED
* الإضاءة الحمراء متصلة بالطرف PF1 أي منفذ Port F والطرف Pin 1
* الإضاءة الزرقاء متصلة بالطرف PF2 أي منفذ Port F والطرف Pin 2
* الإضاءة الخضراء متصلة بالطرف PF3 أي منفذ Port F والطرف Pin 3 

8. المتحكم TM4C123GH6PM  (وهي القطعة السوداء في المنتصف):
* من نوع ARM Cortex-M4 
* سرعة المعالج 80MHz
* ذاكرة وصول عشوائي RAM بسعة 32kilobyte
* ذاكرة تخزين ROM بسعة 256kilobyte
* عدد الأطراف pins يساوي 43
* تدعم الـ floating point numbers  على مستوى الهاردوير

9. المفتاح SW1 أي Switch 1 متصل بالطرف PF4 أي منفذ Port F والطرف Pin 4

10. المفتاح SW2 أي Switch 2 متصل بالطرف PF0 أي منفذ Port F والطرف Pin 0

<blockquote class="note">
<p>فيما يتعلق بالمنافذ ports والأطراف pins فإنه سيتم شرحها في درس لاحق بإذن لله</p>
</blockquote>

ومن المهم التنويه مرة أخرى على أن أهم ما في البورد هو المتحكم الذي في منتصف البورد (8) والباقي عبارة عن وسائل مساعدة لبرمجة المتحكم واكتشاف أخطاء البرنامج عليه ومجموعة من المفاتيح switches وإضاءات Leds تسهل علينا إختبار البورد.

وفي العادة فإن بورد الـ Tiva C Launchpad تعتبر بورد تطوير development board مفيدة لتعلم برمجة المتحكم وبالإمكان أيضاً إستخدامها في مشاريع الهواة، أما في حالة تطوير منتج إحترافي يمكن تسويقه فإننا نستخدم المتحكم بشكل مستقل من دون بورد، كما يظهر لنا في الصورة التاليه:

{% include image.html url="assets/files/article_04/TM4C123GH6PM.jpg" border="1" %}

ونستخدمها في بورد printed circuit board - pcb  خاصة بنا ومن تطويرنا:

{% include image.html url="assets/files/article_04/pcb.jpg" border="1" %}


## []()تحميل دليل البيانات
دليل البيانات datasheet عبارة عن مستند أساسي ومهم يوضح مكونات القطعة الإلكترونية ومواصفاتها، والى حد ما كيفية إستخدامها. ويمكن تحميل الدليل الخاص بالمتحكم من الرابط المباشر:

<http://www.ti.com/lit/gpn/tm4c123gh6pm>

أو عن طريق الذهاب الى الصفحة التالية :

<http://www.ti.com/product/tm4c123gh6pm>

والضغط على الرابط الموضح في الصورة:

{% include image.html url="assets/files/article_04/datasheet_download_link.png" border="1" %}

