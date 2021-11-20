#ifndef _H_VOICE_MANAGER_
#define _H_VOICE_MANAGER_

#include "types.h"
#include "utils.hpp"

class VoiceManager {
public:
    VoiceManager();
    ~VoiceManager();
    void addVoice(ArpNoteEvent event);
    void freeVoice(ArpNoteEvent event);
    int findVoiceIndexInBuffer(uint8_t noteToFind);
    bool voiceActive(uint8_t noteToFind);
    bool freeVoice(uint8_t midiNote);
    void sort();
    void freeAll();
    ArpNoteEvent getEvent(int n);
protected:
    ArpNoteEvent arpVoice[NUM_VOICES];
    ArpNoteEvent emptyVoice;
private:
    ArpUtils utils;
};

#endif //_H_VOICE_MANAGER_
