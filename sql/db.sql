CREATE TABLE users (
    id serial primary key,
    name text not null,
    password text not null,
    email text not null,
    about text not null default '',
    registration timestamptz not null,
    last_login timestamptz not null,
    notify bool not null default true,
    license text not null default 'Copyright'
);

CREATE TABLE tracks (
    id serial primary key,
    user_id integer not null REFERENCES users(id),
    title text not null,
    date timestamptz not null,
    license text not null default 'Copyright',
    hits integer not null default 0,
    visible boolean not null default 'f',
    notes text not null default '',
    airable boolean not null default 't',
    tags text[] not null default ARRAY[]::text[]
);

CREATE TABLE sessions (
    sid text not null,
    host inet not null,
    user_id integer not null REFERENCES users(id),
    date timestamptz not null,
    UNIQUE (sid, host)
);

CREATE TABLE ticker (
    id serial primary key,
    title text not null,
    url text not null,
    expire timestamptz not null
);

CREATE TYPE comment_type AS ENUM ('track', 'user');
CREATE TABLE comments (
    ref integer not null,
    type comment_type not null,
    author_id integer not null,
    author_name text not null,
    contents text not null,
    date timestamptz not null
);

CREATE TYPE favorite_type AS ENUM ('track', 'artist');
CREATE TABLE favorites (
    user_id integer not null,
    ref integer not null,
    type favorite_type not null
);

CREATE TYPE contest_state AS ENUM ('submissions', 'voting', 'closed');
CREATE TABLE contests (
    id serial primary key,
    name text not null,
    description text,
    state contest_state not null default 'submissions'
);
CREATE TABLE contest_submissions (
    contest_id int not null REFERENCES contests(id),
    track_id int not null REFERENCES tracks(id),
    votes int not null default 0,
    UNIQUE (contest_id, track_id)
);
CREATE TABLE votes (
    host inet not null,
    user_id integer,
    track_id integer not null REFERENCES tracks(id),
    contest_id integer not null REFERENCES contests(id)
);

CREATE TABLE featured_tracks (
    track_id integer not null REFERENCES tracks(id),
    date timestamptz not null
);

CREATE TABLE resets (
    user_id int not null REFERENCES users(id),
    token text unique not null
);

CREATE TYPE event_type AS ENUM ('publish', 'comment', 'favorite', 'follow');
CREATE TABLE events (
    id serial primary key,
    type event_type not null,
    date timestamptz not null,
    target_id int,
    target_name text,
    source_id int,
    source_name text,
    track_id int,
    track_title text,
    message text not null
);

CREATE TABLE playlists (
    id serial primary key,
    user_id int,
    name text,
    description text,
    track_ids int[] not null
);

CREATE TABLE youtube_refresh_tokens (
    user_id int not null,
    token text not null
);

CREATE TABLE youtube_access_tokens (
    user_id int not null unique,
    token text not null,
    expire timestamptz not null
);
