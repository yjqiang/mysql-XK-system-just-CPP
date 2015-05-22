#include <iostream>
#include <memory> 
#include <cstring> 
#include <cppconn\driver.h> 
#include <cppconn\exception.h> 
#include <cppconn\resultset.h> 
#include <cppconn\statement.h> 
#include <cppconn\prepared_statement.h> 
#include <driver.h>
#include <connection.h>
#include <statement.h>
#include <prepared_statement.h>
#include <resultset.h>
#include <metadata.h>
#include <resultset_metadata.h>
#include <exception.h>
#include <warning.h>
#include <cstring>
#include <sstream>
#include <windows.h>

sql::Statement * stmt;


int May_in(std::string cin_name, std::string cin_password, int &who, std::string &department);

void Course_chosen(std::string id);
int Choose_course(std::string id,bool can);
int Drop_course(std::string id);
int Course_chosen_teacher(std::string id,int &num);
int Block(std::string id, std::string department);
int Unblock(std::string id, std::string department);
int Add_student(std::string fromwhere);
int Add_course(std::string cid, std::string cname, std::string tid, int max_num, std::string fromwhere, int a[10][2]);
int Add_course(std::string cid, std::string cname, std::string tid, int max_num, std::string fromwhere, int a[10][2],std::string for_who[10]);
int Change_password(std::string id);
int Delete_student(std::string id, std::string superteacher_department);
int Delete_teacher(std::string id, std::string superteacher_department);
int  Course_condition(std::string cid);

void For_student(std::string id, std::string department);
void For_teacher(std::string id);
void For_superteacher(std::string id, std::string department);

int main()
{
	//Start
	std::string temp;
	std::cout << "connect database:text start " << std::endl;
	//prepare
	try
	{
		std::cout << "Connecting";
		Sleep(1000);
		std::cout << ".";
		Sleep(1000); 
		std::cout << ".";
		Sleep(1000);
		std::cout << "." << std::endl;
		Sleep(1000);
		sql::Driver* driver = get_driver_instance();
		sql::Connection *con = (driver->connect("tcp://127.0.0.1:3305", "root", "password"));
		stmt = con->createStatement();
		stmt->execute("use text");
		std::cout << "Welcome to our system!!" << std::endl;
	}
	catch (...)
	{
		std::cout << "Sorry,our system is being fixed." << std::endl;
		system("pause");
		exit(1);
	}

	////
	//std::string a;
	//std::cin >> a;
	//Course_condition(a);
	////

	std::string department;//רҵ
	std::string cin_name;
	std::string cin_password;
	int who_are_you;
	while (1)
	{
		for (int try_time = 0;; try_time++)
		{
			if (try_time >= 5)
			{
				std::cout << "SORRY" << std::endl;
				stmt->close();
				system("pause");
				exit(-1);
			}
			std::cout << "please print your id and password" << std::endl;
			std::cin >> cin_name >> cin_password;
			int may_in = May_in(cin_name, cin_password, who_are_you, department);
			if (may_in == 0) break;
		}
		//for student :1.courses choosen 2.chose course 3.exit
		//who :1 student;2teacher;3 superteacher
		switch (who_are_you)
		{
		case 1: For_student(cin_name, department); break;
		case 2: For_teacher(cin_name); break;
		case 3: For_superteacher(cin_name, department); break;
		}
		std::cout << "Would you like to quit? Y/N" << std::endl;
		std::cin >> temp;
		if (temp != std::string("N"))
			break;
	}
	stmt->close();
	system("pause");
	return 0;
}

