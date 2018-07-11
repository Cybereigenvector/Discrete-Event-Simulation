using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include "FEL_m.h"
#include "randomgen.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//These variables are accessable from all the functions		  											   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CLOCK,l_busy,w_busy,ptime,l_index,w_index,t_index,completed,morethan30,response,total_response;
float mean_response;
int Queue_status,loader_status,weigh_status,loading_time[100],weighing_time[100],travelling_time[100];																   
FEL checkout,l_queue,w_queue;								
randomgen rand_gen;
int load_seed[]={123,456,789,753,159};
int weight_seed[]={864,491,316,781,641};
int travel_seed[]={153,359,751,648,754};
int arrival_time[]={0,0,0,0,0,0};

/////////////////////////////////////////////////////////////////////
//Probability distribution for generating loading time             //
/////////////////////////////////////////////////////////////////////
int loading_t(float randomnumber)
{
	if(randomnumber<=0.3)
	{
		return 5;
	}
	else if(randomnumber<=0.8 && randomnumber>0.3)
	{
		return 10;
	}
	else
	{
		return 15;
	}
}
/////////////////////////////////////////////////////////////////////
//Probability distribution for generating weighing time            //
/////////////////////////////////////////////////////////////////////
int weighing_t(float randomnumber)
{
	if(randomnumber<=0.7)
	{
		return 12;
	}
	else
	{
		return 16;
	}
}
/////////////////////////////////////////////////////////////////////
//Probability distribution for generating travelling time          //
/////////////////////////////////////////////////////////////////////
int travelling_t(float randomnumber)
{
	if(randomnumber<=0.4)
	{
		return 40;
	}
	else if(randomnumber<=0.7 && randomnumber>0.4)
	{
		return 60;
	}
	else if(randomnumber<=0.9 && randomnumber>0.7)
	{
		return 80;
	}
	else
	{
		return 100;
	}
}
/////////////////////////////////////////////////////////////////////
//This function generates the loading time, weighing time and the  //
//travelling time using the probability distributions			   //
/////////////////////////////////////////////////////////////////////
void generate_time(int k)
{	
	rand_gen.assign_seed(load_seed[k]);	
    for(int i=0;i<100;i++)
    {	
    	loading_time[i]=loading_t(rand_gen.random_number());
    }
    rand_gen.assign_seed(weight_seed[k]);
    for(int i=0;i<100;i++)
    {
    	weighing_time[i]=weighing_t(rand_gen.random_number());
    }
    rand_gen.assign_seed(travel_seed[k]);
    for(int i=0;i<100;i++)
    {
    	travelling_time[i]=travelling_t(rand_gen.random_number());
    }		
}
////////////////////////////////////////////////////////////////
//This function prints the title of the table and the first   //
//row of the table is also printed.							  //
////////////////////////////////////////////////////////////////
void print_table_header(void)
{
    cout<<" ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<< '|' << setw(7) << "CLOCK" << '|'<< setw(5) << "LQ(t)" << '|'<< setw(4) << "L(t)" << '|'<< setw(5) << "WQ(t)" << '|'<< setw(4) << "W(t)" << '|'<< setw(4) << "Bl" << '|'<< setw(4) << "Bs" << '|'<<setw(25) << "Loader Queue" << '|'<< setw(25) << "Weight Queue"<< '|'<< setw(90) << "                   Future Event List                                                    " << '|'<< endl; 
    cout<<" ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<< '|' << setw(7) << CLOCK << '|'<< setw(5) << l_queue.length() << '|'<< setw(4) << loader_status << '|'<< setw(5) << w_queue.length() << '|'<< setw(4) << weigh_status << '|'<< setw(4) <<l_busy <<'|'<<setw(4)<<w_busy<<'|';
    l_queue.print(1);
    w_queue.print(1); 
    checkout.print(0);
    cout<<"\n ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
}

////////////////////////////////////////////////////////////////
//This function prints the rows of the table 				  //
//Hence after each event this function can be called to 	  //
//print an entire row of the simulation table 				  //	
////////////////////////////////////////////////////////////////
void print_table_row(void)
{
    cout<<endl<< '|' << setw(7) << CLOCK << '|'<< setw(5) << l_queue.length() << '|'<< setw(4) << loader_status << '|'<< setw(5) << w_queue.length() << '|'<< setw(4) << weigh_status << '|'<< setw(4) <<l_busy <<'|'<<setw(4)<<w_busy<<'|';
    l_queue.print(1);
    w_queue.print(1); 
    checkout.print(0);
    cout<<"\n ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
}

