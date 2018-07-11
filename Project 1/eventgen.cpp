/*This program generates the random arrival event and the get next event for test 1*/
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>


int main(void)
{
	int randnumber,samples,max;
	string number,temp;
	ofstream txtgenerator;
	char buffer[10];
	txtgenerator.open("Test_1a.txt");
	stringstream ss;
	printf("Type the number of event that you want to generate \n");
	cin>>samples;
	
	printf("Provide the upper limit of arrival time (1-max)\n");
	cin>>max;
	
	for(int i=0;i<samples;i++)
	{
		if(i<samples/2)
		{
			temp="insert with priority, A, ";
			randnumber=rand() %max + 1;
			ss<<randnumber;
			ss>>number;
			ss.clear();
			temp.append(number);
			temp.append(", Arrival at time ");
			temp.append(number);
			cout<<temp<<endl;
			txtgenerator<<temp;
		}	
		else if(i>=samples/2)
		{
			txtgenerator<<"get next";	
		}
		if(i+1!=samples)
		{
			txtgenerator<<endl;
		}
	}
	txtgenerator.close();
}
