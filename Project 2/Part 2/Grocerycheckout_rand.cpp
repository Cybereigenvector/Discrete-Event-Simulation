using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <math.h>
#include "FEL_m.h"
#include "randomgen.h"

#define PI 3.1415927

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//These variables are accessable from all the functions		  											   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
float CLOCK,busytime,max_queue,ptime;	  																   
int number0fservicein,numberofdeparture,a_index,s_index,NumNormal,Queue_status,Server_status;			   
float Interarrival_time[100];																  			   
float Service_time[100];																				   
FEL checkout;												 											   
string number[]={"First","Second","Third","Fourth","Fifth","Sixth","Seventh","Eighth","Nineth","Tenth","Eleventh","Twelveth","Thirteenth","Fourteenth","Fifteenth","Sixteenth","Seventeenth","Eigtheenth","Nineteenth","Twentieth","Twentyfirst","TwentySecond","twentythird","twentyfourth","twentyfifth","Twentysixth","Twentyseventh"}; 
string number_vec[]={"One","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Eleven","Twelve","Thirteen","Fourteen","Fifteen","Sixteen","Seventeen","Eigtheen","Nineteen","Twenty","Twentyone","TwentyTwo","TwentyThree","TwentyFour","TwentyFive","TwentySix","TwentySeven"};			       
string comment;																							   


///////////////////////////////////////////////////////////////
//Extra variables added for performance measurement and 	 //
//random number generation									 //
///////////////////////////////////////////////////////////////
float MeanInterArrivalTime,MeanServiceTime,SIGMA,firstarrival,SaveNormal,arrivaltimeofcustomerbeingdeparted,SumResponsetime;
int LongService;
randomgen randomnumber;

////////////////////////////////////////////////////////////////
//This Block Initializes parameters of the simulation         //
//and the variables used to keep statistics		    		  //
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

	NumNormal=0;
	MeanInterArrivalTime=4.5;
	MeanServiceTime=3.2;
	SIGMA=0.6;
	LongService=0;
	arrivaltimeofcustomerbeingdeparted=0;
	SumResponsetime=0;
}					

///////////////////////////////////////////////////////////////
//Exponentially distributed random number generator			 //
///////////////////////////////////////////////////////////////
float exp_interarrival(float mean)
{
	return -mean*log(randomnumber.random_number());
}

///////////////////////////////////////////////////////////////
//Normally distributed random number generator				 //
///////////////////////////////////////////////////////////////
float normal_service(float mean,float sigma)
{
	float ReturnNormal;
	if(NumNormal==0)
	{
		float r1=randomnumber.random_number();
		float r2=randomnumber.random_number();
		ReturnNormal=sqrt(-2*log(r1))*cos(2*PI*r2);
		SaveNormal=sqrt(-2*log(r1))*sin(2*PI*r2);
		NumNormal=1;
	}
	else
	{
		NumNormal=0;
		ReturnNormal=SaveNormal;
	}
	return ((ReturnNormal*sigma) + mean);
}

////////////////////////////////////////////////////////////////
//This function call the required distribution and initializes//
//the Interarrival time and the Service time 				  //
////////////////////////////////////////////////////////////////
void generate_time(void)
{
      for(int i=0;i<100;i++)
      {
      	Interarrival_time[i]=exp_interarrival(MeanInterArrivalTime);
      	Service_time[i]=normal_service(MeanServiceTime,SIGMA);
      }
}

////////////////////////////////////////////////////////////////
//This function returns the arrival time of customer for given// 
//customer number 											  //
////////////////////////////////////////////////////////////////
float give_arrival_time(int customer_number)
{
	float sum=0;
	
	for(int i=0;i<=customer_number;i++)
	{
		
		sum=sum+Interarrival_time[i];
	}	
	return sum;

}