////////////////////////////////////////////////////////////////
//This Block Initializes parameters of the simulation         //
//and the variables used to keep statistics		    		  //
////////////////////////////////////////////////////////////////
void Initialize(void)								       	  
{															  		
	CLOCK=0;												  	
	Queue_status=0;											  	
	loader_status=0;	
	weigh_status=0;									  	
	l_busy=0;												   									      				
	l_index=0;		//Loading										  
	w_index=0;		//Weighing								 
	t_index=0;		//Travel
	ptime=0; 	
	l_busy=0;
	w_busy=0;

	morethan30=0;
	completed=0;
	response=0;
	total_response=0;
	mean_response=0;
	for(int l=0;l<6;l++)
	{
		arrival_time[l]=0;
	}
}			
/////////////////////////////////////////////////////////////////
//This function defines the initial condition of the simulation//
/////////////////////////////////////////////////////////////////
void Initial_condition(void)			  	
{															  															  	
	string eventtype,eventdescription;

	loader_status=2;
	weigh_status=1;
	
	eventtype="Loadingqueue";
	eventdescription="DT4";
	l_queue.insert_with_priority(eventtype.c_str(),1,eventdescription.c_str());

	eventdescription="DT5";
	l_queue.insert_with_priority(eventtype.c_str(),2,eventdescription.c_str());

	eventdescription="DT6";
	l_queue.insert_with_priority(eventtype.c_str(),3,eventdescription.c_str());

	eventtype="EW"; 
	eventdescription="DT1";             
	checkout.insert_with_priority(eventtype.c_str(),CLOCK+weighing_time[l_index],eventdescription.c_str());
	w_index++;

	eventtype="EL"; 
	eventdescription="DT2";             
	checkout.insert_with_priority(eventtype.c_str(),CLOCK+loading_time[l_index],eventdescription.c_str());
	l_index++;

	eventtype="EL"; 
	eventdescription="DT3";             
	checkout.insert_with_priority(eventtype.c_str(),CLOCK+loading_time[l_index],eventdescription.c_str());
	l_index++;

}															  
////////////////////////////////////////////////////////////////
//This function handles the ALQ event in the dump truck       //
//simulation 											 	  //
////////////////////////////////////////////////////////////////
void arrival_load(string eventdescription)
{
	string eventtype,truck;	
	int loader_queue_length=l_queue.length();

	int truck_number;
	
	truck=eventdescription.substr(2,2);
	truck_number=atoi(truck.c_str());
	arrival_time[truck_number-1]=CLOCK;
					  
	if(loader_status<2)
	{
		loader_status++;
		eventtype="EL";              
		checkout.insert_with_priority(eventtype.c_str(),CLOCK+loading_time[l_index],eventdescription.c_str());
		l_index++;
	}
	else
	{
		Queue_status++;
		eventtype="Loadqueue";
		if(loader_queue_length)
		{
			l_queue.insert_with_priority(eventtype.c_str(),l_queue.TAILPTR->TIME+1,eventdescription.c_str());
		}
		else
		{
			l_queue.insert_with_priority(eventtype.c_str(),1,eventdescription.c_str());
		}
		
	}
}