//0 yes;1 locked;2 wrong;3 no found//who :1 student;2teacher;3 superteacher
int May_in(std::string cin_name, std::string cin_password, int &who, std::string &department)
{
	sql::ResultSet *result0=NULL;
	who = -1;
	sql::ResultSet *res = stmt->executeQuery("select * from alpassword where sid='" + cin_name + "'");
	if (res->next())
	{
		std::cout << "the reall id = " << res->getString(1) << "  who = " << res->getString(2) << " password=" << res->getString(3) << std::endl;
		if (res->getString(3) == cin_password)
		{
			if (res->getString(4) == std::string("T"))
			{
				std::cout << "BINGO!" << std::endl;
				if (res->getString(2) == std::string("S"))
					who = 1;
				else if (res->getString(2) == std::string("T"))
					who = 2;
				else if (res->getString(2) == std::string("B"))
					who = 3;
				switch (who)
				{
				case 1:
						result0 = stmt->executeQuery("select fromwhere from student where sid='" + cin_name + "'");
						break;
				case 2:
						result0 = stmt->executeQuery("select fromwhere from teacher where tid='" + cin_name + "'");
						break;
				case 3:
					result0 = stmt->executeQuery("select fromwhere from superteacher where stid='" + cin_name + "'");
					break;
				}
				result0->next();
				department = result0->getString(1);
				std::cout << "id:  " << cin_name << "  password:   " << cin_password << " who:  " << who << " department:   " << department << std::endl;
				return 0;
			}
			else
			{
				std::cout << "Sorry, you have been locked." << std::endl;
				return 1;
			}
		}
		else
		{
			std::cout << "Wrong password." << std::endl;
			return 2;
		}
	}
	else
	{
		std::cout << "NO found" << std::endl;
		return 3;
	}

}
void For_student(std::string id, std::string department)
{
	int i;
	while (1)
	{
		std::cout << "1.courses chosen 2.choose course 3.drop one 4.change password 5.exit" << std::endl;
		std::cout << "choose one" << std::endl;
		std::cin >> i;
		if (i == 1)
			Course_chosen(id);
		if (i == 2)
		{
			Choose_course(id,false);
		}
		
		if (i == 3)
		{
			Drop_course(id);
		}
		if (i == 4)
			Change_password(id);
		if (i == 5) 
			return;
	}
}
void For_teacher(std::string id)
{
	int i;
	int num;
	while (1)
	{
		std::cout << "1.courses chosen 2.change password  3.exit" << std::endl;
		std::cout << "choose one" << std::endl;
		std::cin >> i;
		if (i == 1)
		{
			Course_chosen_teacher(id,num);
			std::cout << "the number is:" << num << std::endl;
		}
		if (i == 2)
			Change_password(id);
		if (i == 3)
			return;
	}


}
//new!!
void For_superteacher(std::string tid, std::string department)
{
	int i;
	std::string id;
	std::string course;
	while (1)
	{
		std::cout << "1.choose course 2.drop one course 3.block 4.unbock 5.add course 6.add student 7.change password 8.delete student 9.delete teacher 10.exit" << std::endl;
		std::wcout << "choose one" << std::endl;
		std::cin >> i;
		if (i == 1)
		{
			std::cin >> id;
			std::string who;
			sql::ResultSet *result0 = stmt->executeQuery("select fromwhere from student where sid='" + id + "'");
			if (result0->next())
				who = result0->getString(1);
			else{
				std::cout << "no found"<< std::endl;
				continue;
			}
			if (who != department)
			{
				std::cout << "You can't choose it. It belongs to others." << std::endl;
				continue;
			}
			Choose_course(id,true);
		}
		if (i == 2)
		{
			std::cin >> id;
			std::string who;
			sql::ResultSet *result0 = stmt->executeQuery("select fromwhere from student where sid='" + id + "'");
			if (result0->next())
				who = result0->getString(1);
			else{
				std::cout << "no found" << std::endl;
				continue;
			}
			if (who != department)
			{
				std::cout << "You can't choose it. It belongs to others." << std::endl;
				continue;
			}
			Drop_course(id);
		}
		if (i == 3)
		{
			std::cin >> id;
			Block(id,department);
		}
		if (i == 4)
		{
			std::cin >> id;
			Unblock(id,department);
		}
		if (i == 5)
		{
			int temp0, temp1;
			std::string cid;
			std::string cname;
			std::string tid;
			int max_num;
			int a[10][2];
			memset(a, 0, sizeof(a));
			int i=0;
			while (std::cin >> temp0 >> temp1)
			{
				if (temp0 == -1)
					break;
				bool equal = false;
				//void repeat
				for (int j = 0; j < i; j++)
				{
					if (a[j][0] == temp0&&a[j][1] == temp1)
						equal=true;
				}
				if (equal) continue;
				a[i][0] = temp0;
				a[i][1] = temp1;
				i++;
			}
			if (i <= 0)
			{
				std::cout << "time erro" << std::endl;
				continue;
			}
			std::cin >> cid >> cname >> tid >> max_num;
			//new!!
			std::string for_who[10];
			std::string x;
			i = 0;
			while (std::cin>>x&&x!=std::string("over"))
			{
				for_who[i] = x;
				i++;
			}
			for_who[i] = x;
			Add_course(cid, cname, tid, max_num, department, a, for_who);
			//Add_course(cid, cname, tid, max_num, department, a);
		}
		if (i == 6)
		{

			Add_student(department);
		}
		if (i == 7)
			Change_password(tid);
		if (i == 8)
		{
			std::cin >> id;
			Delete_student(id, department);
		}
		if (i == 9)
		{
			std::cin >> id;
			Delete_teacher(id, department);
		}
		if (i == 10)
			return;
		
		
	}



}
void Course_chosen(std::string id)
{
	bool resist=false;
	sql::ResultSet *res = stmt->executeQuery("select cs.cid,course.cname,teacher.sname,ctime.ctimest,ctime.ctimend from cs,ctime,teacher,course where cs.sid='" + id + "'and cs.cid=course.cid and teacher.tid=course.tid and ctime.cid=cs.cid");
	while (res->next())
	{
		resist = true;
		std::cout << "course= " << res->getString(1) << "   :: " << res->getString(2) << " teacher=" << res->getString(3) << "  time:  "<<res->getInt(4)<<res->getInt(5) << std::endl;
	}
	if (resist == false)
		std::cout << "NO found" << std::endl;

}
//0 Ok;1 not found;3 chosen; 4 max;5 time conflict 6.wrong id 7.fromwhere
//new!!
int Choose_course(std::string id,bool can)
{
	std::string course;
	std::cout << "Which class do you want to choose?" << std::endl;
	std::cin >> course;
	std::string who;
	sql::ResultSet *result0 = stmt->executeQuery("select fromwhere from student where sid='" + id + "'");
	if (result0->next())
		who = result0->getString(1);
	else
	{
		std::cout << "No such student" << std::endl;
		return 6;
	}
	sql::ResultSet *res1 = stmt->executeQuery("select * from course where cid='" + course + "'");
	if (!res1->next())
	{
		std::cout << "no found" << std::endl;
		return 1;
	}
	sql::ResultSet *res2 = stmt->executeQuery("select * from cs where cid='" + course + "'and sid='" + id + "'");
	std::cout << "id = " << res1->getString(1) << "     " << res1->getString(2) << "  " << res1->getString(3) << "  " << res1->getString(4) << std::endl;
	if (res2->next())
	{
		std::cout << "You have chosen it." << std::endl;
		return 3;
	}
	/*
	if (res1->getString(4) != who&&res1->getString(4) != std::string("ALL"))
	{
		std::cout << "You can't choose it. It belongs to others." << std::endl;
		return 7;
	}*/

	
	sql::ResultSet *res00 = stmt->executeQuery("select * from student,courseset where student.fromwhere=courseset.fromwhere and sid='" + id + "'");
	if (res00->next())
	{
	std::cout << "You can't choose it. It belongs to others." << std::endl;
	return 7;
	}
	
	
	sql::ResultSet *res3 = stmt->executeQuery("select y.ctimend,y.ctimest,x.cid,y.cid from ctime as x,ctime as y,cs where cs.sid='" + id + "'and cs.cid=x.cid and y.cid='" + course + "'and y.ctimest=x.ctimest and y.ctimend=x.ctimend");
	if (res3->next())
	{
		std::cout << res3->getInt(1) << res3->getInt(2) << res3->getString(3) << res3->getString(3) << std::endl;
		std::cout << "Time conflict!!" << std::endl;
		return 5;
	}
	if (res1->getInt(5) <= res1->getInt(6))
	{
		if (!can)
		{
			std::cout << "Max!!" << std::endl;
			return 4;
		}
		else
			stmt->executeUpdate("update course set max_num=max_num+1 where cid='" + course + "'");
	}

	stmt->execute("insert into cs values ('" + id + "', '" + course+ "')");
	stmt->executeUpdate("update course set now_num=now_num+1 where cid='" + course + "'");
	std::cout << "Congratulations!" << std::endl;
	return 0;
}
//0 OK 1 no found
int Drop_course(std::string id)
{
	std::string course;
	std::cout << "Which class do you want to drop?" << std::endl;
	std::cin >> course;
	sql::ResultSet *res = stmt->executeQuery("select * from cs where cid='" + course + "'and sid='" + id + "'");
	if (res->next())
	{
		stmt->execute("delete from cs  where cid='" + course + "'and sid='" + id + "'");
		stmt->executeUpdate("update course set now_num=now_num-1 where cid='" + course + "'");
		std::cout << "OK" << std::endl;
		return 0;
	}
	else
	{ 
		std::cout << "no found.Are you kidding?" << std::endl;
		return 1;
	}

}
//1 no found 0 OK
int Course_chosen_teacher(std::string id,int&num)
{
	std::string course;
	std::cout << "which course?" << std::endl;
	std::cin >> course;
	bool resist = false;
	num = 0;
	sql::ResultSet *res0 = stmt->executeQuery("select * from course where tid='" + id + "'and cid='" + course + "'");
	if (!res0->next())
	{
		std::cout << "Sorry,can't be found" << std::endl;
		return 1;
	}
	sql::ResultSet *res2 = stmt->executeQuery("select sid from cs where cid='" + course + "'");
	while (res2->next())
	{
		resist = true;
		num++;
		std::cout << "course= " << res2->getString(1) << std::endl;
	}
	if (resist == false)
		std::cout << "zore" << std::endl;
	return 0;
}
//0 OK ;1 nofound;2 superteacher;3 fromwhere
int Block(std::string id, std::string department)
{
	int who;
	std::string department_who;
	sql::ResultSet *result0=NULL;
	sql::ResultSet *res = stmt->executeQuery("select * from alpassword where sid='" + id + "'");
	if (res->next())
	{
		std::cout << "id = " << res->getString(1) << "  who = " << res->getString(2) << " password=" << res->getString(3) << std::endl;
		if (res->getString(2) == std::string("B"))
		{
			std::cout << "He/She is a superteacher.You can't do that." << std::endl;
			return 2;
		}
		else
		{
			if (res->getString(2) == std::string("S"))
				who = 1;
			if (res->getString(2) == std::string("T"))
				who = 2;
			switch (who)
			{
			case 1:
				result0 = stmt->executeQuery("select fromwhere from student where sid='" + id + "'");
				break;
			case 2:
				result0 = stmt->executeQuery("select fromwhere from teacher where tid='" + id + "'");
				break;
			}
			result0->next();
			department_who = result0->getString(1);
			if (department_who!=department)
			{
				std::cout << "You can't choose it. It belongs to others." << std::endl;
				return 3;
			}
			stmt->executeUpdate("update alpassword set mayin='F' where sid='" + id + "'");
			std::cout << "Blocked." << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "NO found" << std::endl;
		return 1;
	}
}
int Unblock(std::string id, std::string department)
{
	int who;
	std::string department_who;
	sql::ResultSet *result0 = NULL;
	sql::ResultSet *res = stmt->executeQuery("select * from alpassword where sid='" + id + "'");
	if (res->next())
	{
		std::cout << "id = " << res->getString(1) << "  who = " << res->getString(2) << " password=" << res->getString(3) << std::endl;
		if (res->getString(2) == std::string("B"))
		{
			std::cout << "He/She is a superteacher.You can't do that." << std::endl;
			return 2;
		}
		else
		{
			if (res->getString(2) == std::string("S"))
				who = 1;
			if (res->getString(2) == std::string("T"))
				who = 2;
			switch (who)
			{
			case 1:
				result0 = stmt->executeQuery("select fromwhere from student where sid='" + id + "'");
				break;
			case 2:
				result0 = stmt->executeQuery("select fromwhere from teacher where tid='" + id + "'");
				break;
			}
			result0->next();
			department_who = result0->getString(1);
			if (department_who != department)
			{
				std::cout << "You can't choose it. It belongs to others." << std::endl;
				return 3;
			}
			stmt->executeUpdate("update alpassword set mayin='T' where sid='" + id + "'");
			std::cout << "UNBlocked." << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "NO found" << std::endl;
		return 1;
	}
}
//0 Ok;1 the same sid 
int Add_student(std::string fromwhere)
{
	std::string sid;
	std::string sname;
	std::string password;
	std::cout << "sid  sname password" << std::endl;
	std::cin >> sid >> sname >> password;
	sql::ResultSet *res = stmt->executeQuery("select * from student where sid='" + sid + "'");
	if (res->next())
	{
		std::cout << "We have the sid now." << std::endl;
		return 1;
	}
	else
	{
		stmt->executeUpdate("insert into student values('" + sid + "','" + sname + "','" + fromwhere + "')");
		stmt->executeUpdate("insert into alpassword values('" + sid + "','S','" + password + "','T')");
		return 0;
	}
	
	return 0;
}
//0 OK;1 the same id; 2 time conflict;3 no such teacher;4 max
int Add_course(std::string cid, std::string cname, std::string tid, int max_num, std::string fromwhere, int a[10][2])
{
	if (max_num <= 0)
	{
		std::cout << "Oh no" << std::endl;
		return 4;
	}
	sql::ResultSet *res1 = stmt->executeQuery("select * from course where cid='" + cid + "'");
	if (res1->next())
	{
		std::cout << "The same name" << std::endl;
		return 1;
	}
	sql::ResultSet *res3 = stmt->executeQuery("select * from teacher where tid='" + tid + "'");
	if (!res3->next())
	{
		std::cout << "No such teacher" << std::endl;
		return 3;
	}
	for (int i = 0; i < 10 && a[i][0] != 0; i++)
	{
		std::stringstream day;
		day << a[i][0];
		std::string s0 = day.str();
		std::stringstream when;
		when << a[i][1];
		std::string s1 = when.str();

		sql::ResultSet *res2 = stmt->executeQuery("select * from ctime,course where tid='" + tid + "'and course.cid=ctime.cid and ctime.ctimest="+s0+" and ctimend="+s1+"");
		if (res2->next())
		{
			std::cout << a[i][0] << a[i][1] << std::endl;
			std::cout << "time conflict" << std::endl;
			return 2;
		}
	}
	std::stringstream temp;
	temp << max_num;
	std::string ttemp = temp.str();
	stmt->executeUpdate("insert into course values('" + cid + "','" + cname + "','" + tid + "','" + fromwhere + "',"+ttemp+",0)");
	for (int i = 0; i < 10 && a[i][0] != 0; i++)
	{
		std::stringstream day;
		day << a[i][0];
		std::string s0 = day.str();
		std::stringstream when;
		when << a[i][1];
		std::string s1 = when.str();
		stmt->executeUpdate("insert into ctime values(" + s0 + "," + s1 + ",'" + cid + "')");
	}
	std::cout << "Insert success!" << std::endl;
		return 0;
}
//0 OK;1 old_password wrong;2 new_password conflict
int Change_password(std::string id)
{
	std::string old_password;
	std::string new_password0;
	std::string new_password1;
	std::cout << "Your 3 passwords" << std::endl;
	std::cin >> old_password >> new_password0 >> new_password1;
	sql::ResultSet *res = stmt->executeQuery("select password from alpassword where sid='" + id + "'");
	res->next();
	if (res->getString(1) != old_password)
	{
		std::cout << "password wrong" << std::endl;
		return 1;
	}
	if (new_password0 != new_password1)
	{
		std::cout << "password conflict" << std::endl;
		return 2;
	}
	stmt->executeUpdate("update alpassword set password='" + new_password0 + "' where sid='" + id + "'");
	return 0;
}
//0 OK;1 no found;2 department
//student ,course-> now_num,cs,password
int Delete_student(std::string id, std::string superteacher_department)
{
	sql::ResultSet *res0 = stmt->executeQuery("select fromwhere from student where sid='" + id + "'");
	if (res0->next())
	{
		if (res0->getString(1) != superteacher_department)
		{
			std::cout << "doesn't belong to you" << std::endl;
			return 2;
		}
	}
	else
	{
		std::cout << "no found" << std::endl;
		return 1;
	}
	stmt->executeUpdate("update course set now_num=now_num-1 where cid in (select cid from cs where sid='" + id + "')");
	stmt->executeUpdate("delete from cs where sid='" + id + "'");
	stmt->executeUpdate("delete from student where sid='" + id + "'");
	stmt->executeUpdate("delete from alpassword where sid='" + id + "'");
	std::cout << "delete him succesful" << std::endl;
	return 0;
}
 
//0 OK;1 no found;2 department
//teacher ,course,cs,password,ctime
int Delete_teacher(std::string id, std::string superteacher_department)
{
	sql::ResultSet *res0 = stmt->executeQuery("select fromwhere from teacher where tid='" + id + "'");
	if (res0->next())
	{
		if (res0->getString(1) != superteacher_department)
		{
			std::cout << "doesn't belong to you" << std::endl;
			return 2;
		}
	}
	else
	{
		std::cout << "no found" << std::endl;
		return 1;
	}
	stmt->executeUpdate("delete from cs where cid in (select cid from course where tid='" + id + "')");
	stmt->executeUpdate("delete from ctime where cid in (select cid from course where tid='" + id + "')");
	stmt->executeUpdate("delete from course where tid='" + id + "'");
	stmt->executeUpdate("delete from teacher where tid='" + id + "'");
	stmt->executeUpdate("delete from alpassword where sid='" + id + "'");
	std::cout << "delete him succesful" << std::endl;
	return 0;
}
//new!!!
int Add_course(std::string cid, std::string cname, std::string tid, int max_num, std::string fromwhere, int a[10][2], std::string for_who[10])
{
	if (max_num <= 0)
	{
		std::cout << "Oh no" << std::endl;
		return 4;
	}
	sql::ResultSet *res1 = stmt->executeQuery("select * from course where cid='" + cid + "'");
	if (res1->next())
	{
		std::cout << "The same name" << std::endl;
		return 1;
	}
	sql::ResultSet *res3 = stmt->executeQuery("select * from teacher where tid='" + tid + "'");
	if (!res3->next())
	{
		std::cout << "No such teacher" << std::endl;
		return 3;
	}
	for (int i = 0; i < 10 && a[i][0] != 0; i++)
	{
		std::stringstream day;
		day << a[i][0];
		std::string s0 = day.str();
		std::stringstream when;
		when << a[i][1];
		std::string s1 = when.str();

		sql::ResultSet *res2 = stmt->executeQuery("select * from ctime,course where tid='" + tid + "'and course.cid=ctime.cid and ctime.ctimest=" + s0 + " and ctimend=" + s1 + "");
		if (res2->next())
		{
			std::cout << a[i][0] << a[i][1] << std::endl;
			std::cout << "time conflict" << std::endl;
			return 2;
		}
	}
	std::stringstream temp;
	temp << max_num;
	std::string ttemp = temp.str();
	stmt->executeUpdate("insert into course values('" + cid + "','" + cname + "','" + tid + "','" + fromwhere + "'," + ttemp + ",0)");
	for (int i = 0; i < 10 && a[i][0] != 0; i++)
	{
		std::stringstream day;
		day << a[i][0];
		std::string s0 = day.str();
		std::stringstream when;
		when << a[i][1];
		std::string s1 = when.str();
		stmt->executeUpdate("insert into ctime values(" + s0 + "," + s1 + ",'" + cid + "')");
	}
	for (int i = 0; i < 10 && for_who[i]!= std::string("over"); i++)
	{
		stmt->executeUpdate("insert into courseset values('" + cid + "','" + for_who[i] + "')");
	}
	std::cout << "Insert success!" << std::endl;
	return 0;

}











//Well remain checking
//sql::ResultSet *res1 = stmt->executeQuery("select * from student/teacher/superteacher where cid='" + cid + "'");
//0 OK;1 no found;2 department
//course,cs,ctime
int Delete_course(std::string id, std::string superteacher_department)
{
	sql::ResultSet *res0 = stmt->executeQuery("select fromwhere from course where cid='" + id + "'");
	if (res0->next())
	{
		if (res0->getString(1) != superteacher_department)
		{
			std::cout << "doesn't belong to you" << std::endl;
			return 2;
		}
	}
	else
	{
		std::cout << "no found" << std::endl;
		return 1;
	}
	stmt->executeUpdate("delete from cs where cid='" + id + "'");
	stmt->executeUpdate("delete from ctime where cid '" + id + "'");
	stmt->executeUpdate("delete from course where cid='" + id + "'");
	std::cout << "delete it succesful" << std::endl;
	return 0;
}
//0 ok
void Course_chosen_teacher(std::string id)
{
	bool resist = false;
	sql::ResultSet *res = stmt->executeQuery("select cid,cname from course where tid='" + id + "'");
	while (res->next())
	{
		resist = true;
		std::cout << "course= " << res->getString(1) << "   :: " << res->getString(2) << std::endl;
	}
	if (resist == false)
		std::cout << "NO found" << std::endl;
}
//0 OK 1 No found
int  Course_condition(std::string cid)
{
	sql::ResultSet *res = stmt->executeQuery("select cid,cname,fromwhere,max_num,now_num  from course where cid = '" + cid + "'");
	if (res->next())
	{

		std::cout << "course= " << res->getString(1) << "   :: " << res->getString(2) << "   department " << res->getString(3) << "  max" << res->getInt(4)<<"   " << res->getInt(5) << std::endl;
		return 0;
	}
	return 1;


}
//
