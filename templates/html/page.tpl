{{%AUTOESCAPE context="HTML"}}
<!DOCTYPE html>
<html>
    <head>
        <title>{{#HAS_TITLE}}{{TITLE}} - {{/HAS_TITLE}}Equestrian Beats</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <link rel="stylesheet" type="text/css" href="/static/style.css" />
        <link rel="shortcut icon" href="/static/favicon.ico" id="favicon-link" />
        <script type="text/javascript" src="/static/sm2.js"></script>
        <script type="text/javascript" src="/static/player.js"></script>
        {{#HAS_OEMBED}}
        <link rel="alternate" type="application/json+oembed" href="http://eqbeats.org/oembed?format=json&amp;url={{EQBEATS_URL:u}}/track/{{TID}}">
        <link rel="alternate" type="application/xml+oembed" href="http://eqbeats.org/oembed?format=xml&amp;url={{EQBEATS_URL:u}}/track/{{TID}}">
        {{/HAS_OEMBED}}
        {{#FEED}}<link href="{{FEED_URL}}" type="application/atom+xml" rel="alternate" />{{/FEED}}
    </head>
    <body>
        <div id="main">
            <div id="header">
                <h1><a href="/">Equestrian Beats</a></h1>
                <div id="logstatus">
                    {{#LOGGED_USER}}
                    Hi <b><a href="/user/{{ULINK}}">{{USERNAME}}</a></b>.
                    <a href="/logout{{#HAS_REDIRECT}}?redirect={{REDIRECT}}{{/HAS_REDIRECT}}">Logout</a>
                    {{/LOGGED_USER}}
                    {{#LOGGED_OUT}}
                    <a href="/login{{#HAS_REDIRECT}}?redirect={{REDIRECT}}{{/HAS_REDIRECT}}">Login</a>
                    {{/LOGGED_OUT}}
                </div>
                <div id="navbar">
                    <a href="/">Home</a>
                    <a href="/artists">Artists</a>
                    <img alt="-" src="/static/icons/cm-nav.png"/>
                    <a href="http://blog.eqbeats.org/">Blog</a>
                    <a href="/faq">FAQ</a>
                    <a title="#eqbeats on irc.ponychat.net" href="http://ponychat.net/webchat?nick={{#LOGGED_USER}}{{USERNAME:x-irc}}{{/LOGGED_USER}}{{#LOGGED_OUT}}pony_.....{{/LOGGED_OUT}}&amp;autojoin=eqbeats">Chat</a>
                </div>
                <div style="clear:both;"></div>
            </div>
            <div id="contents">
                {{>BODY}}
            </div>
            <div id="footer">
                Contact: &#99;&#111;&#110;&#116;&#97;&#99;&#116;&#64;&#101;&#113;&#98;&#101;&#97;&#116;&#115;&#46;&#111;&#114;&#103;{{! contact@eqbeats.org}}
                <br /><a href="/credits">Credits</a>
                <!--Generated in {{GENERATION_TIME}} by {{PID}}.-->
            </div>
        </div>
        <script type="text/javascript">document.body.className = 'js';</script>
    </body>
</html>
