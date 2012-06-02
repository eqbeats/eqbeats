<div id="{{LIST}}-{{COUNT}}" class="player"></div>
<script>
    registerTrack({
        id: "{{LIST}}-{{COUNT}}",
        mp3: "{{MP3_URL}}?stream",
        vorbis: "{{VORBIS_URL}}?stream",
        list: "{{LIST}}"
    });
</script>

{{#FALLBACK}}
<noscript>"
    <audio controls>
        <source type="audio/ogg" src="{{VORBIS_URL}}?stream" />
        <source type="audio/mpeg" src="{{VORBIS_URL}}?stream" />
    </audio>
</noscript>
{{/FALLBACK}
