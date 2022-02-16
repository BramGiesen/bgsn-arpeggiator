#include "clock.hpp"

PluginClock::PluginClock() :
    gate(false),
    trigger(false),
    beatSync(true),
    playing(false),
    previousPlaying(false),
    endOfBar(false),
    init(false),
    tempoMultiplyEnabled(false),
    tempoMultiplyActive(false),
    multiplierSet(false),
    tempoHasChanged(false),
    period(0),
    smallestDivisionPeriod(0),
    prev_period(0),
    halfWavelength(0),
    quarterWaveLength(0),
    pos(0),
    beatsPerBar(1.0),
    bpm(120.0),
    internalBpm(120.0),
    hostBpm(120.0),
    previousBpm(0),
    sampleRate(48000.0),
    division(1),
    divisionValue(9),
    swing(0),
    hostBarBeat(0.0),
    triggerIndex(0),
    syncMode(1),
    previousSyncMode(0),
    hostBeat(0),
    numBarsElapsed(0),
    previousBeat(0),
    tempoMultiplyFactor(1)
{
}

PluginClock::~PluginClock()
{
}

void PluginClock::transmitHostInfo(const bool playing, const float beatsPerBar,
        const int hostBeat, const float hostBarBeat, const float hostBpm)
{
    this->beatsPerBar = beatsPerBar;
    this->hostBeat = hostBeat;
    this->hostBarBeat = hostBarBeat;
    this->hostBpm = hostBpm;
    this->playing = playing;

    if (playing && !previousPlaying && beatSync) {
        syncClock();
    }
    if (playing != previousPlaying) {
        previousPlaying = playing;
    }

    if (!init) {
        calcPeriod();
        init = true;
    }
}

void PluginClock::setSyncMode(int mode)
{
    switch (mode)
    {
        case FREE_RUNNING:
            beatSync = false;
            break;
        case HOST_BPM_SYNC:
            beatSync = false;
            break;
        case HOST_QUANTIZED_SYNC:
            beatSync = true;
            break;
    }

    this->syncMode = mode;
}

void PluginClock::setInternalBpmValue(float internalBpm)
{
    this->internalBpm = internalBpm;
}

void PluginClock::setTempoMultiplyFactor(int factor)
{
    this->tempoMultiplyFactor = factor;
    tempoHasChanged = true;
}

void PluginClock::setTempoMultiplyEnabled(bool tempoMultiplyEnabled)
{
    this->tempoMultiplyEnabled = tempoMultiplyEnabled;
    calcSmallestPeriodInDivision();
    multiplierSet = true;

    //if (tempoMultiplyEnabled) {
    //    // calculate the smallest division value that is within the same metric
    //    // Once the pos will hit this point, the tempoMultiplier will be enabled
    //} else {
    //    // turn off tempo multiplier
    //    enableTempoMultipy(false);
    //    tempoHasChanged = true;
    //}
}

void PluginClock::setBpm(float bpm)
{
    if (tempoMultiplyActive) {
        bpm = bpm * tempoMultiplyFactor;
    }

    this->bpm = (bpm / 2.0);

    calcPeriod();
}

void PluginClock::setSwing(float swing)
{
    this->swing = swing;
}

void PluginClock::setSampleRate(float sampleRate)
{
    this->sampleRate = sampleRate;
    calcPeriod();
}

void PluginClock::setDivision(int setDivision)
{
    this->division = setDivision;
    this->divisionValue = divisionValues[setDivision];

    calcPeriod();
}

void PluginClock::syncClock()
{
    float tempoMultiply = 1.0;
    // TODO restructure
    if (tempoMultiplyActive) {
        tempoMultiply = tempoMultiplyFactor;
    }

    pos = static_cast<uint32_t>(fmod(sampleRate * ((60.0f / (bpm * 2.0)) * tempoMultiply) * (hostBarBeat + (numBarsElapsed * beatsPerBar)), sampleRate * (60.0f / ((bpm * (divisionValue / 2.0f))))));
}

void PluginClock::setNumBarsElapsed(uint32_t numBarsElapsed)
{
    this->numBarsElapsed = numBarsElapsed;
}

void PluginClock::calcPeriod()
{
    period = static_cast<uint32_t>(sampleRate * (60.0f / (bpm * (divisionValue / 2.0f))));
    halfWavelength = static_cast<uint32_t>(period / 2.0f);
    quarterWaveLength = static_cast<uint32_t>(halfWavelength / 2.0f);
    period = (period == 0) ? 1 : period;
}

