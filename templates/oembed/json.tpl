{{%AUTOESCAPE context="JSON"}}
{
    "version": "1.0",
    "provider_name": "Equestrian Beats",
    "provider_url": "{{EQBEATS_URL}}",
    "type": "video",
    "url": "{{EQBEATS_URL}}/track/{{TLINK}}",
    "width": {{WIDTH}},
    "height": 150,
    "html": "{{>EMBED_CODE:json_escape}}",
    "title": "{{TITLE}}",
    "author_name": "{{USERNAME}}",
    "author_url": "{{EQBEATS_URL}}/user/{{ULINK}}"
}
