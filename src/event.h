#ifndef EVENT_H
#define EVENT_H

#include "track.h"
#include "user.h"

class Event {

    public:
        Event(){}

        enum Type { Publish=1, Comment=2, Favorite=4, Follow=8 };
        Type type() const { return _type; }

        User target() const { return _target; }
        User source() const { return _source; }
        Track track() const { return _track; }
        std::string message() const { return _message; }

        static void publish(const Track &t);
        static void favorite(const Track &t, const User &src);
        static void comment(const Track &t, const User &src, std::string msg);
        static void comment(const User &tgt, const User &src, std::string msg);
        static void follow(const User &tgt, const User &src);

        static std::vector<Event> userEvents(const User &u);
        static std::vector<Event> trackEvents(const Track &t);

    private:
        Event(Type nType, User nSource=User(), User nTarget=User(), Track nTrack=Track(), std::string nMessage="");
        static std::vector<Event> select(std::string cond);
        void push();
        Type _type;
        User _target;
        User _source;
        Track _track;
        std::string _message;

};

#endif // EVENT_H
