{{%AUTOESCAPE context="HTML"}}
<div class="playlist">

    <h2>{{PLAYLIST_NAME}}</h2>
    <h4>by <a href="/user/{{UID}}">{{USERNAME}}</a></h4>

    {{#FIRSTRUN}}
    <div class="message">Playlist created.<br/> To add a track to it, navigate to the track's page and look for an <img src="/static/icons/playlist-add.png" alt=""/> icon next to the title.</div>
    {{/FIRSTRUN}}

    {{#HAS_DESCRIPTION}}
    <div class="notes">{{DESCRIPTION:x-format}}</div>
    {{/HAS_DESCRIPTION}}

    <a name="tracks"></a>
    {{>TRACKLIST}}

    {{#IS_SELF}}
    <div class="dialog">
        <h3><img src="/static/icons/pencil.png" /> Edit</h3>
        <form method="post" action="/playlist/{{PLAYLIST_ID}}/edit">
            <table>
                <tr>
                    <td><img src="/static/icons/rename.png"> Name:</td>
                    <td><input type="text" name="name" value="{{PLAYLIST_NAME}}" /></td>
                </tr>
                <tr>
                    <td>
                        <img src="/static/icons/card-pencil.png"> Notes:<br />
                        <span class="legend">(tags: [b]old, [u]nderline, [i]talic)</span>
                    </td>
                    <td><textarea name="desc">{{#HAS_DESCRIPTION}}{{DESCRIPTION:pre_escape}}{{/HAS_DESCRIPTION}}</textarea></td>
                </tr>
                <tr>
                    <td></td>
                    <td><input type="submit" value="Update" /></td>
                </tr>
            </table>
            <input name="nonce" type="hidden" value="{{NONCE}}"/>
        </form>
        <a class="delete" href="/playlist/{{PLAYLIST_ID}}/delete">Delete playlist</a>
        <div style="clear:both;"></div>
    </div>
    {{/IS_SELF}}

</div>
