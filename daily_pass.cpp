/*  License:
	Created By : Vishwa Prakash H V,Shreyas P V and Vishruth D S
	Created on : 22-OCT-2016
	Description:


   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/*Standerd C++ Libraries*/

#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <string>
#include <stdio.h>
#include <sstream>

/*MySQL Libraries*/

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


using namespace std;
using namespace sql;

void error_msg( );
void valid();

int main( int argc , char* argv[])
{
	if (argc != 2)
	{
		cout << "error : \n"
			 << "usage .. :  daily <fingerprint>\n";
		exit(1);
	}
	
	string fingerprint = argv[1];
	
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306","root","#Include<>");
	stmt = con->createStatement();
	stmt->execute("use buspass_server_db");
	
	stringstream qry1;
	qry1 << "select fingerprint from daily_user_data where fingerprint=" << "'" << fingerprint << "'" ;
	res = stmt->executeQuery(qry1.str());
	
	// As "Daily_User_Data" is flushed daily , checking the existance of fingerprint is the validation itself!.
	if ( res->rowsCount() == 0 )
	{
		error_msg( );
		exit(1);
	}
	else
	{
		valid();
	}
	
	return 0;
}

void error_msg()
{
	cout << "Sorry!, you don't have a daily pass.\n";
}
void valid()
{
	cout << "You are good to go!\n";
}
