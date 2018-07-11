#include <stdlib.h>
//////////////////////////////////////////////////////////
//An instance of the this class can be used to obtain   //
//the random numbers. A specific seed can also be used	//	
//while generating the random number.					//
//////////////////////////////////////////////////////////
class randomgen
{
	//////////////////////////////////////////////////////
	//This function assigns the provided seed to the 	//
	//random number generator							//
	//////////////////////////////////////////////////////
	public:void assign_seed(int seed_in)
	{
		srand(seed_in);
	}

	//////////////////////////////////////////////////////
	//After the seed is set by the previous function 	//
	//this function provides the rnadom number according//
	//to the seed.										//
	////////////////////////////////////////////////////// 
	public:float random_number()
	{
		float z=RAND_MAX;
		return (rand()/z);
	}
};
