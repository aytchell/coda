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

static char *rcsid = "$Header: /afs/cs.cmu.edu/project/coda-braam/src/coda-4.0.1/RCSLINK/./coda-src/volutil/dump.h,v 1.1 1996/11/22 19:13:34 braam Exp $";
#endif /*_BLURB_*/






#ifndef _DUMP_H_
#define _DUMP_H_ 1

#define DUMPVERSION	2

#define DUMPENDMAGIC	0x3A214B6E
#define DUMPBEGINMAGIC	0xB3A11322

#define D_DUMPHEADER	1
#define D_VOLUMEHEADER  2
#define	D_SMALLINDEX	3
#define	D_LARGEINDEX	4
#define D_VNODE		5
#define	D_VV		6
#define	D_ENDVV		7
#define D_DUMPEND	8
#define D_VOLUMEDISKDATA 9
#define	D_NULLVNODE	10
#define D_DIRPAGES	11
#define D_FILEDATA	12
#define D_RMVNODE	13
#define D_BADINODE	14
#define D_MAX		20

#define MAXDUMPTIMES	50

struct DumpHeader {
    int version;
    VolumeId volumeId;
    VolumeId parentId;
    char volumeName[VNAMESIZE];
    long Incremental;
    Date_t backupDate;
    long oldest, latest;
};

typedef struct {
    byte *DumpBuf;		/* Start of buffer for spooling */
    byte *DumpBufPtr;		/* Current position in buffer */
    byte *DumpBufEnd;		/* End of buffer */
    unsigned long offset;	/* Number of bytes read or written so far. */
    RPC2_Handle rpcid;		/* RPCid of connection, NULL if dumping to file */
    int DumpFd;			/* fd to which to flush or VolId if using RPC */
    unsigned long nbytes;	/* Count of total bytes transferred. */
    unsigned long secs;		/* Elapsed time for transfers -- not whole op */
} DumpBuffer_t;
#define VOLID DumpFd		/* Overload this field if using newstyle dump */
    
/* Exported Routines (from dumpstuff.c) */
extern DumpBuffer_t *InitDumpBuf(byte *, long, VolumeId, RPC2_Handle);
extern DumpBuffer_t *InitDumpBuf(byte *, long, int);
extern int DumpDouble(DumpBuffer_t *, byte, unsigned long, unsigned long);
extern int DumpLong(DumpBuffer_t *, byte tag, unsigned long value);
extern int DumpByte(DumpBuffer_t *, byte tag, byte value);
extern int DumpBool(DumpBuffer_t *, byte tag, unsigned int value);
extern int DumpArrayLong(DumpBuffer_t *, byte, unsigned long *, int nelem);
extern int DumpShort(DumpBuffer_t *, byte tag, unsigned int value);
extern int DumpString(DumpBuffer_t *, byte tag, register char *s);
extern int DumpByteString(DumpBuffer_t *, byte tag, byte *bs, int nbytes);
extern int DumpVV(DumpBuffer_t *, byte tag, struct ViceVersionVector *vv);
extern int DumpFile(DumpBuffer_t *, byte tag, int fd, int vnode);
extern int DumpTag(DumpBuffer_t *, register byte tag);
extern int DumpEnd(DumpBuffer_t *);

/* Exported Routines (from readstuff.c) */
extern char ReadTag(DumpBuffer_t *);
extern int PutTag(char, DumpBuffer_t *);
extern int ReadShort(DumpBuffer_t *,  unsigned short *sp); 
extern int ReadLong(DumpBuffer_t *, unsigned long *lp);
extern int ReadString(register DumpBuffer_t *, register char *to, register int max);
extern int ReadByteString(DumpBuffer_t *, byte *to,int size);
extern int ReadDumpHeader(DumpBuffer_t *, struct DumpHeader *hp);
extern int ReadVolumeDiskData(DumpBuffer_t *, VolumeDiskData *vol);
extern int ReadVV(register DumpBuffer_t *, register vv_t *vv);
extern int ReadFile(DumpBuffer_t *, FILE *);
extern int EndOfDump(DumpBuffer_t *);
#endif _DUMP_H_
