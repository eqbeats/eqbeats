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

function setFavicon(href){
    var oldIcon = document.getElementById('favicon-link');
    if(oldIcon){
        var icon = document.createElement('link');
        icon.id = 'favicon-link';
        icon.rel = 'shortcut icon';
        icon.href = href;
        document.head.replaceChild(icon, oldIcon);
    }
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
        volume: globalVolume,
        whileloading: function(){
            player.playtime.innerHTML = prettyTime(this.position) + '/' + prettyTime(this.durationEstimate);
        },
        url: [
            {url:player.track.opus, type:'audio/ogg; codecs=opus'},
            {url:player.track.vorbis, type:'audio/ogg; codecs=vorbis'},
            {url:player.track.aac, type:'audio/aac'},
            {url:player.track.mp3, type:'audio/mp3'}
        ]
    });
    snd.triggered = false;
    playing = player;
    playing.volume.slider.inner.style.width = parseInt(playing.volume.slider.style.width) * globalVolume / 100 + 'px';
    playing.style.display = 'block';
}

function pause(player){
    snd.pause();
    player.className = 'player paused';
    setFavicon('/static/favicon.ico');
}

function play(player){
    if(player != playing)
        load(player);
    snd.play({
        whileplaying: function(){
            playing.scrubber.style.width = (100 * this.position / this.durationEstimate) + '%';
            playing.playtime.innerHTML = prettyTime(this.position) + '/' + prettyTime(this.durationEstimate);
            if(this.position > this.durationEstimate / 3 && this.triggered == false){
                this.triggered = true;
                var xhr = XMLHttpRequest();
                xhr.open("POST", "/track/" + playing.track.tid + "/played", true);
                xhr.send();
            }
        },
        onfinish: function(){
            if(!scrubbing){
                pause(playing);
                if(playing.next) play(playing.next);
            }
        }
    });
    player.className = 'player playing';
    setFavicon('/static/playing.ico');
}
function toggle(player){
    if(player == playing && player.className == 'player playing')
        pause(player);
    else
        play(player);
}
function pp(e){
    e.stopPropagation();
    toggle(this.parentNode);
}

function vol(e){
    e.stopPropagation();
    with(playing.volume.slider){
        inner.style.width = e.clientX - (inner.offsetLeft + inner.offsetParent.offsetLeft) + 'px';
        globalVolume = parseInt(inner.style.width) * 100 / parseInt(style.width);
        document.cookie = 'volume=' + globalVolume + '; path=/';
        snd.setVolume(globalVolume);
    }
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
    player.volume = document.createElement('div');
    with (player){
        playpause.className = 'playpause';
        scrubberbar.className = 'scrubberbar';
        scrubber.className = 'scrubber';
        playtime.className = 'playtime';
        volume.className = 'volume';
        appendChild(playpause);
        scrubberbar.appendChild(scrubber);
        appendChild(scrubberbar);
        appendChild(playtime);
        // Volume
        volume.slider = document.createElement('div');
        volume.icon = document.createElement('img');
        with(volume){
            slider.className = 'slider';
            slider.style.width = '50px';
            slider.inner = document.createElement('div');
            slider.appendChild(slider.inner);
            icon.src = '/static/icons/volume.png';
            appendChild(slider);
            appendChild(icon);
        }
        appendChild(volume);
        // Events
        addListener(playpause, 'click', pp);
        addListener(volume.slider, 'click', vol);
        addListener(scrubberbar, 'click', function(e){
            e.stopPropagation();
        });
        addListener(scrubberbar, 'mousedown', function(e){
            e.preventDefault();
            snd.pause();
            scrubbing = true;
            addListener(document, 'mousemove', scrub);
            scrub(e);
        });
    }
    if (player.parentNode.tagName == 'LI'){
        preventBubbling(player.parentNode, 'a');
        preventBubbling(player.parentNode, 'button');
    }
    if(lists[t.list])
        lists[t.list].next = player;
    lists[t.list] = player;
    if(!playing) playing = player; // preload the first player
}

function preventBubbling(el, tagname){
    elts = el.getElementsByTagName(tagname);
    for(var i = 0; i < elts.length; i++){
        addListener(elts[i], 'click', function(e){
            e.stopPropagation();
        });
    }
}

var playing;
var tracks = [];
var lists = Object();
var snd;
var scrubbing = false;
var globalVolume = 100;

function parseCookies(){
    var i = document.cookie.indexOf('volume=');
    if(i >= 0)
        globalVolume = parseInt(document.cookie.substr(i+7));
}

function registerTrack(t){ tracks.push(t); }

soundManager.url = '/static/';
soundManager.preferFlash = false;
soundManager.audioFormats.mp3.required=false;
soundManager.onready(function(){
    if(!document.head)
        document.head = document.getElementsByTagName("HEAD")[0];
    for(var i=0; i<tracks.length; i++)
        initTrack(tracks[i]);
    if(!playing) return;
    parseCookies();
    load(playing);
    addListener(document, 'mouseup', function(){
        if(scrubbing){
            play(playing);
            scrubbing = false;
            removeListener(document, 'mousemove', scrub);
        }
    });
});

