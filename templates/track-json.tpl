{{%AUTOESCAPE context="JSON"}}
{
    "id": {{TID}},
    "title": "{{TITLE}}",
    {{#HAS_DESCRIPTION}}
    "description": "{{DESCRIPTION}}",
    "html_description": "{{HTML_DESCRIPTION}}",
    {{/HAS_DESCRIPTION}}
    "artist": {
        "id": {{UID}},
        "name": "{{USERNAME}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
    },
    "link": "{{EQBEATS_URL}}/track/{{TID}}",
    "download": {
        {{#HAS_ART}}"art": "{{EQBEATS_URL}}/track/{{TID}}/art",{{/HAS_ART}}
        "mp3": "{{EQBEATS_URL}}/track/{{TID}}/mp3",
        "vorbis": "{{EQBEATS_URL}}/track/{{TID}}/vorbis"
    }
}