////////////////////////////////////////////////////////////////
//This function handles the EL event of the dump truck 		  //
//simulation 												  //										  
////////////////////////////////////////////////////////////////
void end_load(string eventdescription)//EL
{
	string eventtype;
	struct FEL::node *next_truck;

	int loader_queue_length=l_queue.length();
	int weight_queue_length=w_queue.length();
	
	loader_status--;

	if(weigh_status==1)
	{
		eventtype="Weightqueue";
		if(weight_queue_length)
		{
			w_queue.insert_with_priority(eventtype.c_str(),w_queue.TAILPTR->TIME+1,eventdescription.c_str());
		}
		else
		{
			w_queue.insert_with_priority(eventtype.c_str(),1,eventdescription.c_str());
		}
	}
	else if(weigh_status==0)
	{
		eventtype="EW";          
		checkout.insert_with_priority(eventtype.c_str(),CLOCK+weighing_time[w_index],eventdescription.c_str());
		w_index++;
		weigh_status++;
	}
	if(loader_queue_length>0)
	{	
		next_truck=l_queue.get_next();

		eventtype="EL"; 
		eventdescription=next_truck->DESCRIPTION;             
		checkout.insert_with_priority(eventtype.c_str(),CLOCK+loading_time[l_index],eventdescription.c_str());
		l_index++;
		loader_status++;
	} 

}
////////////////////////////////////////////////////////////////
//This function handles the EW event of the dump truck 		  //
//simulation 												  //
////////////////////////////////////////////////////////////////
void end_weigh(string eventdescription)
{
	string eventtype,truck;
	struct FEL::node *next_truck;
	int loader_queue_length=l_queue.length();
	int weight_queue_length=w_queue.length();
	int truck_number;
	
	truck=eventdescription.substr(2,2);
	truck_number=atoi(truck.c_str());
	response=CLOCK-arrival_time[truck_number-1];
	completed++;
	if(response>30)
	{
		morethan30++; 
	}
	total_response=total_response+response;
	weigh_status--;
	eventtype="ALQ";             
	checkout.insert_with_priority(eventtype.c_str(),CLOCK+travelling_time[t_index],eventdescription.c_str());
	t_index++;

	if(weight_queue_length>0)
	{	
		next_truck=w_queue.get_next();

		eventtype="EW"; 
		eventdescription=next_truck->DESCRIPTION;             
		checkout.insert_with_priority(eventtype.c_str(),CLOCK+weighing_time[w_index],eventdescription.c_str());
		w_index++;
		weigh_status++;
	} 
}

////////////////////////////////////////////////////////////////
//This function processes the topmost event from the 		  //
//future event list and calls the appropriate functions. 	  //
//The edntime argument determines the stopping time of the 	  //
//simulation 												  //
////////////////////////////////////////////////////////////////
void interprete_data(int endtime)
{
	struct FEL::node *event;
    Initialize();
    Initial_condition();
    //print_table_header();

    while(CLOCK < endtime)
    {
    	goback:
    	char numstr[11];
    	event=checkout.get_next();
    	CLOCK=event->TIME;
    	if(CLOCK>endtime)
    	{
    		break;
    	}
    	l_busy=l_busy+loader_status*(CLOCK-ptime);
    	w_busy=w_busy+weigh_status*(CLOCK-ptime);
 
    	if(event->TYPE=="ALQ")
    	{
    		arrival_load(event->DESCRIPTION);
    	}
    	else if(event->TYPE=="EL")
    	{
    		end_load(event->DESCRIPTION);    	}
    	else if(event->TYPE=="EW")
    	{
    		end_weigh(event->DESCRIPTION);	
    	}
    	if(CLOCK==checkout.peek()->TIME)
    	{	
    		//print_table_row();
    		ptime=CLOCK;
    		goto goback;
    	}
    	
    	//print_table_row();
    	ptime=CLOCK;
    }
    
}
////////////////////////////////////////////////////////////////
//This function generates the report of the simulation		  //
////////////////////////////////////////////////////////////////
void generate_report(void)
{
	float alu,asu,prop;
    alu=(float)(l_busy/2)/ptime;
    asu=(float)w_busy/ptime;
    prop=(float)morethan30/completed;
    mean_response=(float)total_response/completed;
    cout<<endl<< "Average loader utilization=\t\t\t"<<alu<<endl;	
    cout<<"Average scale utilization=\t\t\t"<<asu<<endl;
    cout<<"Total response time=\t\t\t\t"<<total_response<<endl;
    cout<<"Number of Truck completed=\t\t\t"<<completed<<endl;
    cout<<"Number of Truck over 30 time unit=\t\t"<<morethan30<<endl;
    cout<<"Proportion of truck over 30 time unit=\t\t"<<prop<<endl;
    cout<<"Mean response time=\t\t\t\t"<<mean_response<<endl;
}
////////////////////////////////////////////////////////////////
//This is the main function. This function calls the required //
//functions to simulate the dump truck operation			  //
////////////////////////////////////////////////////////////////
int main(void)
{
	for(int j=0;j<5;j++)
	{
		cout<<endl<<"SIMULATION NUMBER\t"<<j+1<<endl;
		generate_time(j);
		interprete_data(100);
		generate_report();
		while(checkout.get_next()!=NULL)
		;
		while(l_queue.get_next()!=NULL)
		;
		while(w_queue.get_next()!=NULL)
		;
	}
	return 0;
}
