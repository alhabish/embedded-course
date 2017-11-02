---
layout: post
title: "2. تنصيب البرامج اللازمة"
lang: ar-SA
comments: true
---

تختلف برمجة الأنظمة المدمجة عن أنواع البرمجة الأخرى المختلفة مثل برمجة مواقع الإنترنت والبرامج المكتبية وبرامج الجوالات حيث أنها ترتبط إرتباط مباشر بالهاردوير الذي سيعمل عليها هذا البرنامج ولذا وجب على المبرمج أن يكون على معرفة تامة بكيفية عمل والتعامل مع هذا الهاردوير بالإضافة الى أي قطعة هاردوير أخرى قد يتعامل معها هذا البرنامج. وبذلك فإنه يستحيل العمل عليها دون الرجوع الى المستندات المتعلقة بها والموفرة من الشركة المصنعة وبخاصة الـ datasheet والتي سنناقشها بتفصيل أكبر في مقالة مقبلة بإذن الله.

وعند العمل على هذه البرمجيات وتطويرها فإن ذلك يكون على جهاز مكتبي أو لابتوب مبني على هيكلة x64 Instruction Set Architecture (ISA) على سبيل المثال ولكنه ينفذ executed ويصحح debugged على هيكلة مختلفة ARM Cortex M مثلا، وهذا ما يسمى بالـ cross compiling. ولغة البرمجة المستخدمة لبرمجة الأنظمة المدمجة في العادة  هي C والى حد ما Assembly و ++C.  وأهم أسباب ذلك يعود لسرعتها في التنفيذ وتوفر الأدوات البرمجية toolchains لها.

### []()البرامج التي سنقوم بتحميلها
* **Keil uVision IDE** بيئة التطوير التي سنستخدمها بإذن الله في برمجة المتحكم
* **Stellaris ICDI Drivers** برنامج تشغيل driver يمكّن أجهزتنا المكتبية من التخاطب مع المتحكم
{% comment %}
* **TivaWare Development Kit** مجموعة من المكتبات البرمجية المساعدة في عملية التطوير
* **ucon** طرفية terminal تمكننا من ارسال أوامر اكتشاف الأخطاء debugging للمتحكم واستقبال المعلومات منها عبر منفذ UART0
{% endcomment %}

### []()Keil uVision IDE
يوجد لدينا عدة بيئات تطوير Integrated Development Environment (IDE) يمكن الأختيار فيما بينها لبرمجة الـ TI TM4C123GXL LaunchPad. هذه البرامج توفر لنا إمكانية إدارة وتعديل الكود البرمجي، اكتشاف الأخطاء debugging، تحويله الى برنامج compile ليتمكن المتحكم من تشغيله، كتابة البرنامج على البورد flash، ومحاكاة طريقة عمل البرنامج على البورد simulation.

سنقوم بتحميل النسخة المجانية من Keil uVision وهي نسخة للمشاريح الصغيرة المحدودة والتي لا يتعدى حجم الكود فيها 32 kbyte. نقوم بذلك بالذهاب أولاً الى الرابط التالي وتعبئة النموذج:
<https://www.keil.com/demo/eval/arm.htm>

{% include image.html url="assets/files/article_02/keil_registration_form.png" border="1" %}

بعد تعبئة النموذج والضغط على Next سيتم توجيهك الى صفحة التحميل. 

{% include image.html url="assets/files/article_02/mdk_arm_download.png" border="1" %}

قم بالضغط على الرابط MDK524A.EXE لتحميل البرنامج.

عند إكتمال تحميل البرنامج، قم بالنقر مرتان على الملف mdk524a.exe لتشغيله وإبتداء عملية التنصيب وإستمر في الخطوات المعتادة.

{% include image.html url="assets/files/article_02/uvision_splash_screen.png" border="1" %}

عند الإنتهاء وفتح البرنامج لأول مرة سترى نافذة الـ Pack Installer لتحميل الملفات المتعلقة بالبورد Device Family Pack. 

{% include image.html url="assets/files/article_02/pack_installer.png" border="1" %}

وفي حالة إنك لم تراها فبإمكانك فتحها عن طريق الضغط على الزر التالي:

