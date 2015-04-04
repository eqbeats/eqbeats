{{%AUTOESCAPE context="HTML"}}
<h2>Your account</h2>

{{#MESSAGE}}<div class="message">{{MESSAGE}}</div>{{/MESSAGE}}
{{#ERROR}}<div class="error">{{ERROR}}</div>{{/ERROR}}

<form action="/account" method="post">
    <table>
        <tr>
            <td><label for="r_name">Display name:</label>
            <br/><span class="legend">(will show up in artist tag for your tracks)</span>
            </td>

            <td><input id="r_name" name="name" value="{{USERNAME}}" maxlength="200" /></td>
        </tr>
        <tr>
            <td><label for="r_email">Email:</label>
            <br/><span class="legend">(publicly visible)</span>
            </td>
            <td><input id="r_email" name="email" value="{{EMAIL}}" /></td>
        </tr>
        <tr>
            <td><label for="r_notify">Email notifications:</label>
            <br/><span class="legend">(for followed artists' releases, comments)</span>
            </td>
            <td><input id="r_notify" name="notify" type="checkbox" {{#NOTIFY}}checked="checked"{{/NOTIFY}} /></td>
        </tr>
        <tr>
            <td><label for="r_about">Description:</label><br />
            <span class="legend">(tags: [b]old, [u]nderline, [i]talic)</span>
            <td><textarea id="r_about" name="about">{{#HAS_ABOUT}}{{ABOUT:pre_escape}}{{/HAS_ABOUT}}</textarea></td>
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
    <input name="nonce" type="hidden" value="{{NONCE}}"/>
    <input type="submit" value="Update" />
</form>

<h4><img src="/static/icons/cross.png" alt="" style="margin-top: 0px;" /> Delete</h4>
<form action="/account/delete" method="get">
    <input type="submit" value="Delete your account"/>
</form>
