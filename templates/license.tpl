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
        <td><input name="license" type="radio" value="CC BY-NC" id="lnc" {{#CC-BY-NC}}checked="checked"{{/CC-BY-NC}} /></td>
        <td>
            <b><label for="lnc">Creative Commons: Attribution-NonCommercial (CC BY-NC)</label></b>
            <div>
                &ldquo;This license lets others remix, tweak, and build upon your work non-commercially, and although their new works must also acknowledge you and be non-commercial, they don’t have to license their derivative works on the same terms.&rdquo;<br />
                <a href="http://creativecommons.org/licenses/">More about Creative Commons licenses.</a><br /><br />
                Other CC licenses:
                <input name="license" type="radio" value="CC BY" id="lby" {{#CC-BY-NC}}checked="checked"{{/CC-BY-NC}} /> <label for="lby">BY</label>
                <input name="license" type="radio" value="CC BY-SA" id="lsa" {{#CC-BY-SA}}checked="checked"{{/CC-BY-SA}} /> <label for="lsa">BY-SA</label>
                <input name="license" type="radio" value="CC BY-ND" id="lnd" {{#CC-BY-ND}}checked="checked"{{/CC-BY-ND}} /> <label for="lnd">BY-ND</label>
                <input name="license" type="radio" value="CC BY-NC-SA" id="lncsa" {{#CC-BY-NC-SA}}checked="checked"{{/CC-BY-NC-SA}} /> <label for="lncsa">BY-NC-SA</label>
                <input name="license" type="radio" value="CC BY-NC-ND" id="lncnd" {{#CC-BY-NC-ND}}checked="checked"{{/CC-BY-NC-ND}} /> <label for="lncnd">BY-NC-ND</label>
            </div>
        </td>
    </tr>

    <tr>
        <td><input name="license" type="radio" value="Public domain" id="lpub" {{#PUBLIC}}checked="checked"{{/PUBLIC}} /></td>
        <td>
            <b><label for="lpub">Public Domain</label></b>
            <div>Very permissive. Use that if you make your music for fun and don't care about what people do with it.</div>
        </td>
    </tr>

    <tr>
        <td><input name="license" type="radio" value="custom" id="lcus" {{#CUSTOM}}checked="checked" {{/CUSTOM}} /></td>
        <td>
            <b><label for="lcus">Custom:</label> </b>
            <input name="custom-license" " << (found?"":"value=""+Html::escape(t.license())+"" ") << "/>
        </td>
    </tr>

    {{#TRACK}}
    <tr>
        <td><input name="mkdefault" type="checkbox" id="ldef" /></td>
        <td><label for="ldef">Make it the default license.</label></td>
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
