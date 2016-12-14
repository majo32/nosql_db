#ifndef READER_H
#define READER_H

#include <Common.h>

#ifdef __cplusplus
extern "C" {
#endif

    Bool_t Reader_readLine(char * dest, Uint32_t len);
    Uint32_t Reader_getPart(Uint32_t pos, const char * src, char * dest,
            Uint32_t len, Uint32_t lenDest);

#ifdef __cplusplus
}
#endif

#endif /* READER_H */

