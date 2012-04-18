#include "contest.h"
#include "db.h"
#include "number.h"
#include "track.h"
#include "cgi.h"
#include "session.h"
#include "user.h"
#include "log.h"

Contest::Contest(int id){
    _id = 0;
    if(id <= 0) return;
    DB::Result r = DB::query("SELECT name, state FROM contests WHERE id = " + number(id));
    if(!r.empty()){
        _id = id;
        _name = r[0][0];
        _state = r[0][1] == "submissions" ? Contest::Submissions : r[0][1] == "voting" ? Contest::Voting : Contest::Closed;
    }
}


std::string Contest::getDescription() const{
    DB::Result r = DB::query(
        "SELECT description FROM contests "
        "WHERE id = " + number(_id)
    );
    return r.empty()?"":r[0][0];
}

void Contest::addTrack(int tid){
    if(state() != Submissions) return;
    DB::query(
        "INSERT INTO contest_submissions (track_id, contest_id) "
        "VALUES (" + number(tid) + ", " + number(_id) + ")"
    );
}

void Contest::vote(int tid){
    if(state() != Voting) return;
    std::string host = cgi.getEnvironment().getRemoteAddr();
    User u = Session::user();
    // Check if the user didn't already vote
    if(u) log("u!");
    else log(host);
    DB::Result r = DB::query(
        "SELECT 1 FROM votes "
        "WHERE contest_id = " + number(_id) +
        " AND track_id = " + number(tid) +
        " AND " +
         (u.id()!=0?"user_id = "+number(u.id()):
            "host = '"+host+"' AND user_id ISNULL")
        );
    if(!r.empty()) return;
    // Check if the track is linked to the contest
    r = DB::query(
        "SELECT 1 FROM contest_submissions WHERE contest_id = " + number(_id) +
        " AND track_id = " + number(tid)
    );
    if(r.empty()) return;
    // Update
    DB::query(
        "INSERT INTO votes (host, track_id, contest_id, user_id) "
        "VALUES ($1, "+number(tid)+", "+number(_id)+", "+(u?number(u.id()):"null")+")"
        , host);
    DB::query(
        "UPDATE contest_submissions "
        "SET votes = votes + 1"
        "WHERE track_id = " + number(tid) +
        "AND contest_id = " + number(_id)
        );
}

void Contest::unvote(int tid){
    if(state() != Voting) return;
    User u = Session::user();
    DB::Result r = DB::query(
        "DELETE FROM votes "
        "WHERE contest_id = " + number(_id) +
        " AND track_id = " + number(tid) +
     (u?" AND user_id = " + number(u.id()):
        " AND host = '"+cgi.getEnvironment().getRemoteAddr()+"'"
        " AND user_id ISNULL") +
        " RETURNING 1");
    if(!r.empty())
        DB::query(
            "UPDATE contest_submissions "
            "SET votes = votes - 1"
            "WHERE track_id = " + number(tid) +
            "AND contest_id = " + number(_id)
            );
}

std::map<int, int> Contest::getVotes() const{
    DB::Result r = DB::query(
        "SELECT track_id, votes FROM contest_submissions "
        "WHERE contest_id = " + number(_id)
        );
    std::map<int, int> tracks;
    for(DB::Result::iterator i = r.begin(); i != r.end(); i++)
        tracks[number((*i)[0])] = number((*i)[1]);
    return tracks;
}

std::string Contest::url(int id){
    return "/contest/" + number(id);
}

std::vector<int> Contest::usersVotes() const{
    User u = Session::user();
    DB::Result r = DB::query(
        "SELECT track_id FROM votes "
        "WHERE contest_id = " + number(_id) +
     (u?" AND user_id = " + number(u.id()):
        " AND host = '" + cgi.getEnvironment().getRemoteAddr() + "'"
        " AND user_id ISNULL"));
    std::vector<int> tracks;
    for(DB::Result::iterator i = r.begin(); i != r.end(); i++)
        tracks.push_back(number((*i)[0]));
    return tracks;
}

std::vector<Track> Contest::submissions(){
    return Track::select("contest_submissions", 
        "tracks.id = contest_submissions.track_id AND contest_submissions.contest_id = " + number(_id),
        "tracks.date ASC");
}
