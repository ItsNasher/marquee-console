// Compilation:
// clang++ -static -std=c++20 marquee.cpp -o marquee.exe
#include <float.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;

// Set cursor position using Windows API
void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, coord);
}

void initText(){
    system("cls");
    cout << "*******************************************" << endl;
    cout << "*       This is a marquee console         *" << endl;
    cout << "*******************************************" << endl;
}

int main() {
    string message = "message";
    initText();
    // Row position of message
    int row = 5;

    while (true) {
        // Move cursor from left to right across 50 columns
        for (int col = 0; col <= 50; ++col) {
            setCursorPosition(col, row);
            
            cout << message;
            
            // Move cursor to a different location
            setCursorPosition(1, 10);
            
            // Flush the output to ensure the character appears immediately
            cout.flush();
            
            // Wait for 100 milliseconds to create animation effect
            this_thread::sleep_for(chrono::milliseconds(100));
            
            // Clear the character before moving to the next position
            setCursorPosition(col, row);
            cout << string(message.size(), ' ');
        }
    }
    return 0;
}