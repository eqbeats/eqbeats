var delay = 3000;

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

// News ticker
var entry = document.getElementById('newsticker').getElementsByTagName("a")[0];
entry.style.opacity = '1';
var i = 0;
function setEntry(){
    entry.href = "/news/" + news[i].id;
    entry.replaceChild(document.createTextNode(news[i].title), entry.firstChild);
}
function newsWait() { setTimeout(newsNext, delay); }
function newsNext(){
    i = (i+1) % news.length;
    fadeOut(entry, newsSet);
}
function newsSet(){ setEntry(); fadeIn(entry, newsWait); }

newsWait();
