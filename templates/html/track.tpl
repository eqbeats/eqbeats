{{%AUTOESCAPE context="HTML"}}
<div class="track">
    <h2>{{TITLE}}</h2>
    <h4>by <a href="/user/{{UID}}">{{USERNAME}}</a> <span class="date">on {{DAY}}</span></h4>

    {{#HAS_ART}}<img class="art" alt="" src="/track/{{TID}}/art/medium" />{{/HAS_ART}}

    {{#READY}}{{>PLAYER}}{{/READY}}
    {{#HAS_STATUS}}<div class="status">Status: {{STATUS}}</div>{{/HAS_STATUS}}

    <div class="toolbar">
        {{#READY}}
        <span><img alt="" src="/static/icons/drive-download.png" /> Download:
        <ul class="downloads">
            {{#MP3_SOURCE}}<li><a href="/track/{{TID}}/original"><span>Original MP3
                <span class="codecinfo">Highest quality</span>
            </span></a></li>{{/MP3_SOURCE}}
            {{#OTHER_SOURCE}}
            <li><a href="/track/{{TID}}/original"><span>Original ({{EXTENSION}})
                <span class="codecinfo">Highest quality</span>
                <span class="settings">(Untouched upload)</span>
            </span></a></li>
            <li><a href="/track/{{TID}}/mp3"><span>MP3
                <span class="codecinfo">Great quality, widely supported</span>
                <span class="settings">(libmp3lame VBR 0)</span>
            </span></a></li>
            {{/OTHER_SOURCE}}
            <li><a href="/track/{{TID}}/opus"><span>Opus
                <span class="codecinfo">Great quality, small filesize, experimental</span>
                <span class="settings">(libopus ABR ~128kbps)</span>
            </span></a></li>
            <li><a href="/track/{{TID}}/vorbis"><span>OGG Vorbis
                <span class="codecinfo">Good quality, small filesize</span>
                <span class="settings">(libvorbis VBR 4)</span>
            </span></a></li>
            <li><a href="/track/{{TID}}/aac"><span>AAC
                <span class="codecinfo">Okay quality, small filesize</span>
                <span class="settings">(libfdk_aac VBR 3)</span>
            </span></a></li>
            {{#HAS_ART}}<li><a href="/track/{{TID}}/art" target="_blank"><span>Cover art</span></a></li>{{/HAS_ART}}
        </ul>
        </span>
        {{/READY}}
        <span><img alt="" src="/static/icons/balloon-white-left.png" /> Share: <a href="#embedcode" onclick="document.getElementById('embedcode').style.display='block';return false;">Embed</a></span>
    </div>
    <textarea id="embedcode" style="display:none;">{{>EMBED_CODE:html_escape}}</textarea>

    {{#HAS_TAGS}}
    <div class="toolbar tags"><img alt="" src="/static/icons/tag.png" /> Tags:
        {{#TAG}}<a href="/tracks/tag/{{TAG}}">{{TAG}}</a>{{/TAG}}
    </div>
    {{/HAS_TAGS}}

    <div class="license">
        {{#COPYRIGHT}} Copyright &copy; {{USERNAME}} {{/COPYRIGHT}}
        {{#OTHER_LICENSE}} License:
            {{#CC_BY}}<a href="http://creativecommons.org/licenses/by/3.0/">CC BY</a>{{/CC_BY}}
            {{#CC_BY_NC}}<a href="http://creativecommons.org/licenses/by-nc/3.0/">CC BY-NC</a>{{/CC_BY_NC}}
            {{#CC_BY_SA}}<a href="http://creativecommons.org/licenses/by-sa/3.0/">CC BY-SA</a>{{/CC_BY_SA}}
            {{#CC_BY_ND}}<a href="http://creativecommons.org/licenses/by-nd/3.0/">CC BY-ND</a>{{/CC_BY_ND}}
            {{#CC_BY_NC_SA}}<a href="http://creativecommons.org/licenses/by-nc-sa/3.0/">CC BY-NC-SA</a>{{/CC_BY_NC_SA}}
            {{#CC_BY_NC_ND}}<a href="http://creativecommons.org/licenses/by-nc-nd/3.0/">CC BY</a>{{/CC_BY_NC_ND}}
            {{#PUBLIC}}Public domain <a href="https://creativecommons.org/publicdomain/zero/1.0/">(CC0)</a>{{/PUBLIC}}
            {{#CUSTOM}}{{LICENSE}}{{/CUSTOM}}
        {{/OTHER_LICENSE}}
    </div>

    {{#HAS_NOTES}}<div class="notes">{{NOTES:x-format}}</div>{{/HAS_NOTES}}

    {{>EVENTS}}
</div>

