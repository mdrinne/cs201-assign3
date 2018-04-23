#ifndef __GST_INCLUDED__
#define __GST_INCLUDED__

#include <stdio.h>

typedef struct gval GVAL;
typedef struct gst GST;

extern GST *newGST(void (*)(void *,FILE *),int (*)(void *,void *),void (*)(void *));
extern void insertGST(GST *,void *);
extern int findGSTcount(GST *,void *);
extern void *findGST(GST *,void *);
extern void *deleteGST(GST *,void *);
extern int sizeGST(GST *);
extern int duplicates(GST *);
extern void statisticsGST(GST *,FILE *);
extern void displayGST(GST *,FILE *);
extern void displayGSTdebug(GST *,FILE *);
extern void freeGST(GST *);

#endif
