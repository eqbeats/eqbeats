#include "news.h"
#include "db.h"
#include "utils.h"

News::News(int nId){
    _id = 0;
    DB::Result r = DB::query("SELECT title, contents FROM news WHERE id = " + number(nId));
    if(!r.empty()){
        _id = nId;
        _title = r[0][0];
        _contents = r[0][1];
    }
}

std::string News::getContents(){
    if(_contents.empty()){
        DB::Result r = DB::query("SELECT contents FROM news WHERE id = " + number(_id));
        if(!r.empty())
            _contents = r[0][0];
    }
    return _contents;
}

std::string News::url(int nid){
    return "/news/" + number(nid);
}

std::vector<News> newsHelper(std::string query){
    DB::Result r = DB::query("SELECT id, title FROM news " + query);
    std::vector<News> news(r.size());
    for(unsigned i=0; i<r.size(); i++){
        news[i] = News(number(r[i][0]), r[i][1]);
    }
    return news;
}
std::vector<News> News::latest(int n){
    return newsHelper("ORDER BY date DESC LIMIT " + number(n));
}

std::vector<News> News::recent(int days){
    return newsHelper("WHERE 'now' - date < '" + number(days) + "d' ORDER BY date DESC");
}
