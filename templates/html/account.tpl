<h2>Your account</h2>

{{#MESSAGE}}<div class="message">{{MESSAGE}}</div>{{/MESSAGE}}
{{#ERROR}}<div class="error">{{ERROR}}</div>{{/ERROR}}

<form action="/account" method="post">
    <table>
        <tr>
            <td><label for="r_name">Display name:</label></td>
            <td><input id="r_name" name="name" value="{{USERNAME}}" maxlength="200" /></td>
        </tr>
        <tr>
            <td><label for="r_email">Email:</label></td>
            <td><input id="r_email" name="email" value="{{EMAIL}}" /></td>
        </tr>
        <tr>
            <td><label for="r_notify">Email notifications:</label></td>
            <td><input id="r_notify" name="notify" type="checkbox" {{#NOTIFY}}checked="checked"{{/NOTIFY}} /></td>
        </tr>
        <tr>
            <td><label for="r_about">Description:</label><br />
            <span class="legend">(tags: [b]old, [u]nderline, [i]talic)</span>
            <td><textarea id="r_about" name="about">{{#HAS_ABOUT}}{{ABOUT}}{{/HAS_ABOUT}}</textarea></td>
        </tr>
        <tr>
            <td><label for="r_license">Default license:</label></td>
            <td><b>{{LICENSE}}</b> <a href="/account/license">(change)</a></td>
        </tr>
        <tr>
            <td><label for="r_oldpw">Old password:</label></td>
            <td><input type="password" id="r_oldpw" name="oldpw" value="{{OLD_PASSWORD}}" /></td>
        </tr>
        <tr>
            <td><label for="r_newpw">New password:</label></td>
            <td><input type="password" id="r_newpw" name="newpw" /></td>
        </tr>
        <tr>
            <td><label for="r_newpwconf">Confirm new password:</label></td>
            <td><input type="password" id="r_newpwconf" name="newpwconf" /></td>
        </tr>
    </table>
    <input type="submit" value="Update" />
</form>

{{#YOUTUBE}}
<form action="/oauth/yt/unlink">
    Your YouTube account is linked. <a class="huh" href="/faq#youtube">Huh?</a><br/>
    <input type="submit" value="Unlink your youtube account" />
</form>
{{/YOUTUBE}}

{{#NO_YOUTUBE}}
{{! https://accounts.google.com/o/oauth2/auth?response_type=code&client_id=767490682254.apps.googleusercontent.com&approval_prompt=force&scope=https%3A%2F%2Fuploads.gdata.youtube.com%2Ffeeds%2Fapi%2Fusers%2Fdefault%2Fuploads&access_type=offline&redirect_uri=http%3A%2F%2Feqbeats.org%2Foauth%2Fyt }}
<form action="https://accounts.google.com/o/oauth2/auth">
    Your YouTube account is not linked. <a class="huh" href="/faq#youtube">Huh?</a><br/>
    <input type="submit" value="Link your youtube account"/>
    <input type="hidden" name="response_type" value="code"/>
    <input type="hidden" name="client_id" value="767490682254.apps.googleusercontent.com"/>
    <input type="hidden" name="scope" value="https://uploads.gdata.youtube.com/feeds/api/users/default/uploads"/>
    <input type="hidden" name="access_type" value="offline"/>
    <input type="hidden" name="redirect_uri" value="http://eqbeats.org/oauth/yt"/>
    <input type="hidden" name="approval_prompt" value="force"/>
</form>
{{/NO_YOUTUBE}}
