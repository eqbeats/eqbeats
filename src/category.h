#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <vector>

class Category{

    public:
        Category() { _id = 0; }
        Category(int cid);
        Category(int cid, std::string nName)
            { _id = cid; _name = nName; }
        int id() const { return _id; }
        std::string name() const { return _name; }
        std::string url() const;
        static std::vector<Category> list();
        bool operator==(const Category &c) { return c._id == _id; }
        operator bool() const { return _id > 0; }

        void addTrack(int tid);
        void removeTrack(int tid);
        static std::vector<Category> forTrack(int tid);

    private:
        int _id;
        std::string _name;

};

#endif // CATEGORY_H
