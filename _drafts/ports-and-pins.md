---
layout: post
title: المنافذ والطرفيات
lang: ar-SA
comments: true
---

لهذا المتحكم 6 منافذ ports ولكل منفذ مجموعة من الأطراف pins وهي كما يلي: 
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

يمكن تحميل دليل البيانات datasheet من الرابط المباشر:

<http://www.ti.com/lit/gpn/tm4c123gh6pm>

أو عن طريق الذهاب الى الصفحة التالية :

<http://www.ti.com/product/tm4c123gh6pm>

والضغط على الرابط الموضح في الصورة:

{% include image.html url="assets/files/ports-and-pins/datasheet_download_link.png" border="1" %}


وفي الفصل العاشر chapter 22 من دليل البيانات datasheet (ص 1328) نجد الرسم التخطيطي للأطراف في المتحكم. 
{% include image.html url="assets/files/ports-and-pins/pin_diagram.png" border="1" %}

أرجو التنويه الى أنه يوجد مسارين لنقل البيانات المتعلقة بالـ Input/Output ports وهي:
* Advanced Peripheral Bus - **APB**
* Advanced High-Performance Bus - **AHB**

الأول يعتبر أبطأ ولن هو الذي سيتم التعامل معه حيث أنك لو أرد مواصلة تعلمك على هذا المتحكم فإنك ستجد أغلب الأكواد تتعامل مع هذا المسار.


### []()عناوين المنافذ في الذاكرة على المسار APB
* **Port A**: من 0x4000.4000 الى 0x4000.4FFF
* **Port B**: من 0x4000.5000 الى 0x4000.5FFF
* **Port C**: من 0x4000.6000 الى 0x4000.6FFF
* **Port D**: من 0x4000.7000 الى 0x4000.7FFF
* **Port E**: من 0x4002.4000 الى 0x4002.4FFF
* **Port F**: من 0x4002.5000 الى 0x4002.5FFF

{% include image.html url="assets/files/ports-and-pins/gpio_memory_map.png" border="1" %}

نلاحظ أنه لكل منفذ مساحة 4kbyte من الذاكرة حيث أنها تحتوي على سجلات registers عديدة للتحكم بالمنفذ.



The ARM microcontrollers use 4 bytes of memory space for 8-bit peripheral I/O ports.

