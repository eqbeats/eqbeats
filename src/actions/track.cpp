#include <stdio.h>
#include "actions.h"
#include "../html/html.h"
#include "../session.h"
#include "../user.h"
#include "../track.h"
#include "../db.h"
#include "../utils.h"

using namespace std;
using namespace Html;

std::string Action::newTrack(cgicc::Cgicc &cgi){
    User u = Session::user();
    if(!u) return redirect("/");
    if(cgi("title").empty()) return redirect("/");//"/user/..")

    char _id[20];
    sprintf(_id, "%d", u.id());
    const char* val[] = { _id, cgi("title").c_str() };
    PGresult* result = PQexecParams(db, 
        "INSERT INTO tracks (user_id, title, date, license) VALUES "
        "($1, $2, 'now', 'None supplied') "
        "RETURNING id",
        2, 0, val, 0, 0, 0);
    if(PQntuples(result) == 0){ // error?
        PQclear(result);
        return redirect("/");
    }
    int tid = atoi(PQgetvalue(result, 0, 0));
    PQclear(result);
    return redirect(Track::pageUrl(tid));
}

std::string Action::trackVisibility(int id, cgicc::Cgicc &cgi){
    User u = Session::user();
    Track t(id);
    if(u.id() != t.artistId() || !u || 
       ( cgi("visible") != "t" && cgi("visible") != "f" ) ||
       cgi.getEnvironment().getRequestMethod() != "POST")
        return redirect("/");
    t.setVisible(cgi("visible")=="t");
    return redirect(u.pageUrl());
}

std::string Action::renameTrack(int id, cgicc::Cgicc &cgi){
    User u = Session::user();
    Track t(id);
    if(!( u.id() != t.artistId() || !u || cgi("title").empty() ||
       cgi.getEnvironment().getRequestMethod() != "POST" ))
        t.setTitle(cgi("title"));
    return redirect(t.pageUrl());
}

std::string deletionForm(const Track &t){
    return header("Track deletion") +
        "<form method=\"post\">"
            "Do you really want to delete <b>" + escape(t.title()) + "</b> ?"
            "<input type=\"submit\" value=\"Delete\" name=\"confirm\" />"
        "</form>"
        "<a href=\"" + t.pageUrl() + "\">Cancel</a>"
        + footer();
}

std::string Action::deleteTrack(int id, cgicc::Cgicc &cgi){
    User u = Session::user();
    Track t(id);
    if(u.id()!=t.artistId() || !u)
        return redirect(t.pageUrl());
    if(cgi.getEnvironment().getRequestMethod()!="POST" || cgi("confirm")!="Delete")
        return deletionForm(t);
    string query = "DELETE FROM tracks WHERE id = " + number(id);
    PQclear(PQexec(db, query.c_str()));
    string path = "/srv/eqbeats/tracks/" + number(id) + ".flac";
    unlink(path.c_str());
    return redirect(u.pageUrl() + "?msg=trackdeleted");
}
