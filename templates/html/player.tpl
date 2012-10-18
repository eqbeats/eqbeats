<div id="{{LIST}}-{{COUNT}}" class="player"></div>
<script>
    registerTrack({
        id: "{{LIST}}-{{COUNT}}",
        mp3: "/track/{{TLINK}}/mp3?stream",
        vorbis: "/track/{{TLINK}}/vorbis?stream",
        list: "{{LIST}}"
    });
</script>

{{#FALLBACK}}
<noscript>
    <audio controls>
        <source type="audio/ogg" src="/track/{{TLINK}}/vorbis?stream" />
        <source type="audio/mpeg" src="/track/{{TLINK}}/mp3?stream" />
    </audio>
</noscript>
{{/FALLBACK}}
