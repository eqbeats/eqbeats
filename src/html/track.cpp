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
            "<noscript>"
                "<form action=\"" << action << "\" method=\"post\" enctype=\"multipart/form-data\">"
                    "<input type=\"file\" accept=\"audio/flac\" name=\"file\" />"
                    "<input type=\"submit\" value=\"Upload a FLAC\" />"
                "</form>"
            "</noscript>"
        "</div>"
        "<script src=\"/static/fileuploader.js\" type=\"text/javascript\"></script>"
        "<script>"
            "function createUploader(){"
                "var uploader = new qq.FileUploader({"
                    "element: document.getElementById('track-uploader'),"
                    "allowedExtensions: ['flac'],"
                    "action: '" << action << "'"
                "});"
            "}"
            "window.onload = createUploader;"
        "</script>";
    return s.str();
}

string visibilityForm(const Track &t){
    return
        "<form action=\"" + t.url() + "/visibility\" method=\"post\">"
          "This track is " +
          (t.visible()?"public." : "hidden.") +
          " <input type=\"submit\" value=\"" + (t.visible()?"Hide":"Show") + "\"/>"
          "<input type=\"hidden\" name=\"visible\" value=\"" + (t.visible()?"f":"t") + "\"/>"
      "</form>";
}

string player(const Track &t){
    return
        "<audio controls>"
            "<source type=\"audio/ogg; codecs=vorbis\" src=\"" + t.url(Track::Vorbis) + "\" />"
            "<source type=\"audio/mpeg; codecs=mp3\" src=\"" + t.url(Track::MP3) + "\" />"
        "</audio>";
}

string Html::trackPage(int tid){
    Track t(tid);
    if(!t)
        return notFound("Track");
    if(!t.visible() && t.artistId() != Session::user().id())
        return header("Hidden track", 403) + footer();
    stringstream s;
    bool edition = Session::user().id() == t.artistId();
    if(!edition)
        t.hit();
    s << header(escape(t.title()))
      << "<div class=\"track\">"
            "<h3 style=\"margin:10px;\">by <a href=\"" << User::url(t.artistId()) <<  "\">"
                    << escape(t.artist()) << "</a></h3>"
      << player(t)
      << " Download : "
      << " <a class=\"download\" href=\"" << t.url(Track::Vorbis) << "\">OGG Vorbis</a>"
         " <a class=\"download\" href=\"" << t.url(Track::MP3) << "\">MP3</a>"
         " <a class=\"download\" href=\"" << t.url(Track::FLAC) << "\">FLAC</a>";
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
          << "<a class=\"danger\" href=\"" << t.url() << "/delete\">Delete</a>";
    s << comments(Comment::forTrack(t.id()))
      << commentForm(t.url() + "/comment")
      << "</div>"
      << footer();
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
    if(!t) return Html::header("Track not found", 404) + Html::footer();
    if(!t.visible() && t.artistId() != Session::user().id())
        return header("Hidden track", 403) + footer();
    string ext = f == Track::Vorbis ? "ogg" : f == Track::MP3 ? "mp3" : "flac";
    string mime = f == Track::Vorbis ? "ogg" : f == Track::MP3 ? "mpeg" : "flac";
    return
        "X-Accel-Redirect: /downloads/tracks/" + number(tid) + "."+ext+"\n"
        "Content-Disposition: attachment; filename=\"" + escapeFilename(t.artist() + " - " + t.title()) + "."+ext+"\"\n"
        "Content-Type: audio/"+mime+"\n\n";
}
