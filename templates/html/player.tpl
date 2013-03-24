<div id="{{LIST}}-{{COUNT}}" class="player"></div>
<script>
    registerTrack({
        id: "{{LIST}}-{{COUNT}}",
        tid: "{{TID}}",
        title: "{{TITLE:h}}",
        artist: "{{USERNAME:h}}",
        mp3: "/track/{{TID}}/mp3?stream=1",
        vorbis: "/track/{{TID}}/vorbis?stream=1",
        aac: "/track/{{TID}}/aac?stream=1",
        opus: "/track/{{TID}}/opus?stream=1",
        mood: "/track/{{TID}}/mood",
        list: "{{LIST}}"
    });
</script>

{{#FALLBACK}}
<noscript>
    <audio controls="">
        <source type="audio/ogg; codecs=opus" src="/track/{{TID}}/opus?stream=1" />
        <source type="audio/aac" src="/track/{{TID}}/aac?stream=1" />
        <source type="audio/ogg" src="/track/{{TID}}/vorbis?stream=1" />
        <source type="audio/mpeg" src="/track/{{TID}}/mp3?stream=1" />
    </audio>
</noscript>
{{/FALLBACK}}
