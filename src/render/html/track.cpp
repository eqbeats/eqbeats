#include "track.h"
#include "page.h"
#include "escape.h"
#include "player.h"
#include "comment.h"
#include "forms.h"
#include "../http.h"
#include "../render.h"
#include "../../art.h"
#include "../../comment.h"
#include "../../category.h"
#include "../../session.h"
#include "../../number.h"
#include "../../path.h"
#include <algorithm>

using namespace std;
using namespace Render;

void cats(const Track &t, bool edition){
    vector<Category> cs = Category::forTrack(t.id());
    if(cs.empty() && !edition) return;
    o << "<div class=\"toolbar cats\"><img src=\"/static/tag.png\" /> Categories:";
    if(edition)
        o << "<form action=\"" << t.url() << "/cat\" method=\"post\">";
    for(vector<Category>::const_iterator i=cs.begin(); i!=cs.end(); i++){
        if(edition)
            o << " <input type=\"checkbox\" name=\"" << i->id() << "\" />";
        o << " <a href=\"" << i->url() << "\">" << i->name() << "</a>";
    }
    if(edition){
        if(!cs.empty())
            o << " <input type=\"submit\" name=\"rmcats\" value=\"Remove selected\" />";
        bool rendered = false;
        vector<Category> all = Category::list();
        for(vector<Category>::const_iterator i=all.begin(); i!=all.end(); i++){
            if(find(cs.begin(),cs.end(),*i) == cs.end()){
                if(!rendered){
                    o << " <select name=\"cat\">";
                    rendered = true;
                }
                o << "<option value=\"" << i->id() << "\">" << i->name() << "</option>";
            }
        }
        if(rendered)
            o << "</select>"
                 "<input type=\"submit\" name=\"addcat\" value=\"Add\" />";
        o << "</form>";
    }
    o << "</div>";
}

void Html::trackPage(int tid){
    Track t(tid);
    if(!t) return notFound("Track");
    bool edition = Session::user().id() == t.artistId();
    Art art(tid);
    if(!edition)
        t.hit();
    string path = getPath();

    // Header
    header(t.title(),
            "<link rel=\"alternate\" type=\"application/json+oembed\" href=\"" + eqbeatsUrl() + "/oembed?url=http%3A//eqbeats.org" + path + "&format=json\">"
            "<link rel=\"alternate\" type=\"text/xml+oembed\" href=\"" + eqbeatsUrl() + "/oembed?url=http%3A//eqbeats.org" + path + "&format=xml\">");

    // Title
    o << "<div class=\"track\">"
      << "<h2>" << escape(t.title()) << " ";
    bool isFav = Session::user().isFavorite(t.id());
    o << "<a href=\"" << (Session::user() ? "" : "/login?redirect=") << t.url() << "/" << (isFav?"un":"") << "favorite\""
         " title=\"" << (isFav?"Remove from favorites":"Add to favorites") << "\">"
           "<img src=\"/static/star" << (isFav?"":"-empty") << ".png\" /></a>"
         "</h2>"
         "<h4>by <a href=\"" << User::url(t.artistId()) <<  "\">" << escape(t.artist()) << "</a></h4>"

      << (art?"<img class=\"art\" src=\"" + art.url(Art::Medium) + "\" />":"");

    player(t);

    // Toolbar
    o << "<div class=\"toolbar\">"
         "<span><img src=\"/static/drive-download.png\" /> Download : "
         " <a href=\"" << t.url(Track::Vorbis) << "\">OGG Vorbis</a>"
         " <a href=\"" << t.url(Track::MP3) << "\">MP3</a>"
      << (art?" <a href=\"" + art.url() + "\" target=\"_blank\">Art</a>":"")
      << "</span> <span><img src=\"/static/balloon-white-left.png\" alt=\"Share\" /> Share : <a href=\"#embedcode\" onclick=\"document.getElementById('embedcode').style.display='block';return false;\">Embed</a></span>";
    if(edition)
        o << " <span><img src=\"/static/edit-number.png\" /> Hits : " << t.getHits()
          << "</span> <span><img src=\"/static/star.png\" /> Favourites : " << t.favoritesCount() << "</span>";
    o << "</div>"

    // Embed
      << "<textarea id=\"embedcode\">" << Html::embedTrackCode(t) << "</textarea>"
         "<script>document.getElementById('embedcode').style.display='none';</script>";

    // Notes
    string notes = t.getNotes();
    if(!notes.empty())
        o << "<div class=\"notes\">" << format(notes) << "</div>";

    // Categories
    cats(t, edition);

    if(edition){
        o << "<h3><img src=\"/static/pencil.png\" /> Edit</h3>"
             "<div class=\"edit\">";
        // Publishing
        if(!t.visible())
            o << "<form class=\"publish\" action=\"" << t.url() << "/publish\" method=\"post\">"
                     "This track is not yet published. "
                     " <input type=\"submit\" value=\"Publish\"/>"
                     "<input type=\"hidden\" name=\"tid\" value=\"" << number(t.id()) << "\"/>"
                 "</form>";
        o << "<div class=\"column\">"
        // Rename
             "<h4>Rename</h4>"
             "<form method=\"post\" action=\"" << t.url() << "/rename\">"
                 "<input type=\"text\" name=\"title\" value=\"" << Html::escape(t.title()) << "\" />"
                 "<input type=\"submit\" value=\"Rename\" />"
             "</form>"
        // Reupload
          << "<h4>Re-upload</h4>";
        uploadForm(t.url()+"/upload");
        // Art
        o << "<h4>Art</h4>"
             "<form action=\"" << t.url() << "/art/upload\" method=\"post\" enctype=\"multipart/form-data\">"
                 "<input type=\"file\" name=\"file\" /><br />"
                 "<input type=\"submit\" value=\"Upload a picture\" />"
             "</form>"
             "</div>"
             "<div class=\"column\">"
        // Notes
             "<h4>Notes</h4>"
             "<form action=\"" << t.url() << "/notes\" method=\"post\">"
                 "<textarea name=\"notes\">" << escape(notes) << "</textarea><br />"
                 "<input type=\"submit\" value=\"Update description\" />"
             "</form>"
        // End
             "</div>"
             "<a class=\"delete\" href=\"" << t.url() << "/delete\">Delete track</a>"
             "<div style=\"clear:both;\"></div>"
             "</div>";
    }

    // Comments
    comments(Comment::forTrack(t.id()));
    commentForm(t.url() + "/comment");

    o << "</div>";
    footer();
}
