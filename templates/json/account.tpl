{
    "id": {{UID}},
    "name": "{{USERNAME:o}}",
    {{#HAS_ABOUT}}
    "description": "{{ABOUT:o}}",
    "html_description": "{{ABOUT:x-format:o}}",
    {{/HAS_ABOUT}}
    "tracks": [{{#TRACK}}{{>TRACK}}{{#TRACK_separator}},{{/TRACK_separator}}{{/TRACK}}],
    "playlists": [{{#PLAYLIST}}{{>PLAYLIST}}{{#PLAYLIST_separator}},{{/PLAYLIST_separator}}{{/PLAYLIST}}],
    "link": "{{EQBEATS_URL}}/user/{{UID}}"
}
