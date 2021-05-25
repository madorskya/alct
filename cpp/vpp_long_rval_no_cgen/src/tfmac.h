// Copyright (c) 2002, Alexander Madorsky, University of Florida/Physics. All rights reserved.

#ifndef _TFMAC_H_FILE_
#define _TFMAC_H_FILE_


#define MAX_TQ 1000000 // maximum number of time quantums allowed
#define NEXT_TQ  if(__tq_index__==__tq_num__++)
#define FIRST_TQ __tq_num__ = 0;
#define SKIP_TQ(a) __tq_num__+=a;
#define SIMULATE_TQ int __tq_num__ = MAX_TQ; for (int __tq_index__ = 0; __tq_index__ < __tq_num__+5; __tq_index__++ )
#define PRINT_TQ printf("%06d\t", __tq_index__);
#define ptab "\t"


#endif
