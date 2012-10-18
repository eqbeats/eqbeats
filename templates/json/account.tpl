{{%AUTOESCAPE context="JSON"}}
{
    "id": {{UID}},
    "name": "{{USERNAME}}",
    {{#HAS_ABOUT}}
    "description": "{{ABOUT}}",
    "html_description": "{{ABOUT:x-format:json_escape}}",
    {{/HAS_ABOUT}}
    "tracks": [{{#TRACK}}{{>TRACK}}{{#TRACK_separator}},{{/TRACK_separator}}{{/TRACK}}],
    "playlists": [{{#PLAYLIST}}{{>PLAYLIST}}{{#PLAYLIST_separator}},{{/PLAYLIST_separator}}{{/PLAYLIST}}],
    "link": "{{EQBEATS_URL}}/user/{{ULINK}}"
}
