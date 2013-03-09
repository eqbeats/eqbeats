{{%AUTOESCAPE context="HTML"}}
<div class="user">

{{#WELCOME}}
<div class="message">
All set! You can now start following artists and making playlists. If you are an artist yourself you may want to <a href="#track-uploader">upload tracks</a> below and edit your <a href="/account">profile</a>.
</div>
{{/WELCOME}}

{{>AVATAR}}

<h2>
    {{USERNAME}}
    {{#IS_FOLLOWED}}
    <a class="follow" href="{{#LOGGED_OUT}}/login?redirect={{/LOGGED_OUT}}/user/{{UID}}{{#LOGGED_USER}}/unfollow?nonce={{NONCE}}{{/LOGGED_USER}}"
    ><span>Stop following</span></a>
    {{/IS_FOLLOWED}}
    {{#NOT_FOLLOWED}}
    <a class="follow disabled" href="{{#LOGGED_OUT}}/login?redirect={{/LOGGED_OUT}}/user/{{UID}}{{#LOGGED_USER}}/follow?nonce={{NONCE}}{{/LOGGED_USER}}"
    ><span>Follow</span></a>
    {{/NOT_FOLLOWED}}
</h2>

<div class="items">
<div class="item"><img src="/static/icons/mail.png" alt="" /> Email: {{EMAIL:x-email}}</div>
{{#IS_SELF}}<div class="item"><img src="/static/icons/card-pencil.png" alt="" /> <a href="/account">Edit</a></div>{{/IS_SELF}}
{{#HAS_FAVS}}<div class="item"><img src="/static/icons/star.png" alt="" /> <a href="/user/{{UID}}/favorites">Favorite tracks ({{NUM_FAVS}})</a></div>{{/HAS_FAVS}}
{{#HAS_FOLLOWERS}}<div class="item"><img src="/static/icons/users.png" alt="" />{{NUM_FOLLOWERS}} follower{{#FOLLOWERS_PLURAL}}s{{/FOLLOWERS_PLURAL}}</div>{{/HAS_FOLLOWERS}}
</div>
<div style="clear:both;"></div>

{{#HAS_FEATURED}}
<div class="featurebox">
    {{#IS_SELF}}
    <form class="defeature" action="/user/{{UID}}/defeature" method="post">
        <button class="fakelink" type="submit"><span>Remove featured content</span> &#10006;</button>
        <input name="nonce" type="hidden" value="{{NONCE}}"/>
    </form>
    {{/IS_SELF}}
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
{{#IS_SELF}}{{>UPLOADER}}{{/IS_SELF}}

{{#IS_SELF}}
<form class="newplaylist" action="/playlist/new" method="post">
    <input type="text" name="name" placeholder="Name" />
    <input type="submit" value="New playlist" />
    <input name="nonce" type="hidden" value="{{NONCE}}"/>
</form>
{{/IS_SELF}}
<h3>Playlists</h3>
{{#NO_PLAYLIST}}<div class="playlists empty">Nothing here yet.</div>{{/NO_PLAYLIST}}
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

{{#IS_SELF}}
<h3><img src="/static/icons/plus-circle.png" alt="" /> Artists you follow</h3>
{{>FOLLOWED_USERS}}
{{/IS_SELF}}
