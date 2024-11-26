#pragma once
#include <cstdint>
#include <array>
#include <cassert>
#include <iostream>
#include <numbers>

void* __cdecl operator new[](size_t size, size_t alignment, size_t offset, const char* name, int flags, unsigned debugFlags, const char* file, int line);
void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line);