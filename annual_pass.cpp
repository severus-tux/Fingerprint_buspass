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

void error_msg( int );
int check_date( char[] );
int check_route( string, string, string, string );
const string current_date(); // Get current date/time, format is YYYY-MM-DD
void valid();


int main( int argc, char* argv[] )
{
	
	if (argc != 5)
	{
		cout<<"Error:\nusage : passverify <FingerPrint Key> <Current Location> <route number>  <Direction>\n";
		return 1;
	}
	
	string fingerprint = argv[1];
	string cur_loc = argv[2];
	string route_number = argv[3];
	string direction = argv[4];
	


	//Sql Class Objects
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt_user, *stmt_routes;
	sql::ResultSet *res_user, *res_routes;

	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306","root","#Include<>");
	stmt_user = con->createStatement();
	stmt_routes = con->createStatement();
	stmt_user->execute("use buspass_server_db");
	stmt_routes->execute("use buspass_server_db");
	
	stringstream qry1;
	qry1 << "select fingerprint,time_of_exp,route from user_data where fingerprint=" << "'" << fingerprint << "'" ;
	res_user = stmt_user->executeQuery(qry1.str());
	
	stringstream qry2;
	qry2 << "select * from route_data where route_no=" << "'" << route_number << "'" ;
	res_routes = stmt_routes->executeQuery(qry2.str());
	
	if (res_routes->rowsCount() == 0)
	{
		cout << "Invalid Route number\n";
		exit(1);
	}
	
	if ( res_user->rowsCount() == 0 )
	{
		error_msg(1);
		exit(1);
	}
	
	
	//Search the db for finger_print
	while (res_user->next() && res_routes->next())
	{
		
		if ( current_date() < res_user->getString("time_of_exp") )
		{				
			
			if ( check_route( res_user->getString("route"), res_routes->getString("route"), cur_loc, direction ))
			{
				valid();
				exit(0);
				/*
				cout << res_user->getString("fingerprint") << endl;
				cout << res_user->getString("route") << endl;
				cout << res_user->getString("time_of_exp") << endl;
				cout << res_routes->getString("route") << endl;
				cout << res_routes->getString("route_no") << endl;
				*/
				
			}
			else
				error_msg(3); //Invalid Route!
		}
		else
			error_msg(2); //Pass Expired!
		
	}
	
	delete con;
	delete stmt_user;
	delete stmt_routes;

	return 0;
}

void error_msg(int error_type)
{
	switch( error_type )
	{
		case 1 :
				cout << "\n Pass doesn't exist. Please buy a ticket\n";
				break;
		case 2 :
				cout << "\n Pass has expired! Please buy a ticket\n";
				break;
		case 3 :
				cout << "\n Invalid Route! Please buy a ticket\n";
				break;
	}
}


const string current_date()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[11];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	return buf;
}

void valid()
{
	cout<<"Valid pass.\n";
}

int check_route( string user_route, string bus_route, string cur_station, string direction)
{
	int cst, dir, urt, brt, ust=-1, bst=-1, i=0, u_route[200], b_route[200], u_size=0, b_size=0;
	stringstream dir_str(direction);
	dir_str >> dir;
	stringstream cur_str(cur_station);
	cur_str >> cst;
	stringstream user_str(user_route);
	
	while ( user_str >> urt)
	{
		if(!user_str)
			break;
		u_route[i++] = urt;
		if (urt == cst)
			ust = i-1;  // bst now contains index to current loc in User route array
	}
	u_size=i+1;
	stringstream bus_str(bus_route);
	
	i=0;
	while ( bus_str >> brt)
	{
		if(!bus_str)
			break;
		b_route[i++] = brt;
		if (brt == cst)
			bst = i-1; // bst now contains index to current loc in Bus route array
	}
	b_size=i+1;
	
	if (ust == -1 || bst == -1 || u_size < 2 || b_size < 2) 
		return 0;
	if ( (dir == 0 && cst == b_route[b_size-1]) || (dir == 1 && cst == b_route[0]) )
		return 0;
	if ( ( dir == 0 && u_route[ust+1] == b_route[bst+1] ) || ( dir == 1 && u_route[ust-1] == b_route[bst-1] ) )
		return 1;
	if ( ust>=1 && bst+1 <= b_size-1 )
		if ( dir == 0 && u_route[ust-1] == b_route[bst+1] )
			return 1;
	if ( bst>=1 && ust+1<=u_size-1 )
		if ( dir == 1 && u_route[ust+1] == b_route[bst-1] )
			return 1;
	return 0;
}
