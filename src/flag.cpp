#include "flag.h"
#include "db.h"
#include "track.h"
#include "number.h"
#include <sstream>
#include <algorithm>

Flag::Flag(int fid){
    DB::Result r = DB::query("SELECT name FROM categories WHERE id = "+number(fid));
    _id = 0;
    tid = 0;
    if(r.empty())
        return;
    _id   = fid;
    _name = r[0][0];
    _type = Category;
    _set  = true;
}

std::string Flag::url() const{
    return "/cat/" + number(_id);
}

Flag& Flag::operator=(bool set){
    if(!tid || !_id || _set==set) return *this;
    DB::query( set
              ? "INSERT INTO track_categories (track_id, cat_id) VALUES (" + number(tid)+", "+number(_id) + ")"
              : "DELETE FROM track_categories WHERE track_id = " + number(tid) + " AND cat_id = " + number(_id));
    _set = set;
    return *this;
}

std::vector<Track> Flag::tracks(){
    return Track::select("track_categories",
        "track_categories.track_id = tracks.id AND track_categories.cat_id = " + number(_id));
}

// Flags

Flags::Flags(int tid){
    DB::Result r = DB::query(
        "SELECT id, name, EXISTS (SELECT 1 FROM track_categories WHERE cat_id=id AND track_id="+number(tid)+") FROM categories ORDER BY name ASC");
    reserve(r.size());
    for(DB::Result::const_iterator i=r.begin(); i!=r.end(); i++)
        push_back(Flag(number((*i)[0]), (*i)[1], Flag::Category, (*i)[2][0]=='t', tid));
}

Flag Flags::operator[](int fid){
    for(const_iterator i=begin(); i!=end(); i++){
        if(i->id() == fid)
            return *i;
    }
    return Flag();
}

Flag Flags::operator[](const std::string &flag){
    for(const_iterator i=begin(); i!=end(); i++){
        if(i->name() == flag)
            return *i;
    }
    return Flag();
}

Flags Flags::all(){
    DB::Result r = DB::query("SELECT id, name FROM categories ORDER BY name ASC");
    Flags fs;
    fs.resize(r.size());
    for(unsigned i=0; i<r.size(); i++)
        fs.push_back(Flag(number(r[i][0]), r[i][1], Flag::Category));
    return fs;
}

Flags Flags::categories(){
    Flags fs;
    for(const_iterator i=begin(); i!=end(); i++){
        if(i->type() == Flag::Category)
            fs.push_back(*i);
    }
    return fs;
}

Flags Flags::operator()(bool set){
    Flags fs;
    for(const_iterator i=begin(); i!=end(); i++){
        if(*i == set)
            fs.push_back(*i);
    }
    return fs;
}
