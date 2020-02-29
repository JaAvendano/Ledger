//
// Created by mojoncaliente on 2/13/20.
//

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

using namespace std;

class Account {
private:
    int m_id;
    string m_instName;
    string m_accName;
    int m_accType;
    string m_typeStr;
    int m_interest;
    string m_interestStr;
    int m_active;
    string m_activeStr;
    long double m_balance;
    long double m_proj_bal;
    string m_openDate;
    string m_rDate;
    int m_day;
    int m_month;
    int m_year;
    long double m_apy;
public:
    // This constructor is used for adding an Account to the DB
    Account(string, int, string, int, long double, int, string, int, int, int);
    // This constructor is used for adding Account data to a container for viewing or modification
    Account(int id, string inst, string typeStr, string name, int active, string activeStr, long double balance, long double proj_bal, int interest, string opened) {
        m_id = id;
        m_instName = inst;
        m_typeStr = typeStr;
        m_accName = name;
        m_active = active;
        m_activeStr = activeStr;
        m_balance = balance;
        m_proj_bal = proj_bal;
        m_interest = interest;
        m_rDate = opened;
    }
    // This constructor allows for instantiating a new class object when not needing to add data to the DB or to a container
    Account() {}
    ~Account() {}

    void addAcc(); // Add Account to DB
    void editAcc(); // Edit details of a selected Account

    void viewAccHeader(); // Displays a header for Accounts list
    void viewAccList(); // Displays formatted list of Accounts currently stored in DB
    void viewAccTotals(); // Displays total value of assets
    void accPreviewHeader();  //
    void accPreviewList();    // These functions are used for the Accounts Preview
    void accPreviewTotals();  // at the begginning of the program

    int accSel(int, int);

    // Setter Methods
    string setInstName();
    string setAccName();
    int setAccType();
    int setAccInterest();
    int setAccStatus();
    long double setAccBal();
    void setOpenDate(vector<int>&);

    // methods for updating individual members in the DB
    void DBUpdateOpenDate(int);
    void DBUpdateInstName(int);
    void DBUpdateAccName(int);
    void DBUpdateAccType(int);
    void DBUpdateAccStatus(int);
    void DBUpdateAccBal(int, long double);
    void DBUpdateAccInterest(int);
    int warnConf();

    // Accessor methods
    string getAccName(int);
    string getAccType(int);
    long double getBalance(int);

    // Pre Checks
    bool checkAcc();
    bool checkAPY();

    // Interest
    void setAPY();
    void viewAPYHeader();
    void viewAPY(int);
    void editAPY(int);

    // Edit APY data
    void setAPYAcc(int);
    void setAccAPY(int);
    void setAPYAmount(int);

    // Calculate Account Balance
    long double calcBal(long double, long double);
};


#endif
