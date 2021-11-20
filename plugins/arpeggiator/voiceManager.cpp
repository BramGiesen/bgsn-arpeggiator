#include "voiceManager.hpp"
#include <iostream>

VoiceManager::VoiceManager()
{
    for (unsigned i = 0; i < NUM_VOICES; i++) {
        arpVoice[i].midiNote = 0;
        arpVoice[i].channel = 0;
        arpVoice[i].active = false;
        emptyVoice.active = false;
    }
}

VoiceManager::~VoiceManager()
{
}

void VoiceManager::addVoice(ArpNoteEvent event)
{
    int searchNote = 0;

    while (searchNote < NUM_VOICES)
    {
        if (!arpVoice[searchNote].active)
        {
            arpVoice[searchNote].midiNote = event.midiNote;
            arpVoice[searchNote].channel = event.channel;
            arpVoice[searchNote].active = event.active;

            return;
        }
        searchNote++;
    }
}

ArpNoteEvent VoiceManager::getEvent(int n)
{
    int step = 0;

    for (int i = 0; i < NUM_VOICES; i++) {
        if (arpVoice[i].active)
        {
            if (step == n) {
                std::cout << "return step: " << n << std::endl;
                return arpVoice[i];
            }
            step++;
        }
    }
    return emptyVoice;
}

int VoiceManager::findVoiceIndexInBuffer(uint8_t noteToFind)
{
    uint8_t searchNote = 0;

    while (searchNote < NUM_VOICES)
    {
        if (arpVoice[searchNote].midiNote == noteToFind
                && arpVoice[searchNote].active)
        {
            return searchNote;
        }
        searchNote++;
    }
    return -1;
}

bool VoiceManager::voiceActive(uint8_t midiNote)
{
    int activeNoteIndex = findVoiceIndexInBuffer(midiNote);

    if (activeNoteIndex != -1) {
        return true;
    } else {
        return false;
    }
}

bool VoiceManager::freeVoice(uint8_t midiNote)
{
    int activeNoteIndex = findVoiceIndexInBuffer(midiNote);

    if (activeNoteIndex != -1) {
        arpVoice[activeNoteIndex].active = false;
        return true;
    } else {
        return false;
    }
}

void VoiceManager::sort()
{
    // TODO FIX ME
    //utils.quicksort(arpVoice, 0, NUM_VOICES - 1);
}

void VoiceManager::freeAll()
{
    for (unsigned clear_notes = 0; clear_notes < NUM_VOICES; clear_notes++) {
        arpVoice[clear_notes].midiNote = 0;
        arpVoice[clear_notes].channel = 0;
        arpVoice[clear_notes].active = false;
    }
}
