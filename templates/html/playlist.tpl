{{%AUTOESCAPE context="HTML"}}
<div class="playlist">

    <h2>{{PLAYLIST_NAME}}</h2>
    <h4>by <a href="/user/{{UID}}">{{USERNAME}}</a></h4>

    {{#HAS_DESCRIPTION}}
    <div class="notes">{{DESCRIPTION:x-format}}</div>
    {{/HAS_DESCRIPTION}}

    <a name="tracks"></a>
    {{>TRACKLIST}}
</div>
