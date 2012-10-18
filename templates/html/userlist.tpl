{{%AUTOESCAPE context="HTML"}}

<ul class="userlist">

    {{#EMPTY}}<li class="empty">Nobody here yet</li>{{/EMPTY}}

    {{#USER}}
    <li>
        {{>AVATAR}}
        <a class="name" href="/user/{{ULINK}}">{{USERNAME}}</a>
        {{#HAS_ABOUT}}<div class="about">{{ABOUT:x-format}}</div>{{/HAS_ABOUT}}
        <div style="clear:both;"></div>
    </li>
    {{/USER}}

</ul>
