<div id="track-uploader">
    <form action="{{ACTION}}?js=no" method="post" enctype="multipart/form-data" id="track-uploader-form">
        <input type="file" name="qqfile" />
        <input type="submit" value="Upload a song" />
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
    window.onload = createUploader;
</script>
