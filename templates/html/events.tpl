<ul class="events">
    {{#HAS_HEADER}}
    <h4><img src="/static/icons/fire-small.png" alt="" />{{HEADER:none}}</h4>
    {{/HAS_HEADER}}
    {{#EVENTS}}
    <li class="event" name="{{EVENT_ID}}">
        <span class="date" title="{{UTCDATE}} UTC">{{DATE}}</span>
            {{#PUBLISH}}
            <img src="/static/icons/disc-arrow.png" alt="" />
            <a href="/user/{{SOURCE_ID}}">{{SOURCE_NAME}}</a> published <a href="/track/{{TRACK_ID}}">{{TRACK_NAME}}</a>
            {{/PUBLISH}}

            {{#COMMENT}}
            <img src="/static/icons/balloon-white-left.png" alt="" />
            {{#SOURCE_SELF}}You{{/SOURCE_SELF}}
            {{#SOURCE_USER}}<a href="/user/{{SOURCE_ID}}">{{SOURCE_NAME}}</a>{{/SOURCE_USER}}
            {{#SOURCE_GUEST}}{{SOURCE_NAME}}{{/SOURCE_GUEST}}
            {{#ANONYMOUS}}Anonymous{{/ANONYMOUS}}

            {{#ON_TRACK}}posted a comment{{/ONTRACK}}
            {{#ON_USER}}said{{/ON_USER}}
            {{#CONTEXT}}
            on
            {{#ON_TRACK}}<a href="/track/{{TID}}">{{TRACK_NAME}}</a> by <a href="/user/{{ARTIST_ID}}">{{ARTIST_NAME}}</a>{{/ONTRACK}}
            {{#ON_USER}}
            <a href="/user/{{TARGET_ID}}">
            {{#ON_SELF}} your profile page {{/ON_SELF}}
            {{#ON_OTHER}} {{TARGET_NAME}}'s profile {{/ON_OTHER}}
            </a>
            {{/ON_USER}}
            {{/CONTEXT}}
            :<p class="comment">{{MESSAGE}}</p>
            {{/COMMENT}}

            {{#FOLLOW}}
            <img src="/static/icons/plus.png" alt="" />
            {{#SOURCE_SELF}}You{{/SOURCE_SELF}}
            {{#SOURCE_USER}}<a href="/user/{{SOURCE_ID}}">{{SOURCE_NAME}}</a>{{/SOURCE_USER}}
            started following <a href="/user/{{TARGET_ID}}">{{TARGET_NAME}}</a>
            {{/FOLLOW}}

            {{#FAVORITE}}
            <img src="/static/icons/star.png" alt="" />
            {{#SOURCE_SELF}}You{{/SOURCE_SELF}}
            {{#SOURCE_USER}}<a href="/user/{{SOURCE_ID}}">{{SOURCE_NAME}}</a>{{/SOURCE_USER}}
            favorited <a href="/track/{{TRACK_ID}}">{{TRACK_NAME}}</a>
            by <a href="/user/{{TARGET_ID}}">{{TARGET_NAME}}</a>
            {{/FAVORITE}}
            .
    </li>
    {{/EVENTS}}
    {{#NO_EVENT}}
    <li class="empty">Nothing here yet.</li>
    {{/NO_EVENT}}
</ul>
