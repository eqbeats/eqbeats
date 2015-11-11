import os
import psycopg2
from flask import g

def get_db():
    try:
        return g.db
    except AttributeError:
        db = g.db = psycopg2.connect(os.getenv("EQBEATS_POSTGRES", ""))
        return db

class User:
    def __init__(self, sid=None):
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
