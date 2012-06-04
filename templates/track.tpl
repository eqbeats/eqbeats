<div class="track">
    <h2>{{TITLE}}
        {{#IS_NOT_FAV}}
        <a href="{{#LOGGED_OUT}}/login?redirect={{/LOGGED_OUT}}/track/{{TID}}/favorite"
           title="Add to favorites">
            <img src="/static/icons/star-empty.png" alt="Add to favorites" />
        </a>
        {{/IS_NOT_FAV}}
        {{#IS_FAV}}
        <a href="/track/{{TID}}/unfavorite"
           title="Remove from favorites">
            <img src="/static/icons/star.png" alt="Remove from favorites" />
        </a>
        {{/IS_FAV}}
        {{#IS_VISIBLE}}{{#HAS_PLAYLISTS}}
        <span id="addplaylist">
            <img src="/static/icons/playlist-add.png" alt="Playlists" title="Playlists">
            <form action="/track/{{TID}}/playlist" method="post">
                <select name="playlist">
                {{#PLAYLISTS}}
                    <option value="{{PID}}">{{NAME}}</option>
                {{/PLAYLISTS}}
                </select>
            <input type="submit" value="Add to this playlist"/>
            </form>
        </span>
        {{/HAS_PLAYLISTS}}{{/IS_VISIBLE}}
    </h2>
    <h4>by <a href="/user/{{ARTIST_ID}}">{{ARTIST_NAME}}</a> <span class="date">on {{DATE}}</span></h4>

    {{#HAS_ART}}<img class="art" alt="" src="/track/{{TID}}/art/medium" />{{/HAS_ART}}

    {{#NO_STATUS}}{{>PLAYER}}{{/NO_STATUS}}
    {{#HAS_STATUS}}<div class="status">Status: {{STATUS}}</div>{{/HAS_STATUS}}

    <div class="toolbar">
        {{#NO_STATUS}}
        <span><img alt="" src="/static/icons/drive-download.png" /> Download :
            <a href="/track/{{TID}}/vorbis">OGG Vorbis</a>
            {{#MP3_SOURCE}}<a href="/track/{{TID}}/mp3">Original MP3</a>{{/MP3_SOURCE}}
            {{#OTHER_SOURCE}}
            <a href="/track/{{TID}}/mp3">MP3</a>
            <a href="/track/{{TID}}/original">Original (.{{EXTENSION}})</a>
            {{/OTHER_SOURCE}}
            <a href="/track/{{TID}}/art" target="_blank">Art</a>
        </span>
        {{/NO_STATUS}}
        <span><img alt="" src="/static/icons/balloon-white-left.png" /> Share : <a href="#embedcode" onclick="document.getElementById('embedcode').style.display='block';return false;">Embed</a></span>

        {{#IS_OWNER}}
        {{#HAS_HITS}} <span><img alt="" src="/static/icons/edit-number.png" /> Hits : {{HITCOUNT}}</span>{{/HAS_HITS}}
        <span><img alt="" src="/static/icons/star.png" /> Favourites : {{FAVCOUNT}}</span>
        {{/IS_OWNER}}
        {{#IS_NOT_OWNER}}
        <form action="/track/{{TID}}/report" method="post">
            <button type="submit" class="report"><img alt="" src="/static/icon/flag.png" /><span> Report</span></button>
        </form>
        {{/IS_NOT_OWNER}}
    </div>
    <textarea id="embedcode" style="display:none;">{{>EMBED}}</textarea>

    {{#IS_OWNER}}
    <div class="toolbar tags"><img alt="" src="/static/icons/tag.png" /> Tags:
        <form action="/track/{{TID}}/tags" method="post">
            <input name="tags" value="{{#TAGS}}{{TAG}}, {{/TAGS}}" />
            <input type="submit" value="Update" />
            <span class="legend">(comma-separated, e.g. instrumental, electronic)</span>
        </form>
    </div>
    {{/IS_OWNER}}
    {{#IS_NOT_OWNER}} {{#HAS_TAGS}}
    <div class="toolbar tags"><img alt="" src="/static/icons/tag.png" /> Tags:
        {{#TAGS}}
            <a href="/tracks/tag/{{TAG}}">{{TAG}}</a>
        {{/TAGS}}
    </div>
    {{/HAS_TAGS}} {{/IS_NOT_OWNER}}

    <div class="license">
        {{#COPYRIGHT}} Copyright &copy; {{ARTIST_NAME}} {{/COPYRIGHT}}
        {{#OTHER_LICENSE}} License: {{LICENSE}} {{/OTHER_LICENSE}}

        {{#IS_OWNER}} <a href="/tracks{{ID}}/license">(change)</a> {{/IS_OWNER}}
    </div>

    {{#HAS_NOTES}}
    <div class="notes">{{NOTES}}</div>
    {{/HAS_NOTES}}

    {{#IS_OWNER}}
    <div class="edit">
         <h3><img src="/static/icon/pencil.png" alt="" /> Edit</h3>
         {{#IS_HIDDEN}}
         <form class="publish" action="/track/{{TID}}/publish" method="post">
            <img src="/static/icons/disc-arrow.png" alt="" />
            This track is not yet published.
            <input type="submit" value="Publish"/>
            <input type="hidden" name="tid" value="{{TID}}"/>
        </form>
        {{/IS_HIDDEN}}

        <div class="rename">
            <h4><img alt="" src="/static/icons/rename.png" /> Rename</h4>
            <form method="post" action="/track/{{TID}}/rename">
                <b>{{ARTIST_NAME}}</b> -
                <input type="text" name="title" value="{{TITLE}}" />
                <input type="submit" value="Rename" />
            </form>
        </div>


        <div class="column">

            <h4><img src="/static/icons/drive-upload.png" alt="" /> Re-upload</h4>
            {{>UPLOAD_FORM}}

            <h4><img src="/static/icons/picture.png" alt="" /> Art</h4>
            <form action="/track/{{TID}}/art/upload" method="post" enctype="multipart/form-data">
                <input type="file" name="file" />
                <input type="submit" value="Upload a picture" />
            </form>

            <h4><img src="/static/icons/balloon-sound.png"> Broadcast</h4>
            <form action="/track/{{TID}}/flags" method="post">
                <input type="checkbox" name="airable" {{#IS_AIRABLE}} checked="checked" {{/IS_AIRABLE}} />
                Celestia Radio<br />
                <input type="submit" value="Update" />
            </form>
            <h4><img src="/static/icons/youtube-up.png" alt=""> Youtube</h4>
            {{#HAS_YOUTUBE}}
            <form action="/track/{{TID}}/youtube_upload">
                <input type="submit" value="Upload this to YouTube"/>
                <a class="huh" href="/faq#youtube">Huh?</a>
            </form>
            {{/HAS_YOUTUBE}}
            {{#NO_YOUTUBE}}
            <form action="https://accounts.google.com/o/oauth2/auth">
                Your YouTube account is not linked.
                <a class="huh" href="/faq#youtube">Huh?</a>
                </br>
                <input type="submit" value="Link your YouTube account"/>
                <input type="hidden" name="response_type" value="code"/>
                <input type="hidden" name="client_id" value="767490682254.apps.googleusercontent.com"/>
                <input type="hidden" name="scope" value="https://uploads.gdata.youtube.com/feeds/api/users/default/uploads"/>
                <input type="hidden" name="access_type" value="offline"/>
                <input type="hidden" name="redirect_uri" value="http://eqbeats.org/oauth/yt"/>
                <input type="hidden" name="approval_prompt" value="force"/>
            </form>
            {{/NO_YOUTUBE}}
        </div>

        <div class="column">
            <h4><img src="/static/icons/card-pencil.png" /> Notes</h4>
            <form action="/track/{{TID}}/notes" method="post">
                <textarea name="notes">{{NOTES}}</textarea><br />
                <div class="legend">(tags: [b]old, [u]nderline, [i]talic)</div>
                <input type="submit" value="Update description" />
            </form>
        </div>
        <a class="delete" href="/track/{{TID}}/delete">Delete track</a>
        <div style="clear:both;"></div>
    </div>
    {{/IS_OWNER}}

    {{<EVENTS}}
    {{<COMMENT_FORM}}
</div>

