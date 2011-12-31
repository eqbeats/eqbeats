#include "html.h"
#include "../session.h"
#include "../utils.h"
#include <sstream>

using namespace std;

string renameForm(const Track &t){
    return
        "<form method=\"post\" action=\"" + t.url() + "/rename\">"
            "<input type=\"text\" name=\"title\" value=\"" + Html::escape(t.title()) + "\" />"
            "<input type=\"submit\" value=\"Rename\" />"
        "</form>";
}

string Html::uploadForm(const std::string &action){
    stringstream s;
    s << "<div id=\"track-uploader\">"
            "<form action=\"" << action << "\" method=\"post\" enctype=\"multipart/form-data\" id=\"track-uploader-form\">"
                "<input type=\"file\" accept=\"audio/mpeg\" name=\"file\" />"
                "<input type=\"submit\" value=\"Upload an MP3\" />"
            "</form>"
        "</div>"
        "<script src=\"/static/fileuploader.js\" type=\"text/javascript\"></script>"
        "<script>"
            "function createUploader(){"
                "var uploader = new qq.FileUploader({"
                    "element: document.getElementById('track-uploader'),"
                    "allowedExtensions: ['mp3'],"
                    "action: '" << action << "'"
                "});"
            "}"
            "window.onload = createUploader;"
        "</script>";
    return s.str();
}

string artUploadForm(const Track &t){
    return
        "<form action=\"" + t.url() + "/art/upload\" method=\"post\" enctype=\"multipart/form-data\">"
            "<input type=\"file\" name=\"file\" />"
            "<input type=\"submit\" value=\"Upload a picture\" />"
        "</form>";
}

string visibilityForm(const Track &t){
    return
        "<form action=\"" + t.url() + "/visibility\" method=\"post\">"
          "This track is " +
          (t.visible()?"public." : "<b>hidden</b>.") +
          " <input type=\"submit\" value=\"" + (t.visible()?"Hide":"Show") + "\"/>"
          "<input type=\"hidden\" name=\"visible\" value=\"" + (t.visible()?"f":"t") + "\"/>"
      "</form>";
}

string player(const Track &t){
    return
        "<noscript>"
            "<audio controls>"
                "<source type=\"audio/ogg\" src=\"" + t.url(Track::Vorbis) + "\" />"
                "<source type=\"audio/mpeg\" src=\"" + t.url(Track::MP3) + "\" />"
            "</audio>"
        "</noscript>"
        "<div id=\"player\"></div>"
        "<script src=\"/static/sm2.js\"></script>"
        "<script>"
            "var mp3path = \"" + t.url(Track::MP3) +"\";"
            "var oggpath = \"" + t.url(Track::Vorbis) +"\";"
        "</script>"
        "<script src=\"/static/player.js\"></script>";
}

#define URL "http://eqbeats.org"

string embedCode(const Track &t){
    return "<textarea id=\"embedcode\">"
            "<iframe width=\"500px\" height=\"150px\" frameborder=\"0\" src=\""URL + t.url() + "/embed\">"
            "<a href=\""URL + t.url() + "\" target=\"_blank\">" + Html::escape(t.title()) + "</a>"
            " by <a href=\""URL + User::url(t.artistId()) + "\" target=\"_blank\">" + Html::escape(t.artist()) + "</a>"
            "</iframe>"
      "</textarea>"
      "<script>document.getElementById('embedcode').style.display='none';</script>";
}

string art(const Track &t){
    std::string img = t.artUrl();
    if(img.empty()) return "";
    return "<img class=\"art\" src=\"" + img + "\" />";
}

string Html::trackPage(int tid){
    Track t(tid);
    if(!t) return notFound("Track");
    stringstream s;
    bool edition = Session::user().id() == t.artistId();
    if(!edition)
        t.hit();
    s << header(escape(t.title()))
      << "<div class=\"track\">" // << art(t) <<
            "<h3 style=\"margin:10px;\">by <a href=\"" << User::url(t.artistId()) <<  "\">"
                    << escape(t.artist()) << "</a></h3>"
      << player(t)
      << "<div class=\"download\">Download : "
      << " <a href=\"" << t.url(Track::Vorbis) << "\">OGG Vorbis</a>"
         " <a href=\"" << t.url(Track::MP3) << "\">MP3</a>"
         " &nbsp; Share : <a href=\"#embedcode\" onclick=\"document.getElementById('embedcode').style.display='block';return false;\">Embed</a>";
    if(edition)
        s << " &nbsp; Hits : " << t.getHits();
    s << "</div>" << embedCode(t);
    string notes = t.getNotes();
    if(!notes.empty())
        s << "<div class=\"notes\">" << format(notes) << "</div>";
    if(edition)
        s << "<h4>Edit</h4>"
          << renameForm(t)
          << visibilityForm(t)
          << "<form action=\"" << t.url() << "/notes\" method=\"post\">"
                 "<textarea name=\"notes\">" << escape(notes) << "</textarea><br />"
                 "<input type=\"submit\" value=\"Update description\" />"
             "</form>"
          << uploadForm(t.url()+"/upload")
          //<< artUploadForm(t)
          << "<a class=\"danger\" href=\"" << t.url() << "/delete\">Delete</a>";
    s << comments(Comment::forTrack(t.id()))
      << commentForm(t.url() + "/comment")
      << "</div>"
      << footer();
    return s.str();
}

