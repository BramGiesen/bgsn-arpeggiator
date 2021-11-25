#ifndef _H_PROBABILITY_
#define _H_PROBABILITY_

#include <cstdlib>
#include <ctime>

#define MAX_NUM_STEPS 8

#define LO 0
#define HI 100

class ProbabilityPattern {
public:
    enum {
        STATIC = 0,
        PATTERN
    };
    ProbabilityPattern();
    ~ProbabilityPattern();

    void reset();
    void setNumSteps(int steps);
    void setProbability(int index, int value);
    int getNumSteps() const;
    int getProbability(int index) const;
    void goToNextStep();
    bool getStep();

private:
    int randomValue;
    int step;
    int numSteps;
    int probabilityPattern[MAX_NUM_STEPS];
    bool playNote;
};

#endif // _H_PROBABILITY_
