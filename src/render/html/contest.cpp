#include "../html.h"
#include "../../contest.h"
#include <sstream>
#include <algorithm>
#include "../../utils.h"
#include "../../session.h"

std::string Html::contest(int cid, const std::string &host){
    Contest c(cid);
    if(!c) return notFound("Contest");
    std::stringstream s;
    s << header(c.name())
      << "<h2>" + escape(c.name()) + "</h2>"
         "<div class=\"contest\">";
    std::string descr = c.getDescription();
    if(!descr.empty())
        s << "<div class=\"description\">" << descr << "</div>";
    s << "<h3 id=\"submissions\"><img src=\"/static/disc.png\" /> Submissions</h3>";
    std::vector<Track> tracks = Track::forContest(c.id());
    if(tracks.empty())
        s << "<div class=\"empty tracklist\">No submissions yet.</div>";
    else {
        bool voting = c.state()==Contest::Voting;
        std::vector<int> uvotes;
        if(voting){
            uvotes = c.usersVotes(host);
            s << "<form action=\"" + c.url() + "/vote\" method=\"post\">";
        }
        std::map<int,int> votes = c.getVotes();
        s << "<ul>";
        for(std::vector<Track>::const_iterator i=tracks.begin(); i!=tracks.end(); i++){
            s << "<li>";
            if(voting){
                bool vote = std::find(uvotes.begin(),uvotes.end(),i->id()) != uvotes.end();
                s << (std::string) "<button type=\"submit\" name=\"tid\" title=\"" + (vote?"Cancel vote":"Vote") + "\" value=\"" + (vote?"-":"+") + number(i->id()) + "\"><img src=\"/static/" + (vote?"vote":"star-empty") + ".png\"alt=\"" + (vote?"Cancel vote":"Vote") + "\" /></button> ";
            }
            s << "<a href=\"" << i->url() << "\">" << escape(i->title()) << "</a>"
                 " <span class=\"by\">"
                 "by <a href=\"" << User::url(i->artistId()) << "\">"
              << escape(i->artist()) << "</a>";
            int vcount = votes[i->id()];
            if(vcount>0)
                s << " (" + number(vcount) + " vote" + (vcount>1?"s":"") + ")";
            s << "</span></li>";
        }
        s << "</ul>";
        if(voting) s << "</form>";
    }
    if(Session::user() && c.state()==Contest::Submissions){
        std::vector<Track> utracks = Track::byArtist(Session::user().id());
        std::vector<Track> toSubmit;
        for(std::vector<Track>::const_iterator i=utracks.begin(); i!=utracks.end(); i++){
            if(std::find(tracks.begin(),tracks.end(),*i)==tracks.end())
                toSubmit.push_back(*i);
        }
        if(!toSubmit.empty()){
            s << "<form action=\"" + c.url() + "/submit\" method=\"post\">"
                 "Submit a track : "
                 "<select name=\"tid\">";
            for(std::vector<Track>::const_iterator i=toSubmit.begin(); i!=toSubmit.end(); i++)
                s << "<option value=\"" << number(i->id()) << "\">" << escape(i->title()) << "</option>";
            s << "</select>"
                 " <input type=\"submit\" value=\"Submit\" />"
                 "</form>";
        }
    }
    s << "</div>" << footer();
    return s.str();
}
