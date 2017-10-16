---
layout: post
title: (5) الذاكرة  Memory
lang: ar-SA
comments: true
---

يوجد لدينا عدة أنواع من الذاكرة على النظام:
* ROM أو Flash memory ومن الممكن أن يكون لدينا أكثر من ذاكرة ROM على النظام المدمج. وهذا الجزء الذي يحفظ فيه الكود code والثوابت constants. وحجمه 256kbyte
* RAM وهنا تحفظ البيانات data او المتغيرات variables أثناء عمل برنامجنا بالإظافة الى الـ stack (والذي سنشرحه في درس لاحق بإذن الله)  وحجمه 32kbyte
* IO Ports نشير الى أطراف الـ IO بطريقة تسمى memory mapped IO وهي تعني أنه عندما نريد أن نشير الى منفذ IO فإنه كل الذي علينا عمله هو قرائة أو كتابة مكان مخصص في الذاكرة وكل bit فيها يشير الى طرف pin في المنفذ.

نلاحظ وجود بعض الفراغات بين الأنواع المختلفة للذاكرة وهي غير مستخدمة ولا يمكن الإشارة اليها ومن الخطأ القيام بذلك.

ومن الجميل جداً أنه من الممكن التعامل مع هذه الأنواع المختلفه من الذاكرة كجدول موحد والرجوع الى كل منها حسب عنوان 32bit address وفيما يلي عناوين الذاكرة:
* ROM من 0x00000000 الى 0x0003FFFF
* RAM من 0x20000000 الى 0x20007FFF
* IO   من 0x40000000 الى 0x400FFFFF

{% include image.html url="assets/files/article_05/memory_map.png" border="1" %}



مختلف عناوين الذاكرة التي يمكن التعامل معها تسمى address space وفي معمارية الـ ARM ISA نشير اليها بإستخدام 32bits وتبدأ من 0x00000000 وتنتهي في 0xFFFFFFFF. كل عنوان من هذه العناوين المختلفة يحتوي على 8bits من المعلومات أي 1byte. فبو تعاملت مع العنوان التالي في الذاكرة مثلاً، 0x00000004 فإنه من الممكن أن يحتوي على المعلومات التاليه: 0010100100


{% include image.html url="assets/files/article_05/memory_map_layout.png" border="1" %}

memory model in datasheet 2.1




1 KiB = 2^10 bytes = 1024 bytes
1 MiB = 2^20 bytes = 1,048,576 bytes
1 GiB = 2^30 bytes = 1,073,741,824 bytes

In the c language,addresses can be stored inside variables called pointers, 

`int *p = &nnnn;
`*p  <- derefrence


`p = 0x20000002U  <- address (does not work)
`p = (int *)0x20000002U <- works
`*p = 0xDEADBEEF;



ARM Cortex M4F
256kb of ROM
32kb of SRAM

The TM4C123GH6PM chip has 256K bytes (256KB) of on-chip Flash memory for code, 32KB of on-chip
SRAM for data, and a large number of on-chip peripherals


the ARM has 4GB (Giga bytes) of memory space. It also uses memory mapped I/O
meaning the I/O peripheral ports are mapped into the 4GB memory space.






