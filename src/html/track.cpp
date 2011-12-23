#include "html.h"
#include "../session.h"
#include "../utils.h"
#include <sstream>

using namespace std;

string renameForm(const Track &t){
    return
        "<form method=\"post\" action=\"" + t.pageUrl() + "/rename\">"
            "<input type=\"text\" name=\"title\" value=\"" + Html::escape(t.title()) + "\" />"
            "<input type=\"submit\" value=\"Rename\" />"
        "</form>";
}

string uploadForm(int id, const std::string &error){
    stringstream s;
    if(error=="flac")
        s << "<div class=\"error\">Only FLAC files are accepted.</div>";
    s << "<div id=\"track-uploader\">"
            "<noscript>"
                "<form action=\"" << Track::pageUrl(id) << "/upload\" method=\"post\" enctype=\"multipart/form-data\">"
                    "<input type=\"file\" name=\"file\" />"
                    "<input type=\"submit\" />"
                "</form>"
            "</noscript>"
        "</div>"
        "<script src=\"/static/fileuploader.js\" type=\"text/javascript\"></script>"
        "<script>"
            "function createUploader(){"
                "var uploader = new qq.FileUploader({"
                    "element: document.getElementById('track-uploader'),"
                    "action: '" << Track::pageUrl(id) << "/upload',"
                "});"
            "}"
            "window.onload = createUploader;"
        "</script>";
    return s.str();
}

string player(const std::string &url){
    return
        "<script type=\"text/javascript\" src=\"/static/soundmanager2.js\"></script>"
        "<script type=\"text/javascript\">"
            "soundManager.url = '/static/';"
            "soundManager.onready(function(){"
                "var sound = soundManager.createSound({"
                    "id: 'snd',"
                    "url: '" + url + "'"
                "});"
                "document.getElementById(\"playpause\").addEventListener(\"click\", sound.togglePause, false);"
            "});"
        "</script>"
        "<input id=\"playpause\" type=\"button\" value=\"Play\" />"
        ;
}

string Html::trackPage(const Track &track, const std::string error){
    if(!track)
        return header("Track not found", 404) + "Track not found" + footer();
    stringstream s;
    s << header(escape(track.title()), "<link rel=\"stylesheet\" href=\"/static/fileuploader.css\" />")
      << "<div class=\"track\">"
            "<h3>" << escape(track.title()) << "</h3>"
            "<h4><a href=\"" << User::pageUrl(track.artistId()) <<  "\">"
                    << escape(track.artist()) << "</a></h4>";
    string u = track.mp3Url();
    if(!u.empty())
        s << player(u);
    u = track.flacUrl();
    if(!u.empty())
        s << "<a href=\"" << u << "\">FLAC download</a>";
    s << "</div>";
    if(Session::user().id() == track.artistId())
        s << renameForm(track)
          << uploadForm(track.id(), error)
          << "<a href=\"" << track.pageUrl() << "/delete\">Delete</a>";
    s <<  footer();
    return s.str();
}

string Html::trackList(const vector<Track> &tracks, List l){
    stringstream s;
    s <<  "<table class=\"tracklist\">";
    for(vector<Track>::const_iterator i=tracks.begin(); i!=tracks.end(); i++){
        s << "<tr><td><a href=\"" << i->pageUrl() << "\">"
          << escape(i->title()) << "</a></td>";
        if(l == Standard)
            s << "<td><a href=\"" << User::pageUrl(i->artistId()) << "\">"
              << escape(i->artist()) << "</a></td>";
        if(l == Edition)
            s << "<td><form action=\"" << i->pageUrl() << "/visibility\" method=\"post\">"
                    "<input type=\"hidden\" name=\"visible\" value=\"" << (i->visible()?"f":"t") << "\" />"
                    "<input type=\"submit\" value=\"" << (i->visible()?"Hide":"Show") << "\" />"
                "</form></td>";
        s << "</tr>";
    }
    s << "</table>";
    return s.str();
}

string escapeFilename(const string &str){
    string buf;
    for(string::const_iterator i=str.begin(); i!=str.end(); i++){
        if     (*i == '"') buf += "\\\"";
        else if(*i >= ' ') buf += *i;
    }
    return buf;
}

string downloadHelper(int id, const std::string &ext, const std::string &mime){
    Track t(id);
    if(!t) return Html::header("Track not found", 404) + "Track not found" + Html::footer();
    return
        "X-Accel-Redirect: /downloads/tracks/" + number(id) + "."+ext+"\n"
        "Content-Disposition: attachment; filename=\"" + escapeFilename(t.artist() + " - " + t.title()) + "."+ext+"\"\n"
        "Content-Type: audio/"+mime+"\n\n";
}

string Html::downloadFlac(int id){
    return downloadHelper(id, "flac", "x-flac");
}

string Html::downloadMp3(int id){
    return downloadHelper(id, "mp3", "mpeg");
}
