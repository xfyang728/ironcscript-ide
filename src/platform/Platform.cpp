#include "Platform.h"
#include "K210Support.h"
#include <iostream>

Platform* PlatformFactory::createPlatform() {
    // 这里可以根据实际平台创建相应的平台实例
    // 暂时返回 K210 平台实例
    return new K210Support();
}
