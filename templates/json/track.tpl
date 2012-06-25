{{%AUTOESCAPE context="JSON"}}
{
    "id": {{TID}},
    "title": "{{TITLE}}",
    {{#HAS_NOTES}}
    "description": "{{NOTES}}",
    "html_description": "{{NOTES:x-format:json_escape}}",
    {{/HAS_NOTES}}
    {{#HAS_TAGS}}"tags": [{{#TAG}}"{{TAG}}"{{#TAG_separator}},{{/TAG_separator}}{{/TAG}}],{{/HAS_TAGS}}
    "license": "{{LICENSE}}",
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
