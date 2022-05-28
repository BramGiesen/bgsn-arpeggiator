#include "velocityHandler.hpp"

VelocityHandler::VelocityHandler() :
    velocity(0),
    step(0),
    numSteps(1),
    velocityMode(0)
{
    for (unsigned int i = 0; i < (unsigned int)MAX_NUM_STEPS; i++) {
        velocityPattern[i] = 0;
    }
}

VelocityHandler::~VelocityHandler()
{
}

void VelocityHandler::setMode(int velocityMode)
{
    this->velocityMode = velocityMode;
}

void VelocityHandler::setNumSteps(int numSteps)
{
    if ((numSteps > 0) && (numSteps < MAX_NUM_STEPS)) {
        this->numSteps = numSteps;
    }
}

void VelocityHandler::setVelocityPattern(int index, uint8_t value)
{
    velocityPattern[index] = value;
}

int VelocityHandler::getVelocityPattern(int index) const
{
    return velocityPattern[index];
}

void VelocityHandler::goToNextStep()
{
    step = (step + 1) % numSteps;
}

int VelocityHandler::getMode() const
{
    return velocityMode;
}

int VelocityHandler::getNumSteps() const
{
    return numSteps;
}

uint8_t VelocityHandler::getVelocity()
{
    return velocity;
}

void VelocityHandler::process()
{
    switch (velocityMode)
    {
        case STATIC:
            velocity = 100;
            break;
        case PATTERN:
            velocity = velocityPattern[step];
            break;
        default:
            break;
    }
}
