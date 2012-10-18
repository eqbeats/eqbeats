{{%AUTOESCAPE context="XML"}}
<?xml version="1.0" encoding="UTF-8" ?>
<feed xmlns="http://www.w3.org/2005/Atom">
    <id>{{EQBEATS_URL}}{{FEED_URL}}</id>
    <title>{{WHAT}} on EqBeats</title>
    <updated>{{UPDATED}}</updated>
    {{#TRACK}}
    <entry>
        <title>{{TITLE}}</title>
        <updated>{{DATE}}</updated>
        <id>{{EQBEATS_URL}}/track/{{TLINK}}</id>
        <link rel="alternate" href="{{EQBEATS_URL}}/track/{{TLINK}}" />
        <link rel="enclosure" type="audio/ogg"  href="{{EQBEATS_URL}}/track/{{TLINK}}/vorbis" />
        <link rel="enclosure" type="audio/mpeg" href="{{EQBEATS_URL}}/track/{{TLINK}}/mp3" />
        <author>
            <name>{{USERNAME}}</name>
            <uri>{{EQBEATS_URL}}/user/{{ULINK}}</uri>
        </author>
    </entry>
    {{/TRACK}}
</feed>
