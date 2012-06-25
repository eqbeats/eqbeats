<h2>Register</h2>

{{#ERROR}}<div class="error">{{ERROR}}</div>{{/ERROR}}

<form action="/register" method="post">

<table>
    <tr>
        <td><label for="r_name">Display name:</label></td>
        <td><input id="r_name" name="name" /></td>
    </tr>
    <tr>
        <td><label for="r_email">Email:</label></td>
        <td><input id="r_email" name="email" /></td>
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
