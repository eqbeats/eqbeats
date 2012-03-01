#ifndef FLAG_H
#define FLAG_H

#include <string>
#include <vector>

class Track;

class Flag{

    public:

        enum Type { Attribute, Category };

        Flag() { _id = 0; tid = 0; }
        Flag(int fid);
        Flag(int fid, std::string nName, Type nType, bool nSet=false, int nTid=0)
            { _id = fid; _name = nName; _type = nType; _set = nSet; tid = nTid; }

        Type type() const { return _type; }
        int id() const { return _id; }
        std::string name() const { return _name; }

        std::string url() const;

        operator bool() const { return _id > 0 && _set; }
        Flag& operator=(bool set);

        std::vector<Track> tracks();

    private:
        int tid;
        int _id;
        std::string _name;
        Type _type;
        bool _set;

};

class Flags: public std::vector<Flag> {

    public:
        Flags() {}
        Flags(int tid);

        Flag operator[](int fid);
        Flag operator[](const std::string &flag);

        static Flags all();

        Flags categories();
        Flags operator()(bool set);

};

#endif // FLAG_H
