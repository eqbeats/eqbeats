<form class="postcomment" action="{{ACTION}}" method="post">

    {{#QUICK}}
    {{#LOGGED_OUT}}<input type="text" name="name" placeholder="Name"/>{{/LOGGED_OUT}}
    <input class="honeypot" type="text" name="url" placeholder="If you can see this, don't fill it in" />
    <input type="text" name="msg" placeholder="Leave a note" />
    <input type="submit" value="Post" onclick="this.form.submit();this.disabled=true;return false;" />
    {{/QUICK}}

    {{#LARGE}}
    {{#LOGGED_OUT}}Name : <input type="text" name="name" /><br />{{/LOGGED_OUT}}
    <input class="honeypot" type="text" name="url" placeholder="If you can see this, don't fill it in" />
    <textarea name="msg"></textarea><br />
    <input type="submit" value="Post a comment" onclick="this.form.submit();this.disabled=true;return false;" />
    {{/LARGE}}

</form>
