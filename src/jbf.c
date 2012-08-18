#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timex.h>
#include <unistd.h>
#include <math.h>
int a=0;
char b[]="VGhpcyBpcyB0aGUgcHJvZ3JhbSwgaXQncyBzb21ldGhpbmcuIFlheSwgdGhlcmUgaXQgaXMuIE5vdGhpbmcgZWxzZSBoZXJlLiBTb21ldGltZXMgdGhlIHNob3cncyBpbnRlcmVzdGluZyBvdGhlciB0aW1lcyBpdCdzIGJvcmluZy4gT3ZlcmFsbCBpdCBzZWVtcyB0byBiZSBvbiB0aGUgdXAgYW5kIHVwLg==";
static const size_t A=4,B=3,C=63;
static const unsigned char D[0x80]={
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0x3e,0xFF,0xFF,0xFF,0x3f,
    0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,
    0x3c,0x3d,0xFF,0xFF,0xFF,0x40,0xFF,0xFF,
    0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06,
    0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,
    0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
    0x17,0x18,0x19,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,
    0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
    0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,
    0x31,0x32,0x33,0xFF,0xFF,0xFF,0xFF,0xFF
};

size_t aa(void* ba,const char* bb,size_t bc){
    if (ba && bb && (bc%A==0)){
        unsigned char*bd=(unsigned char*)ba;
        size_t be=bc;
        size_t bf=0;
        unsigned char bg,bh,bi,bj;

        while (be>=1){
            /* 4 inputs */
            bg=*bb++;
            bh=*bb++;
            bi=*bb++;
            bj=*bb++;
            be-=A;

            /* Validate ascii */
            if (bg>=0x80||bh>=0x80||bi>=0x80||bj>=0x80){
                return 0; /*ERROR-invalid base64 character*/
            }
            /* Convert ascii to base64 */
            bg=D[bg];
            bh=D[bh];
            bi=D[bi];
            bj=D[bj];

            /* Validate base64 */
            if (bg>C||bh>C){
                return 0; /*ERROR-invalid base64 character*/
            /*the following can be padding*/
            }
            if (bi>C+1||bj>C+1){
                return 0; /*ERROR-invalid base64 character*/
            }
            /* 3 outputs */
            *bd++=((bg&0x3f)<<2)|((bh&0x30)>>4);
            *bd++=((bh&0x0f)<<4)|((bi&0x3c)>>2);
            *bd++=((bi&0x03)<<6)|(bj&0x3f);
            bf+=B;

            /* Padding */
            if (bj==C+1){
               --bf;
                if (bi==C+1){
                   --bf;
                }
            }
        }
        *bd++='\x0'; /*append terminator*/

        return bf;
    }
    else{
        return 0; /*ERROR-null pointer,or size isn't a multiple of 4*/
    }
}


void ab(char a){
    putchar(a);
}
short ac(short ba){
    return ba^a;
   ++a;
}
unsigned long ad(void){
	struct timeval ba;
	gettimeofday(&ba,NULL);
	return ba.tv_usec;
}
unsigned int c=0,d=0,e=0,f=0;
void ae(){
    c=(unsigned int)ad();
    c=d=f=e;
    d^=(c<<13); d=(d>>17); d^=(d<<5);
    e^=(d<<13); e=(e>>17); e^=(e<<5);
    f^=(e<<13); f=(f>>17); f^=(f<<5);
}
unsigned int af(unsigned int ba,unsigned int bb){
    unsigned int bc;
    bc=(c^(c<<11));
    c=d;
    d=e;
    e=f;
    unsigned int rand;
    rand=((f^(f>>19))^(bc^(bc>>8)));
    rand=floor((((float)rand/UINT_MAX)*(bb-ba))+ba);
    return rand;
}
int main(void) {
    int ba=strlen(b);
    int z=0;
    char bb[400];
    aa(bb,b,strlen(b));
    for(z=0;z<ba;++z){
        bb[z]=ac(bb[z]);
    }
    ba=strlen(bb);
    z=0;
    ae();
    unsigned int sleep_min=100000000;
    unsigned int sleep_max=200000000;
    struct timespec remaining;
    struct timespec to_sleep;
    char a[]="\n\0";
    while(ba>=z){
        to_sleep.tv_nsec=af(sleep_min,sleep_max);
        to_sleep.tv_sec=0;
        nanosleep(&to_sleep,&remaining);
                ab((char)bb[z]);
                        if(bb[z]=="."){
                            ab(a[0]);
        }
       ++z;
    }
    printf("\n");
     
    

    return (EXIT_SUCCESS);
}

