INSERT INTO users (name, password, email, registration, last_login) VALUES
    ('foo', crypt('foo', gen_salt('bf')), 'foo@foo.foo', timestamp 'now' - interval '2 years', 'now'),
    ('bar', crypt('bar', gen_salt('bf')), 'bar@foo.foo', timestamp 'now' - interval '3 months', 'now'),
    ('Baz!', crypt('baz', gen_salt('bf')), 'baz@foo.foo', 'now', 'now')
;
INSERT INTO tracks (user_id, title, date, license, visible) VALUES
    (1, 'Track A', timestamp 'now' - interval '1 month', 'custom', 't'),
    (1, 'Track B', timestamp 'now' - interval '1 hour', 'custom', 't'),
    (1, 'Track C', 'now', 'custom', 'f'),
    (3, 'Track D', 'now', 'custom', 't')
;
