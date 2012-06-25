<div id="{{LIST}}-{{COUNT}}" class="player"></div>
<script>
    registerTrack({
        id: "{{LIST}}-{{COUNT}}",
        mp3: "/track/{{TID}}/mp3?stream",
        vorbis: "/track/{{TID}}/vorbis?stream",
        list: "{{LIST}}"
    });
</script>

{{#FALLBACK}}
<noscript>
    <audio controls>
        <source type="audio/ogg" src="/track/{{TID}}/vorbis?stream" />
        <source type="audio/mpeg" src="/track/{{TID}}/mp3?stream" />
    </audio>
</noscript>
{{/FALLBACK}}
