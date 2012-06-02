<div class="news">
    <h2>Latest news :
        <a href="{{LATEST_NEWS_URL}}">{{LATEST_NEWS_TITLE}}</a>
    </h2>
    <h4>on {{LATEST_NEWS_DATE}}</h4>
    <div class="news-contents">{{LATEST_NEWS_CONTENTS}}</div>
</div>
<a class="more" href="{{LATEST_NEWS_URL}}#comments">Comments ({{COMMENT_COUNT}})</a>

<div class="oldnews">
    <h3><img src="{{ICON}}/newspaper-24.png" /> Older news</h3>
    <ul>
        {{NEWS}}
        <li><a href="{{URL}}">{{TITLE}}</a></li>
        {{/NEWS}}
    </ul>
</div>