{% include image.html url="assets/files/article_02/pack_installer_button.png" border="1" %}

في نفس النافذه وفي تبويب Devices على اليسار، إختر Texas Instruments ثم Tiva C Series ثم TM4C123x Series

{% include image.html url="assets/files/article_02/pi_tivac_tm4c123x.png" border="1" %}

في تبويب الـ Packs على اليمين، إضغط على الزر بجانب Keil::TM4C_DFP وبجانب ARM::CMSIS

{% include image.html url="assets/files/article_02/pi_packs.png" border="1" %}

سيبدأ الـ Pack Installer في تحميل الملفات المطلوبة

{% include image.html url="assets/files/article_02/pi_packs_output.png" border="1" %}

تأكد من أنه تم تحميل جميع الملفات بشكل كامل قبل إغلاق uVision.


### []()Stellaris ICDI Drivers
سنقوم الآن بتحميل برنامج التشغيل driver والذي يمكننا من التخاطب مع المتحكم، وذلك عبر الرابط التالي:

<http://www.ti.com/tool/stellaris_icdi_drivers>

يمكننا تحميل برنامج التشغيل عن طريق الضغط على الزر التالي والموجود في أعلى الصفحة:

{% include image.html url="assets/files/article_02/icdi_driver_download.png" border="1" %}

قم بفك ضغط الملف spmc016a.zip بعد إكتمال تحميله لتحصل على المجلد stellaris_icdi_drivers.

وفي أسفل الصفحة السابقة، يمكن تحميل ملف pdf يحتوي على خطوات التحميل والتنصيب.

{% include image.html url="assets/files/article_02/icdi_driver_guide.png" border="1" %}




فيما يلي ساذكر بإذن الله خطوات التنصيب بإختصار، ولمزيد من التفصيل يمكن الرجوع الى مستند الـ pdf الذي تم تحميله سابقاً spmu287c.pdf.

أولاً، لتهيئة البورد بشكل صحيح، قم بتوصيل كيبل الـ usb في المنفذ المحدد في الصورة التالية. أيضاً، قم بتحريك زر الـ Power mode الى اليمين ليصبح على وضعية الـ Debug.

{% include image.html url="assets/files/article_02/tivac_setup.png" border="1" %}

بعد قيامك بتوصيل الطرف الآخر لكيبل الـ usb بجهاز الحاسب، قم بفتح نافذة **إدارة الأجهزة** Device Manager في وندوز. وفي قسم الـ **أجهزة أخرى** Other Devices سترى ما يلي:

{% include image.html url="assets/files/article_02/other_devices.png" border="1" %}

أضغط عليها باليمين وأختر **تحديث برنامج التشغيل...** Update Driver Software

{% include image.html url="assets/files/article_02/other_devices_update.png" border="1" %}

وفي النافذة الجديدة إختر **إستعراض الكمبيوتر بحثاً عن برنامج التشغيل**  Browse my computer for driver software.

في النافذة التالية ابحث عن مجلد stellaris_icdi_drivers الذي قمنا بفك ضغطه في خطوة سابقة.

{% include image.html url="assets/files/article_02/browse_for_driver.png" border="1" %}

قم بالضغط على زر **التالي**  Next ليبدأ ويندوز بتثبيت برنامج التشغل. 

إستمر في خطوات التنصيب الى أن ترى نافذة تخبرك بأن عملية التنصيب تمت بنجاح.

قم بتحديث برامج تشغيل جميع الأجهزة تحت تصنيف **أجهزة أخرى**  Other devices بنفس الطريقة السابقة الى أن لا يتبقى منها شيئ.

عند الإنتهاء من تنصيب جميع برامج التشغيل بنجاح سترى التالي في نافذة **إدارة الأجهزة** Device Manager:

{% include image.html url="assets/files/article_02/successful_driver_setup.png" border="1" %}

وبذلك أنتهينا من تحميل وتنصيب البرامج اللازمة لبرمجة المتحكمات والأنظمة المدمجة. وسنقوم في المقالة القادمة بإذن الله بتشغيل برنامج uVision وتشغيل كود عشوائي على البورد لنتأكد من أن الخطوات السابقة تمت بشكل صحيح وأن عملية تنصيب البرامج تمت بنجاح.









