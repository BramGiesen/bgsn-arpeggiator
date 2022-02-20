#ifndef H_VOICE_MANAGER
#define H_VOICE_MANAGER

#include "types.h"
#include "utils.hpp"

#include <cstdint>

class VoiceManager {
public:
    VoiceManager(void);
    ~VoiceManager(void);
    void addVoice(ArpNoteEvent event);
    void freeVoice(ArpNoteEvent event);
    int findVoiceIndexInBuffer(uint8_t noteToFind);
    bool voiceActive(uint8_t noteToFind);
    bool freeVoice(uint8_t midiNote);
    void sort(void);
    void freeAll(void);
    ArpNoteEvent getEvent(int n);
protected:
    ArpNoteEvent arpVoice[NUM_VOICES];
    ArpNoteEvent emptyVoice;
private:
    bool addVoiceToFreeSlot(ArpNoteEvent event);
    void overWriteVoice(ArpNoteEvent event);
    ArpUtils utils;

    int overwrite_idx;
};

#endif // H_VOICE_MANAGER
