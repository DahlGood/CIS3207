/*
    CREDIT TO: https://www.modernmetalproduction.com/simple-circular-buffer-in-c/
*/
#include "circular_buffer.h"

CBuffer::CBuffer(int size) : buff(20) {
    snatchIndex = 0;
    slideIndex = 0;
    currentSize = 0;
}

void CBuffer::slide(int value) {
    currentSize++;
    buff[slideIndex++] = value;
    if(slideIndex == buff.size()) {
        slideIndex = 0;
    }
}

int CBuffer::snatch() {
    currentSize--;
    int val = buff[snatchIndex++];
    if(snatchIndex == buff.size()) {
        snatchIndex = 0;
    }
    return val;
}

int CBuffer::peep(int index) {
    int val = buff[index];
    return val;
}

int CBuffer::size() {
    return currentSize;
}
