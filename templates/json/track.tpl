{
    "id": {{TID}},
    "title": "{{TITLE:o}}",
    "link": "{{EQBEATS_URL}}/track/{{TID}}",
    {{#HAS_NOTES}}
    "description": "{{NOTES:o}}",
    "html_description": "{{NOTES:x-format:o}}",
    {{/HAS_NOTES}}
    {{#HAS_TAGS}}"tags": [{{#TAG}}"{{TAG:o}}"{{#TAG_separator}},{{/TAG_separator}}{{/TAG}}],{{/HAS_TAGS}}
    {{#HAS_LICENSE}}"license": "{{LICENSE:o}}",{{/HAS_LICENSE}}
    {{#ARTIST}}
    "artist": {
        "id": {{UID}},
        "name": "{{USERNAME:o}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
    },
    {{/ARTIST}}
    "download": {
        {{#HAS_ART}}"art": "{{EQBEATS_URL}}/track/{{TID}}/art",{{/HAS_ART}}
        "opus": "{{EQBEATS_URL}}/track/{{TID}}/opus",
        "vorbis": "{{EQBEATS_URL}}/track/{{TID}}/vorbis",
        "aac": "{{EQBEATS_URL}}/track/{{TID}}/aac",
        "mp3": "{{EQBEATS_URL}}/track/{{TID}}/mp3",
        "original": "{{EQBEATS_URL}}/track/{{TID}}/original"
    },
    "published": {{#IS_VISIBLE}}true{{/IS_VISIBLE}}
                 {{#IS_HIDDEN}}false{{/IS_HIDDEN}},
    "status": "OK"
}
