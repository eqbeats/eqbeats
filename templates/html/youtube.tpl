<div class="ytpreview">

    <h2>{{#ARTIST}}{{NAME}}{{/ARTIST}} - {{TRUNC_TITLE}}</h2>

    <div class="ytplayer">
       {{#HAS_COVER}}<img src="{{COVER_URL}}" />{{/HAS_COVER}}
       {{#NO_COVER}}<img src="/static/placeholder.jpg" />{{/NO_COVER}}
       <div class="overlay"></div>
    </div>

    <p>
        {{DESCRIPTION}}
        <br />
        --
        <br />
        <a href="{{URL}}">{{URL}}</a><br />
        Download: <a href="{{MP3_URL}}">{{MP3_URL}}</a>
    </p>

    <div class="tags">
        Tags :<br />
        {{#TAG}}<span class="tag">{{TAG_NAME}}</span>{{/TAG}}
    </div>

</div>

<div class="ytsidebar">
    This is what your video will look like. If you have cover art, make sure you upload it before you continue.
    <form method="post">
        <input type="submit" value="Continue" /> &nbsp; <a class="danger" href="{{URL}}">Cancel</a>
    </form>
</div>

<div style="clear:both;"></div>
