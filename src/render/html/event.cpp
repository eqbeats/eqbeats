#include "event.h"
#include "escape.h"
#include "../render.h"
#include "../../number.h"
#include "../../session.h"
#include <stdlib.h>
#include <cstring>
#include <time.h>

#include "../../log.h"

std::string pprintDate(std::string isoDate, bool capitalize = true){
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(isoDate.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
    time_t now = time(0);
    int secs = difftime(mktime(gmtime(&now)), mktime(&tm));
    if(secs < 0){
        return (std::string)(capitalize?"I":"i") + "n the future";
    } else if(secs < 5){
        return (std::string)(capitalize?"A":"a") + " few seconds ago";
    } else if(secs < 60){
        return (std::string)(capitalize?"L":"l") + "ess than a minute ago";
    } else if(secs < 120){
        return (std::string)(capitalize?"A":"a") + " minute ago";
    } else if(secs < 60*60*3){
        int mins = (secs / 60) % 60;
        int hours = secs / 3600;
        return (hours!=0?(hours==1?(std::string)(capitalize?"A":"a") + "n hour, ":number(hours)+" hours, "):"") + number(mins) + " minute" + (mins<=1?"":"s") + " ago";
    } else if(secs < 60*60*48){
        return number(secs/(60*60)) + " hours ago";
    } else if (secs < 60*60*24*60){
        return number(secs/(60*60*24)) + " days ago";
    } else if (secs < 60*60*24*30*24){
        return number(secs/(60*60*24*30)) + " months ago";
    } else {
        return number(secs/(60*60*24*30*12)) + " years ago";
    }
}

void Render::Html::event(const Event &e, bool context){
    o << "<li class=\"event\" name=\""+number(e.id())+"\">"
         "<span class=\"date\" title=\""+e.date()+" UTC\">" + pprintDate(e.date()) + "</span>"
         "<img alt=\"\" src=\"";
    if(e.type() == Event::Comment) o << "/static/icons/balloon-white-left.png";
    else if(e.type() == Event::Publish) o << "/static/icons/disc-arrow.png";
    else if(e.type() == Event::Follow) o << "/static/icons/plus.png";
    else if(e.type() == Event::Favorite) o << "/static/icons/star.png";
    o << "\"/>";
    std::string name = e.source().name();
    if(!e.source()) o << escape(name.empty()?"Anonymous":name);
    else if(Session::user() == e.source()) o << "You";
    else o << "<a href=\"" << e.source().url() << "\">" << escape(name) << "</a>";
    o << " ";
    if(e.type() == Event::Comment){
        if(e.track())
            o << "posted a comment";
        else
            o << "said";
        if(context){
            o << " on ";
            if(e.track())
                o << "<a href=\"" + e.track().url() << "\">" << escape(e.track().title()) << "</a>"
                  << (e.target()==Session::user() ? "" : " by <a href=\""+e.target().url()+"\">"+escape(e.target().name())) << "</a>";
            else {
                o << "<a href=\"" << e.target().url() << "\">"
                  << (e.target()==Session::user() ? "your profile page</a>" : escape(e.target().name())+"</a>'s profile");
            }
        }
        o << ":<p class=\"comment\">" << format(e.message()) << "</p>";
    }
    else if(e.type() == Event::Publish){
        o << "published <a href=\"" << e.track().url() + "\">" << escape(e.track().title()) << "</a>.";
    }
    else if(e.type() == Event::Follow)
        o << "started following " << (e.target()==Session::user() ? "you" : "<a href=\""+e.target().url()+"\">"+escape(e.target().name())+"</a>") << ".";
    else if(e.type() == Event::Favorite)
        o << "favorited <a href=\"" + e.track().url() + "\">" + escape(e.track().title()) + "</a>"
          << (e.target()==Session::user() ? "" : " by <a href=\""+e.target().url()+"\">"+escape(e.target().name())+"</a>") << ".";
    o << "</li>";
}

void Render::Html::eventStream(const std::vector<Event> &events, std::string header, bool context){
    o << "<ul class=\"events\">"
      << (header.empty()?"":"<h4><img src=\"/static/icons/fire-small.png\" alt=\"\"/>"+header+"</h4>");
    for(std::vector<Event>::const_iterator i=events.begin(); i!=events.end(); i++){
        event(*i, context);
    }
    if(events.size() == 0)
        o << "<li class=\"empty\">Nothing here yet.</li>";
    o << "</ul>";
}
