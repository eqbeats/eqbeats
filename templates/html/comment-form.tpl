<form class="postcomment" action="{{ACTION}}" method="post">
    {{#LOGGED_OUT}}Name : <input type="text" name="name" /><br />{{/LOGGED_OUT}}
    <input class="honeypot" type="text" name="url" placeholder="If you can see this, don't fill it in" />
    <textarea name="msg"></textarea><br />
    <input type="submit" value="Post a comment" onclick="this.form.submit();this.disabled=true;return false;" />
</form>
