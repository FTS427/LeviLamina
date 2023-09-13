#pragma once
#include "liteloader/api/base/Global.h"
#include <string>

LLAPI std::string CalcMD5(const std::string& str);
LLAPI std::string CalcSHA1(const std::string& str);