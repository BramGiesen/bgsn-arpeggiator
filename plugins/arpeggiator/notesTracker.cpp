#include "notesTracker.hpp"

NotesTracker::NotesTracker() : numKeysPressed(0), numActiveNotes(0)
{
}

NotesTracker::~NotesTracker()
{
}

void NotesTracker::keyReleased()
{
    numKeysPressed = (numKeysPressed > 0) ? numKeysPressed - 1 : 0;
}

void NotesTracker::registerNewPressedKey()
{
    numKeysPressed++;
}

void NotesTracker::registerNewActiveNote()
{
    numActiveNotes++;
}

void NotesTracker::removeActiveNote()
{
    numActiveNotes = (numActiveNotes > 0) ? numActiveNotes - 1 : 0;
}

void NotesTracker::setNumActiveNotes(int numActiveNotes)
{
    this->numActiveNotes = numActiveNotes;

}

void NotesTracker::resetAll()
{
    numActiveNotes = 0;
    numKeysPressed = 0;
}

int NotesTracker::getNumKeysPressed()
{
    return numKeysPressed;
}

int NotesTracker::getNumActiveNotes()
{
    return numActiveNotes;
}
