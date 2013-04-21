<!DOCTYPE html>
<html>
    <head>
        <title>{{TITLE}} - Equestrian Beats</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
        <meta name="robots" content="noindex" />
        <link rel="stylesheet" type="text/css" href="/static/player.css" />
        <script type="text/javascript" src="/static/sm2.js"></script>
        <script type="text/javascript" src="/static/player.js"></script>
    </head>
    <body>
        <div id="player-embed">
            <div class="nohtml5-notice">Sorry, your browser does not support HTML5 audio. We recommend you upgrade to a better browser, like <a href="http://mozilla.org/firefox/">Firefox</a> or <a href="http://chrome.google.com/">Chrome</a>.</div>

            {{#FOUND}}
            <a href="/track/{{TID}}" target="_blank" class="pic">
                {{#HAS_ART}}<img src="/track/{{TID}}/art/thumb" class="cover" alt="Cover" />{{/HAS_ART}}
                {{#NO_ART}}<img src="/static/logo.png" class="logo" alt="Equestrian Beats" />{{/NO_ART}}
            </a>
            <h3><a href="/track/{{TID}}" target="_blank" title="{{TITLE}}">{{TITLE}}</a></h3>
            <h4>by <a href="/user/{{UID}}" target="_blank" title="{{USERNAME}}">{{USERNAME}}</a></h4>
            {{>PLAYER}}
            {{/FOUND}}

            {{#NOT_FOUND}}
            <a href="/" target="_blank" class="pic">
                <img src="/static/logo.png" class="logo" alt="Equestrian Beats" />
            </a>
            <h3 style="margin-bottom:10px;">Track not found</h3>
            {{/NOT_FOUND}}

            <div style="clear:both;"></div>
        </div>
    </body>
</html>
