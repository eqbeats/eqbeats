<div class="user">

{{>AVATAR}}

<h2>
    {{USERNAME}}
    {{#FOLLOWED}}<a class="follow disabled" href="/user/{{UID}}/unfollow"><span>Stop following</span></a>{{/FOLLOWED}}
    {{#NOT_FOLLOWED}}<a class="follow" href="/user/{{UID}}/follow"><span>Follow</span></a>{{/NOT_FOLLOWED}}
</h2>

<div class="item"><img src="/static/icons/mail.png" alt="" /> Email: {{EMAIL:x-email}}</div>
{{#IS_SELF}}<div class="item"><img src="/static/icons/card-pencil.png" alt="" /> <a href="/account">Edit</a></div>{{/IS_SELF}}
<div class="item"><img src="/static/icons/star.png" alt="" /> <a href="/user/{{UID}}/favorites">Favorite tracks</a></div>
<div style="clear:both;"></div>

{{#HAS_ABOUT}}<div class="notes">{{ABOUT:x-format}}</div>{{/HAS_ABOUT}}

</div>

<div class="events_wrapper">
    {{>COMMENT_FORM}}
    {{>EVENTS}}
</div>

<h3><img src="/static/icons/disc.png" alt="" /> Tracks {{>FEED_ICON}}</h3>
{{>TRACK_LIST}}
{{>UPLOADER}}

{{#IS_SELF}}
<form class="newplaylist" action="/playlist/new" method="POST">
    <input type="text" name="name" placeholder="Name" />
    <input type="submit" value="New playlist" />
</form>
{{/IS_SELF}}
<h3>Playlists</h3>
{{#NO_PLAYLIST}}<div class="playlists empty">Nothing here yet.</div>{{/NO_PLAYLIST}}
{{#HAS_PLAYLISTS}}
<ul class="playlists">
{{#PLAYLIST}}
    <li>
        <div class="title">
            <a href="{{URL}}">{{NAME}}</a>
            <span class="count">{{TRACK_COUNT}}</span>
        </div>
        {{#HAS_DESCRIPTION}}
        <div class="description">{{DESCRIPTION}}</div>
        {{/HAS_DESCRIPTION}}
    </li>
{{/PLAYLIST}}
</ul>
{{/HAS_PLAYLISTS}}

{{#IS_SELF}}
<h3><img src="/static/icons/plus-circle.png" alt="" /> Artists you follow</h3>
{{>USER_LIST}}
{{/IS_SELF}}
