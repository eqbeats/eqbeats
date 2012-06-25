<div class="playlist">

    <h2>{{NAME}}</h2>
    {{#AUTHOR}}by <a href="{{URL}}">{{NAME}}</a></h4>

    {{#HAS_DESCRIPTION}}
    <div class="notes">{{DESCRIPTION}}</div>
    {{/HAS_DESCRIPTION}}

    {{>TRACKLIST}}

    {{#EDITION}}
    <div class="edit">
        <h3><img src="{{ICON}}/pencil.png" /> Edit</h3>
        <form method="post" action="{{URL}}/edit">
            <table>
                <tr>
                    <td><img src="{{ICON}}/rename.png"> Name:</td>
                    <td><input type="text" name="name" value="{{NAME}}" /></td>
                </tr>
                <tr>
                    <td>
                        <img src="{{ICON}}/card_pencil.png"> Notes:
                        <div class=\"legend\">(tags: [b]old, [u]nderline, [i]talic)</div>
                    </td>
                    <td><textarea name="desc">{{DESCRIPTION}}</textarea></td>
                </tr>
                <tr>
                    <td></td>
                    <td><input type="submit" value="Update" /></td>
                </tr>
            </table>
        </form>
        <a class="delete" href="{{URL}}/delete">Delete playlist</a>
        <div style="clear:both;"></div>
    </div>
    {{/EDITION}}

</div>
