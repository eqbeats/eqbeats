<div id="track-uploader">
    <form action="{{ACTION}}?js=no" method="post" enctype="multipart/form-data" id="track-uploader-form">
        <input type="file" name="qqfile" />
        <input type="submit" value="Upload a song" /><br/>
        <a class="huh" href="/faq#formats">(A note about formats)</a>
    </form>
</div>
<script src="/static/fileuploader.js" type="text/javascript"></script>
<script>
    function createUploader(){
        var uploader = new qq.FileUploader({
            element: document.getElementById('track-uploader'),
            action: '{{ACTION}}'
        });
    }
    function noop(){}

    var oldonload = window.onload || noop;
    window.onload = function(){ this(); createUploader(); }.bind(oldonload);
</script>
