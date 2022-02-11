#ifndef _H_CLOCK_
#define _H_CLOCK_

#include <cstdint>
#include <math.h>

enum SyncMode {
    FREE_RUNNING = 0,
    HOST_BPM_SYNC,
    HOST_QUANTIZED_SYNC
};

class PluginClock {
public:
    PluginClock();
    ~PluginClock();
    void transmitHostInfo(const bool playing, const float beatstPerBar,
            const int hostBeat, const float hostBarBeat, const float hostBpm);
    void setSampleRate(float sampleRate);
    void setSyncMode(int mode);
    void setInternalBpmValue(float internalBpm);
    void setDivision(int division);
    void setTempoMultiplyFactor(int factor);
    void setTempoMultiplyEnabled(bool enabled);
    void setSwing(float swing);
    void syncClock();
    void setPos(uint32_t pos);
    void setNumBarsElapsed(uint32_t numBarsElapsed);
    void countElapsedBars(void);
    void calcPeriod(void);
    void closeGate(void);
    void reset(void);
    bool getGate(void) const;
    float getSampleRate(void) const;
    int getSyncMode(void) const;
    float getInternalBpmValue(void) const;
    int getDivision(void) const;
    float getSwing(void) const;
    int getTempoMultiplyFactor(void) const;
    bool getTempoMultiplyEnabled(void) const;
    uint32_t getPeriod(void) const;
    uint32_t getClockCycleDuration(void) const;
    uint32_t getPos(void) const;
    void tick(void);

private:
    void setBpm(float bpm);
    void checkForTempoChange(void);
    void applyTempoSettings(void);

    bool gate;
    bool trigger;
    bool beatSync;
    bool phaseReset;
    bool playing;
    bool previousPlaying;
    bool endOfBar;
    bool init;
    bool updateTempo;
    bool tempoMultiplyEnabled;
    bool multiplierChanged;
    bool tempoHasChanged;

    uint32_t period;
    uint32_t prev_period;
    uint32_t halfWavelength;
    uint32_t quarterWaveLength;
    uint32_t posA;
    uint32_t posB;

    float beatsPerBar;
    float bpm;
    float internalBpm;
    float hostBpm;
    float previousBpm;
    float sampleRate;
    int division;
    float divisionValue;
    float swing;

    float hostBarBeat;
    float beatTick;
    int triggerIndex;
    int syncMode;
    int previousSyncMode;
    int hostTick;
    int hostBeat;
    int barLength;
    int numBarsElapsed;
    int previousBeat;
    int tempoMultiplyFactor;

    int arpMode;

    // "1/1" "1/2" "1/3" "1/4" "1/4." "1/4T" "1/8" "1/8." "1/8T" "1/16" "1/16." "1/16T" "1/32"
    float divisionValues[13] {0.5, 1, 1.5, 2.0, 2.66666, 3.0, 4.0, 5.33333, 6.0, 8.0, 10.66666, 12.0, 16.0};
};

#endif
