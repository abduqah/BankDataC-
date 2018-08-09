#include <iostream>
#include <fstream>
#include <iomanip>
#include "ClientData.h"
#include "IndexP.h"
#include "IndexS.h"

using namespace std;

int enterChoice();

void createTextFile(fstream &);

void updateRecord(fstream &);

void newRecord(fstream &);

void deleteRecord(fstream &);

void outputLine(ostream &, const ClientData &);

void outputLine(ostream &, const IndexP &);

void outputLine(ostream &, const IndexS &);

int getAccount(const char *const);

void Backup(fstream &);

void Restore(fstream &);

void Create_Index_Files(fstream &);

void Create_text_files_for_Index_Files(fstream &, fstream &);

enum Choices {
    PRINT = 1, UPDATE, NEW, DELETE, BACKUP, RESTOR, INDEX, INDEXTXT, END
};

int main() {
    // open file for reading and writing
    fstream inOutCredit("/home/abdu/ClionProjects/File_T_5/credit.dat", ios::in | ios::out | ios::binary);
    fstream indexP("/home/abdu/ClionProjects/File_T_5/primary_index.dat", ios::in | ios::out | ios::binary);
    fstream indexS("/home/abdu/ClionProjects/File_T_5/secondary_index.dat", ios::in | ios::out | ios::binary);
    // exit program if fstream cannot open file
    if (!inOutCredit) {
        inOutCredit.open("/home/abdu/ClionProjects/File_T_5/credit.dat", ios::in | ios::out | ios::binary | ios::trunc);
        ClientData client;
        for (int i = 1; i <= 100; ++i) {
            inOutCredit.write(reinterpret_cast<const char *> (&client),
                              sizeof(ClientData));
        }
    } // end if
    if (!indexP)
        indexP.open("/home/abdu/ClionProjects/File_T_5/primary_index.dat",
                    ios::in | ios::out | ios::binary | ios::trunc);
    if (!indexS)
        indexP.open("/home/abdu/ClionProjects/File_T_5/secondary_index.dat",
                    ios::in | ios::out | ios::binary | ios::trunc);
    int choice; // store user choice
    // enable user to specify action
    while ((choice = enterChoice()) != END) {
        switch (choice) {
            case PRINT: // create text file from record file
                createTextFile(inOutCredit);
                break;
            case UPDATE: // update record
                updateRecord(inOutCredit);
                break;
            case DELETE: // delete existing record
                deleteRecord(inOutCredit);
                break;
            case NEW: // create record
                newRecord(inOutCredit);
                break;
            case BACKUP:
                Backup(inOutCredit);
                break;
            case RESTOR:
                Restore(inOutCredit);
                break;
            case INDEX:
                Create_Index_Files(inOutCredit);
                break;
            case INDEXTXT:
                Create_text_files_for_Index_Files(indexP, indexS);
                break;
            default: // display error if user does not select valid choice
                cerr << "Incorrect choice" << endl;
                break;
        } // end switch
        inOutCredit.clear(); // reset end-of-file indicator
        indexP.clear();
        indexS.clear();
    } // end while
} // end main
// enable user to input menu choice

int enterChoice() {
    // display available options
    cout << "\nEnter your choice" << endl
    << "1 - store a formatted text file of accounts" << endl
    << "    called \"print.txt\" for printing" << endl
    << "2 - update an account" << endl
    << "3 - add a new account" << endl
    << "4 - delete an account" << endl
    << "5 - create backup" << endl
    << "6 - restor data" << endl
    << "7 - create \"index.dat\"" << endl
    << "8 - create \"index.txt\"" << endl
    << "9 - end program\n? ";
    int menuChoice;
    cin >> menuChoice; // input menu selection from user
    return menuChoice;
} // end function enterChoice
// create formatted text file for printing

void createTextFile(fstream &readFromFile) {
    // create text file
    ofstream outPrintFile("/home/abdu/ClionProjects/File_T_5/print.txt", ios::out);
    // exit program if ofstream cannot create file
    if (!outPrintFile) {
        cerr << "File could not be created." << endl;
        exit(1);
    } // end if
    outPrintFile << left << setw(10) << "Account" << setw(13) << "Branch ID" << setw(16)
    << "Last Name" << setw(11) << "First Name" << right
    << setw(10) << "Balance" << endl;
    // set file-position pointer to beginning of readFromFile
    readFromFile.seekg(0);
    // read first record from record file
    ClientData client;
    readFromFile.read(reinterpret_cast<char *> (&client),
                      sizeof(ClientData));
    // copy all records from record file into text file
    while (!readFromFile.eof()) {
        // write single record to text file
        if (client.getAccountNumber() != 0) // skip empty records
            outputLine(outPrintFile, client);
        // read next record from record file
        readFromFile.read(reinterpret_cast<char *> (&client),
                          sizeof(ClientData));
    } // end while
} // end function createTextFile
// update balance in record

