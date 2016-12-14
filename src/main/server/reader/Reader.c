#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <reader/Reader.h>

Bool_t Reader_readLine(char * dest, Uint32_t len) {
    bzero(dest, len);
    return fgets(dest, len, stdin) != NULL;

}
static Bool_t Reader_isWhiteSpace(char c){
    if((c=='\0') || (c==' ') || (c=='\t') ||(c=='\n') ||(c=='\r')){
        return TRUE;
    }else{
        return FALSE;
    }
}
Uint32_t Reader_getPart(Uint32_t pos, const char * src, char * dest, 
        Uint32_t len, Uint32_t lenDest) {
    Uint32_t i;
    char c;
    Uint32_t part = 0;
    Uint32_t j = 0;
    char cprev = 0;
    
    dest[0] = '\0';
    
    for (i = 0; (i < (len-1)) && (part <= pos) && (src[i] != '\0'); i++) {
        c = src[i];
        if ((Reader_isWhiteSpace(c) == TRUE) && 
                (Reader_isWhiteSpace(cprev) == FALSE)) {
            part++;
        }
        if (part == pos) {
            if ((Reader_isWhiteSpace(c) == FALSE) && (j < (lenDest-1))) {
                dest[j] = c;
                dest[j + 1] = '\0';
                j++;
            }
        }
        cprev = c;
    }
    
    return j;
}