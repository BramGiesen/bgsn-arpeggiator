#ifndef H_UTILS
#define H_UTILS

#include <cstdint>

#include "types.h"

class ArpUtils {
public:
    ArpUtils(void);
    ~ArpUtils(void);
    void quicksort(ArpNoteEvent *events, int l, int r);
private:
    void swap(ArpNoteEvent *a, ArpNoteEvent *b);
};

#endif // H_UTILS
