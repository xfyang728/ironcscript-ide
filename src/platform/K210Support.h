#pragma once

#include "Platform.h"
#include <string>

class K210Support : public Platform {
public:
    K210Support();
    ~K210Support();

    std::string getPlatformName() const override;
    bool isSupported() const override;

    bool compileScript(const std::string& code, const std::string& outputPath);
    bool transferToK210(const std::string& filePath);
    bool executeOnK210();
    std::string getExecutionResult();

private:
    std::string executionResult;
};