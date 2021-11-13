#ifndef _H_TYPES_
#define _H_TYPES_

struct ArpNoteEvent {
    uint8_t midiNote;
    uint8_t channel;
    bool active;
};

struct ArpNoteOffEvent {
    ArpNoteEvent noteEvent;
    uint32_t timer;
};

#endif //_H_TYPES_
