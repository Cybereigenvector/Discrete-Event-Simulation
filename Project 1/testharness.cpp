/*This program is a test harness. This is used to test the FEL.h. 
The operations are inputted from the text file and the methods from the FEL.h are called accordingly*/
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include "FEL.h"

int find_index(string,int);

int main(void)
{
	int length;
	string line,temp,filename;
	ifstream test;
	FEL queue;
	//The name of the text file is taken as an input
	printf("\nType the name of the file which would be used for testing. Example: Abc.txt\n");
	std::getline(cin,filename);

	test.open(filename.c_str(),std::ios::in);					//openning the txt file to read the operations

	if(test)									//Testing if the file was openned succesfully
	{
		while(!test.eof())	
		{
			std::getline(test,line);					//Reading from the text file
			length=line.length();						//The length of the string
			if(line.find("get")<length)					//Checking if the operation is "get next"
			{
				queue.get_next();
			}
			else if(line.find("peek")<length)				//Checking if the operation is "peek"
			{
				queue.peek();
			}
			else if(line.find("length")<length)				//Checking if the operation is "length"
			{
				queue.length();
			}
			else if(line.find("insert")<length)				//Checking if the operation is "insert with priority"
			{       
				int index=find_index(line,25);
				int eventtime;
				string eventtype,eventtimestr,eventdescription;
				eventtype=line.substr(22,1);
				eventdescription=line.substr(index+2,(length-index-2));
				eventtimestr=line.substr(25,index-25);
				eventtime=atoi(eventtimestr.c_str());
				queue.insert_with_priority(eventtype,eventtime,eventdescription);
				queue.print();
			}
			else if(line.find("delete")<length)				//Checking if the operation is "delete"
			{
				int index=find_index(line,11);
				int eventtime;
				string eventtype,eventtimestr,eventdescription;
				eventtype=line.substr(8,1);
				eventdescription=line.substr(index+2,(length-index-2));
				eventtimestr=line.substr(11,index-11);
				eventtime=atof(eventtimestr.c_str());
				queue.delete_node(eventtype,eventtime,eventdescription);
				queue.print();
			}	
		}				
	}
	else
	{
		cout<<"Error:No such file"<<endl;
	}
	test.close();
	return 0;
}

/*Searches the number in the String*/
int find_index(string s,int start)
{
	int i=start;
	string temp=",";
	string temp2;
	while(temp!=temp2)
	{
		temp2=s.at(i+1);
		i++;
	}
	return i;
}

