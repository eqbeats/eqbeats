var step = 0.2;
var delay = 60;

var news_entry = document.getElementsByClassName("newsticker")[0].getElementsByTagName("a")[0];
function fade(news, out){
    var opacity = parseFloat(news_entry.style.opacity);
    if(!out && opacity > 1-2*step){
        news_entry.style.opacity = 1;
        window.setTimeout(next_entry, 5000);
        return;
    }
    else if(out && opacity < 2*step){
        out = false;
        opacity = 0;
        news_entry.href = "/news/" + news.id;
        news_entry.innerHTML = news.title;
    }
    else
        opacity += (out?-step:step);
    news_entry.style.opacity = opacity;
    window.setTimeout(fade, delay, news, out);
}
function next_entry(){
    news = news_entries.shift();
    news_entries.push(news);
    window.setTimeout(fade, delay, news, true);
}

if(news_entry != undefined && news_entries != undefined){
    news_entry.style.opacity = 1;
    news = news_entries.shift();
    news_entries.push(news);
    window.setTimeout(next_entry, 5000);
}
