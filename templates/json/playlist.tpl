{{%AUTOESCAPE context="JSON"}}
{
    "id": {{PLAYLIST_ID}},
    "name": "{{PLAYLIST_NAME}}",
    "author": {
        "id": {{UID}},
        "name": "{{USERNAME}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
    },
    {{#HAS_DESCRIPTION}}
    "description": "{{DESCRIPTION}}",
    "html_description": "{{DESCRIPTION:x-format}}",
    {{/HAS_DESCRIPTION}}
    "tracks": {{>TRACKS}},
    "link": "{{EQBEATS_URL}}{{PLAYLIST_URL}}"
}
