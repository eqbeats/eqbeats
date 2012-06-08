{{%AUTOESCAPE context="HTML"}}

<ul class="tracklist">

    {{#EMPTY}}
    <li class="empty">Nothing here yet</li>
    {{/EMPTY}}

    {{#TRACK}}
    <li {{#IS_HIDDEN}}class="hidden"{{/IS_HIDDEN}} onclick="toggle(this.getElementsByClassName('player')[0]);">
        {{#HAS_ART}}<img class="cover" alt="" src="/track/{{TID}}/art/thumb" />{{/HAS_ART}}
        <div class="title"><a href="/track/{{TID}}" title="{{TITLE}}">{{TITLE}}</a></div>
        <div class="by">by <a href="/user/{{UID}}" title="{{USERNAME}}">{{USERNAME}}</a></div>
        <div style="clear:both;"></div>
        {{>PLAYER}}
    </li>
    {{/TRACK}}

</ul>
