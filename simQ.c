#include <stdio.h> /* printf */
#include <stddef.h> /* for, while, if/else, sizeof */
#include <stdlib.h> /* rand RAND_MAX */
#include <limits.h> /* datatype sizes and limits */
#include <string.h> /* string manipulation */
#include <gsl/gsl_rng.h> /* gsl library */
#include <gsl/gsl_randist.h> /* more gsl library */

/*
At each time interval the following happens or may happen, in this order:

1. Zero or more fulfilled customers have finished being served and leave the system.

2. A service point that is not serving a customer will start to serve the next available customer,
and that customer will leave the queue.

3. If a customer in the queue has reached their waiting tolerance limit, then they leave the queue
and become a timed-out customer.

4. Zero or more customers arrive into the system, and either join the back of the queue, or, if the
queue is full, leave the system as an unfulfilled customer.
*/


/*----- structure definitions  ---------------------- */

/* A linked list node to store a customer queue entry */
struct customer{
    int customerId; /* unique id assigned to each customer in order of arrival */
    int timeout; /* time before getting bored and leaving the queue */
    int waitTime; /* time before being served at service point */
    struct customer* next; 
};

typedef struct customer C;

struct servicePoint{
    int servicePointId; /* unique id assigned to each service point */
    int timeTillFinished; /* time units until customer has finished being served*/
    struct customer *serving; /* pointer to the customer currently being served */
};

typedef struct servicePoint SP;

/* The queue, front stores the customer at the front and rear
   stores the last customer */
struct queue { 
    struct customer *front, *rear; 
};


/* function prototypes ------------------------------ */
SP *createServicePoints(int);
void checkFinishedServing(SP*, int);
struct customer* newNode(int);
struct queue* createQueue();
void enQueue(struct queue*);
void deQueue(struct queue*);


/* no global variables allowed in final version */

/* read these in from the command line and delete all global variables */
int numSims; /* the number of times the simulatio must be repeated */
/* add function prototype definitions here */
char fileIn[]; /* input file name containing parameters */
char fileOut[]; /* output file name where results are to be stored */

/* read these in from the input file and delete all global variables */
int averageNewCustomersPerInterval; /* average whole number of customers per time interval */
int averageTimeTakenToServeCustomer; /* number of time intervals between arrival and being served */
int averageWaitingToleranceOfCustomer; /* average waiting time before customer leaves unfulfilled */
int closingTime; /* time units until post office closes and no new customer can join the queue */
int numServicePoints = 3; /* the number of service points at the post office */
int maxQueueLength; /* the maximum number of customers waiting in the queue */
/* can be -1 if the queue has no maximum length */


/* main function ------------------------------------ */
int main(){
    /* initialise an empty list to use as our queue */
    C *root = NULL;

    SP* servicePoints = createServicePoints(numServicePoints);
    
    checkFinishedServing(servicePoints, numServicePoints);
}


/* functions to manage the queue */

/* utility function to create a new linked list node. */
struct customer* newNode(int custId)
{
    struct customer* temp = (C*)malloc(sizeof(C)); 
    temp->customerId = custId; 
    temp->next = NULL; 
    return temp; 
}

/* utility function to create an empty queue */
struct queue* createQueue()
{
    struct queue* q = (struct queue*)malloc(sizeof(struct queue)); 
    q->front = q->rear = NULL; 
    return q; 
}

/* function to add a customer to the queue */
void enQueue(struct queue* q)
{   
    static int custId = 0;
    custId++;
    /* Create a new LL node */
    struct customer* temp = newNode(custId); 

    /* If queue is empty, then new node is front and rear both */
    if (q->rear == NULL) { 
        q->front = q->rear = temp; 
        return; 
    } 

    /* Add the new node at the end of queue and change rear */
    q->rear->next = temp; 
    q->rear = temp; 
}

/* function to remove a customer from the queue */
void deQueue(struct queue* q)
{
    /* If queue is empty, return NULL. */
    if (q->front == NULL) 
        return; 

    /* Store previous front and move front one node ahead */
    struct customer* temp = q->front; 

    q->front = q->front->next; 

    /* If front becomes NULL, then change rear also as NULL */
    if (q->front == NULL) 
        q->rear = NULL; 

    free(temp); 
}


SP* createServicePoints(int numServicePoints){
    int x;
    SP* servicePoints = malloc(numServicePoints * sizeof *servicePoints); /* servicePoints is an array of pointers to SP structures */
    printf("Size of servicePoints pointer: %d\n",sizeof servicePoints);
    printf("Location of the pointer servicePoints: %p\n",&servicePoints);
    for (x = 0; x < numServicePoints; x++){
        servicePoints[x].servicePointId = x + 1;
        servicePoints[x].timeTillFinished = 0;
        servicePoints[x].serving = NULL;
    }

    for (x = 0; x < numServicePoints; x++)
        printf("SP identifier: %i\n",servicePoints[x].servicePointId);
    return servicePoints;
}

void checkFinishedServing(SP* servicePoints, int numServicePoints){
    int x;
    int timeLeft;
    C *serving;
    for (x = 0; x < numServicePoints; x++){
        serving = servicePoints[x].serving;
        if (serving == NULL){

        }
        
        timeLeft = servicePoints[x].timeTillFinished;
        if (timeLeft == 0){

        }
        printf("%d\n", timeLeft);
    }
}
