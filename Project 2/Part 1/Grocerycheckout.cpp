using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include "FEL.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//These variables are the parameters necessary for the simulation										   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CLOCK,Queue_status,Server_status,busytime,max_queue;	  								 			   //
int number0fservicein,numberofdeparture,a_index,s_index,ptime;											   //
int Interarrival_time[10]={1,1,6,3,7,5,2,4,1};												  			   //
int	Service_time[10]={4,2,5,4,1,5,4,1,4};																   //
FEL checkout;												 											   //
string number[10]={"First","Second","Third","Fourth","Fifth","Sixth","Seventh","Eighth","Nineth","Tenth"}; //
string number_vec[10]={"One","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten"};			   //
string comment;																							   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//This Block Initializes parameters of the simulation         //
//and the variables used to measure statistics	    		  //
////////////////////////////////////////////////////////////////
void Initialize(void)								       	  
{															  		
	CLOCK=0;												  	
	Queue_status=0;											  	
	Server_status=0;										  	
	busytime=0;												   	
	max_queue=0;											  	
	number0fservicein=0;									  	
	numberofdeparture=0;								      				
	a_index=0;												  
	s_index=0;												 
	ptime=0; 
	comment=number[0];			
}															  

////////////////////////////////////////////////////////////////
//This function prints the title of the table and the first   //
//row of the table is also printed.							  //
////////////////////////////////////////////////////////////////
void print_table_header(void)
{
    cout<<" --------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<< '|' << setw(5) << "CLOCK" << '|'<< setw(12) << "Queue Length" << '|'<< setw(13) << "Server Status" << '|'<< setw(9) << "Busy Time" << '|'<< setw(20) << "Maximum Queue Length" <<  '|'<<setw(78) << "     Comment     " << '|'<< setw(26) << "     Future Eventlist     "<< '|'<< endl; 
    cout<<" --------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
    printf("\n");
    cout<< '|' << setw(5) << CLOCK << '|'<< setw(12) << Queue_status << '|'<< setw(13) << Server_status << '|'<< setw(9) << busytime << '|'<< setw(20) << max_queue << '|'<< setw(78) <<comment <<'|'<<setw(10);
    checkout.print(); 
    cout<<"\n --------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
}

////////////////////////////////////////////////////////////////
//This function prints the rows of the table 				  //
//Hence after each event this function can be called to 	  //
//print an entire row of the simulation table 				  //	
////////////////////////////////////////////////////////////////
void print_table_row(void)
{
	printf("\n");
    cout<< '|' << setw(5) << CLOCK << '|'<< setw(12) << Queue_status << '|'<< setw(13) << Server_status <<'|'<< setw(9) << busytime << '|'<< setw(20) << max_queue << '|'<< setw(78) << comment<< '|'<<setw(10); 
    checkout.print();
    cout<<"\n --------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
    	 	
}

////////////////////////////////////////////////////////////////
//This function creates the first event.We are assuming that  //	
//the first event occurs at CLOCK time 0.					  //
////////////////////////////////////////////////////////////////
void first_event(int end)			  	
{															  															  	
	Server_status=1;										  
	string eventtype,eventdescription;						  
	eventtype="D";  										              
	eventdescription="";									  										  				
	checkout.insert_with_priority(eventtype.c_str(),CLOCK+Service_time[s_index],eventdescription.c_str());
	s_index++;
	comment.append(" A occurs,Schedule next D,");
	number0fservicein++;
	eventtype="A";         									       
	eventdescription="";									  													  	
	checkout.insert_with_priority(eventtype.c_str(),CLOCK+Interarrival_time[a_index],eventdescription.c_str());
	a_index++;
	comment.append("Schedule next A.");
	eventtype="E";              							  
	eventdescription="";									  
	checkout.insert_with_priority(eventtype.c_str(),CLOCK+end,eventdescription.c_str());
}															  


