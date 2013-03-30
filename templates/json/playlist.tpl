{
    "id": {{PLAYLIST_ID}},
    "name": "{{PLAYLIST_NAME:o}}",
    {{#HAS_DESCRIPTION}}
    "description": "{{DESCRIPTION:o}}",
    "html_description": "{{DESCRIPTION:x-format:o}}",
    {{/HAS_DESCRIPTION}}
    {{#LONG}}
    {{#AUTHOR}}
    "author": {
        "id": {{UID}},
        "name": "{{USERNAME:o}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
    },
    {{/AUTHOR}}
    "tracks": {{>TRACKS}},
    {{/LONG}}
    "link": "{{EQBEATS_URL}}{{PLAYLIST_URL}}"
}
