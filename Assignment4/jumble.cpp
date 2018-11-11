///*
// * jumble.cpp
// *
// *  Created on: Nov 4, 2018
// *      Author: Chris Maltais
// */
//
//
#include "jumble.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

BadJumbleException::BadJumbleException(const string& m) : message(m) {}
string& BadJumbleException::what() { return message; }

JumblePuzzle::JumblePuzzle(const string& hiddenWord, const string& difficulty) {
    bool isValidDifficulty = difficulty == "easy" || difficulty == "medium" || difficulty == "hard" || difficulty == "extreme";
    bool isValidHiddenWord = hiddenWord.length() > 0;
    if (isValidDifficulty && isValidHiddenWord) {
        this->hiddenWord = hiddenWord;
        this->difficulty = difficulty;
        // Get size of puzzle to be created
        this->puzzleSize = setSize();
        // Create puzzle of length size with random letters
        this->puzzle = initializePuzzle();
    } else if (!isValidDifficulty) {
        throw BadJumbleException("\nInvalid difficulty - please enter 'easy', 'medium', 'hard' or extreme (case sensitive)\n");
    } else if (!isValidHiddenWord) {
        throw BadJumbleException("\nInvalid word to hide - please enter another word.\n");
    }
}

const int& JumblePuzzle::setSize() {
    if (this->difficulty == "easy") {
        return this->hiddenWord.length() * 2;
    } else if (this->difficulty == "medium") {
        return this->hiddenWord.length() * 3;
    } else if (this->difficulty == "hard" || this->difficulty == "extreme") {
        return this->hiddenWord.length() * 4;
    } 
    return -1;
}

charArrayPtr* JumblePuzzle::initializePuzzle() {
    charArrayPtr* completePuzzle = initializeRandomPuzzle(this->puzzleSize);
    bool wordPlaced = false;
    int row;
    int col;
    int hiddenWordIdx;
    while (!wordPlaced) {
        this->direction = this->directionArr[rand() % 4];
        this->rowPos = rand() % (this->puzzleSize);
        this->colPos = rand() % (this->puzzleSize);
        row = this->rowPos;
        col = this->colPos;
        hiddenWordIdx = 0;
        if (direction == 'n') { // randomPuzzle[row--][col]
            while (row >= 0 && hiddenWordIdx < this->hiddenWord.length()) {
                completePuzzle[row--][col] = this->hiddenWord[hiddenWordIdx++];
            }
            if (hiddenWordIdx == this->hiddenWord.length()) {
                wordPlaced = true;
            }
        } else if (direction == 'e') {  // randomPuzzle[row][col++]
            while (col < this->puzzleSize && hiddenWordIdx < this->hiddenWord.length()) {
                completePuzzle[row][col++] = this->hiddenWord[hiddenWordIdx++];
            }
            if (hiddenWordIdx == this->hiddenWord.length()) {
                wordPlaced = true;
            }
        } else if (direction == 's') { // randomPuzzle[row++][col]
            while (row < this->puzzleSize && hiddenWordIdx < this->hiddenWord.length()) {
                completePuzzle[row++][col] = this->hiddenWord[hiddenWordIdx++];
            }
            if (hiddenWordIdx == this->hiddenWord.length()) {
                wordPlaced = true;
            }
        } else if (direction == 'w') { // randomPuzzle[row][col--]
            while (col >= 0 && hiddenWordIdx < this->hiddenWord.length()) {
                completePuzzle[row][col--] = this->hiddenWord[hiddenWordIdx++];
            }
            if (hiddenWordIdx == this->hiddenWord.length()) {
                wordPlaced = true;
            }
        }
    }
    return completePuzzle;
}

charArrayPtr* JumblePuzzle::initializeRandomPuzzle(const int& size) {
    charArrayPtr* randomPuzzle = new charArrayPtr[size];
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        randomPuzzle[i] = new char[size];
        for (int j = 0; j < size; j++) {
            if (this->difficulty == "extreme") {
                randomPuzzle[i][j] = this->hiddenWord[rand() % this->hiddenWord.length()];
            } else{
                randomPuzzle[i][j] = 'a' + rand() % 26; // Select random letter using ASCII
            }
        }
    }
    return randomPuzzle;
}

// Destructor
JumblePuzzle::~JumblePuzzle() {
    for (int i = 0; i < this->puzzleSize; i++) {
        delete[] puzzle[i];
    }
    delete[] puzzle;
    puzzle = nullptr;
}

// Copy constructor
JumblePuzzle::JumblePuzzle(const JumblePuzzle& jp) {
    this->puzzleSize = jp.getSize();
    this->rowPos = jp.getRowPos();
    this->colPos = jp.getColPos();
    this->direction = jp.getDirection();

    charArrayPtr* existingPuzzle = jp.getJumble();
    charArrayPtr* newPuzzle = new charArrayPtr[this->puzzleSize];

    for (int i = 0; i < this->puzzleSize; i++) {
        newPuzzle[i] = new char[this->puzzleSize];
        for (int j = 0; j < this->puzzleSize; j++) {
            newPuzzle[i][j] = existingPuzzle[i][j];
        }
    }
    this->puzzle = newPuzzle;
    
}

// Assignment Operator
JumblePuzzle& JumblePuzzle::operator=(const JumblePuzzle& rhs) {
    if (this != &rhs) {
        for (int i = 0; i < puzzleSize; i++) {
            delete[] puzzle[i];
        }
        delete[] puzzle;
        puzzleSize = rhs.getSize();
        rowPos = rhs.getRowPos();
        colPos = rhs.getColPos();
        direction = rhs.getDirection();

        charArrayPtr* existingPuzzle = rhs.getJumble();
        puzzle = new charArrayPtr[puzzleSize];

        for (int i = 0; i < puzzleSize; i++) {
            puzzle[i] = new char[puzzleSize];
            for (int j = 0; j < puzzleSize; j++) {
                puzzle[i][j] = existingPuzzle[i][j];
            }
        }
    }
    return *this;
}

int JumblePuzzle::getSize() const {
    return this->puzzleSize;
}

int JumblePuzzle::getRowPos() const {
    return this->rowPos;
}

int JumblePuzzle::getColPos() const {
    return this->colPos;
}

char JumblePuzzle::getDirection() const {
    return this->direction;
}

charArrayPtr* JumblePuzzle::getJumble() const {
    return this->puzzle;
}

bool JumblePuzzle::checkExtreme(const int& guessRow, const int& guessCol, const char& guessDir) {
    if (guessDir == 'n') {
        for (int i = 0; i < this->hiddenWord.length(); i++) {
            if (puzzle[guessRow - i][guessCol] != hiddenWord[i]) {
                return false;
            }
        }
    } else if (guessDir == 'e') {
        for (int i = 0; i < this->hiddenWord.length(); i++) {
            if (puzzle[guessRow][guessCol + i] != hiddenWord[i]) {
                return false;
            }
        }
    } else if (guessDir == 's') {
        for (int i = 0; i < this->hiddenWord.length(); i++) {
            if (puzzle[guessRow + i][guessCol] != hiddenWord[i]) {
                return false;
            }
        }
    } else if (guessDir == 'w') {
        for (int i = 0; i < this->hiddenWord.length(); i++) {
            if (puzzle[guessRow][guessCol - i] != hiddenWord[i]) {
                return false;
            }
        }
    }
    return true;
}

