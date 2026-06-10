// Compilation:
// clang++ -static -std=c++20 marquee.cpp threadworker.cpp -o marquee.exe
#include <iostream>
#include <thread>
#include "threadworker.h"

using namespace std;

int main() {
    // 1. Initialize the terminal UI
    ThreadWorker::showCursor();
    ThreadWorker::drawStaticUI();
    
    // 2. Launch the threads
    thread marqueeThread(ThreadWorker::run_marquee);
    thread inputThread(ThreadWorker::run_input);

    // 3. Wait for the user to press ESC
    inputThread.join();
    marqueeThread.join();

    // 4. Cleanup
    system("cls");
    return 0;
}