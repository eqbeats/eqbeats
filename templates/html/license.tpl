{{%AUTOESCAPE context="HTML"}}
<h2>
    {{#TRACK}}License for <a href="{{URL}}">{{TITLE}}</a>{{/TRACK}}
    {{#NO_TRACK}}Global license{{/NO_TRACK}}
</h2>

<form method="post">
<table class="licenses">

    <tr>
        <td><input name="license" type="radio" value="Copyright" id="lc" {{#COPYRIGHT}}checked="checked"{{/COPYRIGHT}} /></td>
        <td>
            <b><label for="lc">Copyright</label></b>
            <div>Default license. Most restrictive.</div>
        </td>
    </tr>

    <tr>
        <td><input name="license" type="radio" value="CC BY-NC" id="lnc" {{#CC_BY_NC}}checked="checked"{{/CC_BY_NC}} /></td>
        <td>
            <b><label for="lnc">Creative Commons: Attribution-NonCommercial (CC BY-NC)</label></b>
            <div>
                &ldquo;This license lets others remix, tweak, and build upon your work non-commercially, and although their new works must also acknowledge you and be non-commercial, they don’t have to license their derivative works on the same terms.&rdquo;<br />
                <a href="http://creativecommons.org/licenses/">More about Creative Commons licenses.</a><br /><br />
                Other CC licenses:
                <input name="license" type="radio" value="CC BY" id="lby" title="Attribution" {{#CC_BY}}checked="checked"{{/CC_BY}} /> <label for="lby">BY</label>
                <input name="license" type="radio" value="CC BY-SA" id="lsa" title="Attribution-ShareAlike" {{#CC_BY_SA}}checked="checked"{{/CC_BY_SA}} /> <label for="lsa">BY-SA</label>
                <input name="license" type="radio" value="CC BY-ND" id="lnd" title="Attribution-NoDerivs" {{#CC_BY_ND}}checked="checked"{{/CC_BY_ND}} /> <label for="lnd">BY-ND</label>
                <input name="license" type="radio" value="CC BY-NC-SA" id="lncsa" title="Attribution-NonCommercial-ShareAlike" {{#CC_BY_NC_SA}}checked="checked"{{/CC_BY_NC_SA}} /> <label for="lncsa">BY-NC-SA</label>
                <input name="license" type="radio" value="CC BY-NC-ND" id="lncnd" title="Attribution-NonCommercial-NoDerivs" {{#CC_BY_NC_ND}}checked="checked"{{/CC_BY_NC_ND}} /> <label for="lncnd">BY-NC-ND</label>
            </div>
        </td>
    </tr>

    <tr>
        <td><input name="license" type="radio" value="Public domain" id="lpub" {{#PUBLIC}}checked="checked"{{/PUBLIC}} /></td>
        <td>
            <b><label for="lpub">Public Domain</label></b>
            <div>"No rights reserved". Use it if you make your music for fun and want everyone to make the most of it.</div>
        </td>
    </tr>

    <tr>
        <td><input name="license" type="radio" value="custom" id="lcus" {{#CUSTOM}}checked="checked" {{/CUSTOM}} /></td>
        <td>
            <b><label for="lcus">Custom:</label> </b>
            <input name="custom-license" value="{{CUSTOM}}"/>
        </td>
    </tr>

    {{#TRACK}}
    <tr>
        <td><input name="mkdefault" type="checkbox" id="ldef" /></td>
        <td><label for="ldef">Make it the default license for new tracks.</label></td>
    </tr>
    {{/TRACK}}

    <tr>
        <td><input name="retro" type="checkbox" id="lall"/></td>
        <td><label for="lall">Apply to all tracks.</label></td>
    </tr>

    <tr>
        <td colspan="2"><input type="submit" value="Update" /></td>
    </tr>

</table>
</form>
