#!/usr/bin/python
import html
import os
import sys
import time
import re
import json
import random
import socket
import sqlite3
import threading
import psycopg2
import datetime
import http.client as hc
import urllib.parse as up

class YoutubeManager:
    def __init__(self):
        self.socket = socket.socket(socket.AF_UNIX)

    def db(self):
        db = psycopg2.connect("")
        db.set_client_encoding("UTF8")
        return db

    def auth(self, auth_token, uid):
        h = hc.HTTPSConnection("accounts.google.com")
        params = up.urlencode({
            "code": auth_token,
            "client_id": "767490682254.apps.googleusercontent.com",
            "client_secret": "pfgW6I2CZHy8v1cboyM4r8fl",
            "grant_type": "authorization_code",
            "redirect_uri": "http://eqbeats.org/oauth/yt" # dunno why this is required
            })
        headers = {"Content-type": "application/x-www-form-urlencoded"}
        h.request("POST", "/o/oauth2/token", params, headers)
        try:
            data = json.loads(h.getresponse().read().decode("utf-8"))
            db = self.db()
            c = db.cursor()
            c.execute("INSERT INTO youtube_refresh_tokens (user_id, token) VALUES (%s, %s)", (uid, data["refresh_token"]))
            self.insert_access_token(data["access_token"], data["expires_in"], uid)
            db.commit()
            return True
        except KeyError:
            return False

    def new_access_token(self, uid):
        db = self.db()
        c = db.cursor()
        c.execute("SELECT token FROM youtube_refresh_tokens WHERE user_id = %s", (uid,))
        try:
            token = c.fetchone()[0]
            h = hc.HTTPSConnection("accounts.google.com")
            params = up.urlencode({
                "client_id": "767490682254.apps.googleusercontent.com",
                "client_secret": "pfgW6I2CZHy8v1cboyM4r8fl",
                "grant_type": "refresh_token",
                "refresh_token": token
                })
            headers = {"Content-type": "application/x-www-form-urlencoded"}
            h.request("POST", "/o/oauth2/token", params, headers)
            data = json.loads(h.getresponse().read().decode("utf-8"))
            self.insert_access_token(data["access_token"], data["expires_in"], uid)
            return True

        except (TypeError, KeyError) as e:
            c.execute("DELETE FROM youtube_refresh_tokens WHERE user_id = %s", (uid,))
            db.commit()
            print(e)
            return False


    def insert_access_token(self, token, expires_in, uid):
        db = self.db()
        c = db.cursor()
        c.execute("DELETE FROM youtube_access_tokens WHERE user_id = %s", (uid,))
        db.commit()
        c.execute("INSERT INTO youtube_access_tokens (user_id, token, expire) VALUES (%s, %s, %s)", (uid, token, datetime.datetime.fromtimestamp(time.time() + expires_in)))
        db.commit()

    def mkvideo(self, tid):
        os.chdir(os.getenv("EQBEATS_DIR", "."))
        os.makedirs("/tmp/eqrender/", exist_ok=True)
        out = "/tmp/eqrender/" + str(tid) + ".avi"
        if os.access("art/"+str(tid), os.F_OK):
            art = "/tmp/eqrender/"+str(tid)+".bmp"
            os.spawnvp(os.P_WAIT, "convert", ("convert", "-flatten", "art/" + str(tid), art))
        else:
            art = "static/placeholder.jpg"
        os.spawnvp(os.P_WAIT, "ffmpeg", ("ffmpeg","-loop","1","-r","0.1","-shortest","-i", art,"-i","tracks/"+str(tid)+".mp3","-vf","scale=-1:720,scale=trunc(iw/2)*2:0","-vcodec","libtheora","-acodec","copy","-y",out))
        return open(out, "rb")

    def upload(self, tid):
        db = self.db()
        c = db.cursor()
        c.execute("SELECT tracks.title, tracks.notes, tracks.tags, youtube_access_tokens.token FROM tracks, youtube_access_tokens WHERE tracks.id = %s and tracks.user_id = youtube_access_tokens.user_id and youtube_access_tokens.expire > 'now'", (tid,))
        data = c.fetchone()
        if data:
            print(data)
            desc = re.sub("\[/?[bis]\]", "", data[1]).translate(str.maketrans("","","<>"))
            tags = (tag for tag in data[2] if not "<" in tag and not ">" in tag)
            f = self.mkvideo(tid)
            h = hc.HTTPSConnection("uploads.gdata.youtube.com")
            boundary = ("".join(random.choice("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_") for _ in range(15))).encode("utf-8")
            body = b"""
--""" + boundary + b"""
Content-Type: application/atom+xml; charset=UTF-8

<?xml version="1.0"?>
<entry xmlns="http://www.w3.org/2005/Atom"
xmlns:media="http://search.yahoo.com/mrss/"
xmlns:yt="http://gdata.youtube.com/schemas/2007">
<media:group>
    <media:title type="plain">"""+data[0].translate(str.maketrans("", "", "<>")).encode("utf-8")+b"""</media:title>
    <media:description type="plain">
    """+desc.encode("utf-8")+b"""
    </media:description>
    <media:category scheme="http://gdata.youtube.com/schemas/2007/categories.cat">Music</media:category>
    <media:keywords>"""+(html.escape(",".join(tags))).encode("utf-8")+b"""</media:keywords>
</media:group>
<yt:accessControl action='list' permission='denied'/>
</entry>
--"""+boundary+b"""
Content-Type: video/avi
Content-Transfer-Encoding: binary

"""+f.read()+b"""
--"""+boundary+b"""--"""
            headers = {
                    "X-GData-Key": "key=AI39si7oQH78ZCSIMu7d8CFZDj6Lhwj0K6NlGMFSzTbRJo2QU9chKlHNUSVoBaj38F0J5aIiLZ0UQu8PhET17q7kAlcgHCbU9w",
                    "GData-Version": "2",
                    "Slug": "video", # worst bogus filename ever
                    "Content-Type": "multipart/related; boundary="+boundary.decode("utf-8"),
                    "Authorization": "OAuth " + data[3]
                    }
            g = open("/tmp/body", "wb")
            g.write(body)
            del g
            h.request("POST", "/feeds/api/users/default/uploads", body, headers)
            r = h.getresponse()
            print(r.status, r.reason)
            print(r.read())
        else:
            c.execute("SELECT tracks.user_id FROM tracks WHERE tracks.id = %s", (tid,))
            try:
                if self.new_access_token(c.fetchone()[0]):
                    self.upload(tid)
                ### else: warn that account has been unlinked somehow?
            except IndexError:
                pass
    def run(self):
        os.chdir(os.getenv("EQBEATS_DIR"))
        self.socket.bind("./ytmgr.sock")
        self.socket.settimeout(None)
        self.socket.listen(5)
        while True:
            try:
                s, _ = self.socket.accept()
                s.settimeout(3000)
                command = b""
                while command[-1:] != b"\n":
                    command += s.recv(2048)
                print(command)
                if command[:4] == b"auth":
                    args = command[5:-1].split(b" ")
                    args[1] = int(args[1]) # uid
                    if self.auth(*args):
                        s.send(b"OK\n")
                    else:
                        s.send(b"! Key was incorrect\n")
                elif command[:6] == b"upload":
                    tid = int(command[7:-1])
                    threading.Thread(target=self.upload, args=(tid,)).start()
                    s.send(b"OK\n")
                else:
                    print("unknown command")
            except TypeError:
                print("wrong number of arguments")
                s.send(b"! Wrong number of arguments\n")
            except ValueError:
                print("not a number")
                s.send(b"! Illegal argument\n")
            except socket.timeout:
                print("timeout", e)
            except (KeyboardInterrupt, SystemExit):
                print("\nclosing sockets")
                self.socket.shutdown(socket.SHUT_RDWR)
                self.socket.close()
                os.remove("ytmgr.sock")
                exit(0)

yt = YoutubeManager()
yt.run()
