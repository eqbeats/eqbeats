#ifndef CONTEST_H
#define CONTEST_H

#include <string>
#include <map>
#include <vector>

class Track;

class Contest{

    public:
        Contest(int id);

        int id() const { return _id; }
        std::string name() const { return _name; }

        std::string getDescription() const;
        void addTrack(int tid);

        enum State { Submissions, Voting, Closed };
        State state() const { return _state; }

        void vote(int tid);
        void unvote(int tid);

        static std::string url(int id);
        std::string url() const { return url(_id); }

        std::vector<int> usersVotes() const;
        std::map<int, int> getVotes() const;

        std::vector<Track> submissions();

        bool operator==(const Contest &c) { return c._id == _id; }
        operator bool() const { return _id > 0; }

    private:
        int _id;
        std::string _name;
        State _state;
};

#endif // CONTEST_H
