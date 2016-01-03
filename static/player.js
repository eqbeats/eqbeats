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

// https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Function/bind#Compatibility
if (!Function.prototype.bind) {
    Function.prototype.bind = function (oThis) {
        if (typeof this !== "function") {
            // closest thing possible to the ECMAScript 5 internal IsCallable function
            throw new TypeError("Function.prototype.bind - what is trying to be bound is not callable");
        }

        var aArgs = Array.prototype.slice.call(arguments, 1),
            fToBind = this,
            fNOP = function () {},
            fBound = function () {
                return fToBind.apply(this instanceof fNOP && oThis
                        ? this
                        : oThis,
                        aArgs.concat(Array.prototype.slice.call(arguments)));
            };

        fNOP.prototype = this.prototype;
        fBound.prototype = new fNOP();

        return fBound;
    };
}

function prettyTime(s){
    if(isNaN(s)) return '0:00';
    time = [Math.floor(s / 60), Math.floor(s) % 60];
    if(time[1] < 10) time[1] = '0' + time[1];
    return time[0] + ':' + time[1]
}

function load(player, nondisruptive){
    if(!nondisruptive){
        if(playing != player && !nondisruptive){
            var prev = playing;
            playing = player;
            pause(prev);
            prev.scrubber.style.width = '0px';
            prev.playtime.innerHTML = '0:00/0:00';
            if(prev.track.list == "featured_tracks"){
                if(player.track.list == prev.track.list)
                    prev.parentNode.className = '';
            }
            else
                prev.style.display = 'none';
        }
        snd.player = player;
        snd.preload = 'metadata';
        snd.stopped = false;
        snd.operaquirk = false;
        snd.innerHTML = "";
        var opus = document.createElement("source");
        opus.src = player.track.opus;
        opus.type = "audio/ogg; codecs=opus";
        var vorbis = document.createElement("source");
        vorbis.src = player.track.vorbis;
        vorbis.type = "audio/ogg; codecs=vorbis";
        var aac = document.createElement("source");
        aac.src = player.track.aac;
        aac.type = "audio/aac";
        var mp3 = document.createElement("source");
        mp3.src = player.track.mp3;
        mp3.type = "audio/mp3";
        snd.appendChild(opus);
        snd.appendChild(vorbis);
        snd.appendChild(aac);
        snd.appendChild(mp3);
        snd.load();
        snd.volume = globalVolume;
    }
    player.volume.slider.inner.style.width = parseInt(playing.volume.slider.style.width) * globalVolume + 'px';
    player.style.display = 'block';
    if(player.track.list == "featured_tracks"){
        var featurebox = document.querySelector(".featurebox");
        featurebox.style["background"] = "url('/track/" + player.track.tid + "/art/medium') center center, #999";
        // sicknasty preloading
        if(player.next){
            var i = new Image();
            i.src = '/track/' + player.next.track.tid + '/art/medium';
        }
        var active = featurebox.querySelectorAll('.active');
        if(active[0]) active[0].className = '';
        player.parentNode.className = 'active';
    }
}

function pause(player){
    snd.pause();
    snd.stopped = true;
    document.title = pagetitle;
    if(player){
        player.className = 'player paused';
    }
}

function play(player){
    if(player != playing){
        load(player);
    }

    snd.play()

    player.className = 'player playing';
    document.title = "▶ " + pagetitle;
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
    if(this == playing.volume.slider){
        with(playing.volume.slider){
            inner.style.width = e.clientX - recurseoffset(inner)[1] + 'px';
            globalVolume = parseInt(inner.style.width) / parseInt(style.width);
            document.cookie = 'volume=' + globalVolume + '; path=/';
            snd.volume = globalVolume;
        }
    }
}

function recurseoffset(el){
    var top = 0;
    var left = 0;
    while(el){
        top += el.offsetTop;
        left += el.offsetLeft;
        el = el.offsetParent;
    }
    return [top, left];
}
function scrub(e){
    with(playing)
        snd.currentTime = ((e.clientX - recurseoffset(scrubberbar)[1] - 3)*snd.duration/(scrubberbar.clientWidth - 6));
}

