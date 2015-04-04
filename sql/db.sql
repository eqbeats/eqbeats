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
CREATE INDEX user_name_idx ON users(name);
CREATE INDEX user_email_idx ON users(name);

CREATE TABLE tracks (
    id serial primary key,
    user_id integer not null REFERENCES users(id),
    title text not null,
    date timestamptz not null,
    license text not null default 'Copyright',
    visible boolean not null default 'f',
    notes text not null default '',
    airable boolean not null default 't',
    tags text[] not null default ARRAY[]::text[]
);
CREATE INDEX track_title_idx ON tracks(title);
CREATE INDEX track_visible_idx ON tracks(visible);
CREATE INDEX track_uid_idx ON tracks(user_id);

CREATE TABLE sessions (
    sid text not null,
    host inet not null,
    user_id integer not null REFERENCES users(id),
    date timestamptz not null,
    nonce text not null default '',
    UNIQUE (sid, host)
);
CREATE INDEX sess_sid_idx ON sessions(sid, host);

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
    user_id integer not null REFERENCES users(id),
    ref integer not null,
    type favorite_type not null
);
CREATE INDEX fav_idx ON favorites (user_id, type);
CREATE INDEX faved_idx ON favorites (ref, type);


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
CREATE INDEX evt_source_idx ON events(source_id);
CREATE INDEX evt_target_idx ON events(target_id);
CREATE INDEX evt_track_idx ON events(track_id);
CREATE INDEX evt_type_idx ON events(type);

CREATE TABLE playlists (
    id serial primary key,
    user_id int REFERENCES users(id),
    name text,
    description text,
    track_ids int[] not null
);
CREATE INDEX plist_uid_idx ON playlists(user_id);

CREATE TYPE feature_type AS ENUM ('track', 'playlist');
CREATE TABLE user_features (
    user_id integer not null REFERENCES users(id),
    ref integer not null,
    type feature_type not null
);
CREATE INDEX user_features_idx ON user_features(user_id);
