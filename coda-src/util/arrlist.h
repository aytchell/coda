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

static char *rcsid = "$Header: /afs/cs.cmu.edu/project/coda-braam/src/coda-4.0.1/RCSLINK/./coda-src/util/arrlist.h,v 1.1 1996/11/22 19:07:59 braam Exp $";
#endif /*_BLURB_*/





#ifndef _ARRLIST_H_
#define _ARRLIST_H_ 1
// arrlist: 
//	pointers to any type stored as an array 
class arrlist {
  public:
    void **list;
    int	maxsize;
    int cursize;

    int Grow(int =0);
    void init(int);
//  public:
    arrlist();
    arrlist(int);
    ~arrlist();
    void add(void *);
};

class arrlist_iterator {
    arrlist *alp;
    int	previndex;

  public:
    arrlist_iterator(arrlist *);
    ~arrlist_iterator();
    void *operator()();
};

#endif _ARRLIST_H_
