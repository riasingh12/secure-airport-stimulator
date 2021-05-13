//All the includes
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<time.h>
#include<limits.h>


#include"server.h"

void clean_stdin() {
  //Since the default buffer clear function 
  //is system dependent,this function clears the buffer in all cases 
  int c;
  while ((c = getchar()) != '\n' && c != EOF){}
}

void clrscr(){
	//This func clears the buffer 
	clean_stdin();
	//and also clears the screen when you press any key
	//It is system dependent
	
	printf("\nPress Enter key to exit\n");
    #ifdef _WIN64
    	getchar();
        system("cls");
    #endif
    #ifdef _APPLE_
    	getchar();
        system("clear");
    #endif
    #ifdef _linux_
    	getchar();
        system("clear");
    #endif
}

void delay(float seconds)	
{
	//This function mimics the sleep function in python
	//by taking control for an amount of time
	int milli=1000*seconds;
	clock_t start=clock();
	while(clock()<start+milli);
	
}
void initializeQueue(queue* q){
	//This function initializes the queue values
    q->count = 0;
    q->front = 0;
    q->back = -1;
}
void addToQueue(queue* q, plane next){
	//This function adds planes to the queue
    if(q->count >= MAX_POSSIBLE){
    	// if the queue is full, no more planes can be handled
        printf("\nQueue is isFull\n");
        return ;
    }
    //else it adds one more plane to the queue
	// and increases the count by one
    (q->count)++;
    q->back++;
	q->back %= MAX_POSSIBLE;
    q->arrayOfPlanes[q->back] = next ;
}

plane popQueue(queue *q) {
	//This function deletes the elements of the queue
    plane Plane ;

    if(q->count <= 0) {
    	//If the queue is already empty, nothing is popped
        printf("\nQueue is isEmptyQueue.\n");
        Plane.planeID = 0 ;
        Plane.fuelTime = 0 ;
    }else {
    	//else the first element entered is popped
    	//and the second element is made to be the first element
        (q->count)--;
        //the loss of one element reduces the count by one
        Plane=q->arrayOfPlanes[q->front] ;
        q->front=(q->front+ 1) % MAX_POSSIBLE ;
    	//this part does the popping
	}
    return Plane ;
	//returns the plane object
}

int sizeOfQueue(queue q) {
	//this function returns the size of the queue
	//count is an element of the above defined queue object
    return q.count ;
}

int isEmptyQueue(queue q){
	//this returns a boolean value which tells whether the sizeOfQueue
	//is non-zero or not
    return(sizeOfQueue(q) < 1);
}

int isFullQueue(queue q){
	//this function returns a boolean value which tells 
	//whether the queue is full or not
    return(sizeOfQueue(q) >= MAX_POSSIBLE);
}

void initializeAirport(airport *airPort){
    initializeQueue(&(airPort->landingInit));
    initializeQueue(&(airPort->takeoffInit));
	//The above lines call the function which initialises the queues
	//The initialized queues are queue objects, which are replaced
	//by queue pointers for easy handling
    //landingQueue and takeoffQueue are pointers
	airPort->landingQueue = &(airPort->landingInit);
    airPort->takeoffQueue = &(airPort->takeoffInit);
    //the below values are initialised to zero
    airPort->NumOfPlanes = airPort->NumOfLand = airPort->NumOfTakeOffs =0;
	airPort->NumOfRefuses = airPort->idletime = 0 ;
    airPort->waitForLanding = airPort->waitForTakeoff = 0 ;
}

void start(int *totalTime){
	//the function responsible for starting the  whole process
    int flag = 1;
	double temp;

    printf("\nProgram that simulates an airport with only one runway.\n");
    printf("One plane can land or depart in each unit of time.\n");
    printf("Up to %d planes can be waiting to land or take off at any time.\n", MAX_POSSIBLE);
    //the random generator in c will generate the same pattern each time unless we explicitly seed it
    seed();
    while(flag){
    //making sure that the values we take are not meaningless
    	//we use a while loop and break out if everything is fine
        printf("How many units of time will the simulation run?");
    	scanf("%lf", &temp);
    	*totalTime = temp/1;
    	//If the input by mistake is given as a float for total time
    	//the above lines handle that
        //Since its logically meaningless to give negative values
        //we keep on taking the values as long as there are wrong values
        if(*totalTime < 0){
         printf("These numbers must not be negative.\nPlease re-enter the values\n");
       	flag = 1;
        }
        else 
          flag=0;
    }
    	
    printf("\n%d\n",*totalTime);
    printf("\n\n\n");
    return;
}


void createNewPlane(airport *airPort, int curtime, int type) {
	//This function generates a new plane 
    (airPort->NumOfPlanes)++;
    //the id will be 'n' for 'n'th plane
	airPort->p.planeID = airPort->NumOfPlanes ;
    airPort->p.fuelTime = curtime ;
	//alloting the neccessary values
    switch(type) {
    	//printing the statements
    	//since return is used, there is no need for break
        case ARRIVE :
            printf("\nPlane %d ready to land.\n", airPort->NumOfPlanes);
        	return;

        case DEPART :
            printf("\nPlane %d ready to take off.\n", airPort->NumOfPlanes);
            return ;
            
        default:
        	printf("Error");
        	exit(0);
        	break;
    }
}

void refuseLand(airport *airPort) {
	//if there is an incoming queue of landing planes
	//the take offs must be refused, since they are of lower priority
    printf("\nPlane %d told to try later.\n", airPort->p.planeID);
	//since only takeoffs can be refused, there is no need for an identifier
	//for whether its a takeoff or a landing
    (airPort->NumOfRefuses)++;
}

