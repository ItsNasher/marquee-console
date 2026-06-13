#pragma once

#include <thread>
#include <semaphore>
#include <string>
#include <atomic>

class ThreadWorker {
private:
    inline static const int MAX_ACCESS = 1;             
    inline static std::counting_semaphore<1> console_semaphore{MAX_ACCESS};  

    inline static std::atomic<bool> isRunning{true};
    inline static std::string currentInput = "";
    inline static std::string lastCommand = "";

    inline static const int OUTPUT_ROW = 22;
    inline static const int INPUT_ROW = 23;

    static void setCursorPosition(int x, int y);
    static void printPadded(const std::string& text, int width = 100);
    static int calculateY(int x, int baseY, int amplitude);
    static void renderInputArea();
    static void restoreCursor();

public:
    static void showCursor();
    static void drawStaticUI();

    static void run_marquee();
    static void run_input();
};