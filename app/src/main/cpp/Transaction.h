//
// Created by mojoncaliente on 2/13/20.
//

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

using namespace std;

class Transaction {
private:
    int m_id;
    int m_acc;
    int m_destAcc;
    int m_type;
    string m_accName;
    int m_status;
    string m_transName;
    string m_destTransName;
    int m_method;
    string m_methodStr;
    int m_cat;
    int m_destCat;
    string m_catStr;
    long double m_amount;
    long double m_proj_bal;
    long double m_destAmount;
    long double m_balance;
    string m_date;
    string m_rDate;
public:
    // This constructor is used for adding a Transaction to the DB
    Transaction(int, string, int, int, long double, long double, string);
    // This constructor is used for adding Transaction data to a container for viewing or modification
    Transaction(int id, int acc, string accName, int status, string name, string method, string cat, long double amount, long double balance, string date) {
        m_id = id;
        m_acc = acc;
        m_accName = accName;
        m_status = status;
        m_transName = name;
        m_methodStr = method;
        m_catStr = cat;
        m_amount = amount;
        m_balance = balance;
        m_rDate = date;
    }
    // This constructor allows for instantiating a new class object when not needing to add data to the DB or to a container
    Transaction() {}
    ~Transaction() {}

    bool checkTrans(); // Check for Transaction Data
    void addTrans();   // Add a new Transaction
    bool recTrans(int, int);

    int selTrans(int, int);

    // Setter methods
    int setTransType();
    string setTransName();
    int setTransMethod(int);
    int setTransCat(int);
    long double setAmount(int);
    string setDate();

    // Accessor Methods
    int getStatus(int);
    string getMethodName(int);
    string getCatName(int);
    long double getAmount(int);

    int methodSel();
    int catSel();

    // Transaction Viewing
    void viewTransHeader(int);
    void viewTransList(int);
    long double sumPTrans(int);

    // Update DB data
    void updateBalance(int, int); // Updates Transaction balances through iteration
    void DBUpdateStatus(int);
    void DBUpdateName(int);
    void DBUpdateMethod(int);
    void DBUpdateCat(int);
    void DBUpdateAmount(int, int, long double);
    void DBUpdateBalance(int, long double); // Updates Transaction balances in DB directly
    void DBUpdateTransDate(int, string);
    void DBUpdateTransAcc(int, int);

    // Edit a Transaction
    void editTrans(int, int);
    void editAmount(int, int);
    void removeAmount(int, int);
    void editDate(int);
};

#endif
