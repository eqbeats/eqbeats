{
    "id": {{PLAYLIST_ID}},
    "name": "{{PLAYLIST_NAME:o}}",
    "author": {
        "id": {{UID}},
        "name": "{{USERNAME:o}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
        {{#HAS_AVATAR}},
        "avatar": "https://secure.gravatar.com/avatar/{{EMAIL_MD5}}?d=http%3A%2F%2Feqbeats.org%2Fstatic%2Favatar.png"
        {{/HAS_AVATAR}}
    },
    "num_tracks": {{PLAYLIST_TRACK_COUNT}},
    {{#HAS_DESCRIPTION}}
    "description": "{{DESCRIPTION:o}}",
    "html_description": "{{DESCRIPTION:x-format:o}}",
    {{/HAS_DESCRIPTION}}
    {{#HAS_TRACKS}}
    "tracks": {{>TRACKS}},
    {{/HAS_TRACKS}}
    "link": "{{EQBEATS_URL}}{{PLAYLIST_URL}}"
}
