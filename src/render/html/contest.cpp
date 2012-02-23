#include "contest.h"
#include "page.h"
#include "escape.h"
#include "../render.h"
#include "../../contest.h"
#include "../../number.h"
#include "../../track.h"
#include "../../session.h"
#include "../../user.h"
#include <algorithm>

void Render::Html::contest(int cid, const std::string &host){
    Contest c(cid);
    if(!c) return notFound("Contest");
    header(c.name());
    o << "<h2>" + escape(c.name()) + "</h2>"
         "<div class=\"contest\">";
    std::string descr = c.getDescription();
    if(!descr.empty())
        o << "<div class=\"description\">" << descr << "</div>";
    o << "<h3 id=\"submissions\"><img src=\"/static/disc.png\" /> Submissions</h3>";
    std::vector<Track> tracks = c.submissions();
    if(tracks.empty())
        o << "<div class=\"empty tracklist\">No submissions yet.</div>";
    else {
        bool voting = c.state()==Contest::Voting;
        std::vector<int> uvotes;
        if(voting){
            uvotes = c.usersVotes(host);
            o << "<form action=\"" + c.url() + "/vote\" method=\"post\">";
        }
        std::map<int,int> votes = c.getVotes();
        o << "<ul>";
        for(std::vector<Track>::const_iterator i=tracks.begin(); i!=tracks.end(); i++){
            o << "<li>";
            if(voting){
                bool vote = std::find(uvotes.begin(),uvotes.end(),i->id()) != uvotes.end();
                o << (std::string) "<button type=\"submit\" name=\"tid\" title=\"" + (vote?"Cancel vote":"Vote") + "\" value=\"" + (vote?"-":"+") + number(i->id()) + "\"><img src=\"/static/" + (vote?"vote":"star-empty") + ".png\"alt=\"" + (vote?"Cancel vote":"Vote") + "\" /></button> ";
            }
            o << "<a href=\"" << i->url() << "\">" << escape(i->title()) << "</a>"
                 " <span class=\"by\">"
                 "by <a href=\"" << i->artist().url() << "\">"
              << escape(i->artist().name()) << "</a>";
            int vcount = votes[i->id()];
            if(vcount>0)
                o << " (" + number(vcount) + " vote" + (vcount>1?"s":"") + ")";
            o << "</span></li>";
        }
        o << "</ul>";
        if(voting) o << "</form>";
    }
    if(Session::user() && c.state()==Contest::Submissions){
        std::vector<Track> utracks = Session::user().tracks();
        std::vector<Track> toSubmit;
        for(std::vector<Track>::const_iterator i=utracks.begin(); i!=utracks.end(); i++){
            if(std::find(tracks.begin(),tracks.end(),*i)==tracks.end())
                toSubmit.push_back(*i);
        }
        if(!toSubmit.empty()){
            o << "<form action=\"" + c.url() + "/submit\" method=\"post\">"
                 "Submit a track : "
                 "<select name=\"tid\">";
            for(std::vector<Track>::const_iterator i=toSubmit.begin(); i!=toSubmit.end(); i++)
                o << "<option value=\"" << number(i->id()) << "\">" << escape(i->title()) << "</option>";
            o << "</select>"
                 " <input type=\"submit\" value=\"Submit\" />"
                 "</form>";
        }
    }
    o << "</div>";
    footer();
}
