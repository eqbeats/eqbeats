{{%AUTOESCAPE context="HTML"}}

<div class="events_wrapper">

<form class="postcomment" action="{{PATH}}/comment" method="post">
    {{#LOGGED_OUT}}<input type="text" name="name" placeholder="Name"/>{{/LOGGED_OUT}}
    <input class="honeypot" type="text" name="url" placeholder="If you can see this, don't fill it in." />
    <input type="text" name="msg" placeholder="Leave a note" />
    <input type="submit" value="Post" onclick="this.form.submit();this.disabled=true;return false;" />
</form>

<ul class="events">
    <h4><img src="/static/icons/fire-small.png" alt="" /> Recent happenings</h4>
    {{#EVENT}}
    <li class="event" name="{{EVENT_ID}}">
        <span class="date" title="{{UTC_DATE}} UTC">{{FUZZY_TIME}}</span>

        {{#PUBLISH}}
        <img src="/static/icons/disc-arrow.png" alt="" />
        {{#SOURCE}}<a href="/user/{{UID}}">{{USERNAME}}</a>{{/SOURCE}}
        published {{#TRACK}}<a href="/track/{{TID}}">{{TITLE}}</a>{{/TRACK}}.
        {{/PUBLISH}}

        {{#COMMENT}}
        <img src="/static/icons/balloon-white-left.png" alt="" />
        {{#SOURCE}}
        {{#IS_SELF}}You{{/IS_SELF}}
        {{#NOT_SELF}}<a href="/user/{{UID}}">{{USERNAME}}</a>{{/NOT_SELF}}
        {{#GUEST}}{{NAME}}{{/GUEST}}
        {{/SOURCE}}
        {{#TRACK}}posted a comment{{/TRACK}}
        {{#ON_USER}}said{{/ON_USER}}
        on
        {{#TRACK}}<a href="/track/{{TID}}">{{TITLE}}</a> by {{#TARGET}}<a href="/user/{{UID}}">{{USERNAME}}</a>{{/TARGET}}{{/TRACK}}
        {{#ON_USER}}{{#TARGET}}
        {{#IS_SELF}}your profile page{{/IS_SELF}}
        {{#NOT_SELF}}<a href="/user/{{UID}}">{{USERNAME}}'s profile</a>{{/NOT_SELF}}
        {{/TARGET}}{{/ON_USER}}
        :<p class="comment">{{MESSAGE:x-format}}</p>
        {{/COMMENT}}

        {{#FOLLOW}}
        <img src="/static/icons/plus.png" alt="" />
        {{#SOURCE}}
        {{#IS_SELF}}You{{/IS_SELF}}
        {{#NOT_SELF}}<a href="/user/{{UID}}">{{USERNAME}}</a>{{/NOT_SELF}}
        {{/SOURCE}}
        started following {{#TARGET}}<a href="/user/{{UID}}">{{USERNAME}}</a>{{/TARGET}}.
        {{/FOLLOW}}

        {{#FAVORITE}}
        <img src="/static/icons/star.png" alt="" />
        {{#SOURCE}}
        {{#IS_SELF}}You{{/IS_SELF}}
        {{#NOT_SELF}}<a href="/user/{{UID}}">{{USERNAME}}</a>{{/NOT_SELF}}
        {{/SOURCE}}
        favorited {{#TRACK}}<a href="/track/{{TID}}">{{TITLE}}</a>{{/TRACK}}
        by {{#TARGET}}<a href="/user/{{UID}}">{{USERNAME}}</a>{{/TARGET}}.
        {{/FAVORITE}}

    </li>
    {{/EVENT}}
    {{#NO_EVENT}}
    <li class="empty">Nothing here yet.</li>
    {{/NO_EVENT}}
</ul>

</div>
