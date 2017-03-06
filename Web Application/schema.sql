drop table if exists users;
    create table users (
    id integer primary key autoincrement,
    username text not null,
    password text not null
);

drop table if exists places;
create table places (
    id integer primary key autoincrement,
    longitude int,
    latitude int,
    timestamp text
);
