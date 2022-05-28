#ifndef H_VELOCITY_HANDLER
#define H_VELOCITY_HANDLER

#include <cstdint>

#define MAX_NUM_STEPS 8

class VelocityHandler {
public:
    enum {
        STATIC = 0,
        PATTERN
    };
    VelocityHandler(void);
    ~VelocityHandler(void);

    void setMode(int mode);
    void setNumSteps(int steps);
    void setVelocityPattern(int index, uint8_t value);
    int getMode(void) const;
    int getNumSteps(void) const;
    int getVelocityPattern(int index) const;
    void goToNextStep(void);
    uint8_t getVelocity(void);
    void process(void);

private:
    uint8_t velocity;
    int step;
    int numSteps;
    int velocityMode;
    uint8_t velocityPattern[MAX_NUM_STEPS];
};

#endif // H_VELOCITY_HANDLER
