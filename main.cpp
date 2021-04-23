#include<iostream>
#include<pthread.h>
#include<stdlib.h>
#include <semaphore.h>
#include <unistd.h>

using namespace::std;

//Global variables

int wh, we, ah, ae, in_boat, NoH, NoE;
pthread_cond_t waitingToBoardH;
pthread_cond_t waitingToBoardE;
pthread_cond_t waitingToRow;
pthread_mutex_t lock;

void BoardBoat(int p, int id)
{
	if(p==0)
	{
		cout<<"\t\tHacker "<<id<<" is aboard the boat\n";
	}
	else
	{
		cout<<"\t\tEmployee "<<id<<" is aboard the boat\n";
	}
}

void RowBoat()
{
	cout<<"-----------------------------------------------\n";
	cout<<"The Boat is being rowed\n";
	sleep(1);
	cout<<"The Boat has reached the other side\n";
	sleep(1);	
	cout<<"The Boat has returned\n";	
	cout<<"-----------------------------------------------\n";
}

void *person_arrives(void *in) 
{
	int *id = (int *)in;
	
	int p = rand()%2; // 0 -> Hacker, 1 -> Employee

	pthread_mutex_lock(&lock); 
					  // Critical section <--
	if(p==0)
  {
		if(NoH==0)
    {
			p=1;
		}
		else
			NoH-=1;
	}
	else if(p==1)
  {
		if(NoE==0)
    {
			p=0;
		}
		else
			NoE-=1;
	}
	
	if(p==0)		// This thread corresponds to a Hacker
	{
		cout<<"Hacker "<<*id<<" has arrived\n";
		wh++;
  		while (ah == 0) 
  		{
    				// if there's another hacker, and room for 2, allow 2 to board
    		if(in_boat + ah + ae < 4 && wh >=2)
    		{
    			wh -= 2; 
    			ah += 2;
	    		cout<<"\tTwo Hackers have been assigned\n";
	    		pthread_cond_signal(&waitingToBoardH);
    		}   		
    		else	// wait for somebody else to find a pair
    		{
    			pthread_cond_wait(&waitingToBoardH, &lock);
    		}
		}			// board
  		ah--;	
  	}

  	else if(p==1)	// This thread corresponds to an Employee
	{
		cout<<"Employee "<<*id<<" has arrived\n";
		we++;
  		while (ae == 0) 
  		{
 			   		// if there's another employee, and room for 2, allow 2 to board
    		if(in_boat + ah + ae < 4 && we >= 2)
    		{
    			we -= 2; 
    			ae += 2;
    			cout<<"\tTwo Employees have been assigned\n";
    			pthread_cond_signal(&waitingToBoardE);
    		} 		
	    	else	// wait for somebody else to find a pair 
    		{
    	  		pthread_cond_wait(&waitingToBoardE, &lock);
    		}
		}			// board
  		ae--;	
  	}
  		
  	BoardBoat(p, *id);
  	in_boat++;

	if (in_boat == 4)	// Boat is full
  	{  	
    	pthread_cond_broadcast(&waitingToRow);
    	
    	sleep(1);
    	RowBoat();
    					// new boat is empty, let everybody aboard
    	in_boat = 0;
    	pthread_cond_signal(&waitingToBoardH);
    	pthread_cond_signal(&waitingToBoardH);
    	pthread_cond_signal(&waitingToBoardE);
   		pthread_cond_signal(&waitingToBoardE);
  	}
  	else				// wait for the boat to fill
  	{
   		pthread_cond_wait(&waitingToRow, &lock);
 		sleep(2);
 	} 					
 						// Critical section -->
 	pthread_mutex_unlock(&lock);
	return 0;
}

int main(int argc, char *argv[]) // ['./a.out','No_of_Hackers','No_of_Employees']
{
	int i, threads;
	int p_id[40];
	pthread_t t_id[40];

	if(argc < 3)
	{
		cout<<"***Missing input parameters: Expected 2***\n";
		exit(0);
	}
	
	NoH = abs(atoi(argv[1]));
	NoE = abs(atoi(argv[2]));
	threads = NoH + NoE;

	if ((threads)%4!=0 || NoH%2==1 || NoE%2==1)
	{
		cout<<"\n***Not possible to cross the river (as per the given codition)***\n\n";
		exit(0);
	}

	time_t t;
	srand((unsigned) time(&t));

	cout<<"\n-----------------------------------------------\n";
	cout<<"\t\t  *** START ***\n";
	cout<<"-----------------------------------------------\n";
	
	for(i=0;i<threads;i++)
	{
		p_id[i] = i+1;
		pthread_create(&t_id[i], NULL, person_arrives, (void *)&p_id[i]);
	}

	for(int k=0;k<i;k++)
	{
		pthread_join(t_id[k], NULL);
	}
	
	cout<<"-----------------------------------------------\n";
	cout<<"\t\t   *** END ***\n";
	cout<<"-----------------------------------------------\n";

	pthread_mutex_destroy(&lock);
	
	return 0;
}