void land(airport *airPort, plane Plane, int currentTime) {
	//This function effectively lands the plane
	//By changing the variables which must be changed after landing
    int wait;
    wait = currentTime - Plane.fuelTime;
    printf("\n%d: Plane %d landed ", currentTime, Plane.planeID);
    printf("in queue %d units \n", wait);
    (airPort->NumOfLand)++ ;
    //The num of planes landed will increase 
    //The total wait time will also be incremented
	(airPort->waitForLanding) += wait ;
}

void fly(airport *airPort, plane Plane, int curtime) {
	//This function effectively takes off the plane
	//By changing the variables that must be changed after landing
    int wait ;
    wait = curtime - Plane.fuelTime ;
    printf("%d: Plane %d took off ", curtime, Plane.planeID);
    printf("in queue %d units \n", wait);
    (airPort->NumOfTakeOffs)++ ;
    //The num of planes took off will increase 
    //Also the total time waited will also be incremented
    (airPort->waitForTakeoff) += wait ;
}

void idle(airport *airPort, int curtime) {
	//This function calculates the amount of time runway is left idle
    printf("%d: Runway is idle.\n", curtime);
    airPort->idletime++;
}

void Result(airport *airPort, int totalTime) {
	//This functions prints all the results 
	printf("\nResult>>>\n");
    printf("\tSimulation has concluded after %d units.\n", totalTime);
    printf("\tTotal number of planes processed: %d\n", airPort->NumOfPlanes);
    printf("\tNumber of planes landed: %d\n", airPort->NumOfLand);
    printf("\tNumber of planes taken off: %d\n", airPort->NumOfTakeOffs);
    printf("\tNumber of planes refused use: %d\n", airPort->NumOfRefuses);
    printf("\tNumber left ready to land: %d\n", sizeOfAirport(*airPort, ARRIVE));
    printf("\tNumber left ready to take off: %d\n", sizeOfAirport(*airPort, DEPART));
	//the below statements must be executed only if
	//certain values are defined
	
	//eg, if numoflands = 0, then there is no point in calculationg avgWaitForLand
    if(totalTime>0){
		double percentIdleTime =((double)airPort->idletime/totalTime)*100.0;
		printf("\tPercentage of time runway idle: %.3lf \n", percentIdleTime);
	}
	if(airPort->NumOfLand>0){
		double avgWaitTimeForLand =((double)airPort->waitForLanding/airPort->NumOfLand);
		printf("\tAverage wait time to land: %.3lf \n", avgWaitTimeForLand);
	}
	if(airPort->NumOfTakeOffs>0){
	double avgWaitTimeForTakeOff =((double)airPort->waitForTakeoff/airPort->NumOfTakeOffs);
	printf("\tAverage wait time to take off: %.3lf \n",avgWaitTimeForTakeOff);
}
	
}

int randomnumber(void) {
	//This function generates random number
	//The seeding was done beforehand
    int random;
    srand(time(0));
    //Since random number generation in c has a flaw of repetition
    //We have taken quite a few steps to avoid that
	random =(rand() + rand())/2;
	random *=(rand() %63);
	//to generate true random numbers we are combining three random numbers
	//All the hardcoded values chosen here are totally random
	random %= 3;
    return random;
}

void airportAddToQueue(airport *airPort, int type) {
	//This function adds a plane to the queue of the airport
	//it is basically a caller function which decides 
	//whether the parameter must be takeoff or landing queue
    switch(type) {
        case ARRIVE :
              addToQueue(airPort->landingQueue, airPort->p);
              break ;

        case DEPART :
              addToQueue(airPort->takeoffQueue, airPort->p);
              break ;
            
		default:
            printf("Error");
            exit(0);

    }
}

plane airportPopQueue(airport *airPort, int type) {
	//This function deletes a plane from the queue of the airport
	//it is basically a caller function which decides 
	//whether the parameter must be takeoff or landing queue
   	plane p1 ;
	switch(type) {
        case ARRIVE :
            p1 = popQueue(airPort->landingQueue);
            return p1 ;

        case DEPART :
            p1 = popQueue(airPort->takeoffQueue);
            return p1 ;

        default:
            printf("Error");
            exit(0);
    }
}

int sizeOfAirport(airport airPort, int type) {
	//This function finds the size of the airport
	//it is basically a caller function which decides 
	//whether the parameter must be takeoff or landing queue
    switch(type) {
        case ARRIVE :
              return(sizeOfQueue(*(airPort.landingQueue)));

        case DEPART :
              return(sizeOfQueue(*(airPort.takeoffQueue)));

        default:
            printf("Error");
            exit(0);

    }
}

int isFullAirport(airport airPort, int type) {
	//This function finds the size of the airport and 
	//determines whether it is isFullQueue 
	//it is basically a caller function which decides 
	//whether the parameter must be takeoff or landing queue
    switch(type) {
        case ARRIVE :
            return(isFullQueue(*(airPort.landingQueue)));

        case DEPART :
            return(isFullQueue(*(airPort.takeoffQueue)));
    	
    	default:
            printf("Error");
            exit(0);
    }
}

int isEmptyAirport(airport airPort, int type) {
	//This function finds the size of the airport and 
	//determines whether it is empty
	//it is basically a caller function which decides 
	//whether the parameter must be takeoff or landing queue
    switch(type) {
        case ARRIVE :
            return(isEmptyQueue(*(airPort.landingQueue)));

        case DEPART :
            return(isEmptyQueue(*(airPort.takeoffQueue)));
        
        default:
            printf("Error");
            exit(0);
    }
}

void seed(void){
	//This function seeds the rand function
	//making it generate a differnent pattern
    srand((unsigned int)(time(NULL) % 10000));
}
