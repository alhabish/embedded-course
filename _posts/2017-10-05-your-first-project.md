---
layout: post
title: "3. كتابة أول مشروع في Keil uVision"
lang: ar-SA
comments: true
---

في هذه المقالة سنقوم بإذن الله بتشغيل أول كود لنا على البورد لنتأكد من أن الخطوات التي أتبعناها في المقالة السابقة تمت بشكل صحيح، ولفهم طريقة كتابة البرامج في Keil uVision، وذلك بإتباع الخطواط التالية:

(1) قم بفتح Keil uVision

{% include image.html url="assets/files/article_03/uvision_splash_screen.png" border="1" %}

(2) إختر **Project -> New uVision Project**
{% include image.html url="assets/files/article_03/uvision_new_project.png" border="1" %}

(3) قم بتسمية المشروع الجديد first_project واحفظه في المسار التالي C:\embedded_course ثم قم بالضغط على زر Save.
{% include image.html url="assets/files/article_03/create_new_project_window.png" border="1" %}

(4) بعد ذلك سترى نافذه تطلب منك إختيار نوع المتحكم. تحت تصنيف Texas Instruments قم بإسدال التصنيف Tiva C Series وأختر TM4C123GH6PM ثم إضغط على OK. 
{% include image.html url="assets/files/article_03/uvision_select_device.png" border="1" %}

(5) في النافذه التالية قم بإسدال قائمة CMSIS وضع علامة (صح) بجانب CORE. وقم كذلك بإسدال قائمة Device وضع علامة (صح) بجانب Startup أيضاً. عند الإنتهاء، قم بالضغط على OK.
{% include image.html url="assets/files/article_03/uvision_manage_runtime_environment.png" border="1" %}

(6) إختر **Project -> Options for Target Target 1**
{% include image.html url="assets/files/article_03/uvision_project_options_for_target.png" border="1" %}

بإمكانك أيضاً إظهار نافذة الإعدادات من الزر الموجود في الـ toolbar
{% include image.html url="assets/files/article_03/uvision_toolbar_options_for_target.png" border="1" %}

(7) في التبويب Target ، وفي صندوق النص بجانب Xtal (MHz)، قم بتغير قيمة سرعة الساعة (الكرستالة/المذبذب) Oscillator الى 16.0
{% include image.html url="assets/files/article_03/uvision_options_target.png" border="1" %}

حيث أخذنا هذه المعلومة من مواصفات المتحكم:
{% include image.html url="assets/files/article_03/tivac_launchpad_specs.png" border="1" %}

وفي نافذة الإعدادات أيضاً، إذهب الى التبويب Debug، وفي القائمة المنسدلة على اليمين إختر Stellaris ICDI، ثم إضغط على OK.
{% include image.html url="assets/files/article_03/uvision_options_debug.png" border="1" %}

(8) في نافذة Project على اليسار، قم بالنقر على الفأرة على اليمين وأختر Add New Item to Group Source Group 1.
{% include image.html url="assets/files/article_03/uvision_add_new_item.png" border="1" %}

(9) إختر C File .c في اليسار وقم بتسمية الملف main وسيقوم uVision بإضافة الإمتداد c. اذا لم تفعل. إضغط بعد ذلك على OK.
{% include image.html url="assets/files/article_03/uvision_new_file.png" border="1" %}

(10) إكتب الكود التالي في الملف main.c الذي أنشأته

```c
#define SYSCTL_RCGCGPIO_R   (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTF_DIR_R    (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R    (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_DATA_R   (*((volatile unsigned long *)0x400253FC))

#define LED_RED    (1U<<1)		
#define LED_BLUE   (1U<<2)		
#define LED_GREEN  (1U<<3)		

void delay ( ) {
  for (int i=0; i<5000000; i++);
}

int main ( ) {
  SYSCTL_RCGCGPIO_R = 0X20;
  GPIO_PORTF_DIR_R  = 0xE; 
  GPIO_PORTF_DEN_R  = 0xE; 

  while (1) {
    GPIO_PORTF_DATA_R = LED_RED;   
    delay();
    GPIO_PORTF_DATA_R = LED_BLUE;  
    delay();
    GPIO_PORTF_DATA_R = LED_GREEN; 
    delay();
  }	
}
```

أو قم بنسخه من الرابط التالي:

<https://github.com/alhabish/embedded-course/blob/gh-pages/assets/files/article_03/main.c>

(11) إختر **Project -> Build Target** 
{% include image.html url="assets/files/article_03/uvision_project_build_target.png" border="1" %}

أو إضغط على زر Build في الـ toolbar.
{% include image.html url="assets/files/article_03/uvision_toolbar_build.png" border="1" %}

وتأكد من عدم وجود أخطاء في الكود من نافذة الـ  Build Output في الأسفل
{% include image.html url="assets/files/article_03/uvision_build_output.png" border="1" %}

(12) قم بتوصيل البورد بالحاسب

(13) إختر **Flash -> Download** من القائمة الرئيسية
{% include image.html url="assets/files/article_03/uvision_flash_download.png" border="1" %}

أو إضغط على زر Download من الـ toolbar 
{% include image.html url="assets/files/article_03/uvision_toolbar_download.png" border="1" %}

إذا ما تمت العملية بنجاح دون وجود أخطاء فإنك سترى نافذة الـ Build Output مشابهة لما يلي:
{% include image.html url="assets/files/article_03/uvision_build_output_download.png" border="1" %}

(13) إضغط على زر الـ RESET في البورد ليبدأ البرنامج في التنفيذ
{% include image.html url="assets/files/article_03/tivac_launchpad_reset.png" border="1" %}

بعد ذلك سترى الإضائة الملونة RGB LED على البورد تومض باللون الأحمر والأزرق والأخضر كما يلي:
{% include image.html url="assets/files/article_03/VID_20171005_153436.gif" border="1" %}



