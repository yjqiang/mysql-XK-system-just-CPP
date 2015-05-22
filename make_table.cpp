password
drop database text;
create database text;
use text;

create table student(
  sid  varchar(9),
  sname varchar(15),
  fromwhere varchar(15),
  primary key(sid));
  
create table teacher(
  tid varchar(9),
  sname varchar(15),
  fromwhere varchar(15),
  primary key(tid));
  
create table superteacher(
  stid varchar(9),
  stname varchar(15),
  fromwhere varchar(15),
  primary key(stid));

create table course(
  cid varchar(20),
  cname varchar(15),
  tid varchar(9),
  fromwhere varchar(15),
  max_num integer,
  now_num integer,
  primary key(cid));

create table ctime(
  ctimest integer,
  ctimend integer,
  cid varchar(20),
  foreign key(cid)references course(cid));

create table alpassword(
  sid varchar(9),
  who char(1),
  password varchar(20),
  mayin char(1),
  primary key(sid));

create table cs(
  sid varchar(9),
  cid varchar(20),
  foreign key(sid)references student(sid),
  foreign key(cid)references course(cid));
  
create table courseset(
  cid varchar(20),
  fromwhere varchar(15),
  foreign key(cid)references course(cid));
 
 