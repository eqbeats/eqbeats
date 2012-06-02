{{%AUTOESCAPE context="HTML"}}
<!DOCTYPE html>
<html>
    <head>
        <title>{{#HAS_TITLE}}{{TITLE}} - {{/HAS_TITLE}}Equestrian Beats</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
        <link rel="stylesheet" type="text/css" href="/static/style.css" />
        <link rel="shortcut icon" href="/static/favicon.ico" id="favicon-link" />
        <script type="text/javascript" src="/static/sm2.js"></script>
        <script type="text/javascript" src="/static/player.js"></script>
        {{HEAD}}
    </head>
    <body>
        <div id="main">
            <div id="header">
                <h1><a href="/">Equestrian Beats</a></h1>
                <div id="logstatus">
                    {{#LOGGED_USER}}
                    Hi <b><a href="{{URL}}">{{NAME}}</a></b>. <a href="/logout{{#HAS_REDIR}}?redirect={{REDIR}}{{/HAS_REDIR}}">Logout</a>
                    {{/LOGGED_USER}}
                    {{#LOGGED_OUT}}
                    <a href="/login{{#HAS_REDIR}}?redirect={{REDIR}}{{/HAS_REDIR}}">Login</a>
                    {{/LOGGED_OUT}}
                </div>
                <div id="navbar">
                    <a href="/">Home</a>
                    <a href="/artists">Artists</a>
                    <img alt="-" src="/static/icons/cm-nav.png"/>
                    <a href="/news">News</a>
                    <a href="/faq">FAQ</a>
                    <a title="#eqbeats on irc.ponychat.net" href="http://ponychat.net/webchat?nick={{#LOGGED_USER}}{{IRCNICK}}{{/LOGGED_USER}}{{#LOGGED_OUT}}pony_.....{{/LOGGED_OUT}}&autojoin=eqbeats">Chat</a>
                </div>
                <div style="clear:both;"></div>
            </div>
            <div id="contents">

