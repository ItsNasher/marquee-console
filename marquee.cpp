// Compilation:
// clang++ -static -std=c++20 marquee.cpp -o marquee.exe
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <cmath>

using namespace std;

void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, coord);
}

void initText() {
    system("cls");
    cout << "*******************************************" << endl;
    cout << "*            Marquee Console              *" << endl;
    cout << "*******************************************" << endl;
}

// calculate diagonal movement
int calculateY(int x, int baseY, int amplitude) {
    int yOffset = abs((x % (amplitude * 2)) - amplitude);
    return baseY - (amplitude - yOffset);
}

int main() {
    string leader = "[]";
    string follower = "o";
    initText();
    
    int baseY = 12;
    int amplitude = 4;
    int delay_ms = 50;
    int distance = 3;    // How many columns behind the follower trails

    while (true) {
        for (int col = 0; col <= 50 + distance; ++col) {
            
            // calculate coordinates for both shapes
            int leaderX = col;
            int leaderY = calculateY(leaderX, baseY, amplitude);
            
            int followerX = col - distance;
            int followerY = calculateY(followerX, baseY, amplitude);

            // draw the leader and follower if they are within the console bounds
            if (leaderX <= 50) {
                setCursorPosition(leaderX, leaderY);
                cout << leader;
            }
            if (followerX >= 0 && followerX <= 50) {
                setCursorPosition(followerX, followerY);
                cout << follower;
            }
            
            // move cursor out of the way to prevent flickering
            setCursorPosition(0, baseY + 2);
            cout.flush();
            
            // wait to create the animation effect
            this_thread::sleep_for(chrono::milliseconds(delay_ms));
            
            // clear both shapes before moving to the next frame
            if (leaderX <= 50) {
                setCursorPosition(leaderX, leaderY);
                cout << string(leader.size(), ' ');
            }
            if (followerX >= 0 && followerX <= 50) {
                setCursorPosition(followerX, followerY);
                cout << string(follower.size(), ' ');
            }
        }
    }
    return 0;
}