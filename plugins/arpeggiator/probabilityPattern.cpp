#include "probabilityPattern.hpp"

ProbabilityPattern::ProbabilityPattern() :
    randomValue(0),
    step(0),
    numSteps(1),
    playNote(true)
{
    srand (static_cast <unsigned> (time(0)));
    randomValue = (LO + rand()) /(RAND_MAX/(HI-LO));

    for (unsigned i = 0; i < MAX_NUM_STEPS; i++) {
        probabilityPattern[i] = 100;
    }
}

ProbabilityPattern::~ProbabilityPattern()
{
}


void ProbabilityPattern::reset()
{
    step = 0;
}

void ProbabilityPattern::setNumSteps(int numSteps)
{
    if (numSteps > 0 && numSteps <= MAX_NUM_STEPS) {
        this->numSteps = numSteps;
    }
}

void ProbabilityPattern::setProbability(int index, int value)
{
    probabilityPattern[index] = value;
}

int ProbabilityPattern::getProbability(int index) const
{
    return probabilityPattern[index];
}

int ProbabilityPattern::getNumSteps() const
{
    return numSteps;
}

void ProbabilityPattern::goToNextStep()
{
    step = (step + 1) % numSteps;

    randomValue = (LO + rand()) /(RAND_MAX/(HI-LO));
}

bool ProbabilityPattern::getStep()
{
    if (randomValue < probabilityPattern[step]) {
        playNote = true;
    } else {
        playNote = false;
    }
    return playNote;
}
