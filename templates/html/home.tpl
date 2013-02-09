{{#LATEST_NEWS}}
<div id="newsticker">
    <img src="/static/icons/newspaper.png" />
    <b>Latest news</b>: <a href="{{NEWS_URL}}">{{NEWS_TITLE}}</a>
</div>
{{/LATEST_NEWS}}
{{#DYNAMIC_TICKER}}
<script>
    var news = [
        {{#NEWS}}
        { title: "{{NEWS_TITLE}}", url: "{{NEWS_URL}}" }
        {{#NEWS_separator}}, {{/NEWS_separator}}
        {{/NEWS}}
    ];
</script>
<script src="/static/ticker.js"></script>
{{/DYNAMIC_TICKER}}

<div class="featurebox">
    <h3>
        <a href="/tracks/featured">Featured</a>
    </h3>
    {{>FEATURED_TRACKS}}
</div>

<div class="leftcol">
    <h3>
        <a href="/tracks/latest">Latest</a>
        <a class="feed" title="Feed" href="/tracks/latest/atom"><span>(Feed)</span></a>
    </h3>
    {{>LATEST_TRACKS}}
    <a class="more" href="/tracks/latest">More</a>
</div>

<div class="rightcol">
    <h3><a href="/tracks/random">Random</a></h3>
    {{>RANDOM_TRACKS}}
    <a class="more" href="/tracks/random">More</a>
</div>

<div style="clear:both;"></div>
