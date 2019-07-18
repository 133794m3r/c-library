#include "functions.h"
#include "functions.c"
#include <sys/types.h>
#include <unistd.h>

unsigned int xor_32(unsigned int min, unsigned int max){
    if(!min&&!max){
        min=0;
        max=4294967295;
    }
    unsigned int y=microtime();
    unsigned int throw_away=3;
    if(y&1){
        throw_away*=1;
    }
    else{
        throw_away*=2;
    }
    unsigned int i=0;
    for(i=0;i<throw_away;++i){
        y^=(y<<13);
        y=(y>>17);
        y^=(y<<5);
    }
    return y;
}

unsigned int xor_64(unsigned int min, unsigned int max){
    unsigned int x=microtime();
    unsigned int y=(int)getpid();
    unsigned int throw_away=2;
    unsigned int i=0;
    unsigned int t=0;
    if(y&1){
        throw_away*=1;
    }
    else{
        throw_away*=2;
    }
    if(x&1){
        throw_away+=1;
    }
    else{
        throw_away+=2;
    }
    for(i=0;i<throw_away;++i){
        t=(x^(x<<10));
        x=y;
        y=(y^(y>>10))^(t^(t>>13));
    }
    return y;
}

unsigned int xor_128(unsigned int min,unsigned int max){
    unsigned int x=0,y=0,z=0,w=0,t=0;
   
    w=xor_64();
    z^=(w<<1); z=(z>>3); z^=(z<<10);
    x^=(z<<16); z=(x>>5); z^=(x<<1);
    y^=(x>>2); y=(y<<5); y^=(y>>15);
    
    t=(x^(x<<20))^(y^(y>>11))^(z^(z<<27))^(w^(w>>6));
    x=y;y=z;z=w;
    
    return (w=t);
}

unsigned long xor_192(unsigned long min, unsigned long max){
    if(!min&&!max){
        min=0;
        max=4294967295;
    }
    unsigned int x=0,y=0,z=0,w=0,v=0,d=0,t=0;
    unsigned int i=0;
    x=xor_64();
    y^=(x<<1); y=(y>>3); y^=(y<<10);
    z^=(y<<16); z=(z>>5); z^=(z<<1);
    w^=(z>>2); w=(w<<5); w^=(w>>15);
    v^=(w<<20); v=(v>>15); v^=(v<<17);
    d^=(v<<8); d=(d>>23); d^=(d<<7);
        t=(x^(x>>2));
        x=y;y=z;z=w;w=v;
        v=(v^(v<<4))^(t^(t<<1));
        d+=326437;
    return d+v;
}