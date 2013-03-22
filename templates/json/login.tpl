{
    {{#LOGGED_USER}}
    "sid": "{{SID:o}}",
    "user": {
        "id": {{UID}},
        "name": "{{USERNAME:o}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
    }
    {{/LOGGED_USER}}
    {{#ERROR}}
    "error": "{{ERROR:o}}"
    {{/ERROR}}
}
