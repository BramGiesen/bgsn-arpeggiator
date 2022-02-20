#include "voiceManager.hpp"
#include "types.h"

VoiceManager::VoiceManager() : overwrite_idx(0)
{
    for (int i = 0; i < (int)NUM_VOICES; i++) {
        arpVoice[i].midiNote = 0;
        arpVoice[i].velocity = 100;
        arpVoice[i].channel = 0;
        arpVoice[i].active = false;
        emptyVoice.active = false;
    }
}

VoiceManager::~VoiceManager()
{
}

void VoiceManager::overWriteVoice(ArpNoteEvent event)
{
    arpVoice[overwrite_idx].midiNote = event.midiNote;
    arpVoice[overwrite_idx].channel = event.channel;
    arpVoice[overwrite_idx].active = event.active;

    overwrite_idx = (overwrite_idx + 1) % (int)NUM_VOICES;
}

bool VoiceManager::addVoiceToFreeSlot(ArpNoteEvent event)
{
    int searchNote = 0;

    while (searchNote < (int)NUM_VOICES)
    {
        if (!arpVoice[searchNote].active)
        {
            arpVoice[searchNote].midiNote = event.midiNote;
            arpVoice[searchNote].channel = event.channel;
            arpVoice[searchNote].active = event.active;

            return true;
        }
        searchNote++;
    }
    return false;
}

void VoiceManager::addVoice(ArpNoteEvent event)
{
    if (!addVoiceToFreeSlot(event)) {
        // start overwriting voices if nothing is free
        overWriteVoice(event);
    }
}

ArpNoteEvent VoiceManager::getEvent(int n)
{
    int step = 0;

    for (int i = 0; i < (int)NUM_VOICES; i++) {
        if (arpVoice[i].active)
        {
            if (step == n) {
                return arpVoice[i];
            }
            step++;
        }
    }

    return emptyVoice;
}

int VoiceManager::findVoiceIndexInBuffer(uint8_t noteToFind)
{
    int searchNote = 0;

    while (searchNote < (int)NUM_VOICES)
    {
        if (arpVoice[searchNote].midiNote == (uint8_t)noteToFind
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
    utils.quicksort(arpVoice, 0, NUM_VOICES - 1);
}

void VoiceManager::freeAll()
{
    for (int clear_notes = 0; clear_notes < (int)NUM_VOICES; clear_notes++) {
        arpVoice[clear_notes].midiNote = 0;
        arpVoice[clear_notes].channel = 0;
        arpVoice[clear_notes].active = false;
    }
    overwrite_idx = 0;
}
