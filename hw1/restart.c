#include "restart.h"

//From R&R p.72
pid_t r_wait ( int *stat_loc ) {
    int retval;
    while ((( retval = wait ( stat_loc )) == -1 ) && ( errno == EINTR ));
    return retval;
}
