#include <string>
#include <map>
#include <vector>

class Contest{
    public:
        Contest(int id);

        int id() const { return _id; }
        std::string name() const { return _name; }

        std::string getDescription() const;
        void addTrack(int tid);

        enum State { Submissions, Voting, Closed };
        State state() const { return _state; }

        void vote(int tid, std::string host);
        void unvote(int tid, std::string host);

        static std::string url(int id);
        std::string url() const { return url(_id); }

        std::vector<int> usersVotes(std::string host);
        std::map<int, int> getVotes() const;

        bool operator==(const Contest &c) { return c._id == _id; }
        operator bool() const { return _id > 0; }

    private:
        int _id;
        std::string _name;
        State _state;
};
