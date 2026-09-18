#pragma once
// ponytail: build-only shim so globalFuncs.cpp compiles on Linux; never used on Windows
#include <chrono>
#include <thread>
typedef unsigned long DWORD;
inline void Sleep(DWORD ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }
