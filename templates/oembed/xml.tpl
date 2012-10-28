{{%AUTOESCAPE context="XML"}}
<?xml version="1.0" encoding="utf-8" standalone="yes"?>
<oembed>
    <version>1.0</version>
    <provider_name>Equestrian Beats</provider_name>
    <provider_url>{{EQBEATS_URL}}</provider_url>
    <type>video</type>
    <url>{{EQBEATS_URL}}/track/{{TID}}</url>
    <width>{{WIDTH}}</width>
    <height>150</height>
    <html>{{>EMBED_CODE:xml_escape}}</html>
    <title>{{TITLE}}</title>
    <author_name>{{USERNAME}}</author_name>
    <author_url>{{EQBEATS_URL}}/user/{{ULINK}}</author_url>
</oembed>
