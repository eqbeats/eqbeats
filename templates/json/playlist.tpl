{
    "id": {{PLAYLIST_ID}},
    "name": "{{PLAYLIST_NAME:o}}",
    "author": {
        "id": {{UID}},
        "name": "{{USERNAME:o}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
    },
    "track_count": {{PLAYLIST_TRACK_COUNT}},
    {{#HAS_DESCRIPTION}}
    "description": "{{DESCRIPTION:o}}",
    "html_description": "{{DESCRIPTION:x-format:o}}",
    {{/HAS_DESCRIPTION}}
    {{#HAS_TRACKS}}
    "tracks": {{>TRACKS}},
    {{/HAS_TRACKS}}
    "link": "{{EQBEATS_URL}}{{PLAYLIST_URL}}"
}
