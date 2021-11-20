#ifndef _H_ARPEGGIATOR_
#define _H_ARPEGGIATOR_

#include <cstdint>

#include "voiceManager.hpp"
#include "notesTracker.hpp"
#include "../../common/clock.hpp"
#include "../../common/pattern.hpp"
#include "../../common/midiHandler.hpp"
#include "types.h"

class Arpeggiator {
public:
    enum ArpModes {
        ARP_UP = 0,
        ARP_DOWN,
        ARP_UP_DOWN,
        ARP_UP_DOWN_ALT,
        ARP_PLAYED,
        ARP_RANDOM
    };
    Arpeggiator();
    ~Arpeggiator();
    void setArpEnabled(bool arpEnabled);
    void setLatchMode(bool latchMode);
    void setSampleRate(float sampleRate);
    void setSyncMode(int mode);
    void setBpm(double bpm);
    void setSelectedDivision(int selected);
    void updateClockDivision();
    void setDivision(int index, int value);
    void setVelocity(uint8_t velocity);
    void setNoteLength(float noteLength);
    void setOctaveSpread(int octaveSpread);
    void setArpMode(int arpMode);
    void setOctaveMode(int octaveMode);
    void setPanic(bool panic);
    int getPatternSize();
    bool getArpEnabled() const;
    bool getLatchMode() const;
    float getSampleRate() const;
    int getSyncMode() const;
    float getBpm() const;
    int getSelectedDivision() const;
    int getDivision(int index) const;
    uint8_t getVelocity() const;
    float getNoteLength() const;
    int getOctaveSpread() const;
    int getArpMode() const;
    int getOctaveMode() const;
    bool getPanic() const;
    void transmitHostInfo(const bool playing, const float beatsPerBar,
            const int beat, const float barBeat, const double bpm);
    void reset();
    void emptyMidiBuffer();
    void allNotesOff();
    struct MidiBuffer getMidiBuffer();
    void process(const MidiEvent* event, uint32_t eventCount, uint32_t n_frames);

private:
    void setOctavePattern(int patternSize, int octaveSpread);
    void handleMidiInputEvent(const MidiEvent *event, uint8_t status);
    void handleMidiEventDisabledState(const MidiEvent *event, uint8_t status);
    void handleNoteOnEvent(const MidiEvent *event);
    void handleNoteOffEvent(const MidiEvent *event);
    void handleMidiThroughEvent(const MidiEvent *event);
    void handleTimeBasedEvents(uint32_t n_frames);
    void applyOctavePatternToEvent(ArpNoteEvent *event);
    void createNewArpOutEvent(ArpNoteEvent event, size_t currentFrame);
    void noteOffTimer(size_t currentFrame);
    void addEventToNoteOffTimer(ArpNoteEvent event);
    void sendAllNotesOffToOutput();
    void resetArpPattern();

    ArpNoteOffEvent arpNoteOffEvent[NUM_VOICES];

    int currentStep;

    int octaveMode;
    int octaveSpread;
    int arpMode;
    int divisionValues[2];
    int selectedDivision;

    float noteLength;

    uint8_t velocity;
    int timeOutTime;
    int firstNoteTimer;
    float barBeat;

    bool first;
    bool arpEnabled;
    bool latchMode;
    bool previousLatch;
    bool latchPlaying;
    bool trigger;
    bool firstNote;
    bool quantizedStart;
    bool resetPattern;
    bool panic;

    int division;
    float sampleRate;
    double bpm;

    Pattern **arpPattern;
    Pattern **octavePattern;
    MidiHandler midiHandler;
    PluginClock clock;
    VoiceManager *voiceManager;
    NotesTracker notesTracker;
};

#endif //_H_ARPEGGIATOR_
