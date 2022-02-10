#ifndef H_NOTES_TRACKER
#define H_NOTES_TRACKER

class NotesTracker {
public:
    NotesTracker(void);
    ~NotesTracker(void);
    void keyReleased(void);
    void registerNewPressedKey(void);
    void registerNewActiveNote(void);
    void setNumActiveNotes(int numActiveNotes);
    void removeActiveNote(void);
    void allKeysReleased(void);
    void resetActiveNotes(void);
    void resetAll(void);
    int getNumKeysPressed(void);
    int getNumActiveNotes(void);
private:
    int numKeysPressed;
    int numActiveNotes;
};
#endif // H_NOTES_TRACKER
