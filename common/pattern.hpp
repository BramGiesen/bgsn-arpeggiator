#ifndef H_PATTERN
#define H_PATTERN

#include <stdlib.h>
#include <time.h>

class Pattern {

    enum {
        ARP_UP = 0,
        ARP_DOWN
    };

public:
    Pattern(void);
    virtual ~Pattern(void);
    void setPatternSize(int size);
    void setStep(int step);
    void setCycleRange(int range);
    int getSize(void);
    int getStepSize(void);
    int getStep(void);
    int getDirection(void);
    virtual void setDirection(int direction) = 0;
    virtual void reset(void) = 0;
    virtual void goToNextStep(void) = 0;
protected:
    int size;
    int step;
    int direction;
    int range;
};

class PatternUp : public Pattern {
public:
    PatternUp(void);
    ~PatternUp(void);
    void setDirection(int direction) override;
    void reset(void) override;
    void goToNextStep(void) override;
};

class PatternDown : public Pattern {
public:
    PatternDown(void);
    ~PatternDown(void);
    void setDirection(int direction) override;
    void reset(void) override;
    void goToNextStep(void) override;
};

class PatternUpDown : public Pattern {
public:
    PatternUpDown(void);
    ~PatternUpDown(void);
    void setDirection(int direction) override;
    void reset(void) override;
    void goToNextStep(void) override;
};

class PatternUpDownAlt : public Pattern {
public:
    PatternUpDownAlt(void);
    ~PatternUpDownAlt(void);
    void setDirection(int direction) override;
    void reset(void) override;
    void goToNextStep(void) override;
private:
    bool checked;
    bool skip;
};

class PatternRandom : public Pattern {
public:
    PatternRandom(void);
    ~PatternRandom(void);
    void setDirection(int direction) override;
    void reset(void) override;
    void goToNextStep(void) override;
};

class PatternCycle : public Pattern {
public:
    PatternCycle(void);
    ~PatternCycle(void);
    void setDirection(int direction) override;
    void reset(void) override;
    void goToNextStep(void) override;
private:
    int tempStep;
};

#endif // H_PATTERN
