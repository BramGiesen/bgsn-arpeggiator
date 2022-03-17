#include "arpeggiator.hpp"
#include "types.h"
#include <bits/stdint-uintn.h>

Arpeggiator::Arpeggiator() :
    currentStep(0),
    octaveMode(0),
    octaveSpread(1),
    arpMode(0),
    noteLength(0.8),
    velocity(80),
    noteToHold(0),
    timeOutTime(1000),
    firstNoteTimer(0),
    barBeat(0.0),
    first(true),
    arpEnabled(true),
    latchMode(false),
    previousLatch(false),
    latchPlaying(false),
    tempoMultiplierEnabled(false),
    hold(false),
    holdEventDefined(false),
    trigger(false),
    firstNote(false),
    quantizedStart(false),
    resetPattern(false),
    panic(false),
    division(0),
    sampleRate(48000.0),
    bpm(0.0)
{
    clock.transmitHostInfo(0, 4, 1, 1, 120.0);
    clock.setSampleRate(static_cast<float>(48000.0));
    clock.setDivision(7);

    divisionValue = 9;

    arpPattern = new Pattern*[6];

    arpPattern[0] = new PatternUp();
    arpPattern[1] = new PatternDown();
    arpPattern[2] = new PatternUpDown();
    arpPattern[3] = new PatternUpDownAlt();
    arpPattern[4] = new PatternUp();
    arpPattern[5] = new PatternRandom();

    octavePattern = new Pattern*[5];

    octavePattern[0] = new PatternUp();
    octavePattern[1] = new PatternDown();
    octavePattern[2] = new PatternUpDown();
    octavePattern[3] = new PatternUpDownAlt();
    octavePattern[4] = new PatternCycle();

    voiceManager = new VoiceManager();

    for (int i = 0; i < (int)NUM_VOICES; i++) {
        arpNoteOffEvent[i].noteEvent.midiNote = 0;
        arpNoteOffEvent[i].noteEvent.channel = 0;
        arpNoteOffEvent[i].noteEvent.active = false;
        arpNoteOffEvent[i].timer = 0;
    }
}

Arpeggiator::~Arpeggiator()
{

    delete voiceManager;
    voiceManager = nullptr;

    delete arpPattern[0];
    delete arpPattern[1];
    delete arpPattern[2];
    delete arpPattern[3];
    delete arpPattern[4];
    delete arpPattern[5];
    delete octavePattern[0];
    delete octavePattern[1];
    delete octavePattern[2];
    delete octavePattern[3];
    delete octavePattern[4];

    delete[] arpPattern;
    arpPattern = nullptr;
    delete[] octavePattern;
    octavePattern = nullptr;
}

void Arpeggiator::setArpEnabled(bool arpEnabled)
{
    this->arpEnabled = arpEnabled;
}

void Arpeggiator::setLatchMode(bool latchMode)
{
    this->latchMode = latchMode;
}

void Arpeggiator::setSampleRate(float newSampleRate)
{
    if (newSampleRate != sampleRate) {
        clock.setSampleRate(newSampleRate);
        sampleRate = newSampleRate;
    }
}

void Arpeggiator::setSyncMode(int mode)
{
    switch (mode)
    {
        case FREE_RUNNING:
            clock.setSyncMode(FREE_RUNNING);
            quantizedStart = false;
            break;
        case HOST_BPM_SYNC:
            clock.setSyncMode(HOST_BPM_SYNC);
            quantizedStart = false;
            break;
        case HOST_QUANTIZED_SYNC:
            clock.setSyncMode(HOST_QUANTIZED_SYNC);
            quantizedStart = true;
            break;
        default:
            break;
    }
}

void Arpeggiator::setBpm(double newBpm)
{
    if (newBpm != bpm) {
        clock.setInternalBpmValue(static_cast<float>(newBpm));
        bpm = newBpm;
    }
}

void Arpeggiator::setSwing(float swing)
{
    clock.setSwing(swing);
}

void Arpeggiator::setDivision(int value)
{
    divisionValue = value;
    updateClockDivision();
}

void Arpeggiator::setTempoMultiplyFactor(int factor)
{
    clock.setTempoMultiplyFactor(factor);
}

void Arpeggiator::setTempoMultiplyEnabled(bool enabled)
{
    tempoMultiplierEnabled = enabled;
    clock.setTempoMultiplyEnabled(enabled);
}

void Arpeggiator::setEnableHold(bool hold)
{
    this->hold = hold;
}

