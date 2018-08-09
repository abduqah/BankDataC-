//
// Created by abdu on 4/9/16.
//
#include <string>
#include "IndexP.h"

IndexP::IndexP(int n, int b) {
    setAccountNumber(n);
    setByte(b);
}

void IndexP::setAccountNumber(int n) {
    AccountNumber = n;
}

void IndexP::setByte(int b) {
    Byte_offset = b;
}

int IndexP::getAccountNumber() const {
    return AccountNumber;
}

int IndexP::getByte() const {
    return Byte_offset;
}