void updateRecord(fstream &updateFile) {
    // obtain number of account to update
    int accountNumber = getAccount("Enter account to update");
    // move file-position pointer to correct record in file
    updateFile.seekg((accountNumber - 1) * sizeof(ClientData));
    // read first record from file
    ClientData client;
    updateFile.read(reinterpret_cast<char *> (&client),
                    sizeof(ClientData));
    // update record
    if (client.getAccountNumber() != 0) {
        outputLine(cout, client); // display the record
        // request user to specify transaction
        cout << "\nEnter charge (+) or payment (-): ";
        double transaction; // charge or payment
        cin >> transaction;
        // update record balance
        double oldBalance = client.getBalance();
        client.setBalance(oldBalance + transaction);
        outputLine(cout, client); // display the record
        // move file-position pointer to correct record in file
        updateFile.seekp((accountNumber - 1) * sizeof(ClientData));
        // write updated record over old record in file
        updateFile.write(reinterpret_cast<const char *> (&client),
                         sizeof(ClientData));
    }// end if
    else // display error if account does not exist
        cerr << "Account #" << accountNumber
        << " has no information." << endl;
} // end function updateRecord
// create and insert record

void newRecord(fstream &insertInFile) {
    // obtain number of account to create
    int accountNumber = getAccount("Enter new account number");
    // move file-position pointer to correct record in file
    insertInFile.seekg((accountNumber - 1) * sizeof(ClientData));
    // read record from file
    ClientData client;
    insertInFile.read(reinterpret_cast<char *> (&client),
                      sizeof(ClientData));
    // create record, if record does not previously exist
    if (client.getAccountNumber() == 0) {
        string lastName;
        string firstName;
        int brachid;
        double balance;
        // user enters last name, first name and balance
        cout << "Enter Branch ID, lastname, firstname, balance\n? ";
        cin >> setw(13) >> brachid;
        cin >> setw(15) >> lastName;
        cin >> setw(10) >> firstName;
        cin >> balance;
        // use values to populate account values
        client.setBrach_ID(brachid);
        client.setLastName(lastName);
        client.setFirstName(firstName);
        client.setBalance(balance);
        client.setAccountNumber(accountNumber);
        // move file-position pointer to correct record in file
        insertInFile.seekp((accountNumber - 1) * sizeof(ClientData));
        // insert record in file
        insertInFile.write(reinterpret_cast<const char *> (&client),
                           sizeof(ClientData));
    }// end if
    else // display error if account already exists
        cerr << "Account #" << accountNumber
        << " already contains information." << endl;
} // end function newRecord
// delete an existing record

void deleteRecord(fstream &deleteFromFile) {
    // obtain number of account to delete
    int accountNumber = getAccount("Enter account to delete");
    // move file-position pointer to correct record in file
    deleteFromFile.seekg((accountNumber - 1) * sizeof(ClientData));
    // read record from file
    ClientData client;
    deleteFromFile.read(reinterpret_cast<char *> (&client),
                        sizeof(ClientData));
    // delete record, if record exists in file
    if (client.getAccountNumber() != 0) {
        ClientData blankClient; // create blank record
        // move file-position pointer to correct record in file
        deleteFromFile.seekp((accountNumber - 1) *
                             sizeof(ClientData));
        // replace existing record with blank record
        deleteFromFile.write(
                reinterpret_cast<const char *> (&blankClient),
                sizeof(ClientData));
        cout << "Account #" << accountNumber << " deleted.\n";
    }// end if
    else // display error if record does not exist
        cerr << "Account #" << accountNumber << " is empty.\n";
} // end deleteRecord
// display single record

void outputLine(ostream &output, const ClientData &record) {
    output << left << setw(10) << record.getAccountNumber()
    << setw(13) << record.getBrach_ID()
    << setw(16) << record.getLastName()
    << setw(11) << record.getFirstName()
    << setw(10) << setprecision(2) << right << fixed
    << showpoint << record.getBalance() << endl;
} // end function outputLine
// obtain account-number value from user

int getAccount(const char *const prompt) {
    int accountNumber;
    // obtain account-number value
    do {
        cout << prompt << " (1 - 100): ";
        cin >> accountNumber;
    } while (accountNumber < 1 || accountNumber > 100);
    return accountNumber;
} // end function getAccount

void Backup(fstream &inOutCredit) {
    ClientData client;
    fstream inOutBackup("/home/abdu/ClionProjects/File_T_5/backup.dat", ios::in | ios::out | ios::binary | ios::trunc);
    for (int i = 0; i < 100; ++i) {
        inOutCredit.seekg(i * sizeof(ClientData));
        inOutCredit.read(reinterpret_cast<char *> (&client), sizeof(ClientData));
        inOutBackup.seekg(i * sizeof(ClientData));
        inOutBackup.write(reinterpret_cast<const char *> (&client), sizeof(ClientData));
    }
}