void Arpeggiator::updateClockDivision()
{

    if (divisionValue != division) {
        clock.setDivision(divisionValue);
        division = divisionValue;
    }
}

void Arpeggiator::setVelocity(uint8_t velocity)
{
    this->velocity = velocity;
}

void Arpeggiator::setNoteLength(float noteLength)
{
    this->noteLength = noteLength;
}

void Arpeggiator::setOctaveSpread(int octaveSpread)
{
    this->octaveSpread = octaveSpread;
}

void Arpeggiator::setArpMode(int arpMode)
{
    arpPattern[arpMode]->setStep(arpPattern[this->arpMode]->getStep());
    arpPattern[arpMode]->setDirection(arpPattern[this->arpMode]->getDirection());

    this->arpMode = arpMode;
}

void Arpeggiator::setOctaveMode(int octaveMode)
{
    octavePattern[octaveMode]->setStep(octavePattern[this->octaveMode]->getStep());
    octavePattern[octaveMode]->setDirection(octavePattern[this->octaveMode]->getDirection());

    this->octaveMode = octaveMode;
}

void Arpeggiator::setProbabilityPatternSize(int size)
{
    probabilityPattern.setNumSteps(size);
}

void Arpeggiator::setProbability(int index, int value)
{
    probabilityPattern.setProbability(index, value);
}

void Arpeggiator::setPanic(bool panic)
{
    this->panic = panic;
}

bool Arpeggiator::getArpEnabled() const
{
    return arpEnabled;
}

bool Arpeggiator::getLatchMode() const
{
    return latchMode;
}

float Arpeggiator::getSampleRate() const
{
    return clock.getSampleRate();
}

int Arpeggiator::getSyncMode() const
{
    return clock.getSyncMode();
}

float Arpeggiator::getBpm() const
{
    return clock.getInternalBpmValue();
}

float Arpeggiator::getSwing() const
{
    return clock.getSwing();
}

int Arpeggiator::getDivision() const
{
    return divisionValue;
}

int Arpeggiator::getTempoMultiplyFactor() const
{
    return clock.getTempoMultiplyFactor();
}

bool Arpeggiator::getTempoMultiplyEnabled() const
{
    return clock.getTempoMultiplyEnabled();
}

bool Arpeggiator::getEnableHold() const
{
    return hold;
}

uint8_t Arpeggiator::getVelocity() const
{
    return velocity;
}

float Arpeggiator::getNoteLength() const
{
    return noteLength;
}

int Arpeggiator::getOctaveSpread() const
{
    return octaveSpread;
}

int Arpeggiator::getArpMode() const
{
    return arpMode;
}

int Arpeggiator::getOctaveMode() const
{
    return octaveMode;
}

int Arpeggiator::getProbabilityPatternSize() const
{
    return probabilityPattern.getNumSteps();
}

int Arpeggiator::getProbability(int index) const
{
    return probabilityPattern.getProbability(index);
}

bool Arpeggiator::getPanic() const
{
    return panic;
}

void Arpeggiator::transmitHostInfo(const bool playing, const float beatsPerBar,
        const int beat, const float barBeat, const double bpm)
{
    clock.transmitHostInfo(playing, beatsPerBar, beat, barBeat, bpm);
    this->barBeat = barBeat;
}

void Arpeggiator::reset()
{
    clock.reset();
    clock.setNumBarsElapsed(0);

    for (int a = 0; a < (int)NUM_ARP_MODES; a++) {
        arpPattern[arpMode]->reset();
    }
    for (int o = 0; o < (int)NUM_OCTAVE_MODES; o++) {
        octavePattern[o]->reset();
    }
    probabilityPattern.reset();

    firstNoteTimer  = 0;
    currentStep = 0;
    notesTracker.resetAll();
    latchPlaying = false;
    firstNote = false;
    first = true;

    voiceManager->freeAll();
}

void Arpeggiator::emptyMidiBuffer()
{
    midiHandler.emptyMidiBuffer();
}

void Arpeggiator::allNotesOff()
{
    reset();
}

void Arpeggiator::sendAllNotesOffToOutput()
{
    struct MidiEvent midiEvent;
    //send all notes off, on current active MIDI channel
    midiEvent.size = 3;
    midiEvent.data[2] = 0;

    midiEvent.frame = 0;
    midiEvent.data[0] = 0xb0 | arpNoteOffEvent[currentStep].noteEvent.channel;
    midiEvent.data[1] = 0x40; // sustain pedal
    midiHandler.appendMidiMessage(midiEvent);
    midiEvent.data[1] = 0x7b; // all notes off
    midiHandler.appendMidiMessage(midiEvent);
}

