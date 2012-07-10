<h2>Reset your password</h2>

{{#MESSAGE}}<div class="message">{{MESSAGE}}</div>{{/MESSAGE}}
{{#ERROR}}<div class="error">{{ERROR}}</div>{{/ERROR}}

<form action="/account/reset" method="post">
    Email: <input name="email" /> <input type="submit" value="Reset" />
</form>
