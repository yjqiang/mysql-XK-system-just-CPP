# mysql_pj

MYSQL PJ
add 
just C++  后台


《数据库导论》PJ报告
——选课系统
实验环境：
C/S端：Windows 8.1 + C++ + Qt 5.4 + Visual Studio 2013 + MySQL 5.6
前端：肖振宸  后台：杨建强



一、功能介绍
基本功能：
学生——选课、退课、查询余量、查看课程表
教师——查看自己开的课、查询选择特定课程的学生
教务员——注册学生、删除学生与老师、删除课程、开一门课程（允许时间重复）、为学生选课（可以强制选入）

特色功能：
所有人：更改密码
学生：举报
教务员：封号

附注：
1、	所有的表都是事先建立，并存在一些初始数据，包括学生、教师、教务员和课程。
2、	学生、教师、教务员编号由于格式不同，有本质区别。
3、	教师与教务员是内定的，不能在界面内注册。
4、	所有人都有特定的学院（专业）。学生不能选其他专业的课；教师无法查询不是自己开的课；教务员开的课默认为本专业，注册的学生、开设的课程为他所在的专业，不能删除不是自己专业的学生、老师和课程，只能封禁或解封自己专业学生与老师的ID。
5、	教师无权为学生选课、退课，也不能开课、删除课程，需要通过教务员来操作。
6、	教务员添加课程中设置时间需要满足一定的格式（1 2 2 3 -1 -1意味着时间为周一第二节课以及周二第三节课）。







二、概念设计（ER图）：

 



三、数据库逻辑设计（关系模型）：
1、表关系：
学生（学号、姓名、系、教务员编号）
教师（编号、姓名、系、教务员编号）
教务员（编号、姓名、系）
课程（编号、课程名、系、时间）
密码（人物编号、密码值、身份、锁定状况）
选择（学号、课程号）
教授（教师号、课程号）

2、建库、建表语句（含初始数据）：
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



insert into student
     values('10001','Yang Li','CS'),
	       ('10002','Liu Yang','CS'),
		   ('10003','Li Mei','CS'),
		   ('10004','Hong Shuai','CS'),
		   ('10005','Zhong Lizai','SS'),
		   ('10006','Wang Peng','SS'),
		   ('10007','Ben','SS'),
		   ('10008','Sam','SS'),
		   ('10009','Tom','SS'),
		   ('10010','Wang Ren','IS'),
		   ('10011','Tian Tun','IS'),
		   ('10012','Chen Chen','IS'),
		   ('10013','Wang Nima','IS'),
		   ('10014','He He','IS'),
		   ('10015','Liu Peng','IS');
insert into teacher
     values('20001','Lu Na','IS'),
	       ('20002','Lisa','IS'),
		   ('20003','Dean','CS'),
		   ('20004','Jack','SS'),
		   ('20005','Kitty','SS');
insert into superteacher
               values('00001','God','CS'),
			         ('00002','Lin Meng','IS'),
					 ('00003','Li Lei','SS');
insert into course
            values('CHI002','Chinese','20001','IS',2,0),
			      ('MAT004','Maths','20001','ALL',2,2),
				  ('MAT005','Maths','20001','ALL',2,0),
				  ('ENG005','English','20002','SS',2,0),
			      ('ENG006','English','20002','SS',2,0),
				  ('ENG009','English','20002','SS',2,0),
				  ('ENG010','Germany','20005','CS',2,1),
				  ('CHE001','Chemistry','20003','CS',3,0),
				  ('PHY001','Physics','20003','SS',2,0);
				  
insert into ctime
             values(1,1,'CHI002'),
			       (1,2,'CHI002'),
				   (1,3,'MAT004'),
				   (1,4,'MAT004'),
				   (2,1,'MAT005'),
				   (5,2,'MAT005'),
				   (2,1,'ENG005'),
				   (1,3,'ENG005'),
				   (3,11,'ENG006'),
				   (5,1,'ENG009'),
				   (5,1,'ENG010'),
				   (5,2,'ENG010'),
				   (1,2,'CHE001'),
				   (1,4,'CHE001'),
				   (3,11,'PHY001'),
				   (1,3,'PHY001');
insert into alpassword
           values('10001','S','12345','T'),
		         ('10002','S','12345','T'),
				 ('10003','S','12345','T'),
				 ('10004','S','12345','T'),
				 ('10005','S','12345','T'),
				 ('10006','S','12345','T'),
				 ('10007','S','12345','T'),
				 ('10008','S','12345','T'),
				 ('10009','S','12345','F'),
				 ('10010','S','12345','T'),
				 ('10011','S','12345','F'),
				 ('10012','S','12345','T'),
				 ('10013','S','12345','F'),
				 ('10014','S','12345','F'),
				 ('10015','S','12345','T'),
				 ('20001','T','11111','T'),
				 ('20002','T','11111','T'),
				 ('20003','T','11111','T'),
				 ('20004','T','11111','T'),
				 ('20005','T','11111','T'),
				 ('00001','B','00000','T'),
				 ('00002','B','00000','T'),
				 ('00003','B','00000','T');
insert into cs
            values('10001','MAT004'),
			       ('10001','ENG010'),
				   ('10002','MAT004');	



四、系统演示
登陆界面：
 

学生选课界面：
 
课程表界面：
 

教师查课界面：
 
课程选择人数界面：
 












教务员界面：
 
注册学生界面：
 



删除学生与老师界面：
 



添加或删除课程：
 
帮助学生选课界面：
 





封禁账号界面：
 





修改密码界面：
 
五、实验文档总结
这是我们首次使用数据库写PJ，无论是数据库的使用、还是UI的设计，都消耗了大量的经历。虽然花时间、甚至做PJ到深夜是十分辛苦的，但我们也得到了不小的收获。从认识数据库的基本概念到实现数据库功能，从简单理解UI到实现多界面UI，从只会使用选课系统到深入了解选课系统，都是我们一步一步经历过的。
刚开始我们无从下手，然后看到高手们写出的出神入化的网页，我们就打算不走他们的道路。根据自己已有的知识，我们准备实现一个客户端。在这段时间之内我们不知花了多少时间，遇到过数据库链接不成功，也遇到过前端与后台接口不对应，更遇到过一个小错误导致调试很久，但我们都挺过来了。直到DDL的最后一天，终于等到了成功的到来。尽管可能多多少少还有一些小的问题甚至是Bug，或者有考虑不够周全、实现不够完善的地方，但我们总算是完成了PJ。不得不承认，这次PJ让我们受益匪浅，大大提升了自己的实力，为以后的学习做了铺垫。









计算机科学与技术
13307130057 肖振宸
13307130285 杨建强
2015年5月10日

