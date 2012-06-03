<h2>{{NAME}}</h2>
<div class="contest">

{{#HAS_DESCRIPTION}}<div class="description">{{DESCRIPTION}}</div>{{/HAS_DESCRIPTION}}

<h3 id="submissions"><img src="{{ICON}}/disc.png" alt=""> Submission</h3>

{{#EMPTY}}
<div class="empty tracklist">No submissions yet</div>
{{/EMPTY}}

{{#NOT_EMPTY}}
<div class="submissions">
    {{#VOTING}}<form action="{{URL}}/vote" method="post">{{/VOTING}}
    <ul class="tracklist">
        {{#TRACK}}
        <li onclick="toggle(this.getElementsByClassName('player')[0]);">
            {{#HAS_COVER}}<img class="cover" alt="" src="{{THUMB_URL}}" />{{/HAS_COVER}}
            <div class="title">
                {{HAS_VOTES}}<span class="votes">{{VOTES}}</span>{{/HAS_VOTES}}
                {{#HAS_VOTE}}
                    <button type="submit" name="tid" title="Cancel vote" value="-{{TID}}"><img src="{{ICON}}/star-empty.png" alt="Cancel vote" /></button>
                {{/HAS_VOTE}}
                {{#CAN_VOTE}}
                    <button type="submit" name="tid" title="Vote" value="+{{TID}}"><img src="{{ICON}}/vote.png" alt="vote" /></button>
                {{/CAN_VOTE}}
                <a href="{{URL}}" title="{{TITLE}}">{{TITLE}}</a>
            </div>
            {{#ARTIST}}<div class="by">by <a href="{{URL}}">{{NAME}}</a></div>{{/ARTIST}}
            <div style="clear:both;"></div>
            {{>PLAYER}}
        </li>
        {{/TRACK}}
    </ul>
    {{#VOTING}}</form>{{/VOTING}}
</div>
{{/NOT_EMPTY}}

{{#CAN_SUBMIT}}
<form action="{{URL}}/submit" method="post">
    Submit a track :
    <select name="tid">
        {{#TRACK}}<option value="{{TID}}">{{TITLE}}</option>{{/TRACK}}
    </select>
    <input type="submit" value="Submit" />
</form>

</div>
