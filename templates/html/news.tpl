<div class="news">
    <h2>{{TITLE}}</h2>
    <h4>on {{DATE}}</h4>
    <div class="news-contents">{{CONTENTS}}</div>
</div>

<h3 id="comments"><img src="{{ICON}/balloon.png" /> {{TITLE}}</h3>
<div>
    {{#EMPTY}}<div class="empty">No comments yet.</div>{{/EMPTY}}
    {{#COMMENT}}
    <div class="comment">
        {{CONTENTS}}<br />
        <div class="by">by {{#AUTHOR_HAS_URL}}<a href="{{AUTHOR_URL}}">{{/AUTHOR_HAS_URL}}{{AUTHOR_NAME}}{{#AUTHOR_HAS_URL}}</a>{{/AUTHOR_HAS_URL}}</div>
    </div>
</div>
{{>COMMENT_FORM}}
