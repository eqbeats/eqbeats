{
    "id": {{TID}},
    "title": "{{TITLE:o}}",
    {{#HAS_NOTES}}
    "description": "{{NOTES:o}}",
    "html_description": "{{NOTES:x-format:o}}",
    {{/HAS_NOTES}}
    {{#HAS_TAGS}}"tags": [{{#TAG}}"{{TAG:o}}"{{#TAG_separator}},{{/TAG_separator}}{{/TAG}}],{{/HAS_TAGS}}
    "license": "{{LICENSE:o}}",
    {{#ARTIST}}
    "artist": {
        "id": {{UID}},
        "name": "{{USERNAME:o}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
    },
    {{/ARTIST}}
    "link": "{{EQBEATS_URL}}/track/{{TID}}",
    "download": {
        {{#HAS_ART}}"art": "{{EQBEATS_URL}}/track/{{TID}}/art",{{/HAS_ART}}
        "mp3": "{{EQBEATS_URL}}/track/{{TID}}/mp3",
        "vorbis": "{{EQBEATS_URL}}/track/{{TID}}/vorbis"
    }
}
