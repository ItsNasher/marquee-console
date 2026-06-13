// Compilation:
// clang++ -static -std=c++20 marquee.cpp threadworker.cpp -o marquee.exe
#include <iostream>
#include <thread>
#include "threadworker.h"

using namespace std;

int main() {
    ThreadWorker::showCursor();
    ThreadWorker::drawStaticUI();
    
    // launch 2 threads
    thread marqueeThread(ThreadWorker::run_marquee);
    thread inputThread(ThreadWorker::run_input);

    inputThread.join();
    marqueeThread.join();

    system("cls");
    return 0;
}