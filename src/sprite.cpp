#include <iostream>
#include <vector>

using namespace std;

// Function for '0'
vector<uint8_t> getSprite0() {
    return {0xF0, 0x90, 0x90, 0x90, 0xF0};
}

// Function for '1'
vector<uint8_t> getSprite1() {
    return {0x20, 0x60, 0x20, 0x20, 0x70};
}

// Function for '2'
vector<uint8_t> getSprite2() {
    return {0xF0, 0x10, 0xF0, 0x80, 0xF0};
}

// Function for '3'
vector<uint8_t> getSprite3() {
    return {0xF0, 0x10, 0xF0, 0x10, 0xF0};
}

// Function for '4'
vector<uint8_t> getSprite4() {
    return {0x90, 0x90, 0xF0, 0x10, 0x10};
}

// Function for '5'
vector<uint8_t> getSprite5() {
    return {0xF0, 0x80, 0xF0, 0x10, 0xF0};
}

// Function for '6'
vector<uint8_t> getSprite6() {
    return {0xF0, 0x80, 0xF0, 0x90, 0xF0};
}

// Function for '7'
vector<uint8_t> getSprite7() {
    return {0xF0, 0x10, 0x20, 0x40, 0x40};
}

// Function for '8'
vector<uint8_t> getSprite8() {
    return {0xF0, 0x90, 0xF0, 0x90, 0xF0};
}

// Function for '9'
vector<uint8_t> getSprite9() {
    return {0xF0, 0x90, 0xF0, 0x10, 0xF0};
}

// Function for 'A'
vector<uint8_t> getSpriteA() {
    return {0xF0, 0x90, 0xF0, 0x90, 0x90};
}

// Function for 'B'
vector<uint8_t> getSpriteB() {
    return {0xE0, 0x90, 0xE0, 0x90, 0xE0};
}

// Function for 'C'
vector<uint8_t> getSpriteC() {
    return {0xF0, 0x80, 0x80, 0x80, 0xF0};
}

// Function for 'D'
vector<uint8_t> getSpriteD() {
    return {0xE0, 0x90, 0x90, 0x90, 0xE0};
}

// Function for 'E'
vector<uint8_t> getSpriteE() {
    return {0xF0, 0x80, 0xF0, 0x80, 0xF0};
}

// Function for 'F'
vector<uint8_t> getSpriteF() {
    return {0xF0, 0x80, 0xF0, 0x80, 0x80};
}

// Function to display the sprite
void printSprite(const vector<uint8_t>& sprite) {
    for (auto row : sprite) {
        for (int i = 7; i >= 0; --i) {
            cout << ((row >> i) & 1);
        }
        cout << endl;
    }
}

int main() {
    vector<uint8_t> sprite = getSprite0(); // Example for '0'
    printSprite(sprite);
    return 0;
}
