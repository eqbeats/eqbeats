{{%AUTOESCAPE context="HTML"}}
<div class="user">

{{>AVATAR}}

<h2>
    {{USERNAME}}
</h2>

<div class="items">
<div class="item"><img src="/static/icons/mail.png" alt="" /> Email: {{EMAIL:x-email}}</div>
{{#HAS_FAVS}}<div class="item"><img src="/static/icons/star.png" alt="" /> <a href="/user/{{UID}}/favorites">Favorite tracks ({{NUM_FAVS}})</a></div>{{/HAS_FAVS}}
{{#HAS_FOLLOWERS}}<div class="item"><img src="/static/icons/users.png" alt="" /> {{NUM_FOLLOWERS}} follower{{#FOLLOWERS_PLURAL}}s{{/FOLLOWERS_PLURAL}}</div>{{/HAS_FOLLOWERS}}
</div>
<div style="clear:both;"></div>

{{#HAS_FEATURED}}
<div class="featurebox">
    {{#FEATURED_TITLE}}
    <h3>
        <a href="{{FEATURED_LINK}}">{{FEATURED_TITLE}} &raquo;</a>
    </h3>
    {{/FEATURED_TITLE}}
    {{>FEATURED_TRACKS}}
</div>
{{/HAS_FEATURED}}

{{#HAS_ABOUT}}<div class="notes">{{ABOUT:x-format}}</div>{{/HAS_ABOUT}}

</div>

{{>EVENTS}}

<h3><img src="/static/icons/disc.png" alt="" /> Tracks {{>FEED_ICON}}</h3>
{{>TRACK_LIST}}

<h3>Playlists</h3>
{{#NO_PLAYLIST}}<div class="playlists empty">Nothing here.</div>{{/NO_PLAYLIST}}
{{#HAS_PLAYLISTS}}
<ul class="playlists">
{{#PLAYLIST}}
    <li>
        <div class="title">
            <a href="/playlist/{{PLAYLIST_ID}}">{{PLAYLIST_NAME}}</a>
            <span class="count">{{TRACK_COUNT}}</span>
        </div>
        {{#HAS_DESCRIPTION}}
        <div class="description">{{DESCRIPTION:x-format}}</div>
        {{/HAS_DESCRIPTION}}
    </li>
{{/PLAYLIST}}
</ul>
{{/HAS_PLAYLISTS}}
