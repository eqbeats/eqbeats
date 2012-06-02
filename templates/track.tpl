<div class=\"track\">
    <h2>{{TITLE}}
        {{#IS_NOT_FAV}}
        <a href="{{#LOGGED_OUT}}/login?redirect={{/LOGGED_OUT}}/track/{{TID}}/favorite"
           title="Add to favorites">
            <img src="/static/icons/star-empty.png" alt="Add to favorites" />
        </a>
        {{/IS_NOT_FAV}}
        {{#IS_FAV}}
        <a href="/track/{{TID}}/unfavorite"
           title="Remove from favorites">
            <img src="/static/icons/star.png" alt="Remove from favorites" />
        </a>
        {{/IS_FAV}}
    if(Session::user() && t.visible()){
        vector<Playlist> playlists = Playlist::forUser(Session::user().id());
        if(!playlists.empty()){
            o << "<span id=\"addplaylist\">"
                 "<img src=\"/static/icons/playlist-add.png\" alt=\"Playlists\" title=\"Playlists\">"
                 "<form action=\"/track/"+number(tid)+"/playlist\" method=\"post\">"
                 "<select name=\"playlist\">";
            for(vector<Playlist>::iterator p = playlists.begin(); p != playlists.end(); p++)
                o << "<option value=\""+number(p->id())+"\">" + escape(p->name()) + "</option>";
            o << "<input type=\"submit\" value=\"Add to this playlist\"/></form>"
                 "</span>";
        }
    }
    o << "</h2>";
    char date[200];
    strcpy(date, t.date().c_str());
    struct tm tm;
    strptime(date, "%Y-%m-%d", &tm);
    strftime(date, 200, "%B %-d, %Y", &tm);
    o << "<h4>by <a href=\"" << t.artist().url() <<  "\">" << escape(t.artist().name()) << "</a> <span class=\"date\">on " << date << "</span></h4>"

      << (art?"<img class=\"art\" alt=\"\" src=\"" + art.url(Art::Medium) + "\" />":"");

    std::string mstatus = m.status();
    if(m.status().empty())
        player(t);
    else
        o << "<div class=\"status\">Status: " << mstatus << "</div>";

    // Toolbar
    o << "<div class=\"toolbar\">";
    if(mstatus.empty()){
        o << "<span>" << icon("drive-download") << " Download : "
             "<a href=\"" << t.url(Track::Vorbis) << "\">OGG Vorbis</a> ";
        if(m.extension().empty() || m.extension() == ".mp3")
             o << "<a href=\"" << t.url(Track::MP3) << "\">Original MP3</a> ";
        else{
             o << "<a href=\"" << t.url(Track::MP3) << "\">MP3</a> "
                  "<a href=\"" << t.url(Track::Original) << "\">Original (" << escape(m.extension()) << ")</a> ";
        }
        if (art) o << "<a href=\"" + art.url() + "\" target=\"_blank\">Art</a>";
        o << "</span> ";
    }
    o << "<span>" << icon("balloon-white-left") << " Share : <a href=\"#embedcode\" onclick=\"document.getElementById('embedcode').style.display='block';return false;\">Embed</a></span>";
    if(edition){
        if(hits)
            o << " <span>" << icon("edit-number") << " Hits : " << hits << "</span>";
        o << " <span>" << icon("star") << " Favourites : " << t.favoritesCount() << "</span>";
    }
    else
        o << " <form action=\"" << t.url() << "/report\" method=\"post\">"
                "<button type=\"submit\" class=\"report\">" << icon("flag") << "<span> Report</span></button>"
             "</form>";
    o << "</div>"
    // Embed
         "<textarea id=\"embedcode\" style=\"display:none;\">" << Html::embedTrackCode(t) << "</textarea>";

    // Tags
    if(!t.tags().empty() || edition){
        o << "<div class=\"toolbar tags\">" << icon("tag") << " Tags:";
        vector<string> ts = t.tags();
        if(edition){
            o << " <form action=\"" << t.url() << "/tags\" method=\"post\">"
                    "<input name=\"tags\" value=\"";
            for(vector<string>::const_iterator i=ts.begin(); i!=ts.end(); i++){
                if(i != ts.begin()) o << ", ";
                o << escape(*i);
            }
            o <<    "\" /> <input type=\"submit\" value=\"Update\" /> <span class=\"legend\">(comma-separated, e.g. instrumental, electronic)</span></form>";
        }
        else{
            for(vector<string>::const_iterator i=ts.begin(); i!=ts.end(); i++)
                o << " <a href=\"/tracks/tag/" << escape(*i) << "\">" << escape(*i) << "</a>";
        }
        o << "</div>";
    }

    // License
    o << "<div class=\"license\">"
      << (t.license() == "Copyright" ? "Copyright &copy; " + escape(t.artist().name()) : "License: " + escape(t.license()))
      << (edition ? " <a href=\"" + t.url() + "/license\">(change)</a>" : "")
      << "</div>";

    // Notes
    if(!t.notes().empty())
        o << "<div class=\"notes\">" << format(t.notes()) << "</div>";

    if(edition){
        o << "<div class=\"edit\">"
             "<h3>" << icon("pencil") << " Edit</h3>";
        // Publishing
        if(!t.visible())
            o << "<form class=\"publish\" action=\"" << t.url() << "/publish\" method=\"post\">"
                     << icon("disc-arrow") <<
                     " This track is not yet published."
                     " <input type=\"submit\" value=\"Publish\"/>"
                     "<input type=\"hidden\" name=\"tid\" value=\"" << number(t.id()) << "\"/>"
                 "</form>";
        // Rename
        o << "<div class=\"rename\">"
             "<h4>" << icon("rename") << " Rename</h4>"
             "<form method=\"post\" action=\"" << t.url() << "/rename\">"
                 "<b>" << Html::escape(t.artist().name()) << "</b> - "
                 "<input type=\"text\" name=\"title\" value=\"" << Html::escape(t.title()) << "\" />"
                 "<input type=\"submit\" value=\"Rename\" />"
             "</form>"
             "</div>"
             "<div class=\"column\">"
        // Reupload
             "<h4>" << icon("drive-upload") << " Re-upload</h4>";
        uploadForm(t.url()+"/upload");
        // Art
        o << "<h4>" << icon("picture") << " Art</h4>"
             "<form action=\"" << t.url() << "/art/upload\" method=\"post\" enctype=\"multipart/form-data\">"
                 "<input type=\"file\" name=\"file\" />"
                 "<input type=\"submit\" value=\"Upload a picture\" />"
             "</form>"
        // Flags
             "<h4>" << icon("balloon-sound") << " Broadcast</h4>"
             "<form action=\"" << t.url() << "/flags\" method=\"post\">"
                 "<input type=\"checkbox\" name=\"airable\" "
                   << (t.airable() ? "checked=\"checked\" " : "") << " /> "
                 "Celestia Radio</span><br />"
                 "<input type=\"submit\" value=\"Update\" />"
             "</form>"
        // Youtube
             "<h4>" << icon("youtube-up") << "Youtube</h4>";
        if(t.artist().hasYoutube()){
            o << "<form action=\""<<t.url()<<"/youtube_upload\">"
                 "<input type=\"submit\" value=\"Upload this to YouTube\"/>"
                 " <a class=\"huh\" href=\"/faq#youtube\">Huh?</a>"
                 "</form>";
        } else {
            o << "<form action=\"https://accounts.google.com/o/oauth2/auth\">"
                 "Your YouTube account is not linked."
                 " <a class=\"huh\" href=\"/faq#youtube\">Huh?</a>"
                 "</br>"
                 "<input type=\"submit\" value=\"Link your YouTube account\"/>"
                 "<input type=\"hidden\" name=\"response_type\" value=\"code\"/>"
                 "<input type=\"hidden\" name=\"client_id\" value=\"767490682254.apps.googleusercontent.com\"/>"
                 "<input type=\"hidden\" name=\"scope\" value=\"https://uploads.gdata.youtube.com/feeds/api/users/default/uploads\"/>"
                 "<input type=\"hidden\" name=\"access_type\" value=\"offline\"/>"
                 "<input type=\"hidden\" name=\"redirect_uri\" value=\"http://eqbeats.org/oauth/yt\"/>"
                 "<input type=\"hidden\" name=\"approval_prompt\" value=\"force\"/>"
                 "</form>";
        }
        o << "</div>"
             "<div class=\"column\">";
        // Notes
        o << "<h4>" << icon("card-pencil") << " Notes</h4>"
             "<form action=\"" << t.url() << "/notes\" method=\"post\">"
                 "<textarea name=\"notes\">" << escape(t.notes()) << "</textarea><br />"
                 "<div class=\"legend\">(tags: [b]old, [u]nderline, [i]talic)</div>"
                 "<input type=\"submit\" value=\"Update description\" />"
             "</form>"
        // End
             "</div>"
             "<a class=\"delete\" href=\"" << t.url() << "/delete\">Delete track</a>"
             "<div style=\"clear:both;\"></div>"
             "</div>";
    }

    // Events
    eventStream(Event::trackEvents(t), "Latest updates", false);
    commentForm(t.url() + "/comment");

    o << "</div>";

