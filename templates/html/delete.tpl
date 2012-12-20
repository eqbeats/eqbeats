<form method="post">
    Do you really want to delete <b>{{WHAT}}</b> ?
    <input type="submit" value="Delete" name="confirm" />
    <input name="nonce" type="hidden" value="{{NONCE}}"/>
</form>
<a class="danger" href="{{CANCEL_URL}}">Cancel</a>
