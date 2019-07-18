#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
/**
* The basic C Program that they're going to be brute forcing the pin from.
* Macathur Inbody <mdi2455@email.vccs.edu>
* 2019-
* AGPLv3 or Later
*/
static inline unsigned char uchar(char chr){ return chr;};
/**
* This function encodes a string passed to it as an argument into base64. It's
* ~1.5x as fast as the built in base64 program from gnulib. I am not allowing for
* pipes at this time though that is a feature for a future release.
*
*/
int base64_encode(char *dest, const char *src, int srclen){
    const char table[64]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    unsigned int i=0;
    unsigned int j=0;
    unsigned char a=0;
    unsigned char b=0;
    unsigned char c=0;
    unsigned int t=0;
    const char padding[1]="=";
    while (i < srclen){
           
        a=i < srclen ? uchar(src[i++]) : 0;
        b=i < srclen ? uchar(src[i++]) : 0;
        c=i < srclen ? uchar(src[i++]) : 0;
        t= (a << 16) + (b << 8) + c;

        dest[j++]=table[ (t >> (3*6)) & 63];
        dest[j++]=table[ (t >> (2*6)) & 63];  
        dest[j++]=table[ (t >> (1*6)) & 63];
        dest[j++]=table[ (t>>0) & 63];
    }
    if( ( srclen % 3) > 0){
        dest[j-1]=padding[0];
        if(( srclen %3) == 1){
            dest[j-2]=padding[0];
        }
    }
    return 0;
}
int main(int argc,char **argv){
    char *dest=malloc(sizeof *dest);
    char *help="-h";
    char *src_buffer=malloc(sizeof *src_buffer);
    char *total_buffer=malloc(sizeof *total_buffer);
    char *src;
    struct timeval never_wait;   
    never_wait.tv_sec = 0;
    never_wait.tv_usec = 1;
    unsigned int srclen=0;     
    fd_set read_file_descriptors;
    FD_ZERO(&read_file_descriptors);
    FD_SET(STDIN_FILENO,&read_file_descriptors);     
    if(argc > 1 && strncmp(argv[1],help,3) != 0){
        src=argv[1];
        srclen=strlen(src);
        outlen=(srclen*4/3);
        outlen=outlen+(4-(outlen % 4 ));
        dest=malloc(outlen);           
        base64_encode(dest,src,srclen);
        printf("%s",dest);
    }
    
    else if(select(1,&read_file_descriptors,NULL,NULL,&never_wait)){
        int read_bytes=0;
        int read_status=0;
        size_t len=0;
        read_status = getline(&src_buffer, &len, stdin);    
        while (read_status != -1){
        if(read_status!=-1){
            read_bytes+=read_status;
            strcat(total_buffer,src_buffer);
            src_buffer=NULL;            
        }
       
            read_status = getline(&src_buffer, &len, stdin);
        }
        outlen=(read_bytes*8/5);
        outlen=outlen+(8-(outlen % 8 ));
        dest=malloc(outlen);        
        base64_encode(dest,total_buffer,read_bytes);
        printf("%s",dest);
    }
    else{
        printf("%s","If no arguments given it reads from stdin until the first newline character. It's not meant to read entire lines of text so it's only compatible with \n echo -n <string> | base64\n or printf '%s' <string> | base64 \n");
    }
    return 0;
}
