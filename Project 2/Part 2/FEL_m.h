////////////////////////////////////////////////////////////////////////////////
//This Future event list headerfile has been modified to handle floating time //
//values. Previously only integer time was supported. Secondly, a swap 		  //			
//function is added which makes sure that departure events are processed	  //	
//before the arrival events. Thirdly, The print statements were removed from  //
//all the methods other than print()										  //			
////////////////////////////////////////////////////////////////////////////////
class FEL
{
	
/* The structure of a node. This is the basic building block of the  linked list.  */

	public:struct node
	{
		string TYPE;
		float  TIME;
		string DESCRIPTION;
		node *NEXT;	
	};
	
	/*Pointers to the first and last node of the Linked list. These pointers prevents the linked list from getting lost*/
	
	struct node *HEADPTR=NULL;
	struct node *TAILPTR=NULL;
	
	
	/*This function inserts a new event according to priority. The Event is inserted in such a way so that the events in list stays in ascending order*/
	public:void insert_with_priority(string eventtype,float eventtime,string eventdescription)
	{
		//This variable is used to test if the insert operation was successfull or not
		bool done=false;	
		
		//Allocating space for the new event
		struct node *PTR = (struct node*)malloc(sizeof(struct node));
		
		//Testing if the malloc operation was successful or not 
		if(PTR==NULL)
		{ 
			printf("\n Malloc failed \n");
		}
		
		PTR->TYPE=eventtype;							//Insering the new type of event
		PTR->TIME=eventtime;                            //Inserting the new event time
		
	    if(eventdescription=="")						//Node formating in case the description is absent
		{
			PTR->DESCRIPTION="-";
		}	
		else
		{	
			PTR->DESCRIPTION=eventdescription;
		}
		
		if(HEADPTR==NULL)    							//Inserting into an empty list
		{
			PTR->NEXT=NULL;
			HEADPTR=TAILPTR=PTR;
			done=true;
		}
	 	else if(TAILPTR->TIME < PTR->TIME)					//Inserting after the last node of the list
		{
			TAILPTR->NEXT=PTR;
			PTR->NEXT=NULL;
			TAILPTR=PTR;
			done=true;
		}
		else if(PTR->TIME <= HEADPTR->TIME)					//Inserting before the first node in case of same eventime time with the first node
		{
			PTR->NEXT=HEADPTR;
			HEADPTR=PTR;
			done=true;
		}
		else 
		{
			struct node *CURRENT;
			struct node *PREV;
			CURRENT=PREV=HEADPTR;
			for(CURRENT=HEADPTR;CURRENT!=TAILPTR;CURRENT=CURRENT->NEXT)
			{	
				if(CURRENT->TIME > PTR->TIME && HEADPTR==CURRENT) 	// Inserting with priority at the start of the linked list 
				{
					PTR->NEXT=CURRENT;
					HEADPTR=PTR;
					done=true;
					break;	
				}
				else if(CURRENT->TIME >= PTR->TIME) 			// Inserting with priority at the middle of a linked list.
				{
					PREV->NEXT=PTR;
					PTR->NEXT=CURRENT;
					done=true;
					break;
				}
				if(PREV->NEXT!=TAILPTR)				
				{
					PREV=CURRENT;
				}
			}
			if(!done && HEADPTR!=TAILPTR)					//Inserting with priority before the last node
			{
				PREV->NEXT=PTR;
				PTR->NEXT=TAILPTR;
				done=true;
			}
		}
		swap();
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	//This function Sorts the list so that the departure events are before the arrival event//
	//if the event time is same. This is done to make sure that the departure is processed	//
	//before the arrival event 																//
	//////////////////////////////////////////////////////////////////////////////////////////
	void swap(void)
	{
		bool swap=true;
		while(swap)
		{
			swap=false;
			struct node *CURRENT;
			struct node *PREV;
			struct node *PTR;
			CURRENT=PREV=HEADPTR;
			for(CURRENT=HEADPTR;CURRENT!=TAILPTR;CURRENT=CURRENT->NEXT)	 
			{
				if(CURRENT->TIME==PREV->TIME && CURRENT->TYPE!=PREV->TYPE)
				{
					if(PREV->TYPE!="D")
					{
						swap=true;
						if(PREV==HEADPTR)
						{
							PREV->NEXT=CURRENT->NEXT;
							CURRENT->NEXT=PREV;
							HEADPTR=CURRENT;
						}
						else
						{
							for(PTR=HEADPTR;PTR->NEXT!=PREV;PTR=PTR->NEXT) 
							;
							PTR->NEXT=CURRENT;
							PREV->NEXT=CURRENT->NEXT;
							CURRENT->NEXT=PREV; 
						}
					}	
					

				}
				PREV=CURRENT;
			}

		}
	}
	/*This method deletes a specific node according to the given argument. If the node is not present then "No match found" is displayed*/
	public:void delete_node(string eventtype,float eventtime,string eventdescription)
	{
	        struct node *PTR ;
	        struct node *CURRENT;
	        
		//calling the search function to locate the node
		PTR=search(eventtype,eventtime,eventdescription);       
	        if(HEADPTR==NULL) 							//When the list is empty   
	        {
	                printf("\nPlease check operation, Cannot delete from an empty list!! \n");  
	        }
		else if(PTR==NULL)							//If the node is not present in the list
		{
			printf("\nNo match found \n");
		}
		else if(PTR==HEADPTR)							//Deleting node from a list having only one node
		{
			if(HEADPTR==TAILPTR)
			{
				HEADPTR=TAILPTR=NULL;
			}
			else 
			{
				HEADPTR=HEADPTR->NEXT;
			}
		}
		else									//Deleting node from any other position
	        {
			for(CURRENT=HEADPTR;CURRENT->NEXT!=PTR;CURRENT=CURRENT->NEXT)	//This loop stores the node before the node to be deleted 
			;
			CURRENT->NEXT=PTR->NEXT;
			if(PTR==TAILPTR)						//Condition if the last node is to be deleted
			{
				TAILPTR=CURRENT;
			}
			
	        }       
	}
	/*This method is used to locate the node in the linked list.It returns NULL if the node is not present otherwise retuns a pointer to the node being searched*/
	public:struct node *search(string eventtype,float eventtime,string eventdescription)
	{
		
		struct node *CURRENT;
	
		if(HEADPTR==NULL)							//Returns NULL if the list is empty
	        { 
	                return(NULL);
	        }
	
	        
		for(CURRENT=HEADPTR;CURRENT!=TAILPTR;CURRENT=CURRENT->NEXT)		//Searches the list except the last node	
	        {
	        	if(CURRENT->TYPE==eventtype && CURRENT->TIME == eventtime && CURRENT->DESCRIPTION==eventdescription)  
	                {
	                	return(CURRENT);	
				
	                }
	        }
											//This tests if the last node is the node being searched for.If not it returns NULL
		if(TAILPTR->TYPE==eventtype && TAILPTR->TIME == eventtime && TAILPTR->DESCRIPTION==eventdescription)
		{
			return(TAILPTR);
		}
		else
		{
			return(NULL);
		}
	
	}
	/*This method prints the linked list*/
	public:void print(void)
	{
		struct node *CURRENT;
		
		if(HEADPTR==NULL)							// Condition when the list is empty
		{
			printf("The list is empty!\n");
		}
		else if(HEADPTR==TAILPTR)						//Condition when the list has one node
		{
			printf("(%s,%f)",HEADPTR->TYPE.c_str(),HEADPTR->TIME);
		}
		else									//If the length of the node is more than 1
		{
			for(CURRENT=HEADPTR;CURRENT!=TAILPTR;CURRENT=CURRENT->NEXT)
	       	{
	  			printf("(%s,%f)",CURRENT->TYPE.c_str(),CURRENT->TIME);
	       	}
	        printf("(%s,%f)",TAILPTR->TYPE.c_str(),TAILPTR->TIME);
		}
	}
	/*This method prints and retunrs the presnt length of the linked list*/
	public:int length()
	{
		if(HEADPTR==NULL)							// When the list is empty
		{
			printf("\nThe Length of the list is 0 \n");
			return 0;
		}
		else if(HEADPTR==TAILPTR)						// When the list only has one node
		{
			printf("\nThe length of the list is 1 \n");
			return 1;
		}
		else									// When the size of list is more than 1
		{
			int count=1;
			struct node *CURRENT;
			for(CURRENT=HEADPTR;CURRENT!=TAILPTR;CURRENT=CURRENT->NEXT)
			{
				count++;
			}
			printf("\nThe length of the list is %d \n",count);
			return count;
		}
	}
	/*This method removes the first node from the list and returns a pointer to removed node.If the list is empty it returns a NULL pointer*/
	public:struct node *get_next(void)
	{
		struct node *PTR;
		if(HEADPTR==NULL)							//When the list is empty 
		{
			return(NULL);
		}
		else if(HEADPTR==TAILPTR)						//When the list has only one node
		{
			PTR=HEADPTR;
			HEADPTR=TAILPTR=NULL;
			return(PTR);
		}
		else									//When the list has more than one node
		{
			PTR=HEADPTR;
			HEADPTR=HEADPTR->NEXT;
			return(PTR);
		}
	}
	/*Retuns the first node of the list without removing it.Returns NULL if the list empty*/
	public:struct node *peek(void)
	{
		if(HEADPTR!=NULL)							//If the list is empty the method returns a NULL pointer
		{
			return(HEADPTR);
		}
		else									//If the length of the list more than 1					
		{
			return(NULL);
		}
	}
};

