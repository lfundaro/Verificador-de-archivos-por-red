#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
 
char *regexp (char *string, char *patrn, int *begin, int *end) {     
        int i, w=0, len;         
        char *word = NULL;
        regex_t rgT;
        regmatch_t match;
        regcomp(&rgT,patrn,REG_EXTENDED);
        if ((regexec(&rgT,string,1,&match,0)) == 0) {
                *begin = (int)match.rm_so;
                *end = (int)match.rm_eo;
                len = *end-*begin;
                word=smalloc(len+1);
                for (i=*begin; i<*end; i++) {
                        word[w] = string[i];
                        w++; }
                word[w]=0;
        }
        regfree(&rgT);
        return word;
}
 
 
int main() {
    int b,e;
    char *match=regexp("this and7 that","[a-z]+[0-9]",&b,&e);
    printf("->%s<-\n(b=%d e=%d)\n",match,b,e);
    return 0;
}
