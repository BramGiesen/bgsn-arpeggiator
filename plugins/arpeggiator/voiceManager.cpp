#include "voiceManager.hpp"

VoiceManager::VoiceManager(ArpNoteEvent *arpVoices)
{
    this->arpVoices = arpVoices;
}

VoiceManager::~VoiceManager()
{
}

void VoiceManager::addVoice(ArpNoteEvent event)
{
    uint8_t searchNote = 0;

    while (searchNote < NUM_VOICES)
    {
        if (!arpVoices[searchNote].active)
        {
            arpVoices[searchNote] = event;
            return;
        }
        searchNote++;
    }

}

ArpNoteEvent VoiceManager::getEvent(uint8_t n)
{
    uint8_t searchNote = 0;
    uint8_t step = 0;

    //TODO error handling
    // Iterate over voices until Nth step is found
    while (searchNote < NUM_VOICES)
    {
        if (arpVoices[searchNote].active)
        {
            step++;
            if (step == n) {
                return arpVoices[searchNote];
            }
        }
        searchNote++;
    }
}

int VoiceManager::findVoiceIndexInBuffer(uint8_t noteToFind)
{
    uint8_t searchNote = 0;

    while (searchNote < NUM_VOICES)
    {
        if (arpVoices[searchNote].midiNote == noteToFind)
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
        arpVoices[activeNoteIndex].active = false;
        return true;
    } else {
        return false;
    }
}

void VoiceManager::sort()
{
    utils.quicksort(arpVoices, 0, NUM_VOICES - 1);
}

void VoiceManager::freeAll()
{
    for (unsigned clear_notes = 0; clear_notes < NUM_VOICES; clear_notes++) {
        arpVoices[clear_notes].midiNote = 0;
        arpVoices[clear_notes].channel = 0;
        arpVoices[clear_notes].active = false;
    }
}