function initTrack(t){
    var player = document.getElementById(t.id);
    if(lists[t.list]){
        lists[t.list].next = player;
        player.prev = lists[t.list];
    }
    lists[t.list] = player;
    var featured = t.list == 'featured_tracks';
    player.track = t;
    player.playpause = document.createElement('div');
    player.scrubberbar = document.createElement('div');
    player.scrubber = document.createElement('div');
    player.playtime = document.createElement('div');
    player.volume = document.createElement('div');
    if(featured){
        player.prevbtn = document.createElement('span');
        player.nextbtn = document.createElement('span');
    }
    with (player){
        playpause.className = 'playpause';
        scrubberbar.className = 'scrubberbar';
        scrubber.className = 'scrubber';
        playtime.className = 'playtime';
        playtime.innerHTML = '0:00/0:00';
        volume.className = 'volume';
        if(featured){
            prevbtn.className = 'prev disabled';
            if(player.prev){
                prevbtn.className = 'prev';
                player.prev.nextbtn.className = 'next';
                player.prev.nextbtn.innerHTML = 'Next up: ' + t.artist + " - " + t.title;
            }
            nextbtn.className = 'next disabled';
        }
        appendChild(playpause);
        scrubberbar.appendChild(scrubber);
        appendChild(scrubberbar);
        appendChild(playtime);
        if(featured){
            /*parentNode.appendChild(prevbtn);
            parentNode.appendChild(nextbtn);*/
            appendChild(prevbtn);
            appendChild(nextbtn);
        }
        // Volume
        volume.slider = document.createElement('div');
        volume.icon = document.createElement('img');
        volume.slider.inner = document.createElement('div');
        with(volume){
            slider.className = 'slider';
            slider.style.width = '50px';
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
            if(this == playing.scrubberbar){
                wasPlaying = !snd.paused;
                scrubbing = true;
                addListener(document, 'mousemove', scrub);
                scrub(e);
            }
        });
        if(featured){
            addListener(nextbtn, 'click', function(e){
                e.stopPropagation();
                if(player.next){
                    if(player.className == "player playing")
                        play(player.next);
                    else
                        load(player.next, true);
                }
            });
            addListener(prevbtn, 'click', function(e){
                e.stopPropagation();
                if(player.prev){
                    if(player.className == "player playing")
                        play(player.prev);
                    else
                        load(player.prev, true);
                }
            });
        }
    }
    if (player.parentNode.tagName == 'LI'){
        preventBubbling(player.parentNode, 'a');
        preventBubbling(player.parentNode, 'button');
    }
    if(!playing) playing = player; // preload the first player
}

function preventBubbling(el, tagname){
    elts = el.querySelectorAll(tagname);
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
var wasPlaying = false;
var globalVolume = 0.9;

function parseCookies(){
    var i = document.cookie.indexOf('volume=');
    if(i >= 0)
        globalVolume = document.cookie.substr(i+7);
        if(globalVolume > 1 || globalVolume < 0 || isNaN(globalVolume - 0)){
            globalVolume = 0.9
        }
}

function registerTrack(t){ tracks.push(t); }

function getFeatures(){
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/tracks/featured/json", true);
    xhr.onreadystatechange = function(){
        if(xhr.readyState == 4){
            var tracks = eval(xhr.responseText);
            for(var i = 1; i < tracks.length; i++){
                var track = tracks[i];
                registerTrack({
                    id: "featured_tracks-" + i,
                    tid: "" + track.id,
                    mp3: "/track/"+track.id+"/mp3?stream=1",
                    vorbis: "/track/"+track.id+"/vorbis?stream=1",
                    aac: "/track/"+track.id+"/aac?stream=1",
                    opus: "/track/"+track.id+"/opus?stream=1",
                    list: "featured_tracks"
                });
            }
        }
    };
    xhr.send();
}

function loadplayer(){
    if(typeof Audio == "undefined"){
        document.body.className += " nohtml5";
        return;
    }
    snd = new Audio();
    if(!document.head)
        document.head = document.querySelector("HEAD");
    pagetitle = document.querySelector("title").innerHTML;
    for(var i=0; i<tracks.length; i++)
        initTrack(tracks[i]);
    if(!playing) return;
    parseCookies();
    load(playing);
    addListener(document, 'mouseup', function(){
        if(scrubbing){
            if(!wasPlaying)
                pause(playing);
            scrubbing = false;
            removeListener(document, 'mousemove', scrub);
        }
    });
    addListener(snd, "loadedmetadata", function(){
        playing.playtime.innerHTML = prettyTime(this.currentTime) + '/' + prettyTime(this.duration);
    });
    addListener(snd, "playing", function(){
        if(!this.paused && !snd.operaquirk){
            this.pause();
            if(!this.stopped)
                this.play();
            this.operaquirk = true;
        }
    });
    addListener(snd, "timeupdate", function(){
        playing.scrubber.style.width = (100 * this.currentTime / this.duration) + '%';
        playing.playtime.innerHTML = prettyTime(this.currentTime) + '/' + prettyTime(this.duration);
    });
    addListener(snd, "ended", function(){
        if(!scrubbing){
            if(playing.next) play(playing.next);
            else pause(playing);
        }
    });
};

function noop(){}
var oldonload = window.onload || noop;
window.onload = function(){ this(); loadplayer(); }.bind(oldonload);
