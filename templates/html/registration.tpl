{{%AUTOESCAPE context="HTML"}}
<h2>Register</h2>

{{#ERROR}}<div class="error">{{ERROR}}</div>{{/ERROR}}

<form action="/register" method="post">

<table>
    <tr>
        <td><label for="r_name">Display name:</label><br/>
        <span class="legend">(can be changed at any time)</span></td>
        <td><input id="r_name" name="name" value="{{NAME}}" maxlength="200" /></td>
    </tr>
    <tr>
        <td><label for="r_email">Email:</label><br/>
        <span class="legend">(will be public)</span></td>
        <td><input id="r_email" name="email" value="{{EMAIL}}" /></td>
    </tr>
    <tr>
        <td><label for="r_pw">Password:</label></td>
        <td><input type="password" id="r_pw" name="pw" /></td>
    </tr>
    <tr>
        <td><label for="r_pwconf">Confirm password:</label></td>
        <td><input type="password" id="r_pwconf" name="pwconf" /></td>
    </tr>
</table>

<input type="submit" value="Register" />

</form>

<h3>Why register?</h3>
<p>Logged in users can favorite tracks, follow users, and create playlists. They can also upload tracks of their own for everyone else to listen to.</p>
<p>We strive to make sharing and publishing music as effortless as it can be. For that reason, we do not impose limits on formats, filesize or downloads, and make sure that tracks play in the best conditions on every device or browser we can manage.</p>
