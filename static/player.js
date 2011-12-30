// addListener by hyperair
// http://www.webdesignforums.net/threads/ie-addeventlistener-attachevent.26686/#post-172727
function addListener(obj,type,fn) {
    obj[type+fn] = fn;
    obj[type+fn+2] = function(e) {
        if (! e) {
            if (! window.event) {
                opener = '';
            }
            e = window.event;
        }
        if (! e.preventDefault) {
            e.preventDefault = function() { this.returnValue = false; };
        }
        if (! e.stopPropagation) {
            e.stopPropagation = function() { this.cancelBubble = true; };
        }
        if (! e.target) {
            e.target = e.srcElement;
        }
        obj[type+fn](e);
    };
    
    if (obj.addEventListener) {
        obj.addEventListener(type,obj[type+fn+2],false);
    } else if (obj.attachEvent) {
        obj.attachEvent("on"+type,obj[type+fn+2],false);
    } else {
        obj["on"+type] = obj[type+fn+2];
    }
}
function removeListener(obj,type,fn) {
    if (obj.removeEventListener) {
        obj.removeEventListener(type,obj[type+fn+2],false);
    } else if (obj.detachEvent) {
        obj.detachEvent("on"+type,obj[type+fn+2]);
    } else {
        obj["on"+type] = null;
    }
    obj[type+fn] = null;
    obj[type+fn+2] = null;
}


soundManager.url = '/static/';
soundManager.preferFlash = false;
soundManager.audioFormats.mp3.required=false;
soundManager.onready(function(){
    var player = document.getElementById("player");
    player.style.display="block";
    var playpause = document.createElement("div");
    playpause.id = "playpause";
    var scrubberbar = document.createElement("div");
    scrubberbar.id = "scrubberbar";
    var scrubber = document.createElement("div");
    scrubber.id = "scrubber";
    player.appendChild(playpause);
    scrubberbar.appendChild(scrubber);
    player.appendChild(scrubberbar);
    var snd = soundManager.createSound({
        id: 'snd',
        autoLoad: true,
        url: [
            {url:oggpath, type:'audio/ogg'}, 
            {url:mp3path, type:'audio/mpeg'}
        ]
    });
    var pp = function(){
        if(player.className == "playing"){
            snd.pause();
            player.className = "paused";
        } else {
            snd.play({
                whileplaying: function(){ scrubber.style.width = (100 * snd.position / snd.durationEstimate) + "%"; },
                onfinish: function(){ player.className = "paused"; }
            });
            player.className = "playing";
        };
    };
    var scrub = function(e){
        var pos = (e.clientX - (scrubberbar.offsetLeft + scrubberbar.offsetParent.offsetLeft + 3))*snd.durationEstimate/(scrubberbar.clientWidth - 6);
        snd.setPosition(pos);
    }
    addListener(playpause, "click", pp);
    player.scrubbing = false;
    addListener(scrubberbar, "mousedown", function(e){
        e.preventDefault();
        snd.pause();
        player.scrubbing = true;
        addListener(document, "mouseup", function(){
            if(player.className == "playing" && player.scrubbing)
                snd.resume();
            player.scrubbing = false;
            removeListener(document, "mousemove", scrub);
        });
        addListener(document, "mousemove", scrub);
        scrub(e);
    });
});
