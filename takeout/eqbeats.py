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
            one = cur.fetchone()
            if not one: return
            self.id, self.name, self.nonce = one
            self.sid = sid
    def valid(self):
        return self.id > 0
