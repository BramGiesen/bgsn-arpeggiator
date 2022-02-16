#ifndef H_PLUGIN_ARPEGGIATOR
#define H_PLUGIN_ARPEGGIATOR

#include "DistrhoPlugin.hpp"
#include "arpeggiator.hpp"
#include "../../common/clock.hpp"
#include "../../common/pattern.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class PluginArpeggiator : public Plugin {
public:
    enum Parameters {
        paramSyncMode = 0,
        paramBpm,
        paramSwing,
        paramDivision,
        paramTempoMultiplier,
        paramMultiplierEnabled,
        paramHoldEnabled,
        paramVelocity,
        paramNoteLength,
        paramOctaveSpread,
        paramArpMode,
        paramOctaveMode,
        paramLatch,
        paramProbabilityPatternSize,
        paramProbabilityPattern1,
        paramProbabilityPattern2,
        paramProbabilityPattern3,
        paramProbabilityPattern4,
        paramProbabilityPattern5,
        paramProbabilityPattern6,
        paramProbabilityPattern7,
        paramProbabilityPattern8,
        paramPanic,
        paramEnabled,
        paramCount
    };

    PluginArpeggiator(void);

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel(void) const noexcept override {
        return "Arpeggiator";
    }

    const char* getDescription(void) const override {
        return "A MIDI arpeggiator";
    }

    const char* getMaker(void) const noexcept override {
        return "BGSN";
    }

    const char* getHomePage(void) const override {
        return "";
    }

    const char* getLicense(void) const noexcept override {
        return "https://spdx.org/licenses/GPL-2.0-or-later";
    }

    uint32_t getVersion(void) const noexcept override {
        return d_version(1, 1, 2);
    }

    int64_t getUniqueId(void) const noexcept override {
        return d_cconst('M', 'O', 'A', 'P');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void setParameterValue(uint32_t index, float value) override;

    // -------------------------------------------------------------------
    // Optional

    // Optional callback to inform the plugin about a sample rate change.
    void sampleRateChanged(double newSampleRate) override;

    // -------------------------------------------------------------------
    // Process

    void activate(void) override;

    void run(const float**, float**, uint32_t,
            const MidiEvent* midiEvents, uint32_t midiEventCount) override;


    // -------------------------------------------------------------------

private:
    Arpeggiator arpeggiator;
    int syncMode;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginArpeggiator)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  //_H_PLUGIN_ARPEGGIATOR_
