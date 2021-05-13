
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<time.h>
#include<limits.h>


#include"server.h"

void clrscr(){
	
	printf("\nPress Enter key to exit\n");
    getchar();
    system("cls");
}

void delay(float seconds)	
{
	int milli=1000*seconds;
	clock_t start=clock();
	while(clock()<start+milli);
	
}
void initializeQueue(queue* q){
    q->count = 0;
    q->front = 0;
    q->back = -1;
}
void addToQueue(queue* q, plane next){
    if(q->count >= MAX_POSSIBLE){
        printf("\nQueue is isFull\n");
        return ;
    }
    (q->count)++;
    q->back++;
	q->back %= MAX_POSSIBLE;
    q->arrayOfPlanes[q->back] = next ;
}

plane popQueue(queue *q) {
    plane Plane ;

    if(q->count <= 0) {
        printf("\nQueue is isEmptyQueue.\n");
        Plane.planeID = 0 ;
        Plane.fuelTime = 0 ;
    }else {
        (q->count)--;
        Plane=q->arrayOfPlanes[q->front] ;
        q->front=(q->front+ 1) % MAX_POSSIBLE ;
	}
    return Plane ;
}

int sizeOfQueue(queue q) {
    return q.count ;
}

int isEmptyQueue(queue q){
    return(sizeOfQueue(q) < 1);
}

int isFullQueue(queue q){
    return(sizeOfQueue(q) >= MAX_POSSIBLE);
}

void initializeAirport(airport *airPort){
    initializeQueue(&(airPort->landingInit));
    initializeQueue(&(airPort->takeoffInit));
	airPort->landingQueue = &(airPort->landingInit);
    airPort->takeoffQueue = &(airPort->takeoffInit);
    airPort->NumOfPlanes = airPort->NumOfLand = airPort->NumOfTakeOffs =0;
	airPort->NumOfRefuses = airPort->idletime = 0 ;
    airPort->waitForLanding = airPort->waitForTakeoff = 0 ;
}

void start(int *totalTime){
    int flag = 1;
	double temp;

    printf("\nProgram that simulates an airport with only one runway.\n");
    printf("One plane can land or depart in each unit of time.\n");
    printf("Up to %d planes can be waiting to land or take off at any time.\n", MAX_POSSIBLE);
    while(flag){
        printf("How many units of time will the simulation run?");
    	scanf("%lf", &temp);
    	*totalTime = temp/1;
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
    (airPort->NumOfPlanes)++;
	airPort->p.planeID = airPort->NumOfPlanes ;
    airPort->p.fuelTime = curtime ;
    switch(type) {
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
    printf("\nPlane %d told to try later.\n", airPort->p.planeID);
    (airPort->NumOfRefuses)++;
}

void land(airport *airPort, plane Plane, int currentTime) {
    int wait;
    wait = currentTime - Plane.fuelTime;
    printf("\n%d: Plane %d landed ", currentTime, Plane.planeID);
    printf("in queue %d units \n", wait);
    (airPort->NumOfLand)++ ;
	(airPort->waitForLanding) += wait ;
}

void fly(airport *airPort, plane Plane, int curtime) {
    int wait ;
    wait = curtime - Plane.fuelTime ;
    printf("%d: Plane %d took off ", curtime, Plane.planeID);
    printf("in queue %d units \n", wait);
    (airPort->NumOfTakeOffs)++ ;
    (airPort->waitForTakeoff) += wait ;
}

void idle(airport *airPort, int curtime) {
    printf("%d: Runway is idle.\n", curtime);
    airPort->idletime++;
}

void Result(airport *airPort, int totalTime) { 
	printf("\nResult>>>\n");
    printf("\tSimulation has concluded after %d units.\n", totalTime);
    printf("\tTotal number of planes processed: %d\n", airPort->NumOfPlanes);
    printf("\tNumber of planes landed: %d\n", airPort->NumOfLand);
    printf("\tNumber of planes taken off: %d\n", airPort->NumOfTakeOffs);
    printf("\tNumber of planes refused use: %d\n", airPort->NumOfRefuses);
    printf("\tNumber left ready to land: %d\n", sizeOfAirport(*airPort, ARRIVE));
    printf("\tNumber left ready to take off: %d\n", sizeOfAirport(*airPort, DEPART));
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
    int random;
    random=rand()%58;
    return random;
}

void airportAddToQueue(airport *airPort, int type) {
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

