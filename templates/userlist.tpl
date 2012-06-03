{{#HAS_TITLE}}
<h2>{{TITLE}}</h2>
{{/HAS_TITLE}}

{{>SEARCH_FORM}}

{{#EMPTY}}<div class="empty userlist">Nobody here yet</div>{{/EMPTY}}

{{#NOT_EMPTY}}
<ul class="userlist">

    {{#ENTRY}}
    <li>
        {{>AVATAR}}
        <a class="name" href="{{URL}}">{{NAME}}</a>
        <div class=\"about\">{{ABOUT}}</div>
        <div style="clear:both;"></div>
    </li>
    {{/ENTRY}}

</ul>
{{/NOT_EMPTY}}

{{#HAS_PREV}}
    <a href="?p={{PREV}}">&laquo; Previous page</a>
    {{/HAS_NEXT}} - {{/HAS_NEXT}}
{{/HAS_PREV}}
{{#HAS_NEXT}}
    <a href="?p={{NEXT}}">Next page &raquo;</a>
{{/HAS_NEXT}}
