CXX=c++
WARNINGS := -Wall -Wextra
CXXFLAGS := $(WARNINGS) -Isrc $(shell Magick++-config --cxxflags)
MAGICK_LDFLAGS := $(shell Magick++-config --ldflags)
PROJDIRS := src
SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.cpp")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
OBJFILES := $(patsubst %.cpp,%.o,$(SRCFILES))
DEPFILES := $(patsubst %.cpp,%.d,$(SRCFILES))

.PHONY: all clean

all: eqbeats.fcgi tools/dumptracks tools/fqueue tools/mkthumbs tools/stats tools/udpstat tools/updatetags tools/multiplex tools/clint

clean:
	@-$(RM) $(wildcard $(OBJFILES) $(DEPFILES) eqbeats.fcgi)

%.o: %.cpp Makefile
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPFILES)

eqbeats.fcgi: src/account/account.o src/account/list.o src/account/user.o src/account/session.o \
              src/account/pages/account.o src/account/pages/login.o src/account/pages/registration.o src/account/pages/users.o src/account/pages/password.o \
              src/core/db.o src/core/path.o \
              src/log/log.o \
              src/misc/hash.o src/misc/mail.o src/misc/repl.o src/misc/timer.o \
              src/pages/static.o src/pages/home.o src/pages/user.o src/pages/track.o \
              src/playlist/playlist.o src/playlist/pages/actions.o src/playlist/pages/playlist.o \
              src/render/document.o src/render/fcgiio.o src/render/file.o src/render/http.o \
              src/social/event.o src/social/follower.o src/social/pages/actions.o src/social/pages/comment.o src/social/pages/favorites.o \
              src/stat/push.o \
              src/text/date.o src/text/email.o src/text/format.o src/text/irc.o src/text/number.o src/text/string.o \
              src/track/audio.o src/track/art.o src/track/extended.o src/track/list.o src/track/track.o \
              src/track/pages/actions.o src/track/pages/art.o src/track/pages/track.o src/track/pages/tracks.o src/track/pages/upload.o src/track/pages/oembed.o src/track/pages/license.o \
              src/userfeature/feature.o src/userfeature/pages/actions.o \
              src/youtube/pages/oauth.o src/youtube/pages/upload.o src/youtube/youtube.o \
              src/main.o
	@$(CXX) -lcgicc -lctemplate -lfcgi -lfcgi++ -lnettle -lpcrecpp -lpq -ltag $(MAGICK_LDFLAGS) $^ -o $@

tools/mkthumbs: src/tools/mkthumbs.o src/text/number.o src/track/art.o src/core/path.o
	@$(CXX) $(MAGICK_LDFLAGS) $^ -o $@

tools/stats: src/tools/stats.o src/core/db.o
	@$(CXX) -lpq $^ -o $@

tools/dumptracks: src/tools/dumptracks.o src/core/db.o
	@$(CXX) -lpq $^ -o $@

tools/updatetags: src/tools/updatetags.o src/core/db.o src/text/number.o
	@$(CXX) -lpq -ltag $^ -o $@

tools/fqueue: src/tools/fqueue.o src/core/db.o src/core/path.o src/text/number.o
	@$(CXX) -lpq -ltag $^ -o $@

tools/udpstat: src/tools/udpstat.o src/misc/hash.o src/text/number.o
	@$(CXX) -lnettle $^ -o $@

tools/multiplex: src/tools/Multiplex.hs
	@ghc $^ -o $@

tools/clint: src/tools/clint.cpp
	@$(CXX) $^ -o $@
