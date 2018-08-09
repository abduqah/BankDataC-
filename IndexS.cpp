//
// Created by abdu on 4/9/16.
//
#include <string>
#include "IndexS.h"

IndexS::IndexS(string l, int a) {
    setlastname(l);
    setAcountNumbers(a);
}

void IndexS::setAcountNumbers(int A) {
    AccountNumbers.append(A + ", ");
}

void IndexS::setlastname(string l) {
    lastName = l;
}

string IndexS::getAccountNumbers() const {
    return AccountNumbers;
}

string IndexS::getlastname() const {
    return lastName;
}