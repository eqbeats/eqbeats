{
    "id": {{TID}},
    "title": "{{TITLE:o}}",
    {{#HAS_NOTES}}
    "description": "{{NOTES:o}}",
    "html_description": "{{NOTES:x-format:o}}",
    {{/HAS_NOTES}}
    {{#HAS_TAGS}}"tags": [{{#TAG}}"{{TAG:o}}"{{#TAG_separator}},{{/TAG_separator}}{{/TAG}}],{{/HAS_TAGS}}
    {{#HAS_LICENSE}}"license":
    "
	{{#COPYRIGHT}}Copyright \u00a9 {{USERNAME:o}}{{/COPYRIGHT}}
	{{#OTHER_LICENSE}}
		{{#CC_BY}}CC BY (creativecommons.org/licenses/by/3.0/){{/CC_BY}}
		{{#CC_BY_NC}}CC BY-NC (creativecommons.org/licenses/by-nc/3.0/){{/CC_BY_NC}}
		{#CC_BY_NC}}CC BY-NC (creativecommons.org/licenses/by-nc/3.0/){{/CC_BY_NC}}
		{{#CC_BY_SA}}CC BY-SA (creativecommons.org/licenses/by-sa/3.0/){{/CC_BY_SA}}
		{{#CC_BY_ND}}CC BY-ND (creativecommons.org/licenses/by-nd/3.0/){{/CC_BY_ND}}
		{{#CC_BY_NC_SA}}CC BY-NC-SA (creativecommons.org/licenses/by-nc-sa/3.0/){/CC_BY_NC_SA}}
		{{#CC_BY_NC_ND}}CC BY-NC-ND (creativecommons.org/licenses/by-nc-nd/3.0/){{/CC_BY_NC_ND}}
		{{#PUBLIC}}Public domain{{/PUBLIC}}
		{{#CUSTOM}}{{LICENSE:o}}{{/CUSTOM}}
	{{/OTHER_LICENSE}}
    ",{{/HAS_LICENSE}}
    "artist": {
        "id": {{UID}},
        "name": "{{USERNAME:o}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
        {{#HAS_AVATAR}},
        "avatar": "https://secure.gravatar.com/avatar/{{EMAIL_MD5}}?d=http%3A%2F%2Feqbeats.org%2Fstatic%2Favatar.png"
        {{/HAS_AVATAR}}
    },
    "link": "{{EQBEATS_URL}}/track/{{TID}}",
    "download": {
        {{#HAS_ART}}"art": "{{EQBEATS_URL}}/track/{{TID}}/art",{{/HAS_ART}}
        "opus": "{{EQBEATS_URL}}/track/{{TID}}/opus",
        "vorbis": "{{EQBEATS_URL}}/track/{{TID}}/vorbis",
        "aac": "{{EQBEATS_URL}}/track/{{TID}}/aac",
        "mp3": "{{EQBEATS_URL}}/track/{{TID}}/mp3",
        "original": "{{EQBEATS_URL}}/track/{{TID}}/original"
    },
    "stream": {
        "opus": "{{EQBEATS_URL}}/track/{{TID}}/opus?stream=1",
        "vorbis": "{{EQBEATS_URL}}/track/{{TID}}/vorbis?stream=1",
        "aac": "{{EQBEATS_URL}}/track/{{TID}}/aac?stream=1",
        "mp3": "{{EQBEATS_URL}}/track/{{TID}}/mp3?stream=1"
    },
    {{#HAS_STATS}}
    "stats": {{>STATS}},
    {{/HAS_STATS}}
    "timestamp": {{TIMESTAMP}}
}
