{
    "id": {{TID}},
    "title": "{{TITLE:o}}",
    {{#HAS_NOTES}}
    "description": "{{NOTES:o}}",
    "html_description": "{{NOTES:x-format:o}}",
    {{/HAS_NOTES}}
    {{#HAS_TAGS}}"tags": [{{#TAG}}"{{TAG:o}}"{{#TAG_separator}},{{/TAG_separator}}{{/TAG}}],{{/HAS_TAGS}}
    "license": "{{LICENSE:o}}",
    "artist": {
        "id": {{UID}},
        "name": "{{USERNAME:o}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
    },
    "link": "{{EQBEATS_URL}}/track/{{TID}}",
    "download": {
        {{#HAS_ART}}"art": "{{EQBEATS_URL}}/track/{{TID}}/art",{{/HAS_ART}}
        "opus": "{{EQBEATS_URL}}/track/{{TID}}/opus",
        "vorbis": "{{EQBEATS_URL}}/track/{{TID}}/vorbis",
        "aac": "{{EQBEATS_URL}}/track/{{TID}}/aac",
        "mp3": "{{EQBEATS_URL}}/track/{{TID}}/mp3",
        "original": "{{EQBEATS_URL}}/track/{{TID}}/original"
    }
}
