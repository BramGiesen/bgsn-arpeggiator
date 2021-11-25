#ifndef _H_VELOCITY_HANDLER_
#define _H_VELOCITY_HANDLER_

#include <cstdint>

#define MAX_NUM_STEPS 8

class VelocityHandler {
public:
    enum {
        STATIC = 0,
        PATTERN
    };
    VelocityHandler();
    ~VelocityHandler();

    void setSampleRate(float sampleRate);
    void setMode(int mode);
    void setNumSteps(int steps);
    void setVelocityPattern(int index, uint8_t value);
    int getMode() const;
    int getNumSteps() const;
    int getVelocityPattern(int index) const;
    void goToNextStep();
    uint8_t getVelocity();
    void process();

private:
    uint8_t velocity;
    float sampleRate;
    int step;
    int numSteps;
    int velocityMode;
    uint8_t velocityPattern[MAX_NUM_STEPS];
};

#endif // _H_VELOCITY_HANDLER_