int Arpeggiator::getPatternSize()
{
    int patternSize = notesTracker.getNumActiveNotes();

    switch (arpMode)
    {
        case ARP_UP_DOWN:
            patternSize = (patternSize >= 3) ? patternSize + (patternSize - 2) : patternSize;
            break;
        case ARP_UP_DOWN_ALT:
            patternSize = (patternSize >= 3) ? (patternSize * 2) : patternSize;
            break;
        default:
            break;
    }
    return patternSize;
}

void Arpeggiator::setOctavePattern(int patternSize, int octaveSpread)
{
    switch (octaveMode)
    {
        case ONE_OCT_UP_PER_CYCLE:
            octavePattern[octaveMode]->setPatternSize(patternSize);
            octavePattern[octaveMode]->setCycleRange(octaveSpread);
            break;
        default:
            octavePattern[octaveMode]->setPatternSize(octaveSpread);
            break;
    }
}

struct MidiBuffer Arpeggiator::getMidiBuffer()
{
    return midiHandler.getMidiBuffer();
}

void Arpeggiator::handleNoteOnEvent(const MidiEvent *event)
{
    uint8_t channel = event->data[0] & 0x0F;
    uint8_t midiNote = event->data[1];

    bool pitchFound = voiceManager->voiceActive(midiNote);

    if (pitchFound && !latchPlaying) {
        //'New' pitch is already active, disregard
        return;
    }

    if (first) {
        firstNote = true;
    }

    if (notesTracker.getNumKeysPressed() == 0) {
        if (!latchPlaying) { //TODO check if there needs to be an exception when using sync
            octavePattern[octaveMode]->reset();
            if (clock.getSyncMode() != 2) {
                clock.reset();
            }
            currentStep = 0;
        }
        if (latchMode) {
            latchPlaying = true;
            notesTracker.resetAll();
            voiceManager->freeAll();
        }
        resetPattern = true;
    }

    if (notesTracker.getNumActiveNotes() > (NUM_VOICES - 1)) {
        reset();
        return;
    }

    //Add new arp event to voice handler
    ArpNoteEvent arpEvent;
    arpEvent.midiNote = midiNote;
    arpEvent.channel = channel;
    arpEvent.velocity = 0;
    arpEvent.active = true;

    voiceManager->addVoice(arpEvent);

    notesTracker.registerNewPressedKey();
    notesTracker.registerNewActiveNote();

    if (!latchPlaying) {
        notesTracker.setNumActiveNotes(notesTracker.getNumKeysPressed());
    }

    if (arpMode != (int)ARP_PLAYED) {
        voiceManager->sort();
    }
}

void Arpeggiator::handleNoteOffEvent(const MidiEvent *event)
{

    uint8_t midiNote = event->data[1];

    // If the note off is not active in the arp, redirect note off to midithrough
    if (!voiceManager->voiceActive(midiNote)) {
        handleMidiThroughEvent(event);
        return;
    }

    if (!latchMode) {
        latchPlaying = false;
    } else {
        latchPlaying = true;
    }

    if ((notesTracker.getNumActiveNotes() == 0) && !latchPlaying && !latchMode) {
        reset();
        return;
    }

    notesTracker.keyReleased();

    if (!latchPlaying) {
        int keysPressed = notesTracker.getNumKeysPressed();
        notesTracker.setNumActiveNotes(keysPressed);
    }

    if (!latchMode) {
        voiceManager->freeVoice(midiNote);
        if (arpMode != (int)ARP_PLAYED) {
            voiceManager->sort();
        }
    }

    // If by releasing the note the current step is higher then the amount of active notes,
    // decrement the amount of steps until this is back in range
    while ((currentStep > 0) && (currentStep > (notesTracker.getNumActiveNotes() - 1)))
    {
        currentStep--;
        arpPattern[arpMode]->setStep(currentStep);
    }
}

void Arpeggiator::handleMidiThroughEvent(const MidiEvent *event)
{
    struct MidiEvent midiThroughEvent;

    midiThroughEvent.frame = event->frame;
    midiThroughEvent.size = event->size;
    for (unsigned int d = 0; d < midiThroughEvent.size; d++) {
        midiThroughEvent.data[d] = event->data[d];
    }
    midiHandler.appendMidiThroughMessage(midiThroughEvent);
}