string Html::embedTrack(int tid){
    Track t(tid);
    stringstream s;
    s << "Content-Type: text/html\n";
    if(!t) s << "Status: 404 Not Found\n";
    s << "\n<!DOCTYPE html!>"
        "<html><head>"
            "<title>" << (t ? escape(t.title()) : "Track not found") << " - Equestrian Beats</title>"
            "<link rel=\"stylesheet\" type=\"text/css\" href=\"/static/player.css\" />"
        "</head><body><div id=\"player-embed\">";
    if(t)
        s << "<h3><a href=\"" + t.url() + "\" target=\"_blank\">" + escape(t.title()) + "</a></h3>"
             "<h4>by <a href=\"" << User::url(t.artistId()) <<  "\" target=\"_blank\">" << escape(t.artist()) << "</a></h4>"
          << player(t);
    else s << "<h3 style=\"margin-bottom:10px;\">Track not found</h3>";
    s << "</div></body></html>";
    return s.str();
}

string Html::trackList(const vector<Track> &tracks, Html::TrackList l){
    stringstream s;
    s << "<ul class=\"tracklist\">";
    for(vector<Track>::const_iterator i=tracks.begin(); i!=tracks.end(); i++){
        s << "<li";
        if(!i->visible()) s << " class=\"hidden\"";
        s << "><a href=\"" << i->url() << "\">" << i->title() << "</a>";
        if(l == Standard)
            s << " <span class=\"by\">by <a href=\"" << User::url(i->artistId()) << "\">"
              << i->artist() << "</a></span>";
        s << "</li>";
    }
    s << "</ul>";
    return s.str();
}

string Html::tracksPage(){
    return header("Tracks")
         + searchForm("/tracks/search")
         + footer();
}

string Html::trackSearch(const std::string &q){
    stringstream s;
    s << header("Track search")
      << searchForm("/tracks/search", q);
    if(!q.empty()){
        vector<Track> res = Track::search(q);
        if(res.empty())
            s << "No result.";
        else
            s << trackList(res);
    }
    s << footer();
    return s.str();
}

string Html::latestTracks(int n){
    return header("Latest Tracks")
         + searchForm("/tracks/search")
         + trackList(Track::latest(n))
         + footer();
}

string Html::randomTracks(int n){
    return header("Random Tracks")
         + searchForm("/tracks/search")
         + trackList(Track::random(n))
         + footer();
}

string Html::popularTracks(int n){
    return header("Popular Tracks")
         + searchForm("/tracks/search")
         + trackList(Track::popular(n))
         + footer();
}

string escapeFilename(const string &str){
    string buf;
    for(string::const_iterator i=str.begin(); i!=str.end(); i++){
        if     (*i == '"') buf += "\\\"";
        else if(*i >= ' ') buf += *i;
    }
    return buf;
}

string Html::downloadTrack(int tid, Track::Format f){
    Track t(tid);
    if(!t) return notFound("Track");
    string ext = f == Track::Vorbis ? "ogg" : "mp3";
    string mime = f == Track::Vorbis ? "ogg" : "mpeg";
    return
        "X-Accel-Redirect: /downloads/tracks/" + number(tid) + "."+ext+"\n"
        "Content-Disposition: attachment; filename=\"" + escapeFilename(t.artist() + " - " + t.title()) + "."+ext+"\"\n"
        "Content-Type: audio/"+mime+"\n\n";
}

string Html::trackArt(int tid){
    Track t(tid);
    if(!t) return notFound("Track");
    return
        "X-Accel-Redirect: /downloads/art/" + number(tid) + "\n\n";
}
