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

function load(player, nondisruptive){
    if(playing != player && !nondisruptive){
        pause(playing);
        playing.scrubber.style.width = '0px';
        playing.playtime.innerHTML = '0:00/0:00';
        if(playing.track.list == "featured_tracks"){
            if(player.track.list == playing.track.list)
                playing.parentNode.className = '';
        }
        else
            playing.style.display = 'none';
    }
    if(!nondisruptive){
        if(snd) snd.destruct();
        snd = soundManager.createSound({
            id: "snd",
            autoLoad: false,
            volume: globalVolume,
            whileloading: function(){
                player.playtime.innerHTML = prettyTime(this.position) + '/' + prettyTime(this.durationEstimate);
            },
            url: [
                {url:player.track.opus, type:'audio/opus'},
                {url:player.track.vorbis, type:'audio/ogg; codecs=vorbis'},
                {url:player.track.aac, type:'audio/aac'},
                {url:player.track.mp3, type:'audio/mp3'}
            ]
        });
        snd.triggered = false;
        playing = player;
    }
    player.volume.slider.inner.style.width = parseInt(playing.volume.slider.style.width) * globalVolume / 100 + 'px';
    player.style.display = 'block';
    if(player.track.list == "featured_tracks"){
        var featurebox = document.getElementsByClassName("featurebox")[0];
        featurebox.style["background"] = "url('/track/" + player.track.tid + "/art/medium') center center, #999";
        // sicknasty preloading
        if(player.next){
            var i = new Image();
            i.src = '/track/' + player.next.track.tid + '/art/medium';
        }
        var active = featurebox.getElementsByClassName('active');
        if(active[0]) active[0].className = '';
        player.parentNode.className = 'active';
    }
}

function pause(player){
    snd.pause();
    player.className = 'player paused';
    if(player.moodbar)
        player.moodbar.className = 'moodbar';
    document.getElementsByTagName("title")[0].innerHTML = pagetitle;
}

function play(player){
    if(player != playing){
        load(player);
    }
    snd.load({
        whileloading: function(){
            if(!snd.operaquirk){
                snd.pause();
                window.setTimeout(function(){snd.play();},100);
                snd.operaquirk = true;
            }
        }
    });
    snd.play({
        whileplaying: function(){
            playing.scrubber.style.width = (100 * this.position / this.durationEstimate) + '%';
            playing.playtime.innerHTML = prettyTime(this.position) + '/' + prettyTime(this.durationEstimate);
            if(!this.triggered && this.position > 5 && (this.position > this.durationEstimate / 3 || this.position > 90000)){
                this.triggered = true;
                var xhr = new XMLHttpRequest();
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
    if(player.moodbar)
        player.moodbar.className = 'moodbar visible';
    document.getElementsByTagName("title")[0].innerHTML = "▶ " + pagetitle;
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
    if(e.currentTarget == playing.volume.slider){
        with(playing.volume.slider){
            inner.style.width = e.clientX - recurseoffset(inner)[1] + 'px';
            globalVolume = parseInt(inner.style.width) * 100 / parseInt(style.width);
            document.cookie = 'volume=' + globalVolume + '; path=/';
            snd.setVolume(globalVolume);
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
        snd.setPosition((e.clientX - recurseoffset(scrubberbar)[1] - 3)*snd.durationEstimate/(scrubberbar.clientWidth - 6));
}

function loadMoodbar(t, container) {

    var xhr = new XMLHttpRequest();
    // if mimetype overriding isn't supported or d3 isn't loaded, bail out
    if(typeof(d3) == 'undefined' || !xhr.overrideMimeType)
        return false;

    var moodbar = d3.select(container).append("div")
            .classed("moodbar", true)
            .style("display", "none");

    // xhr.open('GET', t.mood, true);
    xhr.open('GET', t.mood, true);

    // Hack to pass bytes through unprocessed.
    xhr.overrideMimeType('text/plain; charset=x-user-defined');

    xhr.onreadystatechange = function(e) {

      if (this.readyState == 4 && this.status == 200) {
        if(this.responseText.length != 3000)
            return;
        var rgb = new Array(this.responseText.length / 3);
        for (var i = 0, len = rgb.length; i < len; i++) {
            rgb[i] = {
                offset: (i / len * 100) + "%",
                r: this.responseText.charCodeAt(i*3+0) & 0xff,
                g: this.responseText.charCodeAt(i*3+1) & 0xff,
                b: this.responseText.charCodeAt(i*3+2) & 0xff
            };
        }

        var svg = moodbar.append("svg")
            .classed("greyscale", true)
          .append("g");

        svg.append("linearGradient")
              .attr("id", "moodbar-gradient-grey-" + t.id)
              .attr("gradientUnits", "userSpaceOnUse")
            .selectAll("stop")
              .data(rgb)
            .enter().append("stop")
              .attr("offset", function(d) { return d.offset; })
              .attr("stop-color", function(d) {
                  var grey = Math.round(0.21*d.r + 0.71*d.g + 0.07*d.b);
                  return "rgb(" + grey + ", "  + grey + ", " + grey + ")";
              });

        svg.append("rect")
              .attr("fill", "url(#moodbar-gradient-grey-" + t.id + ")")
              .attr("x", 0)
              .attr("y", 0)
              .attr("width", "100%")
              .attr("height", "100%")

        var svg = moodbar.append("svg")
            .classed("colored", true)
            .classed("hidden", true)
          .append("g");

        svg.append("linearGradient")
              .attr("id", "moodbar-gradient-" + t.id)
              .attr("gradientUnits", "userSpaceOnUse")
            .selectAll("stop")
              .data(rgb)
            .enter().append("stop")
              .attr("offset", function(d) { return d.offset; })
              .attr("stop-color", function(d) { return "rgb(" + d.r + ", " + d.g + ", " + d.b + ")"; });

        svg.append("rect")
              .attr("fill", "url(#moodbar-gradient-" + t.id + ")")
              .attr("x", 0)
              .attr("y", 0)
              .attr("width", "100%")
              .attr("height", "100%")

        moodbar.style("display", "block");

      }
    };

    xhr.send();

    return moodbar.node();

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
            if(e.currentTarget == playing.scrubberbar){
                snd.pause();
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
    // only do this on standalone tracks and if d3 is loaded
    if (player.parentNode.tagName != 'LI') {
        // loads moodbar (at some point in a deferred manner)
        player.moodbar = loadMoodbar(t, player.scrubberbar);
        if(player.moodbar) {
            player.scrubber.className = 'scrubber withmoodbar';
        }
    }
    if (player.parentNode.tagName == 'LI'){
        preventBubbling(player.parentNode, 'a');
        preventBubbling(player.parentNode, 'button');
    }
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
    else
        globalVolume = 90;
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

soundManager.audioFormats.mp3.required = false;
soundManager.audioFormats['opus'] = {
    type: ['audio/ogg; codecs=opus', 'audio/opus'],
    required: false
};
soundManager.setup({
    url: '/static/',
    html5Test: /^probably$/i,
    preferFlash: false
});
soundManager.onready(function(){
    if(!document.head)
        document.head = document.getElementsByTagName("HEAD")[0];
    pagetitle = document.getElementsByTagName("title")[0].innerHTML;
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

