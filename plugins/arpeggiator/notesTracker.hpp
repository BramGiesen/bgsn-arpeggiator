#ifndef _H_NOTES_TRACKER_
#define _H_NOTES_TRACKER_

class NotesTracker {
public:
    NotesTracker();
    ~NotesTracker();
    void keyReleased();
    void registerNewPressedKey();
    void registerNewActiveNote();
    void setNumActiveNotes(int numActiveNotes);
    void removeActiveNote();
    void allKeysReleased();
    void resetActiveNotes();
    void resetAll();
    int getNumKeysPressed();
    int getNumActiveNotes();
private:
    int numKeysPressed;
    int numActiveNotes;
};
#endif //_H_NOTES_TRACKER_
