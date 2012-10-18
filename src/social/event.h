#ifndef SOCIAL_EVENT_H
#define SOCIAL_EVENT_H

#include <track/track.h>

class Event {

    public:
        Event(){ id = 0; }

        enum Type { Publish=1, Comment=2, Favorite=4, Follow=8 };

        // Publish: source = author, track, no target, no message
        // Comment: source = author, target, possible track, message = comment
        // Favorite: source, target = artist, track, no message
        // Follow: source = follower, target = followed, no track, no message

        int id;
        Type type;
        User source, target;
        Track track;
        std::string message;
        std::string date;

        void push();

        void fill(Dict*) const;

};

class EventList: public std::vector<Event> {

    public:
        EventList(){}

        static EventList user(const User &u, int limit=0);
        static EventList track(const Track &t);

        Dict* fill(Dict*, const std::string &section, bool hasForm) const;

    private:
        EventList(std::string cond, int limit=0);

};

#endif // SOCIAL_EVENT_H
