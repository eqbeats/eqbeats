CREATE TABLE users (
    id serial primary key,
    name text not null,
    password text not null,
    email text not null,
    about text not null default '',
    registration timestamp not null,
    last_login timestamp not null
);
CREATE TABLE tracks (
    id serial primary key,
    user_id integer not null REFERENCES users(id),
    title text not null,
    date timestamp not null,
    license text not null,
    hits integer not null default 0,
    visible boolean not null default 'f',
    notes text not null default ''
    /*genre bit(X) not null default 0*/
);
CREATE TABLE sessions (
    sid text not null,
    host inet not null,
    user_id integer not null REFERENCES users(id),
    date timestamp not null,
    UNIQUE (sid, host)
);
CREATE TABLE news (
    id serial primary key,
    title text not null,
    contents text not null,
    date timestamp not null
);
CREATE TYPE comment_type AS ENUM ('track', 'artist', 'news');
CREATE TABLE comments (
    ref integer not null,
    type comment_type not null,
    author_id integer not null,
    author_name text not null,
    contents text not null,
    date timestamp not null
);
