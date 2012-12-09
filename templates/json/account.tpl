{
    "id": {{UID}},
    "name": "{{USERNAME:o}}",
    {{#HAS_ABOUT}}
    "description": "{{ABOUT:o}}",
    "html_description": "{{ABOUT:x-format:o}}",
    {{/HAS_ABOUT}}
    {{#LONG}}
    "tracks": [{{#ITEM}}{{>DATA}}{{#ITEM_separator}},{{/ITEM_separator}}{{/ITEM}}],
    "playlists": [{{#PLAYLIST}}{{>PLAYLIST}}{{#PLAYLIST_separator}},{{/PLAYLIST_separator}}{{/PLAYLIST}}],
    {{/LONG}}
    "link": "{{EQBEATS_URL}}/user/{{UID}}"
}
