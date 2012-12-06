{
    "id": {{PLAYLIST_ID}},
    "name": "{{PLAYLIST_NAME:o}}",
    "author": {
        "id": {{UID}},
        "name": "{{USERNAME:o}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
    },
    {{#HAS_DESCRIPTION}}
    "description": "{{DESCRIPTION:o}}",
    "html_description": "{{DESCRIPTION:x-format:o}}",
    {{/HAS_DESCRIPTION}}
    "tracks": {{>TRACKS}},
    "link": "{{EQBEATS_URL}}{{PLAYLIST_URL}}"
}
