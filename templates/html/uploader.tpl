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
        var elem = document.getElementById('track-uploader');
        var uploader = new qq.FileUploader({
            element: elem,
            action: '{{ACTION}}',
            template: '<div class="qq-uploader">' +
                    '<div class="qq-upload-drop-area"><span>Drop files here to upload</span></div>' +
                    '<div class="qq-upload-button">Upload a song</div>' +
                    '<a class="huh" href="/faq#formats">(A note about formats)</a>' +
                    '<ul class="qq-upload-list"></ul>' +
                 '</div>',
            fileTemplate: '<li>' +
                    '<a class="qq-upload-file"></a>' +
                    '<span class="qq-upload-spinner"></span>' +
                    '<span class="qq-upload-size"></span>' +
                    '<a class="qq-upload-cancel" href="#">Cancel</a>' +
                    '<span class="qq-upload-failed-text">Failed</span>' +
                '</li>',
            onComplete: function(id, fileName, result){
                var list = qq.getByClass(elem, 'qq-upload-list')[0];
                if(list.childNodes.length == 1)
                    document.location = "/track/" + result.track;
                var item;
                for (item = list.firstChild; item && item.qqFileId != id; item = item.nextItem);
                var f = qq.getByClass(item, 'qq-upload-file')[0];
                f.href = '/track/' + result.track;
                var t = document.createTextNode(result.title);
                f.replaceChild(t, f.firstChild);
                var tracklist = document.getElementsByClassName("tracklist")[0];
                if(tracklist != undefined){
                    if(tracklist.tagName.toLowerCase() == "div"){
                        var ul = document.createElement("ul");
                        ul.className = "tracklist";
                        tracklist.parentNode.replaceChild(ul, tracklist)
                        tracklist = ul;
                    }
                    var track = document.createElement("li");
                    track.className = "hidden";
                    track.appendChild(f.cloneNode(true));
                    track.firstChild.className = "";
                    tracklist.insertBefore(track, tracklist.firstChild);
                }
            }
        });
    }
    function noop(){}

    var oldonload = window.onload || noop;
    window.onload = function(){ this(); createUploader(); }.bind(oldonload);
</script>
