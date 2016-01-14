import os
import psycopg2
from flask import g
import glob
import uuid

def get_db():
    try:
        return g.db
    except AttributeError:
        db = g.db = psycopg2.connect(os.getenv("EQBEATS_POSTGRES", ""))
        db.set_client_encoding("utf-8")
        return db

class User:
    def __init__(self, sid=None, email=None, password=None):
        self.id = 0
        self.name = None
        self.sid = None
        self.nonce = None
        if sid:
            cur = get_db().cursor()
            cur.execute("SELECT users.id, users.name, sessions.nonce FROM users, sessions WHERE sessions.sid = %s AND users.id = sessions.user_id;", (sid,))
            row = cur.fetchone()
            if not row: return
            self.id, self.name, self.nonce = row
            self.sid = sid
        elif email and password:
            cur = get_db().cursor()
            cur.execute("SELECT users.id, users.name FROM users WHERE lower(users.email) = lower(%s) AND users.password = crypt(%s, password);", (email, password))
            row = cur.fetchone()
            if not row: return
            self.id, self.name = row
            self.sid = str(uuid.uuid4())
            cur.execute("INSERT INTO sessions (user_id, sid, host, date) VALUES (%s, %s, '::1', 'now');", (self.id, self.sid))
            g.db.commit();


    def valid(self):
        return self.id > 0

    def tracks(self):
        tracks = []
        cur = get_db().cursor()
        cur.execute("SELECT id, title, visible FROM tracks WHERE user_id = %s;", (self.id,))
        row = cur.fetchone()
        while row:
            track = {}
            track['id'], track['title'], track['visible'] = row
            tracks.append(track)
            row = cur.fetchone()
        return tracks

    def logout(self):
        cur = get_db().cursor()
        cur.execute("DELETE FROM sessions WHERE user_id = %s AND sid = %s;", (self.id, self.sid))
        g.db.commit()

def orig_file(tid):
    directory = os.getenv("EQBEATS_DIR") + "/tracks/"
    glob_result = glob.glob(directory + "%s.orig.*" % (tid,))
    if len(glob_result)> 0:
        return glob_result[0]

def art(tid):
    directory = os.getenv("EQBEATS_DIR") + "/art/"
    glob_result = glob.glob(directory + "%s*" % (tid,))
    if len(glob_result)> 0:
        return glob_result[0]
