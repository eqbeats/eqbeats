{{%AUTOESCAPE context="HTML"}}

<ul class="tracklist">

    {{#EMPTY}}<li class="empty">Nothing here yet</li>{{/EMPTY}}

    {{#TRACK}}
    <li {{#IS_HIDDEN}}class="hidden"{{/IS_HIDDEN}} onclick="toggle(this.getElementsByClassName('player')[0]);">

        {{#BUTTONS}}
        <div class="buttons">
            <form method="post" action="/playlist/{{PLAYLIST_ID}}/remove">
                <button name="item" value="{{POSITION}}" title="Remove this track from the playlist.">
                    <img src="/static/icons/music-minus.png" alt="Remove" />
                </button>
            </form>
            {{#IS_FIRST}}<form style="width: 16px; height: 16px;"></form>{{/IS_FIRST}}
            {{#NOT_FIRST}}
            <form method="post" action="/playlist/{{PLAYLIST_ID}}/move">
                <button name="item" value="{{POSITION}}">
                    <img src="/static/icons/arrow-up.png" alt="Move up" />
                </button>
                <input type="hidden" name="dir" value="up"/>
            </form>
            {{/NOT_FIRST}}
            {{#NOT_LAST}}
            <form method="post" action="/playlist/{{PLAYLIST_ID}}/move">
                <button name="item" value="{{POSITION}}">
                    <img src="/static/icons/arrow-down.png" alt="Move down"/>
                </button>
                <input type="hidden" name="dir" value="down"/>
            </form>
            {{/NOT_LAST}}
        </div>
        {{/BUTTONS}}

        {{#HAS_ART}}<img class="cover" alt="" src="/track/{{TID}}/art/thumb" />{{/HAS_ART}}
        <div class="title"><a href="/track/{{TID}}" title="{{TITLE}}">{{TITLE}}</a></div>
        <div class="by">by <a href="/user/{{ULINK}}" title="{{USERNAME}}">{{USERNAME}}</a></div>
        <div style="clear:both;"></div>
        {{>PLAYER}}

    </li>
    {{/TRACK}}

</ul>
