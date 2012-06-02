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

            {{#TRACK}}
            <a href="{{URL}}" target="_blank" class="pic">
                {{#HAS_COVER}}<img src="{{THUMB_URL}}" class="cover" alt="Cover" />{{/HAS_COVER}}
                {{#NO_COVER}<img src="/static/logo.png" class="logo" alt="Equestrian Beats" />{{/NO_COVER}}
            </a>
            <h3><a href="{{URL}}" target="_blank" title="{{TITLE}}">{{TITLE}}</a></h3>
            {{#ARTIST}}<h4>by <a href="{{URL}}" target="_blank" title="{{ARTIST_NAME}}">{{ARTIST_NAME}}</a></h4>{{/ARTIST}}
            {{/TRACK}}
            {{>PLAYER}}

            {{#NOT_FOUND}}
            <a href="{{EQBEATS_URL}}" target="_blank" class="pic">
                <img src="/static/logo.png" class="logo" alt="Equestrian Beats" />
            </a>
            <h3 style="margin-bottom:10px;">Track not found</h3>
            {{/NOT_FOUND}}

            <div style="clear:both;"></div>
        </div>
    </body>
</html>
