#pragma once
#include <stdint.h>
#include <vector>
#include <map>
#define screenGrid

class screen{
public:
   screen();
    ~screen();
private:
    std::vector<uint8_t> pixelMap;
    std::vector<uint8_t> currentPixels;
