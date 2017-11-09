---
layout: post
title: 9. مؤقت النظام  SysTick Timer
lang: ar-SA
comments: true
---

يسعدنا مؤقت النظام SystTick في حساب الوقت والتعامل مع بعض الإجراءات التي تحتاج الى الوقت للتحكم بها وتفيذ بعض الوظائف كلما مرت فتره معينة محددة مسبقاً. وبالإمكان إستخدام هذا المؤقت لو كان لديك نظام تشغيل مثلاً يتنقل في التنفيذ بين أكثر من thread. وسيناريو آخر لإستخدامها هو لقياس المدة التي يستغرقها الكود في التنفيذ.



يتكون هذا المؤقت من 24 بت يتناقص العدد الذي تحتويه الى أن تصل الى 0 ومن ثم يعاد العد من أول وجديد. 

يتكون المؤقت من 3 سجلات:
* **STCTRL** أو SysTick Control and Status وتستخدم لتهيئة المؤقت ولتفعيله والتحكم بالقيمة التي يبدأ العداد بالتناقص منها.
* **STRELOAD** أو SysTick Reload Value شرح 
* **STCURRENT** أو SysTick Current Value وتحتوي على القيمة الحالية للعداد 

## []()1. السجل STCTRL
وما يهمنا في هذا السجل البتات bits التالية:
* bit 0 وهي الـ Enable والتي تقوم بتفعيل عمل المؤقت
* bit 1 وهي الـ Interrupt Enable والتي تفعّل الـ interrupts (المزيد عنها لاحقاً بإذن الله)
* bit 2 وهي الـ Clock Selection 
* bit 16 وهي الـ Flag ويتم تصفيرها (جعلها صفر) كلما تناقص العداد ووصل للصفر ثم عاد للعدد الأصلي الذي إنشائه عليه 

## []()2. السجل STRELOAD
نستخدم أول 24 بت فقط (من بت 0 الى 23) من هذا السجل لكتابة قيمة العدد الذي نرغب من المؤقت أن يبدأ بالتناقص منه. 

## []()3. السجل STCURRENT
وهذا هو العداد ويحتوي على القيمة الحالية للعداد. ومثل السجل السابق، نستخدم أول 24 بت فقط من هذا السجل. العداد تنازلي أي يعد من العدد الأصلي الذي تم إسناده اليه ويتناقص بمقدار واحد الى أن يصل 0. بعد ذلك يسند 1 الى بت 16 في سجل الـ STCTRL ثم يأخذ القيمة الموجودة في سجل الـ STRELOAD وينسخها الى سجل الـ STCURRENT. 


The timer consists of three registers:
■ SysTick Control and Status (STCTRL): A control and status counter to configure its clock,
enable the counter, enable the SysTick interrupt, and determine counter status.
■ SysTick Reload Value (STRELOAD): The reload value for the counter, used to provide the
counter's wrap value.
■ SysTick Current Value (STCURRENT): The current value of the counter.



The RELOAD register is used to set the period it takes for the timer expire. 


In order to determine the amount of time that has passed between the initial load  and the timer reaching zero, we need to know the frequency at which the clock decrements.  The frequency at which the clock decrements is simply the clock frequency driving the clock peripheral.




The System Tick Time (SysTick) generates interrupt requests on a regular basis. This allows an OS to carry out context switching to support multiple tasking. For applications that do not require an OS, the SysTick can be used for time keeping, time measurement, or as an interrupt source for tasks that need to be executed regularly.




