#include "html.h"
#include "../session.h"
#include "../utils.h"
#include "../art.h"
#include <sstream>
#include <algorithm>

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
            "<input type=\"file\" name=\"file\" /><br />"
            "<input type=\"submit\" value=\"Upload a picture\" />"
        "</form>";
}

string publishForm(const Track &t){
    return
        "<form class=\"publish\" action=\"" + t.url() + "/publish\" method=\"post\">"
          "This track is not yet published. "
          " <input type=\"submit\" value=\"Publish\"/>"
          "<input type=\"hidden\" name=\"tid\" value=\"" + number(t.id()) + "\"/>"
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

string Html::embedTrackCode(const Track &t, int w){
    if(!w) w = 500;
    return
        "<iframe width=\""+number(w)+"px\" height=\"150px\" frameborder=\"0\" src=\"" + eqbeatsUrl() + t.url() + "/embed\">"
            "<a href=\"" + eqbeatsUrl() + t.url() + "\" target=\"_blank\">" + Html::escape(Html::escape(t.title())) + "</a>"
            " by <a href=\"" + eqbeatsUrl() + User::url(t.artistId()) + "\" target=\"_blank\">" + Html::escape(t.artist()) + "</a>"
        "</iframe>";
}

string embedCode(const Track &t){
    return "<textarea id=\"embedcode\">" + Html::embedTrackCode(t) + "</textarea>"
      "<script>document.getElementById('embedcode').style.display='none';</script>";
}

string cats(const Track &t, bool edition){
    vector<Category> cs = t.getCategories();
    if(cs.empty() && !edition) return string();
    stringstream s;
    s << "<div class=\"cats\">Categories:";
    if(edition)
        s << "<form action=\"" << t.url() << "/cat\" method=\"post\">";
    for(vector<Category>::const_iterator i=cs.begin(); i!=cs.end(); i++){
        if(edition)
            s << " <input type=\"checkbox\" name=\"" << i->id() << "\" />";
        s << " <a href=\"" << i->url() << "\">" << i->name() << "</a>";
    }
    if(edition){
        if(!cs.empty())
            s << " <input type=\"submit\" name=\"rmcats\" value=\"Remove selected\" />";
        s << " <select name=\"cat\">";
        vector<Category> all = Category::list();
        for(vector<Category>::const_iterator i=all.begin(); i!=all.end(); i++){
            if(find(cs.begin(),cs.end(),*i) == cs.end())
                s << "<option value=\"" << i->id() << "\">" << i->name() << "</option>";
        }
        s << "</select>"
             "<input type=\"submit\" name=\"addcat\" value=\"Add\" />"
             "</form>";
    }
    s << "</div>";
    return s.str();
}

string favButton(const Track &t, int uid){
    if(uid == 0) return "";
    bool isFav = User(uid).isFavorite(t.id());
    return (string)"<a class=\""+(isFav?"unfavorite":"favorite")+"\" href=\"" + t.url() + "/" + 
                (isFav?"un":"") + "favorite\">"+(isFav?"Remove from favorites":"Add to favorites")+"</a>";
}

string Html::trackPage(int tid){
    Track t(tid);
    if(!t) return notFound("Track");
    stringstream s;
    bool edition = Session::user().id() == t.artistId();
    Art art(tid);
    if(!edition)
        t.hit();
    s << header(escape(t.title()),
            "<link rel=\"alternate\" type=\"application/json+oembed\" href=\"" + eqbeatsUrl() + "/oembed?url=http%3A//eqbeats.org" + path + "&format=json\">"
            "<link rel=\"alternate\" type=\"text/xml+oembed\" href=\"" + eqbeatsUrl() + "/oembed?url=http%3A//eqbeats.org" + path + "&format=xml\">")
      << "<div class=\"track\">"
         << favButton(t, Session::user().id())
         << "<h2>" + escape(t.title()) + "</h2>"
         << "<h4 style=\"margin:10px;\">by <a href=\"" << User::url(t.artistId()) <<  "\">"
                    << escape(t.artist()) << "</a></h3>"
      << (art?"<img class=\"art\" src=\"" + art.url() + "\" />":"")
      << player(t)
      << "<div class=\"download\">Download : "
      << " <a href=\"" << t.url(Track::Vorbis) << "\">OGG Vorbis</a>"
         " <a href=\"" << t.url(Track::MP3) << "\">MP3</a>"
      << (art?" <a href=\"" + art.url() + "\" target=\"_blank\">Art</a>":"")
      << " &nbsp; Share : <a href=\"#embedcode\" onclick=\"document.getElementById('embedcode').style.display='block';return false;\">Embed</a>";
    if(edition)
        s << " &nbsp; Hits : " << t.getHits() << " &nbsp; Favourites : " << t.favoritesCount();
    s << "</div>" << embedCode(t);
    string notes = t.getNotes();
    if(!notes.empty())
        s << "<div class=\"notes\">" << format(notes) << "</div>";
    s << cats(t, edition);
    if(edition)
        s << "<h3>Edit</h3>"
             "<div class=\"edit\">"
          << (t.visible()?"":publishForm(t))
          << "<div class=\"column\">"
          << "<h4>Rename</h4>" << renameForm(t)
          << "<h4>Re-upload</h4>" << uploadForm(t.url()+"/upload")
          << "<h4>Art</h4>" << artUploadForm(t)
          << "</div>"
          << "<div class=\"column\">"
             "<h4>Notes</h4>"
             "<form action=\"" << t.url() << "/notes\" method=\"post\">"
                 "<textarea name=\"notes\">" << escape(notes) << "</textarea><br />"
                 "<input type=\"submit\" value=\"Update description\" />"
             "</form>"
             "</div>"
             "<a class=\"delete\" href=\"" << t.url() << "/delete\">Delete track</a>"
             "<div style=\"clear:both;\"></div>"
             "</div>";
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
        s << "><a href=\"" << i->url() << "\">" << escape(i->title()) << "</a>";
        if(l == Standard)
            s << " <span class=\"by\">by <a href=\"" << User::url(i->artistId()) << "\">"
              << escape(i->artist()) << "</a></span>";
        s << "</li>";
    }
    s << "</ul>";
    return s.str();
}

