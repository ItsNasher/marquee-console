#include "threadworker.h"
#include <iostream>
#include <windows.h>
#include <cmath>
#include <conio.h>

using namespace std;

void ThreadWorker::setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, coord);
}

void ThreadWorker::printPadded(const string& text, int width) {
    cout << text << string(max(0, width - (int)text.length()), ' ');
}

int ThreadWorker::calculateY(int x, int baseY, int amplitude) {
    int yOffset = abs((x % (amplitude * 2)) - amplitude);
    return baseY - (amplitude - yOffset);
}

void ThreadWorker::showCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void ThreadWorker::restoreCursor() {
    int cursorX = 37 + currentInput.length();
    setCursorPosition(cursorX, INPUT_ROW);
}

void ThreadWorker::drawStaticUI() {
    system("cls");
    cout << "*******************************************" << endl;
    cout << "*            Marquee Console              *" << endl;
    cout << "*******************************************" << endl;
}

void ThreadWorker::renderInputArea() {
    console_semaphore.acquire();

    setCursorPosition(0, INPUT_ROW);
    string inputMsg = "Enter a command for MARQUEE_CONSOLE: " + currentInput;
    printPadded(inputMsg);
    
    setCursorPosition(0, OUTPUT_ROW);
    string outputMsg = "Command processed in MARQUEE_CONSOLE: " + lastCommand;
    printPadded(outputMsg);

    restoreCursor();
    cout.flush();
    console_semaphore.release();
}

void ThreadWorker::run_marquee() {
    string leader = "[]";
    string follower = "o";
    int baseY = 12;
    int amplitude = 4;
    int delay_ms = 50;
    int distance = 3;

    while (isRunning) {
        for (int col = 0; col <= 60 + distance && isRunning; ++col) {
            int leaderX = col;
            int leaderY = calculateY(leaderX, baseY, amplitude);
            int followerX = col - distance;
            int followerY = calculateY(followerX, baseY, amplitude);

            console_semaphore.acquire();
            if (leaderX <= 60) {
                setCursorPosition(leaderX, leaderY);
                cout << leader;
            }
            if (followerX >= 0 && followerX <= 60) {
                setCursorPosition(followerX, followerY);
                cout << follower;
            }
            restoreCursor();
            cout.flush();
            console_semaphore.release();

            this_thread::sleep_for(chrono::milliseconds(delay_ms));

            console_semaphore.acquire();
            if (leaderX <= 60) {
                setCursorPosition(leaderX, leaderY);
                cout << string(leader.size(), ' ');
            }
            if (followerX >= 0 && followerX <= 60) {
                setCursorPosition(followerX, followerY);
                cout << string(follower.size(), ' ');
            }
            restoreCursor();
            cout.flush();
            console_semaphore.release();
        }
    }
}

void ThreadWorker::run_input() {
    renderInputArea();

    while (isRunning) {
        if (_kbhit()) {
            char ch = _getch();
            
            if (ch == 27) { // ESC key
                isRunning = false;
                break;
            } else if (ch == '\r') { // Enter key
                if (currentInput == "exit") {
                    isRunning = false;
                    break;
                }
                lastCommand = currentInput;
                currentInput = "";
                renderInputArea();
            } else if (ch == '\b') { // Backspace key
                if (!currentInput.empty()) {
                    currentInput.pop_back();
                    renderInputArea();
                }
            } else if (ch >= 32 && ch <= 126) {
                currentInput += ch;
                renderInputArea();
            }
        }
        this_thread::sleep_for(chrono::milliseconds(10)); 
    }
}