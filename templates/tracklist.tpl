{{#HAS_TITLE}}
<h2>{{TITLE}}</h2>
{{/HAS_TITLE}}

{{>SEARCH_FORM}}

{{#EMPTY}}<div class="empty tracklist">Nothing here yet</div>{{/EMPTY}}

{{#NOT_EMPTY}}
<ul class="tracklist">

    {{#ENTRY}}
    <li {{#HIDDEN}}class="hidden"{{/HIDDEN}} onclick="toggle(this.getElementsByClassName('player')[0]);">
        {{>BUTTONS}}
        {{#HAS_COVER}}<img class="cover" alt="" src="{{THUMB_URL}}" />{{/HAS_COVER}}
        <div class="title"><a href="{{URL}}" title="{{TITLE}}">{{TITLE}}</a></div>
        {{#ARTIST}}<div class="by">by <a href="{{URL}}">{{NAME}}</a></div>{{/ARTIST}}
        <div style="clear:both;"></div>
        {{>PLAYER}}
    </li>
    {{/ENTRY}}

</ul>
{{/NOT_EMPTY}}

{{#HAS_PREV}}
    <a href="?p={{PREV}}">&laquo; Newer tracks</a>
    {{/HAS_NEXT}} - {{/HAS_NEXT}}
{{/HAS_PREV}}
{{#HAS_NEXT}}
    <a href="?p={{NEXT}}">Older tracks &raquo;</a>
{{/HAS_NEXT}}