void Arpeggiator::handleMidiInputEvent(const MidiEvent *event, uint8_t status)
{
    uint8_t midiNote = event->data[1];

    if (arpEnabled) {

        if (!latchPlaying && (midiNote == 0x7b) && (event->size == 3)) {
            allNotesOff();
        }

        switch(status) {
            //TODO check IO
            case MIDI_NOTEON:
                handleNoteOnEvent(event);
                break;
            case MIDI_NOTEOFF:
                handleNoteOffEvent(event);
                break;
            default:
                if (!(event->size > 4 && event->dataExt[0] == MIDI_SYSEX)) {
                    handleMidiThroughEvent(event);
                }
                break;
        }
    }
}

void Arpeggiator::handleMidiEventDisabledState(const MidiEvent *event, uint8_t status)
{
    if (!(event->size > 4 && event->dataExt[0] == MIDI_SYSEX)) {

        uint8_t midiNote = event->data[1];

        if (latchMode) {
            if (status == static_cast<uint8_t>(MIDI_NOTEOFF)) {
                voiceManager->freeVoice(midiNote);
                notesTracker.keyReleased();
            }
        }

        if ((midiNote == 0x7b) && (event->size == 3)) {
            allNotesOff();
        }

        //send MIDI message through
        // TODO validate this bit of code
        midiHandler.appendMidiThroughMessage(*event);
        first = true;
    }
}

void Arpeggiator::addEventToNoteOffTimer(ArpNoteEvent event)
{
    bool noteOffSlotFound = false;

    int i = 0;

    while (!noteOffSlotFound && (i < NUM_NOTE_OFF_SLOTS))
    {
        if (!arpNoteOffEvent[i].noteEvent.active) {
            arpNoteOffEvent[i].noteEvent.midiNote = event.midiNote;
            arpNoteOffEvent[i].noteEvent.channel = event.channel;
            arpNoteOffEvent[i].noteEvent.active = true;
            noteOffSlotFound = true;
        }
        i++;
    }

    if (!noteOffSlotFound)
    {
        sendAllNotesOffToOutput();
    }
}

void Arpeggiator::createNewArpOutEvent(ArpNoteEvent event, size_t currentFrame)
{
    struct MidiEvent midiEvent;

    midiEvent.frame = currentFrame;
    midiEvent.size = 3;
    midiEvent.data[0] = MIDI_NOTEON | event.channel;
    midiEvent.data[1] = event.midiNote;
    midiEvent.data[2] = event.velocity;

    midiHandler.appendMidiMessage(midiEvent);

    // Add this event to the timer for sending a note off later
    addEventToNoteOffTimer(event);
}

void Arpeggiator::resetArpPattern()
{
    int numActiveNotes = notesTracker.getNumActiveNotes();
    octavePattern[octaveMode]->reset();
    if (octaveMode == (int)ARP_DOWN) {
        octavePattern[octaveMode]->setStep(numActiveNotes - 1); //TODO maybe put this in reset()
    }

    arpPattern[arpMode]->reset();
    if (arpMode == (int)ARP_DOWN) {
        arpPattern[arpMode]->setStep(numActiveNotes - 1);
    }

    probabilityPattern.reset();

    resetPattern = false;

    currentStep = arpPattern[arpMode]->getStep();
}

void Arpeggiator::noteOffTimer(size_t currentFrame)
{
    for (size_t i = 0; i < (size_t)NUM_NOTE_OFF_SLOTS; i++) {
        if (arpNoteOffEvent[i].noteEvent.active) {
            arpNoteOffEvent[i].timer += 1;
            uint32_t noteDurationInSamples = static_cast<uint32_t>(clock.getClockCycleDuration() * noteLength);
            if (arpNoteOffEvent[i].timer > noteDurationInSamples) {
                struct MidiEvent midiEvent;
                midiEvent.frame = currentFrame;
                midiEvent.size = 3;
                midiEvent.data[0] = MIDI_NOTEOFF | arpNoteOffEvent[i].noteEvent.channel;
                midiEvent.data[1] = arpNoteOffEvent[i].noteEvent.midiNote;
                midiEvent.data[2] = 0;

                midiHandler.appendMidiMessage(midiEvent);

                arpNoteOffEvent[i].noteEvent.midiNote = 0;
                arpNoteOffEvent[i].noteEvent.channel = 0;
                arpNoteOffEvent[i].noteEvent.active = false;
                arpNoteOffEvent[i].timer = 0;

            }
        }
    }
}

