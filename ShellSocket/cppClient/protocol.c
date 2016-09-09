#include <stdio.h>
#include "protocol.h"

int GetPtl(Header *h)
{
    if(h==NULL){
        return 0;
    }
    return h->line&0x03;
}

void SetPtl(Header *h,int ptl)
{
    if(h==NULL){
        return;
    }
	h->line = h->line|ptl;
}

int GetMsgType(Header *h)
{
    if(h==NULL){
        return 0;
    }
    return h->line>>2;
}

void SetMsgType(Header *h,int ptl)
{
    if(h==NULL){
        return;
    }
	h->line = h->line|(ptl<<2);
}

