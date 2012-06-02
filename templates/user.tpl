<div class="user">

{{>AVATAR}}

<h2>
    {{NAME}}
    {{#FOLLOWED}}<a class="follow disabled" href="{URL}/unfollow"><span>Stop following</span></a>{{/FOLLOWED}
    {{#NOT_FOLLOWED}}<a class="follow" href="{URL}/follow"><span>Follow</span></a>{{/NOT_FOLLOWED}
</h2>

<div class="item"><img src="{{ICON}}/mail.png" alt="" /> Email: {{EMAIL}}</div>
{{#EDITION}}<div class="item"><img src="{{ICON}}/card-pencil.png" alt="" /> <a href="/account">Edit</a></div>{{/EDITION}}
<div class="item"><img src="{{ICON}}/star.png" alt="" /> <a href="{URL}/favorites">Favorite tracks</a></div>
<div style="clear:both;"></div>

{{#HAS_ABOUT}}<div class="notes">{{ABOUT}}</div>{{/HAS_ABOUT}}

</div>

<div class="events_wrapper">
    {{>COMMENT_FORM}}
    {{>EVENTS}}
</div>

{{#SHOW_TRACKS}}
<h3><img src="{{ICON}}/disc.png" alt="" /> Tracks {{>FEED_ICON}}</h3>
{{>TRACK_LIST}}
{{/SHOW_TRACKS}}
{{>UPLOAD_FORM}}

{{#EDITION}}
<form class="newplaylist" action="/playlist/new" method="POST">
    <input type="text" name="name" placeholder="Name" />
    <input type="submit" value="New playlist" />
</form>
{{/EDITION}}
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
    </li>
{{/PLAYLIST}}
</ul>
{{/HAS_PLAYLISTS}}

{{#EDITION}}
<h3><img src="{{ICON}}/plus-circle.png" alt="" /> Artists you follow</h3>
{{>USER_LIST}}
{{/EDITION}}