////////////////////////////////////////////////////////////////
//This function handles the arrival logic of the grocery 	  //
//checkout simulation. The logic used is strictly in 		  //
//accordance with the flowchart given in the textbook		  //
////////////////////////////////////////////////////////////////
void arrival()
{
	string eventtype,eventdescription;	
	char numstr[11];
	number0fservicein++;					  
	if(Server_status!=1)
	{
		Server_status=1;
		eventtype="D";              
		eventdescription="";
		checkout.insert_with_priority(eventtype.c_str(),CLOCK+Service_time[s_index],eventdescription.c_str());
		comment.append(",(s*=");
    	sprintf(numstr,"%d",Service_time[s_index]);
    	comment.append(numstr);
		comment.append(") Schedule next D");
		s_index++;
	}
	else
	{
		Queue_status++;
		busytime=busytime+(CLOCK-ptime);
		ptime=CLOCK;
	}

	eventtype="A";              
	eventdescription="";
	checkout.insert_with_priority(eventtype.c_str(),CLOCK+Interarrival_time[a_index],eventdescription.c_str());
	comment.append(",(a*=");
    sprintf(numstr,"%d",Interarrival_time[a_index]);
    comment.append(numstr);
	comment.append(") Schedule next  A.");
	a_index++;
	if(max_queue<Queue_status)
   	{
   		max_queue=Queue_status;
    }
}

////////////////////////////////////////////////////////////////
//This function handles the departure logic of the grocery 	  //
//checkout simulation.The logic used is strictly in 		  //
//accordance with the flowchart given in the textbook		  //										  
////////////////////////////////////////////////////////////////
void departure()
{
	string eventtype,eventdescription;
	char numstr[11];
	numberofdeparture++;
	if(Queue_status>0)
	{
		Queue_status--;
		eventtype="D";              
		eventdescription="";
		checkout.insert_with_priority(eventtype.c_str(),CLOCK+Service_time[s_index],eventdescription.c_str());
		busytime=busytime+(CLOCK-ptime);
		ptime=CLOCK;	
		comment.append(",(s*=");
    	sprintf(numstr,"%d",Service_time[s_index]);
    	comment.append(numstr);
		comment.append(") Schedule next D");
		s_index++;	
	}
	else
	{
		Server_status=0;
		busytime=busytime+(CLOCK-ptime);
		ptime=CLOCK;
	}
}

////////////////////////////////////////////////////////////////
//This function processes the topmost event from the 		  //
//future event list and calls the appropriate functions. 	  //
//Hence if an arrival event is encountered it calls the 	  //
//arrival() function to process the event and to take steps   //
//according to the logic. The string "comment" is used in the //
//functions arrival,departure and interprete_data to store the// 
//comment that is being displayed in the table. Whenever an   //
//event is processed approprite description is appended to the//
//comment variable.The endtime_event takes the eventime for   //	 
//the endtime event.If endtime argument takes the number of   //
//time unit we want to simulate in that particular simulation //
////////////////////////////////////////////////////////////////
void interprete_data(int endtime_event,int endtime)
{
	struct FEL::node *event;
    first_event(endtime_event);
    print_table_header();
    while(CLOCK < endtime_event && CLOCK < endtime)
    {
    	goback:
    	char numstr[11];
    	event=checkout.get_next();
    	CLOCK=event->TIME;

    	if(event->TYPE=="A")
    	{
    		comment=number[number0fservicein];	
    		comment.append(" A occurs:(A,");
    		sprintf(numstr,"%d",CLOCK);
    	    comment.append(numstr);
    	    comment.append(")");
    		arrival();
    	}
    	else if(event->TYPE=="D")
    	{
    		comment=number[numberofdeparture];
    		comment.append(" D occurs:(D,");
    		sprintf(numstr,"%d",CLOCK);
    	    comment.append(numstr);
    	    comment.append(")");
    		departure();
    	}
    	else if(event->TYPE=="E")
    	{
    		break;	
    	}
    	if(CLOCK==checkout.peek()->TIME)
    	{	
    		cout<<setw(150)<<comment;
    		goto goback;
    	}
    	if(Queue_status>0)
    	{
    		comment.append("(");
    		comment.append(number_vec[Queue_status-1]);
    		comment.append(" Customer delayed)");
    	}
    	print_table_row();
    	ptime=CLOCK;
    	comment="";
    }
}

////////////////////////////////////////////////////////////////
//This is the main function. This function calls the required //
//functions to simulate the grocery checkout scenario.		  //
////////////////////////////////////////////////////////////////
int main(void)
{
	Initialize();
	interprete_data(60,23); // The simulation end event is at 60 and we are simulating for 23 time units
	return 0;
}
