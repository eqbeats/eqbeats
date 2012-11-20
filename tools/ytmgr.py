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

sockloc = "/tmp/ytmgr-"+str(os.getuid())+".sock"

class YoutubeManager:
    def __init__(self):
        self.socket = socket.socket(socket.AF_UNIX)

    def db(self):
        db = psycopg2.connect("")
        db.set_client_encoding("UTF8")
        return db

    def log(self, msg):
        os.chdir(os.getenv("EQBEATS_DIR"))
        f = open("ytmgr.log", "a")
        f.write("\n\n" + datetime.datetime.utcnow().isoformat() + "\n")
        f.write(msg)
        f.write("\n")
        f.close()

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

        except (TypeError, KeyError):
            c.execute("DELETE FROM youtube_refresh_tokens WHERE user_id = %s", (uid,))
            db.commit()
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
        eqrender ="/tmp/eqrender-" + str(os.getuid()) + "/"
        os.makedirs(eqrender, exist_ok=True)
        out = eqrender + str(tid) + ".avi"
        if os.access("art/"+str(tid), os.F_OK):
            art = eqrender + str(tid)+".bmp"
            os.spawnvp(os.P_WAIT, "convert", ("convert", "-flatten", "art/" + str(tid), art))
        else:
            art = "static/placeholder.jpg"
        #os.spawnvp(os.P_WAIT, "ffmpeg", ("ffmpeg","-loop","1","-r","0.1","-shortest","-i", art,"-i","tracks/"+str(tid)+".mp3","-vf","scale=-1:720,scale=trunc(iw/2)*2:0","-vcodec","libtheora","-acodec","copy","-y",out))
        os.spawnvp(os.P_WAIT, "ffmpeg", ("ffmpeg","-loop","1","-r","0.1","-shortest","-i", art,"-i","tracks/"+str(tid)+".mp3","-vf","scale=-1:720","-vcodec","libtheora","-acodec","copy","-y",out))
        return open(out, "rb")

    def upload(self, tid):
        db = self.db()
        c = db.cursor()
        c.execute("SELECT users.name, tracks.title, tracks.notes, tracks.tags, youtube_access_tokens.token, tracks.visible FROM tracks, users, youtube_access_tokens WHERE tracks.id = %s and tracks.user_id = users.id and tracks.user_id = youtube_access_tokens.user_id and youtube_access_tokens.expire > 'now'", (tid,))
        data = c.fetchone()
        if data:
            visible = data[5]
            title = html.escape((data[0] + " - " + data[1]).translate(str.maketrans("", "", "<>"))).encode("utf-8")[:100]
            desc = re.sub("\[/?[bis]\]", "", data[2]).translate(str.maketrans("","","<>")) + "\n--\nhttp://eqbeats.org/track/" + str(tid) + "\nDownload: http://eqbeats.org/track/" + str(tid) + "/mp3"
            tags = (tag for tag in data[3] if not "<" in tag and not ">" in tag and not len(tag.encode("utf-8")) < 2 and not len(tag.encode("utf-8")) > 30)
            tagstring = ""
            for tag in tags:
                if len(tag) + len(tagstring) + 1 > 500:
                    break
                else:
                    tagstring += tag + ","
            tagstring = tagstring[:-1]
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
    <media:title type="plain">"""+title+b"""</media:title>
    <media:description type="plain">
    """+html.escape(desc).encode("utf-8")+b"""
    </media:description>
    <media:category scheme="http://gdata.youtube.com/schemas/2007/categories.cat">Music</media:category>
    <media:keywords>"""+html.escape(tagstring).encode("utf-8")[:500]+b"""</media:keywords>
</media:group>
""" + (b"""<yt:accessControl action='list' permission='denied'/>""" if not visible else b"")
            body += b"""</entry>
--"""+boundary+b"""
Content-Type: video/avi
Content-Transfer-Encoding: binary

"""+f.read()+b"""
--"""+boundary+b"""--"""
            headers = {
                    "X-GData-Key": "key=%s" % os.getenv("EQBEATS_YT_API_KEY"),
                    "GData-Version": "2",
                    "Slug": "video", # worst bogus filename ever
                    "Content-Type": "multipart/related; boundary="+boundary.decode("utf-8"),
                    "Authorization": "OAuth " + data[4]
                    }
            h.request("POST", "/feeds/api/users/default/uploads", body, headers)
            r = h.getresponse()
            if r.status != 201:
                self.log("YT upload failed. Track : " + str(tid) + "\nResponse follows:\n" + r.read().decode("utf-8"))
            else:
                self.log("YT upload successful. Track : " + str(tid))
        else:
            try:
                c.execute("SELECT user_id FROM tracks WHERE id=" + str(tid))
                if self.new_access_token(c.fetchone()[0]):
                    self.upload(tid)
                ### else: warn that account has been unlinked somehow?
            except IndexError:
                pass
    def run(self):
        if(not(os.getenv("EQBEATS_DIR") and os.getenv("EQBEATS_YT_API_KEY"))):
            print("Environment is not set up correctly. Both EQBEATS_DIR and EQBEATS_YT_API_KEY are needed")
            exit(1)
        os.chdir(os.getenv("EQBEATS_DIR"))
        self.socket.bind(sockloc)
        self.socket.settimeout(None)
        self.socket.listen(5)
        while True:
            try:
                s, _ = self.socket.accept()
                s.settimeout(3000)
                command = b""
                while command[-1:] != b"\n":
                    command += s.recv(2048)
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
                elif command[:4] == b"exit":
                    s.send(b"OK\n")
                    s.shutdown(socket.SHUT_RDWR)
                    exit(0)
                else:
                    s.send(b"! Not a command\n")
            except TypeError:
                s.send(b"! Wrong number of arguments\n")
            except ValueError:
                s.send(b"! Illegal argument\n")
            except socket.timeout:
                pass
            except (KeyboardInterrupt, SystemExit):
                print("\nclosing sockets")
                self.socket.shutdown(socket.SHUT_RDWR)
                self.socket.close()
                os.remove(sockloc)
                exit(0)

yt = YoutubeManager()
yt.run()
