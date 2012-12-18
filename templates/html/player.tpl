<div id="{{LIST}}-{{COUNT}}" class="player"></div>
<script>
    registerTrack({
        id: "{{LIST}}-{{COUNT}}",
        tid: "{{TID}}",
        mp3: "/track/{{TID}}/mp3?stream=1",
        vorbis: "/track/{{TID}}/vorbis?stream=1",
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
