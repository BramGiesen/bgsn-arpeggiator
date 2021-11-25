#ifndef _H_TYPES_
#define _H_TYPES_

#include <cstdint>

#define PLUGIN_URI "http://moddevices.com/plugins/mod-devel/arpeggiator"

#define NUM_VOICES 32
#define NUM_NOTE_OFF_SLOTS 32


#define MIDI_NOTEOFF 0x80
#define MIDI_NOTEON  0x90

#define MIDI_NOTE 0
#define MIDI_CHANNEL 1
#define TIMER 2

#define NUM_ARP_MODES 6
#define NUM_OCTAVE_MODES 5

#define NUM_MIDI_CHANNELS 16

#define ONE_OCT_UP_PER_CYCLE 4

struct ArpNoteEvent {
    uint8_t midiNote;
    uint8_t velocity;
    uint8_t channel;
    bool active;
};

struct ArpNoteOffEvent {
    ArpNoteEvent noteEvent;
    uint32_t timer;
};

#endif //_H_TYPES_