string Html::tracksPage(const string &title, const vector<Track> &tracks){
    return header(title)
         + "<h2>" + title + "</h2>"
         + trackList(tracks)
         + footer();
}

string Html::latestTracks(int n){
    return header("Latest tracks", atomFeed("/tracks/latest/atom"))
         + "<h2>Latest tracks " + feedIcon("/tracks/latest/atom") + "</h2>"
         + trackList(Track::latest(n))
         + footer();
}

string Html::trackSearch(const std::string &q){
    stringstream s;
    s << header("Track search")
      << "<h2>Track search</h2>"
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

string Html::category(int cid){
    Category c(cid);
    if(!c) return notFound("Category");
    return header(c.name(), atomFeed(c.url()+"/atom"))
         + "<h2>" + c.name() + "</h2>"
         + trackList(Track::byCategory(cid))
         + footer();
}

string httpFilename(const Track &t){
    string str = t.artist() + " - " + t.title();
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
        "Content-Disposition: attachment; filename=\"" + httpFilename(t) + "."+ext+"\"\n"
        "Content-Type: audio/"+mime+"\n\n";
}

string Html::trackArt(int tid){
    Track t(tid);
    if(!t) return notFound("Track");
    Art art(tid);
    if(!art) return notFound("Art");
    Art::Format f = art.getFormat();
    string ext = f == Art::PNG ? ".png" : f == Art::JPEG ? ".jpg" : "";
    string mime = f == Art::PNG ? "image/png" : f == Art::JPEG ? "image/jpeg" : "";
    return
        "X-Accel-Redirect: /downloads/art/" + number(tid) + "\n"
        "Content-Disposition: inline; filename=\"" + httpFilename(t) + ext + "\"\n"
        + (mime.empty()?"":"Content-Type: " + mime + "\n") + "\n";
}
