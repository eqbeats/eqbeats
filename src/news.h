#ifndef NEWS_H
#define NEWS_H

#include <vector>
#include <string>

class News {

    public:
        News(int nId);
        News() { _id = -1; }

        int id() const { return _id; }
        std::string title() const { return _title; }

        std::string getContents();

        std::string url() const { return url(_id); }
        static std::string url(int nid);

        static std::vector<News> latest(int n);
        static std::vector<News> recent(int days);

        operator bool() const { return _id > 0; }

        News(int nId, const std::string &nTitle)
            { _id = nId; _title = nTitle; }

    private:
        int _id;
        std::string _title;
        std::string _contents;

};

#endif // NEWS_H
