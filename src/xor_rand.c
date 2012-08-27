#include "functions.h"
unsigned long xor_32(unsigned long min, unsigned long max){
    if(!min&&!max){
        min=0;
        max=4294967295;
    }
    unsigned long y=microtime();
    y^=(y<<13);
    y=(y>>17);
    y^=(y<<5);
    return y;
}

unsigned long xor_192(unsigned long min, unsigned long max){
    if(!min&&!max){
        min=0;
        max=4294967295;
    }
    unsigned long x=0,y=0,z=0,w=0,v=0,d=0,t=0;
    unsigned long i=0;
    x=xor_32();
    y^=(x<<13); y=(y>>17); y^=(y<<5);
    z^=(y<<13); z=(z>>17); z^=(z<<5);
    w^=(z<<13); w=(w>>17); w^=(w<<5);
    v^=(w<<13); v=(v>>17); v^=(v<<5);
    d^=(v<<13); d=(d>>17); d^=(d<<5);
    
    for(i=0;i<50;++i){
        t=(x^(x>>2));
        x=y;y=z;z=w;w=v;
        v=(v^(v<<4))^(t^(t<<1));
        d+=326437;
        d+=v;
    }
    return d;
    
}