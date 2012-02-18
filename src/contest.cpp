#include "contest.h"
#include "db.h"
#include "utils.h"

Contest::Contest(int id){
    _id = 0;

    if(id <= 0) return;

    DB::Result r = DB::query(
        "SELECT name, state FROM contests "
        "WHERE id = " + number(id)
    );
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

void Contest::vote(int tid, std::string host){
    if(state() != Voting) return;
    // Check if the user didn't already vote
    DB::Result r = DB::query(
        "SELECT 1 FROM votes "
        "WHERE contest_id = " + number(_id) +
        " AND track_id = " + number(tid) +
        " AND host = $1"
        , host);
    if(!r.empty()) return;
    // Check if the track is linked to the contest
    r = DB::query(
        "SELECT 1 FROM contest_submissions WHERE contest_id = " + number(_id) +
        " AND track_id = " + number(tid)
    );
    if(r.empty()) return;
    // Update
    DB::query(
        "INSERT INTO votes (host, track_id, contest_id) "
        "VALUES ($1, "+number(tid)+", "+number(_id)+")"
        , host);
    DB::query(
        "UPDATE contest_submissions "
        "SET votes = votes + 1"
        "WHERE track_id = " + number(tid) +
        "AND contest_id = " + number(_id)
        );
}

void Contest::unvote(int tid, std::string host){
    if(state() != Voting) return;
    DB::Result r = DB::query(
        "DELETE FROM votes "
        "WHERE contest_id = " + number(_id) +
        " AND track_id = " + number(tid) +
        " AND host = $1 "
        " RETURNING 1"
        , host);
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

std::vector<int> Contest::usersVotes(std::string host){
    DB::Result r = DB::query(
        "SELECT track_id FROM votes "
        "WHERE host = $1 AND contest_id = " + number(_id)
        , host);
    std::vector<int> tracks;
    for(DB::Result::iterator i = r.begin(); i != r.end(); i++)
        tracks.push_back(number((*i)[0]));
    return tracks;
}
