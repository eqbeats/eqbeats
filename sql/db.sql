CREATE TABLE users (
    id serial primary key,
    name text not null,
    password text not null,
    email text not null,
    about text not null default '',
    registration timestamptz not null,
    last_login timestamptz not null
);
CREATE TABLE tracks (
    id serial primary key,
    user_id integer not null REFERENCES users(id),
    title text not null,
    date timestamptz not null,
    license text not null,
    hits integer not null default 0,
    visible boolean not null default 'f',
    notes text not null default '',
    cats integer[] not null default ARRAY[]::Integer[]
);
CREATE TABLE sessions (
    sid text not null,
    host inet not null,
    user_id integer not null REFERENCES users(id),
    date timestamptz not null,
    UNIQUE (sid, host)
);
CREATE TABLE news (
    id serial primary key,
    title text not null,
    contents text not null,
    date timestamptz not null
);
CREATE TYPE comment_type AS ENUM ('track', 'user', 'news');
CREATE TABLE comments (
    ref integer not null,
    type comment_type not null,
    author_id integer not null,
    author_name text not null,
    contents text not null,
    date timestamptz not null
);
CREATE TABLE categories (
    id serial primary key,
    name text not null
);
CREATE TYPE favorite_type AS ENUM ('track', 'artist');
CREATE TABLE favorites (
    user_id integer not null,
    ref integer not null,
    type favorite_type not null
);
