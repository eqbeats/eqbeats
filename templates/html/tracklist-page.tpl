{{%AUTOESCAPE context="HTML"}}
<h2>
    {{TITLE}}
    {{#FEED}}<a class="feed" title="Feed" href="{{FEED_URL}}"><span>(Feed)</span></a>{{/FEED}}
</h2>

{{#SEARCH}}
<div class="search">
    <form action="/tracks/search">
        <img src="/static/icons/magnifier.png" alt="Search" />
        <input type="text" name="q" value="{{SEARCH}}" />
        <input type="submit" value="Search" />
    </form>
</div>
{{/SEARCH}}

{{>TRACKLIST}}

{{#HAS_PREV}}
<a href="?p={{PREV}}">&laquo; Newer tracks</a>
{{#HAS_NEXT}} - {{/HAS_NEXT}}
{{/HAS_PREV}}
{{#HAS_NEXT}}
<a href="?p={{NEXT}}">Older tracks &raquo;</a>
{{/HAS_NEXT}}
