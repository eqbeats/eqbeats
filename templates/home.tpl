<div id="newsticker">
    <img src="{{ICON}}/newspaper.png" />
    <b>Latest news</b>: <a href="{{LATEST_NEWS_URL}}">{{LATEST_NEWS_TITLE}}</a>
</div>
<script>
    var news = [
        {{#NEWS}}
        { title: "{{TITLE}}", url: "{{URL}}" }
        {{#NEWS_separator}},{{/NEWS_separator}}
        {{/NEWS}}
    ];
</script>
<script src="/static/ticker.js></script>

<div class="search">
    <img src="{{ICON}}/magnifier.png" alt="" />
    Search for tracks:
    {{>SEARCH_FORM}}
</div>

<div class="leftcol">
    <h3><a href="/tracks/latest">Latest</a> {{>FEED_ICON_LATEST}}</h3>
    {{>LATEST_TRACKS}}
    <a class="more" href="/tracks/latest">More</a>
</div>

<div class="rightcol">
    <h3><a href="/tracks/featured">Featured</a> {{>FEED_ICON_FEATURED}}</h3>
    {{>FEATURED_TRACKS}}
    <a class="more" href="/tracks/featured">More</a>
</div>

<div class="leftcol">
    <h3><a href="/tracks/random">Random</a> {{>FEED_ICON_RANDOM}}</h3>
    {{>RANDOM_TRACKS}}
    <a class="more" href="/tracks/random">More</a>
</div>

<div class="rightcol">
    <h3>Partners</h3>
    <a href="http://ponify.me" target="_blank" class="partner"><img src="/static/cr.png" alt="Celestia Radio" /></a>
</div>

<div style="clear:both;"></div>
