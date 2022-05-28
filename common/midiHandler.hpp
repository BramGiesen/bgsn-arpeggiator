#ifndef H_MIDI_HANDLER
#define H_MIDI_HANDLER

#include "DistrhoPlugin.hpp"

#include <cstdint>

#define MIDI_BUFFER_SIZE 2048
#define EMPTY_SLOT 200

#define MIDI_NOTEOFF 0x80
#define MIDI_NOTEON  0x90
#define MIDI_SYSTEM_EXCLUSIVE 0xF0
#define MIDI_MTC_QUARTER_FRAME 0xF1
#define MIDI_SONG_POSITION_POINTER 0xF2
#define MIDI_SONG_SELECT 0xF3
#define MIDI_UNDEFINED_F4 0xF4
#define MIDI_UNDEFINED_F5 0xF5
#define MIDI_TUNE_REQUEST 0xF6
#define MIDI_END_OF_EXCLUSIVE 0xF7
#define MIDI_TIMING_CLOCK 0xF8
#define MIDI_UNDEFINED_F9 0xF9
#define MIDI_START 0xFA
#define MIDI_CONTINUE 0xFB
#define MIDI_STOP 0xFC
#define MIDI_UNDEFINED_FD 0xFD
#define MIDI_ACTIVE_SENSING 0xFE
#define MIDI_SYSTEM_RESET 0xFF

struct MidiBuffer {
    unsigned int maxBufferSize = MIDI_BUFFER_SIZE;

    MidiEvent bufferedEvents[MIDI_BUFFER_SIZE];
    unsigned int numBufferedEvents;

    MidiEvent bufferedMidiThroughEvents[MIDI_BUFFER_SIZE];
    unsigned int numBufferedThroughEvents;

    MidiEvent midiOutputBuffer[MIDI_BUFFER_SIZE];
    unsigned int numOutputEvents;
};

class MidiHandler {
public:
    MidiHandler(void);
    ~MidiHandler(void);
    void emptyMidiBuffer(void);
    void appendMidiMessage(MidiEvent event);
    void appendMidiThroughMessage(MidiEvent event);
    void resetBuffer(void);
    int getNumEvents(void);
    void mergeBuffers(void);
    //MidiEvent getMidiEvent(int index);
    struct MidiBuffer getMidiBuffer(void);
private:
    MidiBuffer buffer;
};

#endif //H_MIDI_HANDLER
