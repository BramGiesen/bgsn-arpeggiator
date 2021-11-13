#ifndef _H_UTILS_
#define _H_UTILS_

#include <cstdint>

#include "types.h"

class ArpUtils {
public:
    ArpUtils();
    ~ArpUtils();
    void quicksort(ArpNoteEvent *events, int l, int r);
private:
    void swap(ArpNoteEvent *a, ArpNoteEvent *b);
};

#endif //_H_UTILS_