void Restore(fstream &inOutCredit) {
    ClientData client;
    fstream inOutBackup("/home/abdu/ClionProjects/File_T_5/backup.dat", ios::in | ios::out | ios::binary);
    if (!inOutBackup.is_open())
        cerr << "Backup file dose not exist";
    else
        for (int i = 0; i < 100; ++i) {
            inOutBackup.seekg(i * sizeof(ClientData));
            inOutBackup.read(reinterpret_cast<char *> (&client), sizeof(ClientData));
            inOutCredit.seekg(i * sizeof(ClientData));
            inOutCredit.write(reinterpret_cast<const char *> (&client), sizeof(ClientData));
        }
}

void Create_Index_Files(fstream &OutCredit) {
    IndexP P;
    IndexS S;
    int flag = 1;
    string str;
    fstream indexP("/home/abdu/ClionProjects/File_T_5/primary_index.dat",
                   ios::in | ios::out | ios::binary);
    fstream indexS("/home/abdu/ClionProjects/File_T_5/secondary_index.dat",
                   ios::in | ios::out | ios::binary);
    if (!indexP)
        indexP.open("/home/abdu/ClionProjects/File_T_5/primary_index.dat",
                    ios::in | ios::out | ios::binary | ios::trunc);
    if (!indexS)
        indexS.open("/home/abdu/ClionProjects/File_T_5/secondary_index.dat",
                    ios::in | ios::out | ios::binary | ios::trunc);

    for (int i = 0; i < 100; ++i) {
        P.setAccountNumber(i + 1);
        P.setByte(i * sizeof(ClientData));
        indexP.write(reinterpret_cast<const char *> (&P), sizeof(IndexP));
    }


    ClientData client;
    OutCredit.read(reinterpret_cast<char *> (&client),
                   sizeof(ClientData));
    S.setlastname(client.getLastName());
    S.setAcountNumbers(0);
    indexS.write(reinterpret_cast<const char *> (&S), sizeof(IndexS));
    for (int i = 1; i < 100; ++i) {

        OutCredit.read(reinterpret_cast<char *> (&client),
                       sizeof(ClientData));
        if (client.getAccountNumber() != 0) {
            indexS.seekg(0);
            while (!indexS.eof()) {
                indexS.read(reinterpret_cast<char *> (&S), sizeof(IndexS));
                str = S.getlastname();
                if (str == client.getLastName()) {
                    S.setAcountNumbers(i);
                    indexS.write(reinterpret_cast<const char *> (&S), sizeof(IndexS));
                    flag = -1;
                }
            }
            if (flag == 1) {
                S.setAcountNumbers(client.getAccountNumber());
                S.setlastname(str);
                indexS.write(reinterpret_cast<char *> (&S), sizeof(IndexS));
            }
        }
    }
}

void Create_text_files_for_Index_Files(fstream &readPrime, fstream &readSecondray) {
    ofstream outPrintFile("/home/abdu/ClionProjects/File_T_5/primary_index.txt", ios::out);
    ofstream outSecondary("/home/abdu/ClionProjects/File_T_5/secondary_index.txt", ios::out);
    // exit program if ofstream cannot create file
    if (!outPrintFile) {
        cerr << "File primary_index could not be created." << endl;
        exit(1);
    } // end if
    if (!outSecondary) {
        cerr << "File secondary_index could not be created." << endl;
        exit(1);
    } // end if
    outPrintFile << left << setw(10) << "Account" << setw(13) << "Byte-offset" << endl;
    outSecondary << left << setw(16) << "Last name" << right << setw(19) << "Account Numbers" << endl;
    readPrime.seekg(0);
    readSecondray.seekg(0);
    // read first record from record file
    IndexP client;
    // copy all records from record file into text file
    while (!readPrime.eof()) {
        // write single record to text file
        readPrime.read(reinterpret_cast<char *> (&client),
                       sizeof(IndexP));
            outputLine(outPrintFile, client);
    } // end while
    IndexS clientS;
    // copy all records from record file into text file
    while (!readSecondray.eof()) {
        readSecondray.read(reinterpret_cast<char *> (&clientS),
                           sizeof(IndexS));
        outputLine(outSecondary, clientS);
    }
}

void outputLine(ostream &output, const IndexP &record) {
    output << left << setw(10) << record.getAccountNumber()
    << setw(13) << record.getByte()
    << endl;
}

void outputLine(ostream &output, const IndexS &record) {
    output << left << setw(16) << record.getlastname() << right << setw(19) << record.getAccountNumbers() << endl;
}