////////////////////////////////////////////////////////////////
//This function generates the report of the simulation		  //
////////////////////////////////////////////////////////////////
void generate_report(void)
{
	float RHO=busytime/CLOCK;
	float AVG=SumResponsetime/numberofdeparture;
	float PC4=((double)LongService)/numberofdeparture;

	cout<<"\n\nSINGLE SERVER QUEUE SIMULATION - GROCERY STORE CHECKOUT COUNTER\n";
	cout<<"\tMEAN INTERARRIVAL TIME\t\t\t\t\t"<<MeanInterArrivalTime<<endl;
	cout<<"\tMEAN SERVICE TIME\t\t\t\t\t"<<MeanServiceTime<<endl;
	cout<<"\tSTANDARD DEVIATION OF SERVICE TIME\t\t\t"<<SIGMA<<endl;
	cout<<"\tNUMBER OF CUSTOMER SERVED\t\t\t\t"<<numberofdeparture<<endl<<endl;
	cout<<"\tSERVER UTILIZATION\t\t\t\t\t"<<RHO<<endl;
	cout<<"\tMAXIMUM LINE LENGTH\t\t\t\t\t"<<max_queue<<endl;
	cout<<"\tAVERAGE RESPONSE TIME\t\t\t\t\t"<<AVG<<" Minutes"<<endl;
	cout<<"\tPROPORTION MORE THAN 4 min IN THE SYSTEM\t\t"<<PC4<<endl;
	cout<<"\tSIMULATION RUN LENGTH\t\t\t\t\t"<<CLOCK<<" Minutes"<<endl;
	cout<<"\tNUMBER OF DEPARTURES\t\t\t\t\t"<<numberofdeparture<<endl;
}

////////////////////////////////////////////////////////////////
//This function prints the title of the table and the first   //
//row of the table is also printed.							  //
////////////////////////////////////////////////////////////////
void print_table_header(void)
{
    cout<<" -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<< '|' << setw(7) << "CLOCK" << '|'<< setw(12) << "Queue Length" << '|'<< setw(13) << "Server Status" << '|'<< setw(9) << "Busy Time" << '|'<< setw(20) << "Maximum Queue Length" <<  '|'<<setw(95) << "     Comment     " << '|'<< setw(36) << "     Future Eventlist     "<< '|'<< endl; 
    cout<<" -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
    printf("\n");
    cout<< '|' << setw(7) << CLOCK << '|'<< setw(12) << Queue_status << '|'<< setw(13) << Server_status << '|'<< setw(9) << busytime << '|'<< setw(20) << max_queue << '|'<< setw(95) <<comment <<'|'<<setw(10);
    checkout.print(); 
    cout<<"\n -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
}

////////////////////////////////////////////////////////////////
//This function prints the rows of the table 				  //
//Hence after each event this function can be called to 	  //
//print an entire row of the simulation table 				  //	
////////////////////////////////////////////////////////////////
void print_table_row(void)
{
	printf("\n");
    cout<< '|' << setw(7) << CLOCK << '|'<< setw(12) << Queue_status << '|'<< setw(13) << Server_status <<'|'<< setw(9) << busytime << '|'<< setw(20) << max_queue << '|'<< setw(95) << comment<< '|'<<setw(10); 
    checkout.print();
    cout<<"\n ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
    	
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
	
	number0fservicein++;
	eventtype="A";         									       
	eventdescription="";									  													  	
	checkout.insert_with_priority(eventtype.c_str(),CLOCK+Interarrival_time[a_index],eventdescription.c_str());
	a_index++;
	
	eventtype="E";              							  
	eventdescription="";									  
	checkout.insert_with_priority(eventtype.c_str(),CLOCK+end,eventdescription.c_str());

	comment.append(" A occurs,Schedule next D,");
	comment.append("Schedule next A.");
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
    	sprintf(numstr,"%f",Service_time[s_index]);
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
    sprintf(numstr,"%f",Interarrival_time[a_index]);
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
    	sprintf(numstr,"%f",Service_time[s_index]);
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

	SumResponsetime=SumResponsetime+(CLOCK-arrivaltimeofcustomerbeingdeparted);
	
	if(CLOCK-arrivaltimeofcustomerbeingdeparted> 4)
	{
		LongService++;	
	}
	arrivaltimeofcustomerbeingdeparted=give_arrival_time(numberofdeparture-1);
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
void interprete_data(float endtime_event,float endtime)
{
	struct FEL::node *event;
    first_event(endtime_event);
    //print_table_header();

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
    		sprintf(numstr,"%f",CLOCK);
    	    comment.append(numstr);
    	    comment.append(")");

    		arrival();
    	}
    	else if(event->TYPE=="D")
    	{
    		comment=number[numberofdeparture];
    		comment.append(" D occurs:(D,");
    		sprintf(numstr,"%f",CLOCK);
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
    	//print_table_row();
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
   for(int j=0;j<5;j++)
   {
    Initialize();
    cout<<"\nSIMULATION NUMBER\t"<<j+1<<endl;
    randomnumber.assign_seed(rand());
	generate_time();
	interprete_data(100,150);
	generate_report();
	while(checkout.get_next()!=NULL)
	;
   }
	return 0;
}
