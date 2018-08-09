//
// Created by abdu on 4/9/16.
//

#ifndef FILE_T_5_INDEXS_H
#define FILE_T_5_INDEXS_H

#include <string>

using namespace std;

class IndexS {
public:
    IndexS(string = "", int = 0);

    void setlastname(string);

    void setAcountNumbers(int);

    string getlastname() const ;

    string getAccountNumbers() const ;

private:
    string lastName;
    string AccountNumbers;
};

#endif //FILE_T_5_INDEXS_H
