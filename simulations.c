#include <simulations.h>

void runSimulations(char *fileOut, int *numSims, int *maxQueueLength, unsigned int *numServicePoints,
                    unsigned int *closingTime, unsigned int *averageWaitingToleranceOfCustomer, 
                    unsigned int *averageTimeTakenToServeCustomer, unsigned int *averageNewCustomersPerInterval)
{
    unsigned int existsGSL = 0;
    unsigned int n;
    gsl_rng            *r;

    n = chooseDistribution(1,1,1, &r, &existsGSL); /* option 1: Uniform/Flat */
    /* printf("%u\n", (unsigned int)n); */
    /* n = chooseDistribution(1,1,2, &r, &existsGSL); *//* option 2: Normal/Gaussian */
    /* printf("%u\n", (unsigned int)n); */
    /* n = chooseDistribution(1,1,3, &r, &existsGSL); *//* option 3: Poisson */
    /* printf("%u\n", (unsigned int)n); */
    /* n = chooseDistribution(1,1,4, &r, &existsGSL); *//* option 4: Gamma */
    /* printf("%u\n\n", (unsigned int)n); */

    unsigned int s = 1;
    /* totals across all simulations */
    unsigned int totalTimedOut = 0;
    unsigned int totalFulfilled = 0;
    unsigned int totalUnfulfilled = 0;
    unsigned int totalWaitTime = 0;

    while ( s <= (*numSims) )
    {
        printf("\n\n\nSimulation number: %u\n", s);
        fflush(stdout);
        unsigned int custId = 1;
        /* simulation statistics */
        unsigned int timedOut = 0;
        unsigned int unfulfilled = 0; /* count unfulfilled customers*/
        unsigned int fulfilled = 0; /* count fulfilled customers */
        unsigned int combinedWaitTime = 0; /* count collective wait time elapsed of fulfilled customers */

        /* initialise an empty list to use as our queue */
        struct queue* q = createQueue();

        SP* servicePoints = createServicePoints((*numServicePoints));
        
        unsigned int timeUnits = 0;
        unsigned int notAllEmpty = 0;
        unsigned int count = 0;
        while ( timeUnits < (*closingTime) || (q->front) != NULL || notAllEmpty)
        {
            printf("Time units: %d\n", timeUnits);
            count = getCount(q->front);
            printf("Count: %u\n\n", count);
            fflush(stdout);

            if ( (q->front) != NULL || notAllEmpty )
            {
                /* assigned waiting customer to service points and remove fulfilled customers */
                checkFinishedServing(servicePoints, q, (*numServicePoints), (*averageTimeTakenToServeCustomer), &fulfilled, &combinedWaitTime);
                
                /* remove all nodes that have zero patience remaining */
                checkPatienceLimit(&q->front, &timedOut);

                /* check if customers are still being served */
                notAllEmpty = !( checkAllSPEmpty(servicePoints, (*numServicePoints)) );
            }

            if ( timeUnits < (*closingTime) )
            {
                unsigned int j;
                for ( j = 0; j < (*averageNewCustomersPerInterval); j++ )
                {
                    if ( count < (*maxQueueLength) || (*maxQueueLength) == -1 )
                        enQueue(q, (*averageWaitingToleranceOfCustomer), &custId);
                    else
                        ++(unfulfilled); /* increment unfulfilled customers when they attempt to join a full queue */
                    count = getCount(q->front); /* we initialised count to zero so no need to get count on first iteration */
                }
            }

            timeUnits++;
        }

        /* count = getCount(q->front); */
        printf("\n\nFinal count: %u\n", count);
        printf("End time: %u\n\n", timeUnits);
        free(servicePoints);
        free(q);

        /* calculate seconds from closing time till finished serving all customers in queue */
        printf("Time after closing finished serving: %u\n", (timeUnits-(*closingTime)));
        printf("Timed-out customers: %u\n", timedOut);
        printf("Unfulfilled customers: %u\n", unfulfilled);
        printf("Fulfilled customers: %u\n", fulfilled);
        printf("Combined time spent waiting by fufilled customers: %u\n", combinedWaitTime);

        totalTimedOut += timedOut;
        totalUnfulfilled += unfulfilled;
        totalFulfilled += fulfilled;
        totalWaitTime += combinedWaitTime;
        s++;
    }

    gsl_rng_free(r); /* free the memory allocated to GSL random number generator */

    printf("Total timed-out customers: %u\n", totalTimedOut);
    printf("Total unfulfilled customers: %u\n", totalUnfulfilled);
    printf("Total fulfilled customers: %u\n", totalFulfilled);
    printf("Total combined time spent waiting by fufilled customers: %u\n", totalWaitTime);
}
