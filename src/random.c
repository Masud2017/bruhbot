/******************************************************/
/* includes */
/******************************************************/
#include "random.h"
#include "on_err.h"

/******************************************************/
/* functions */
/******************************************************/
int rand_seed_init()
{
	int err;
	struct timespec t0;

	/* Get the CLOCK_REALTIME time in a timespec struct */
	err = clock_gettime( CLOCK_REALTIME, &t0 );
	on_err(err, "could not attain CLOCK_REALTIME");

        srand48(((long int)123456789));
}

/******************************************************/
int rand_int()
{
	return (int)(1045876.0 * drand48());
}
