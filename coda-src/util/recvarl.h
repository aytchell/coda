#ifndef _BLURB_
#define _BLURB_
/*

            Coda: an Experimental Distributed File System
                             Release 4.0

          Copyright (c) 1987-1996 Carnegie Mellon University
                         All Rights Reserved

Permission  to  use, copy, modify and distribute this software and its
documentation is hereby granted,  provided  that  both  the  copyright
notice  and  this  permission  notice  appear  in  all  copies  of the
software, derivative works or  modified  versions,  and  any  portions
thereof, and that both notices appear in supporting documentation, and
that credit is given to Carnegie Mellon University  in  all  documents
and publicity pertaining to direct or indirect use of this code or its
derivatives.

CODA IS AN EXPERIMENTAL SOFTWARE SYSTEM AND IS  KNOWN  TO  HAVE  BUGS,
SOME  OF  WHICH MAY HAVE SERIOUS CONSEQUENCES.  CARNEGIE MELLON ALLOWS
FREE USE OF THIS SOFTWARE IN ITS "AS IS" CONDITION.   CARNEGIE  MELLON
DISCLAIMS  ANY  LIABILITY  OF  ANY  KIND  FOR  ANY  DAMAGES WHATSOEVER
RESULTING DIRECTLY OR INDIRECTLY FROM THE USE OF THIS SOFTWARE  OR  OF
ANY DERIVATIVE WORK.

Carnegie  Mellon  encourages  users  of  this  software  to return any
improvements or extensions that  they  make,  and  to  grant  Carnegie
Mellon the rights to redistribute these changes without encumbrance.
*/

static char *rcsid = "$Header: /coda/coda.cs.cmu.edu/project/coda/cvs/coda/coda-src/util/recvarl.h,v 4.1 1997/01/08 21:51:10 rvb Exp $";
#endif /*_BLURB_*/





#ifndef _REC_VARL_H
#define _REC_VARL_H 1
/*
 * varl.h
 * declaration of the variable length class
 *	created 02/20/92 -- Puneet Kumar
 *
 */

typedef int recvarl_length_t;  // to allow sizeof() in recvarl::new()
 
class recvarl {
  public:
    recvarl_length_t length; 	/* end of the class */
    unsigned long vfld[1];	/* beginning of variable length part */

    void *operator new(size_t, int); /* the real new */
    void *operator new(size_t);    /* dummy to keep g++ happy */
    void operator delete(void *, size_t);
    recvarl(int); 
    ~recvarl();
    int size();			/* return sizeof(varl) for a particular  class instance */
    void *end();		/* return pointer past end of block */
    void destroy();
};
#endif _REC_VARL_H
