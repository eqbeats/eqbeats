DELETE FROM sessions
WHERE age(now(), date) > '1 month';
