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
        obj.attachEvent('on'+type,obj[type+fn+2],false);
    } else {
        obj['on'+type] = obj[type+fn+2];
    }
}
function removeListener(obj,type,fn) {
    if (obj.removeEventListener) {
        obj.removeEventListener(type,obj[type+fn+2],false);
    } else if (obj.detachEvent) {
        obj.detachEvent('on'+type,obj[type+fn+2]);
    } else {
        obj['on'+type] = null;
    }
    obj[type+fn] = null;
    obj[type+fn+2] = null;
}

function prettyTime(ms){
    if(isNaN(ms)) return '0:00';
    time = [Math.floor(ms / 60000), Math.floor(ms / 1000) % 60];
    if(time[1] < 10) time[1] = '0' + time[1];
    return time[0] + ':' + time[1]
}

function pp(e){
    with(this.parentNode){
        if(className == 'player playing'){
            snd.pause();
            className = 'player paused';
        } else {
            snd.play({
                whileplaying: function(){
                    this.player.scrubber.style.width = (100 * this.position / this.durationEstimate) + '%';
                    playtime.innerHTML = prettyTime(this.position) + '/' + prettyTime(this.durationEstimate);
                },
                onfinish: function(){ this.player.className = 'player paused'; }
            });
            className = 'player playing';
        }
    }
};
function scrub(e){
    with(scrubbingplayer)
        snd.setPosition((e.clientX - (scrubberbar.offsetLeft + scrubberbar.offsetParent.offsetLeft + 3))*snd.durationEstimate/(scrubberbar.clientWidth - 6));
}

function initTrack(t){
    t.player = document.getElementById(t.list + '-' + t.id);
    t.player.playpause = document.createElement('div');
    t.player.scrubberbar = document.createElement('div');
    t.player.scrubber = document.createElement('div');
    t.player.playtime = document.createElement('div');
    t.player.snd = soundManager.getSoundById(t.id);
    t.player.scrubbing = false;
    with (t.player){
        style.display='block';
        playpause.className = 'playpause';
        scrubberbar.className = 'scrubberbar';
        scrubber.className = 'scrubber';
        playtime.className = 'playtime';
        appendChild(playpause);
        scrubberbar.appendChild(scrubber);
        appendChild(scrubberbar);
        appendChild(playtime);
        if(!snd){
            snd = soundManager.createSound({
                id: t.id,
                autoLoad: false,
                whileloading: function(){
                    playtime.innerHTML = prettyTime(this.position) + '/' + prettyTime(this.durationEstimate);
                },
                url: [
                    {url:t.vorbis, type:'audio/ogg'},
                    {url:t.mp3, type:'audio/mp3'}
                ]
            });
        }
        snd.player = t.player;
        addListener(playpause, 'click', pp);
        addListener(scrubberbar, 'mousedown', function(e){
            e.preventDefault();
            var player = this.parentNode;
            with(player){
                snd.pause();
                scrubbing = true;
            }
            scrubbingplayer = player;
            addListener(document, 'mouseup', function(){
                with(scrubbingplayer){
                    if(className == 'player playing' && scrubbing)
                        snd.resume();
                    scrubbing = false;
                    removeListener(document, 'mousemove', scrub);
                }
            });
            addListener(document, 'mousemove', scrub);
            scrub(e);
        });
    }
}

var scrubbingplayer = null;
var lists = Object();

soundManager.url = '/static/';
soundManager.preferFlash = false;
soundManager.audioFormats.mp3.required=false;
soundManager.onready(function(){
    for(var l in lists){
        for(j=0; j < lists[l].length; j++)
            initTrack(lists[l][j]);
    }
});
