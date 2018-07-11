using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include "FEL_m.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//These variables are accessable from all the functions		  											   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CLOCK,l_busy,w_busy,ptime;	  																   
int l_index,w_index,t_index,Queue_status,loader_status,weigh_status;			   
int loading_time[]={10,5,5,10,15,10,10};																  			   
int weighing_time[]={12,12,12,16,12,16};
int travelling_time[]={60,100,40,40,80};																   
FEL checkout,l_queue,w_queue;								

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
void arrival_load(string eventdescription)//ALQ
{
	string eventtype;	
	int loader_queue_length=l_queue.length();
					  
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
	string eventtype;
	struct FEL::node *next_truck;
	int loader_queue_length=l_queue.length();
	int weight_queue_length=w_queue.length();

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
    print_table_header();

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
    		print_table_row();
    		ptime=CLOCK;
    		goto goback;
    	}
    	
    	print_table_row();
    	ptime=CLOCK;
    }
    float alu,asu;
    alu=(float)(l_busy/2)/endtime;
    asu=(float)w_busy/endtime;
    cout<<endl<< "Average loader utilization="<<alu<<endl;	
    cout<<"Average scale utilization="<<asu<<endl;
}

////////////////////////////////////////////////////////////////
//This is the main function. This function calls the required //
//functions to simulate the dump truck operation			  //
////////////////////////////////////////////////////////////////
int main(void)
{	
	interprete_data(76);
	return 0;
}
