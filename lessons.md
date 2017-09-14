---
layout: default
title: "الدروس"
---

# قائمة الدروس
<ul>
  {% assign posts = site.posts | sort: 'date' %}
  {% for post in posts %}
    <li>
      <a href="{{site.url}}{{post.url}}">{{ post.title }}</a>
    </li>
  {% endfor %}
</ul>

<!--[back](./)-->

---
