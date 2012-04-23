/*
* Copyright 2011 Macarthur Inbody
*
* This code is licensed under the LGPLv3. It is not dual licensed.
*/

/*
* from here. http://stackoverflow.com/questions/779875/what-is-the-function-to-replace-string-in-c#779888
*/
char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    if (!orig)
        return NULL;
    if (!rep || !(len_rep = strlen(rep)))
        return NULL;
    if (!(ins = strstr(orig, rep))) 
        return NULL;
    if (!with)
        with = "";
    len_with = strlen(with);

    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

//strncpy(dest, str+offset, len);

char *substring(char *string,int start, ...){
    va_list args;
    int unsigned i=0;
    int num=0;
    long long unsigned end=-1;
    long long length=strlen(string);
    va_start(args,start);
    for(i=start;i;++i){
        i=va_arg(args,int);
    }
    end=i;
    printf("%i\n",i);
    va_end(args);
    if(end==-1){
        end=length;
    }
    char *to_string=malloc(length);
    strncpy(to_string,string+start,end);
    return to_string;
}
