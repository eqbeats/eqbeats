<h2>{{TITLE}}</h2>

{{>TRACKLIST}}

{{#HAS_PREV}}
<a href="?p={{PREV}}">&laquo; Newer tracks</a>
{{#HAS_NEXT}} - {{/HAS_NEXT}}
{{/HAS_PREV}}
{{#HAS_NEXT}}
<a href="?p={{NEXT}}">Older tracks &raquo;</a>
{{/HAS_NEXT}}
