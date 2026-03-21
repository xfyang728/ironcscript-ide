#pragma once

#include <string>

class Platform {
public:
    virtual ~Platform() = default;

    virtual std::string getPlatformName() const = 0;
    virtual bool isSupported() const = 0;
};

class PlatformFactory {
public:
    static Platform* createPlatform();
};