#pragma once

#include <string>

class Engine {
public:
    Engine();
    ~Engine();

    std::string executeScript(const std::string& code);
    bool compileForK210(const std::string& code, const std::string& outputPath);

private:
    // IronCScript 相关的成员变量和 함수
};