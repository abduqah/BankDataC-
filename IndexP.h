//
// Created by abdu on 4/9/16.
//

#ifndef FILE_T_5_INDEXP_H
#define FILE_T_5_INDEXP_H

#include <string>

using namespace std;

class IndexP {
public:
    IndexP(int = 0, int = 0);

    void setByte(int);

    void setAccountNumber(int);

    int getByte() const ;

    int getAccountNumber() const ;

private:
    int AccountNumber;
    int Byte_offset;
};

#endif //FILE_T_5_INDEXP_H
