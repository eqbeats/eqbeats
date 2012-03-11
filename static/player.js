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

function load(player){
    if(playing != player){
        playing.style.display = 'none';
        pause(playing);
        playing.scrubber.style.width = '0px';
        playing.playtime.innerHTML = '0:00/0:00';
    }
    if(snd) snd.destruct();
    snd = soundManager.createSound({
        id: "snd",
        autoLoad: false,
        whileloading: function(){
            player.playtime.innerHTML = prettyTime(this.position) + '/' + prettyTime(this.durationEstimate);
        },
        url: [
            {url:player.track.vorbis, type:'audio/ogg'},
            {url:player.track.mp3, type:'audio/mp3'}
        ]
    });
    playing = player;
    playing.style.display = 'block';
}

function pause(player){
    snd.pause();
    player.className = 'player paused';
}
function play(player){
    if(player != playing)
        load(player);
    snd.play({
        whileplaying: function(){
            playing.scrubber.style.width = (100 * this.position / this.durationEstimate) + '%';
            playing.playtime.innerHTML = prettyTime(this.position) + '/' + prettyTime(this.durationEstimate);
        },
        onfinish: function(){
            playing.className = 'player paused';
            if(playing.next) play(playing.next);
        }
    });
    player.className = 'player playing';
}
function toggle(player){
    if(player == playing && player.className == 'player playing')
        pause(player);
    else
        play(player);
}
function pp(e) { toggle(this.parentNode); }

function display(player){
    if(playing != player)
        play(player);
}

function scrub(e){
    with(playing)
        snd.setPosition((e.clientX - (scrubberbar.offsetLeft + scrubberbar.offsetParent.offsetLeft + 3))*snd.durationEstimate/(scrubberbar.clientWidth - 6));
}

function initTrack(t){
    var player = document.getElementById(t.id);
    player.track = t;
    player.playpause = document.createElement('div');
    player.scrubberbar = document.createElement('div');
    player.scrubber = document.createElement('div');
    player.playtime = document.createElement('div');
    with (player){
        playpause.className = 'playpause';
        scrubberbar.className = 'scrubberbar';
        scrubber.className = 'scrubber';
        playtime.className = 'playtime';
        appendChild(playpause);
        scrubberbar.appendChild(scrubber);
        appendChild(scrubberbar);
        appendChild(playtime);
        addListener(playpause, 'click', pp);
        addListener(scrubberbar, 'mousedown', function(e){
            e.preventDefault();
            snd.pause();
            scrubbing = true;
            addListener(document, 'mousemove', scrub);
            scrub(e);
        });
    }
    if(lists[t.list])
        lists[t.list].next = player;
    lists[t.list] = player;
    if(!playing) playing = player; // preload the first player
}

var playing;
var tracks = [];
var lists = Object();
var snd;
var scrubbing = false;

function registerTrack(t){ tracks.push(t); }

soundManager.url = '/static/';
soundManager.preferFlash = false;
soundManager.audioFormats.mp3.required=false;
soundManager.onready(function(){
    for(var i=0; i<tracks.length; i++)
        initTrack(tracks[i]);
    if(!playing) return;
    load(playing);
    addListener(document, 'mouseup', function(){
        if(scrubbing){
            play(playing);
            scrubbing = false;
            removeListener(document, 'mousemove', scrub);
        }
    });
});

