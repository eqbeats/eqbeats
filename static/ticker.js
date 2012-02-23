var delay = 7000;

// Fading
var step = 0.2;
var fadeDelay = 60;
function fadeIn(elem, callback){
    var opacity = parseFloat(elem.style.opacity);
    if(opacity == 0)
        elem.style.display = elem==entry ? 'inline' : 'block';
    if(opacity > 1-2*step){
        elem.style.opacity = 1;
        callback();
    }
    else{
        elem.style.opacity = opacity + step;
        window.setTimeout(fadeIn, fadeDelay, elem, callback);
    }
}
function fadeOut(elem, callback){
    var opacity = parseFloat(elem.style.opacity);
    if(opacity < 2*step){
        elem.style.opacity = 0;
        elem.style.display = 'none';
        callback();
    }
    else{
        elem.style.opacity = opacity - step;
        window.setTimeout(fadeOut, fadeDelay, elem, callback);
    }
}

// Celestia Radio
var head = document.getElementsByTagName('head')[0];
var script;
var loaded = false;
var artist = document.getElementById('nowplaying-artist');
var track = document.getElementById('nowplaying-track');
var widget = document.getElementById('nowplaying-widget');
widget.className = 'loading';
widget.style.opacity = '0';
function crRequest(){
    if(loaded) head.removeChild(script);
    script = document.createElement('script');
    script.type = 'text/javascript';
    script.src = '/nowplaying/cr?jsonp=crUpdate';
    head.appendChild(script);
    loaded = true;
}
function crUpdate(t){
    // Artist
    var nArtist = document.createElement(t.artist.id ? 'a' : 'span');
    nArtist.id = 'nowplaying-artist';
    nArtist.appendChild(document.createTextNode(t.artist.name));
    if(t.artist.id){
        nArtist.target = '_blank';
        nArtist.href = 'http://eqbeats.org/user/' + t.artist.id;
    }
    artist.parentNode.replaceChild(nArtist, artist)
    artist = nArtist;
    // Track
    var nTrack = document.createElement(t.id ? 'a' : 'span');
    nTrack.id = 'nowplaying-track';
    nTrack.appendChild(document.createTextNode(t.title));
    if(t.id){
        nTrack.target = '_blank';
        nTrack.href = 'http://eqbeats.org/track/' + t.id;
    }
    track.parentNode.replaceChild(nTrack, track)
    track = nTrack;
    // End
    widget.className = 'loaded';
    fadeOut(ticker, crShow);
}
function crShow(){ fadeIn(widget, crWait); }
function crWait(){ setTimeout(fadeOut, delay, widget, newsInit); }

// News ticker
var ticker = document.getElementById("newsticker");
var entry = ticker.getElementsByTagName("a")[0];
ticker.style.opacity = '1';
entry.style.opacity = '1';
var i = 0;
function setEntry(){
    entry.href = "/news/" + news[i].id;
    entry.replaceChild(document.createTextNode(news[i].title), entry.firstChild);
}
function newsInit(){ i = 0; setEntry(); fadeIn(ticker, newsWait); }
function newsWait() { setTimeout(newsNext, delay); }
function newsNext(){
    i++;
    if(i == news.length) crRequest();
    else fadeOut(entry, newsSet);
}
function newsSet(){ setEntry(); fadeIn(entry, newsWait); }

// Init
newsWait();