void PluginClock::calcSmallestPeriodInDivision()
{
    float dValue;
    if (fmod(divisionValue, 0.3)) {
        dValue = 12.0;
    } else if (fmod(divisionValue, 0.3)) {
        dValue = 16.0;
    } else {
        dValue = 10.66666;
    }
    smallestDivisionPeriod = static_cast<uint32_t>(sampleRate * (60.0f / (bpm * (dValue / 2.0f))) / 2.0);
}

void PluginClock::closeGate()
{
    gate = false;
}

void PluginClock::reset()
{
    trigger = false;
    triggerIndex = 0;
    pos = 0;
}

float PluginClock::getSampleRate() const
{
    return sampleRate;
}

bool PluginClock::getGate() const
{
    return gate;
}

int PluginClock::getSyncMode() const
{
    return syncMode;
}

float PluginClock::getInternalBpmValue() const
{
    return internalBpm;
}

int PluginClock::getTempoMultiplyFactor() const
{
    return tempoMultiplyFactor;
}

bool PluginClock::getTempoMultiplyEnabled() const
{
    return tempoMultiplyEnabled;
}

int PluginClock::getDivision() const
{
    return division;
}

float PluginClock::getSwing() const
{
    return swing;
}

uint32_t PluginClock::getPeriod() const
{
    return period;
}

uint32_t PluginClock::getClockCycleDuration() const
{
    return period / 2;
}

uint32_t PluginClock::getPos() const
{
    return pos;
}

void PluginClock::countElapsedBars()
{
    int beat = static_cast<int>(hostBarBeat);

    if (beatsPerBar <= 1) {
        if (hostBarBeat > 0.99 && !endOfBar) {
            endOfBar = true;
        }
        else if (hostBarBeat < 0.1 && endOfBar) {
            numBarsElapsed++;
            endOfBar = false;
        }
    } else {
        if (beat != previousBeat) {
            numBarsElapsed = (beat == 0) ? numBarsElapsed + 1 : numBarsElapsed;
            previousBeat = beat;
        }
    }
}

void PluginClock::checkForTempoChange()
{

    if (syncMode != FREE_RUNNING) {
        float threshold = 0.009; //TODO might not be needed
        if (fabs(previousBpm - hostBpm) > threshold) {
            tempoHasChanged = true;
            previousBpm = hostBpm;
            return;
        }
    } else if ((internalBpm * tempoMultiplyFactor) != previousBpm) {
        tempoHasChanged = true;
        previousBpm = (internalBpm * tempoMultiplyFactor);
        return;
    }

    if (syncMode != previousSyncMode) {
        tempoHasChanged = true;
        previousSyncMode = syncMode;
        return;
    }
}

void PluginClock::applyTempoSettings()
{
    if (multiplierSet) {
        if (beatSync) {
            tempoMultiplyActive = tempoMultiplyEnabled;
            multiplierSet = false;
            tempoHasChanged = true;
        } else if ((tempoMultiplyFactor > 1) && (fmod(pos, smallestDivisionPeriod))) {
            tempoMultiplyActive = tempoMultiplyEnabled;
            multiplierSet = false;
            tempoHasChanged = true;
            reset();
        }
    }

    if (!tempoHasChanged) {
        return;
    }

    switch (syncMode)
    {
        case FREE_RUNNING:
            setBpm(internalBpm);
            break;
        case HOST_BPM_SYNC:
            setBpm(hostBpm);
            break;
        case HOST_QUANTIZED_SYNC:
            setBpm(hostBpm);
            if (playing) {
                syncClock();
            }
            break;
    }
    tempoHasChanged = false;
}

void PluginClock::tick()
{

    if (bpm <= 0) {
        return;
    }

    countElapsedBars();
    checkForTempoChange();
    applyTempoSettings();

    //TODO check this with beatsync
    if (pos > period && !beatSync) {
        pos = 0;
    }

    uint32_t triggerPos[2];
    triggerPos[0] = 0;
    triggerPos[1] = static_cast<uint32_t>(halfWavelength + (halfWavelength * swing));

    if (pos < triggerPos[1] && trigger) {
        triggerIndex ^= 1;
        trigger = false;
    }
    if (pos >= triggerPos[triggerIndex] && !trigger) {
        gate = true;
        trigger = true;
    }

    if (playing && beatSync) {
        syncClock(); //hard-sync to host position
    }
    else if (!beatSync) { //TODO check reseting POS on reset
        pos++;
    }
}
