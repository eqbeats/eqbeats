#include "event.h"
#include "escape.h"
#include "../render.h"
#include "../../session.h"

void Render::Html::event(const Event &e){
    if(!e.source()) o << escape(e.source().name());
    else if(Session::user() == e.source()) o << "You";
    else o << "<a href=\"" << e.source().url() << "\">" << e.source().name() << "</a>";
    o << " ";
    if(e.type() == Event::Comment){
        o << "posted a comment on ";
        if(e.track())
            o << "<a href=\"" + e.track().url() << "\">" << e.track().title() << "</a>"
              << (e.target()==Session::user() ? "" : " by <a href=\""+e.target().url()+"\">"+e.target().name()) << "</a>";
        else {
            o << "<a href=\"" << e.target().url() << "\">"
              << (e.target()==Session::user() ? "your profile page</a>" : e.target().name()+"</a>'s profile");
        }
        o << ":<br />" << format(e.message());
    }
    else if(e.type() == Event::Publish)
        o << "published <a href=\"" << e.track().url() + "\">" << e.track().title() << "</a>.";
    else if(e.type() == Event::Follow)
        o << "started following " << (e.target()==Session::user() ? "you" : "<a href=\""+e.target().url()+"\">"+e.target().name()+"</a>") << ".";
    else if(e.type() == Event::Favorite)
        o << "favorited <a href=\"" + e.track().url() + "\">" + e.track().title() + "</a>"
          << (e.target()==Session::user() ? "" : " by " + e.target().name()) << ".";
}

void Render::Html::eventStream(const std::vector<Event> &events){
    o << "<ul>";
    for(std::vector<Event>::const_iterator i=events.begin(); i!=events.end(); i++){
        o << "<li>";
        event(*i);
        o << "</li>";
    }
    o << "</ul>";
}