void Arpeggiator::applyOctavePatternToEvent(ArpNoteEvent *event)
{
    uint8_t octave = octavePattern[octaveMode]->getStep() * 12;
    event->midiNote = event->midiNote + octave;

    // Clip value if needed, to prevent it going out of range
    event->midiNote = (event->midiNote > 127) ? 127 : event->midiNote;
}

void Arpeggiator::applyProbability(ArpNoteEvent *event)
{
    event->velocity = (event->velocity * probabilityPattern.getStep());
}

void Arpeggiator::applyHoldToEvent(ArpNoteEvent *event)
{
    if (tempoMultiplierEnabled) {
        if (!holdEventDefined) {
            noteToHold = event->midiNote;
            holdEventDefined = true;
        }

        if (hold) {
            event->midiNote = noteToHold;
        } else {
            holdEventDefined = false;
        }
    } else {
        holdEventDefined = false;
    }
}

void Arpeggiator::queNewEvent(unsigned int frame)
{
    ArpNoteEvent event = voiceManager->getEvent(currentStep);
    // Create a MIDI message out
    if (event.active) {
        applyOctavePatternToEvent(&event);
        applyHoldToEvent(&event);
        applyProbability(&event);

        createNewArpOutEvent(event, frame);
        firstNote = false;
    }
}

void Arpeggiator::goToNextStep()
{
    probabilityPattern.goToNextStep();
    octavePattern[octaveMode]->goToNextStep();
    arpPattern[arpMode]->goToNextStep();
    currentStep = arpPattern[arpMode]->getStep();
}

void Arpeggiator::handleTimeBasedEvents(uint32_t n_frames)
{

    for (unsigned int s = 0; s < n_frames; s++) {

        bool timeOut = (firstNoteTimer > (int)timeOutTime) ? false : true;

        static bool prevFirstNote = false;
        if (firstNote != prevFirstNote) {
            prevFirstNote = firstNote;
        }

        if (firstNote) {
            clock.closeGate(); //close gate to prevent opening before timeOut
            firstNoteTimer++;
        }

        if ((clock.getSyncMode() <= 1) && first) {
            clock.reset();
        }

        clock.tick();

        if ((clock.getGate() && !timeOut)) {

            if (arpEnabled) {

                if (resetPattern) {
                    resetArpPattern();
                }

                if (first) {
                    sendAllNotesOffToOutput();
                    first = false;
                }

                queNewEvent(s);

            }
            // Keep pattern running, even when disabled.
            // This makes syncing easier.
            // Only exception is when the 'hold' is set on the tempo multiplier
            goToNextStep();

            clock.closeGate();
        }
        //Run note off timer, that will fire a note-off event if note is active longer then set note lenght
        noteOffTimer(s);
    }
}

void Arpeggiator::processMidiInput(const MidiEvent* events, uint32_t eventCount)
{
    //Process incoming MIDI messages
    for (uint32_t i=0; i<eventCount; ++i) {

        uint8_t status = events[i].data[0] & 0xF0;

        if (arpEnabled) {
            handleMidiInputEvent(&events[i], status);
        } else {
            handleMidiEventDisabledState(&events[i], status);
        }
    }
}

void Arpeggiator::updateArpPattern()
{
    arpPattern[arpMode]->setPatternSize(notesTracker.getNumActiveNotes());
    setOctavePattern(getPatternSize(), octaveSpread);
}

void Arpeggiator::resetArpIfStartOfSequence()
{
    if (!arpEnabled && !latchMode) {
        reset();
    }

    if (!latchMode && previousLatch && notesTracker.getNumKeysPressed() <= 0) {
        reset();
    }
    if (latchMode != previousLatch) {
        previousLatch = latchMode;
    }
}

void Arpeggiator::resetArpIfPanic()
{
    if (panic) {
        sendAllNotesOffToOutput();
        reset();
        panic = false;
    }

}

void Arpeggiator::process(const MidiEvent* events, uint32_t eventCount, uint32_t n_frames)
{
    resetArpIfStartOfSequence();

    resetArpIfPanic();

    processMidiInput(events, eventCount);

    updateArpPattern();

    handleTimeBasedEvents(n_frames);

    midiHandler.mergeBuffers();
}
