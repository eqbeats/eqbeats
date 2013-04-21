{{%AUTOESCAPE context="HTML"}}
<!DOCTYPE html>
<html>
    <head>
        <title>{{#HAS_TITLE}}{{TITLE}} - {{/HAS_TITLE}}Equestrian Beats</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <link rel="stylesheet" type="text/css" href="/static/style.css?2" />
        <link rel="shortcut icon" href="/static/favicon.ico" id="favicon-link" />
        <script type="text/javascript" src="/static/player.js?6"></script>
        {{#REQUIRES_STATS_JS}}
        <script type="text/javascript" src="/static/d3.js"></script>
        <script type="text/javascript" src="/static/crossfilter.js"></script>
        <script type="text/javascript" src="/static/stats.js?5"></script>
        {{/REQUIRES_STATS_JS}}
        {{#HAS_OEMBED}}
        <link rel="alternate" type="application/json+oembed" href="http://eqbeats.org/oembed?format=json&amp;url={{EQBEATS_URL:u}}/track/{{TID}}">
        <link rel="alternate" type="application/xml+oembed" href="http://eqbeats.org/oembed?format=xml&amp;url={{EQBEATS_URL:u}}/track/{{TID}}">
        {{/HAS_OEMBED}}
        {{#FEED}}<link href="{{FEED_URL}}" type="application/atom+xml" rel="alternate" />{{/FEED}}
        <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" />
    </head>
    <body>
        <div id="main">
            <div class="nohtml5-notice"><b>Notice</b>: EqBeats requires a browser with HTML5 audio support, which your browser does not seem to have. You will not be able to play tracks with your current setup. We recommend you upgrade to a modern browser, like <a href="http://mozilla.org/firefox/">Firefox</a> or <a href="http://chrome.google.com/">Chrome</a>.</div>
            <div id="header">
                <h1><a href="/">Equestrian Beats</a></h1>
                <div id="logstatus">
                    {{#LOGGED_USER}}
                    Hi <b><a href="/user/{{UID}}">{{USERNAME}}</a></b>.
                    <a href="/logout{{#HAS_REDIRECT}}?redirect={{REDIRECT}}{{/HAS_REDIRECT}}">Logout</a>
                    {{/LOGGED_USER}}
                    {{#LOGGED_OUT}}
                    <a href="/login{{#HAS_REDIRECT}}?redirect={{REDIRECT}}{{/HAS_REDIRECT}}">Login</a>
                    {{/LOGGED_OUT}}
                </div>
                <div id="navbar">
                    <a href="/">Home</a>
                    <a href="/tracks/latest">Latest</a>
                    <a href="/artists">Artists</a>
                    <form action="/tracks/search">
                        <button>
                            <img alt="Search" src="/static/icons/purple-magnifier.png"/>
                        </button>
                        <input type="text" name="q" placeholder="Search…" value="{{SEARCH}}" />
                    </form>
                </div>
                <div style="clear:both;"></div>
            </div>
            <div id="contents">
                {{>BODY}}
            </div>
            <div id="footer">
                <a href="/faq">FAQ</a>
                <a href="/api">API</a>
                <a href="https://github.com/eqbeats/eqbeats/">Code</a>
                -
                <a href="http://blog.eqbeats.org/">Blog</a>
                <a title="#eqbeats on irc.ponychat.net" href="https://irc.lc/ponychat/eqbeats/{{#LOGGED_USER}}{{USERNAME:x-irc}}{{/LOGGED_USER}}{{#LOGGED_OUT}}pony_.....{{/LOGGED_OUT}}">IRC</a>
                <a href="/thanks">Thanks</a>
                <br/>
                Contact: &#99;&#111;&#110;&#116;&#97;&#99;&#116;&#64;&#101;&#113;&#98;&#101;&#97;&#116;&#115;&#46;&#111;&#114;&#103;{{! contact@eqbeats.org}}
                <!--Generated in {{GENERATION_TIME}} by {{PID}}.-->
                <a class="honeypot" href="http://www1360406159.blogblazer.com/">@</a>
            </div>
        </div>
        <script type="text/javascript">document.body.className = 'js';</script>
    </body>
</html>
