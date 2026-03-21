#pragma once

#include <string>
#include <vector>

class Terminal {
public:
    Terminal();
    ~Terminal();

    void render();
    void appendOutput(const std::string& output);

private:
    std::vector<std::string> outputLines;
    std::string inputBuffer;

    void processInput();
};