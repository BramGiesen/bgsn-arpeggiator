#ifndef H_ARPEGGIATOR
#define H_ARPEGGIATOR

#include <cstdint>

#include "voiceManager.hpp"
#include "notesTracker.hpp"
#include "probabilityPattern.hpp"
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
    Arpeggiator(void);
    ~Arpeggiator(void);
    void setArpEnabled(bool arpEnabled);
    void setLatchMode(bool latchMode);
    void setSampleRate(float sampleRate);
    void setSyncMode(int mode);
    void setBpm(double bpm);
    void setSwing(float swing);
    void updateClockDivision(void);
    void setDivision(int value);
    void setTempoMultiplyFactor(int factor);
    void setTempoMultiplyEnabled(bool enabled);
    void setEnableHold(bool value);
    void setVelocity(uint8_t velocity);
    void setNoteLength(float noteLength);
    void setOctaveSpread(int octaveSpread);
    void setArpMode(int arpMode);
    void setOctaveMode(int octaveMode);
    void setProbabilityPatternSize(int size);
    void setProbability(int index, int value);
    void setPanic(bool panic);
    int getPatternSize(void);
    bool getArpEnabled(void) const;
    bool getLatchMode(void) const;
    float getSampleRate(void) const;
    int getSyncMode(void) const;
    float getBpm(void) const;
    float getSwing(void) const;
    int getSelectedDivision(void) const;
    int getDivision() const;
    int getTempoMultiplyFactor(void) const;
    bool getTempoMultiplyEnabled(void) const;
    bool getEnableHold(void) const;
    uint8_t getVelocity(void) const;
    float getNoteLength(void) const;
    int getOctaveSpread(void) const;
    int getArpMode(void) const;
    int getOctaveMode(void) const;
    int getProbabilityPatternSize(void) const;
    int getProbability(int index) const;
    bool getPanic(void) const;
    void transmitHostInfo(const bool playing, const float beatsPerBar,
            const int beat, const float barBeat, const double bpm);
    void reset(void);
    void emptyMidiBuffer(void);
    void allNotesOff(void);
    struct MidiBuffer getMidiBuffer(void);
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
    void applyProbability(ArpNoteEvent *event);
    void applyHoldToEvent(ArpNoteEvent *event);
    void createNewArpOutEvent(ArpNoteEvent event, size_t currentFrame);
    void noteOffTimer(size_t currentFrame);
    void addEventToNoteOffTimer(ArpNoteEvent event);
    void sendAllNotesOffToOutput(void);
    void resetArpPattern(void);

    ArpNoteOffEvent arpNoteOffEvent[NUM_VOICES];

    int currentStep;

    int octaveMode;
    int octaveSpread;
    int arpMode;
    int divisionValue;

    float noteLength;

    uint8_t velocity;
    uint8_t noteToHold;
    int timeOutTime;
    int firstNoteTimer;
    float barBeat;

    bool first;
    bool arpEnabled;
    bool latchMode;
    bool previousLatch;
    bool latchPlaying;
    bool tempoMultiplierEnabled;
    bool hold;
    bool holdEventDefined;
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
    ProbabilityPattern probabilityPattern;
};

#endif // H_ARPEGGIATOR
