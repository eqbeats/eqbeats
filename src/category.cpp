#include "category.h"
#include "db.h"
#include "number.h"
#include <sstream>
#include <algorithm>

Category::Category(int cid){
    DB::Result r = DB::query("SELECT name FROM categories WHERE id = "+number(cid));
    _id = 0;
    if(r.empty())
        return;
    _id = cid;
    _name = r[0][0];
}

std::string Category::url() const{
    return "/cat/" + number(_id);
}

std::vector<Category> getCats(const std::string &q){
    DB::Result r = DB::query(q);
    std::vector<Category> cats(r.size());
    for(unsigned i=0; i<r.size(); i++)
        cats[i] = Category(number(r[i][0]), r[i][1]);
    return cats;
}

std::vector<Category> Category::list(){
    return getCats("SELECT id, name FROM categories ORDER BY name ASC");
}

void Category::addTrack(int tid){
    DB::query("INSERT INTO track_categories (track_id, cat_id) VALUES (" + number(tid)+", "+number(_id) + ")");
}

void Category::removeTrack(int tid){
    DB::query("DELETE FROM track_categories WHERE track_id = " + number(tid) + " AND cat_id = " + number(_id));
}

std::vector<Category> Category::forTrack(int tid){
    return getCats("SELECT categories.id, categories.name FROM categories, track_categories "
        "WHERE track_categories.track_id="+number(tid)+" AND categories.id=track_categories.cat_id");
}
