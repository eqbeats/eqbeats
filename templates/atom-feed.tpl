{{%AUTOESCAPE context="XML"}}
<?xml version="1.0" encoding="UTF-8" ?>
<feed xmlns="http://www.w3.org/2005/Atom">
    <title>{{WHAT}} on EqBeats</title>
    <link rel="alternate" href="{{EQBEATS_URL}}{{FEED_LINK}}" />
    <id>{{EQBEATS_URL}}{{FEED_LINK}}</id>
    <updated>{{UPDATED}}</updated>
    {{#ENTRY}}
    <entry>
        {{#TRACK}}
        <title>{{TITLE}}</title>
        <updated>{{DATE}}</updated>
        <id>{{EQBEATS_URL}}{{URL}}</id>
        <link rel="alternate" href="{{EQBEATS_URL}}{{URL}}" />
        <link rel="enclosure" type="audio/ogg"  href="{{EQBEATS_URL}}{{VORBIS_URL}}" />
        <link rel="enclosure" type="audio/mpeg" href="{{EQBEATS_URL}}{{MP3_URL}}" />
        {{#ARTIST}}
        <author>
            <name>{{NAME}}</name>
            <uri>{{EQBEATS_URL}}{{URL}}</url>
        </author>
        {{/ARTIST}}
        {{/TRACK}}
    </entry>
    {{/ENTRY}}
</feed>
