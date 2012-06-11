<h2>{{TITLE}}</h2>

<div class="search">
    <form action="/users/search">
        <img src="/static/icons/magnifier.png" alt="Search" />
        <input type="text" name="q" />
        <input type="submit" value="Search" />
    </form>
</div>

{{>USERLIST}}

{{#HAS_PREV}}
<a href="?p={{PREV}}">&laquo; Previous page</a>
{{#HAS_NEXT}} - {{/HAS_NEXT}}
{{/HAS_PREV}}
{{#HAS_NEXT}}
<a href="?p={{NEXT}}">Next page &raquo;</a>
{{/HAS_NEXT}}
