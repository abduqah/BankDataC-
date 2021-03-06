#ifndef CLIENTDATA_H
#define CLIENTDATA_H
#include <string>
using namespace std;

class ClientData {
public:
    // default ClientData constructor
    ClientData(int = 0,int = 0, string = "", string = "", double = 0.0);
    // accessor functions for accountNumber
    void setAccountNumber(int);
    int getAccountNumber() const;
    // accessor functions for lastName
    void setLastName(string);
    string getLastName() const;
    // accessor functions for firstName
    void setFirstName(string);
    string getFirstName() const;
    // accessor functions for balance
    void setBalance(double);
    double getBalance() const;
    void setBrach_ID(int);
    int getBrach_ID() const;
private:
    int accountNumber;
    char lastName[15];
    char firstName[10];
    double balance;
    int BranchID;
}; // end class ClientData
#endif