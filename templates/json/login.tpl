{
    "status": "OK",
    "sid": "{{SID:o}}"
    {{#LOGGED_USER}},
    "user": {
        "id": {{UID}},
        "name": "{{USERNAME:o}}",
        "link": "{{EQBEATS_URL}}/user/{{UID}}"
    }
    {{/LOGGED_USER}}
}
