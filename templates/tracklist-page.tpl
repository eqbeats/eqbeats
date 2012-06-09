<h2>
    {{TITLE}}
    {{#HAS_FEED}}<a class="feed" title="Feed" href="{{FEED_URL}}"><span>(Feed)</span></a>{{/HAS_FEED}}
</h2>

{{>TRACKLIST}}

{{#HAS_PREV}}
<a href="?p={{PREV}}">&laquo; Newer tracks</a>
{{#HAS_NEXT}} - {{/HAS_NEXT}}
{{/HAS_PREV}}
{{#HAS_NEXT}}
<a href="?p={{NEXT}}">Older tracks &raquo;</a>
{{/HAS_NEXT}}
