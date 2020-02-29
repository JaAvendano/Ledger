//
// Created by mojoncaliente on 2/13/20.
//

#include "Account.h"
#include "Transaction.h"
#include "SQL.h"
#include "Date.h"

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <windows.h>
#include <sstream>

// Use namespaces
using namespace std;

/*********************************************************
* Account Constructor for adding a new Account to the DB *
*********************************************************/
Account::Account(string instName, int accType, string accName, int active, long double balance, int interest, string openDate, int day, int month, int year) {
    // Instantiate class instance
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Add user input into DB
        pstmt = sql->con->prepareStatement("INSERT INTO accounts(institution,type,acc_id,active,balance,interest,opened,day,month,year) VALUES (?,?,?,?,?,?,?,?,?,?)");
        pstmt->setString(1, instName);
        pstmt->setInt(2, accType);
        pstmt->setString(3, accName);
        pstmt->setInt(4, active);
        pstmt->setDouble(5, balance);
        pstmt->setInt(6, interest);
        pstmt->setString(7, openDate);
        pstmt->setInt(8, day);
        pstmt->setInt(9, month);
        pstmt->setInt(10, year);

        // Execute INSERT Statement
        pstmt->executeQuery();

        // Delete SQL objects and close connection
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/*****************************************************
* Add new Account:
* Prompts the user for several inputs to add details *
* about about the new Account being added to the DB  *
*****************************************************/
void Account::addAcc() {
    bool complete = false;
    bool valid = false;
    vector<int> dateVec;
    char charSel = ' ';

    // Instantiate class instance
    SQL* sql = new SQL();
    Date* date = new Date();

    cout << "----------------------------------------------------------------------" << endl;
    cout << "Add Accounts" << endl << endl;

    while (!complete) {
        // Get Financial Institution Name
        m_instName = setInstName();
        cout << endl << endl;

        // Get Account Name
        m_accName = setAccName();
        cout << endl << endl;

        // Set Account Type
        m_accType = setAccType();
        cout << endl << endl;

        // Does Account accrue Interest
        m_interest = setAccInterest();
        cout << endl << endl;

        // Is Account Active
        m_active = setAccStatus();
        cout << endl << endl;

        // Set Balance for Account
        m_balance = setAccBal();
        cout << endl << endl;

        // Set Projected Balance to 0.0
        m_proj_bal = 0.0;

        // Set Open Date for Account
        dateVec.clear();
        dateVec.erase(dateVec.begin(), dateVec.end());
        setOpenDate(dateVec);

        // Convert from int to string
        m_openDate = date->dateToString(dateVec);

        // Get a human readable date
        m_rDate = date->rDateString(m_openDate);

        // Print out user input for visual verification
        valid = false;
        while (!valid) {
            // Get string representation for the Account type
            m_typeStr = getAccType(m_accType);

            // For showing if Account accrues interest
            // If interest = 0, display No
            // else display Yes
            if (m_interest == 0) {
                m_interestStr = "No";
            }
            else {
                m_interestStr = "Yes";
            }

            // For is Account open/closed
            // If active = 0, display No
            // Else, display Yes
            if (m_active == 0) {
                m_activeStr = "No";
            }
            else {
                m_activeStr = "Yes";
            }

            // Display user input
            cout << m_rDate << " - " << m_instName << " - " << m_accName << " - " << m_typeStr << " - " << m_interestStr << " - " << m_activeStr << " - " << "$" << m_balance << endl;
            cin.clear();
            cin.ignore();
            cout << "Is this correct? [Y/N]: ";
            cin >> charSel;
            cout << endl << endl;

            // Verify char input
            if (charSel == 'y' || charSel == 'Y' || charSel == 'n' || charSel == 'N') {
                valid = true;
            }
            else {
                cout << charSel << " is not a valid selection! Please try again!" << endl;
                cin.clear();
                cin.ignore();
                cout << "Press [Enter] to continue..." << endl << endl;
                continue;
            }
        }
        if (charSel == 'n' || charSel == 'N') {
            continue;
        }
        else if (charSel == 'y' || charSel == 'Y') {
            // Add user input to DB
            Account* Acct = new Account(m_instName, m_accType, m_accName, m_active, m_balance, m_interest, m_openDate, dateVec[2], dateVec[1], dateVec[0]);

            // Ask User if another Account is needed
            valid = false;
            while (!valid) {
                charSel = ' ';
                cout << "Another?: [Y/N]";
                cin >> charSel;
                cout << endl << endl;
                if (charSel == 'y' || charSel == 'Y' || charSel == 'n' || charSel == 'N') {
                    valid = true;
                }
                else {
                    cout << charSel << " is not a valid selection! Please try again!" << endl;
                    cin.clear();
                    cin.ignore();
                    continue;
                }
            }
            if (charSel == 'y' || charSel == 'Y') {
                continue;
            }
            else if (charSel == 'n' || charSel == 'N') {
                complete = true;
            }
        }
    }
}

/**********************************************
 * Get Institution name to add to new Account *
 *********************************************/
string Account::setInstName() {
    cin.ignore();
    cout << "Enter Institution Name: ";
    getline(cin, m_instName);

    return m_instName;
}

/**********************************************
* Get Account nick name to add to new Account *
**********************************************/
string Account::setAccName() {
    cout << "Enter Account Name: ";
    getline(cin, m_accName);

    return m_accName;
}

/*****************************************
* Get Account type to add to new Account *
*****************************************/
int Account::setAccType() {
    bool valid = false;

    // Instantiate class instance
    SQL* sql = new SQL();

    while (!valid) {
        try {
            // Instantiate SQL objects
            sql->connect();
            PreparedStatement* pstmt;
            ResultSet* res;

            // Select data from acc_type
            pstmt = sql->con->prepareStatement("SELECT * FROM acc_type");
            res = pstmt->executeQuery();

            // Display data from DB for user selection
            cout << "Select an Account type:" << endl;
            while (res->next()) {
                cout << res->getInt("id") << " - " << res->getString("type") << endl;
            }
            cin >> m_accType;

            if (m_accType < 1 || m_accType > res->rowsCount()) {
                cout << m_accType << " is not a valid selection! Please try again!" << endl;
                cin.clear();
                cin.ignore();
                continue;
            }
            else {
                // Delete SQL objects and close connection
                delete res;
                delete pstmt;
                sql->con->close();
                delete sql->con;
                valid = true;
            }
        }
        catch (SQLException & e) {
            string file = __FILE__;
            string func = __func__;
            int line = __LINE__;
            sql->catchSQLE(e, file, func, line);
        }
        catch (std::runtime_error & e) {
            string func = __func__;
            int line = __LINE__;
            sql->catchRuntimeE(e, func, line);
        }
    }
    return m_accType;
}

/*******************************************************
* Set whether the new Account is eligible for interest *
*******************************************************/
int Account::setAccInterest() {
    bool valid = false;

    while (!valid) {
        cout << "Does this account accrue interest?:" << endl;
        cout << "1. Yes  0. No: ";
        cin >> m_interest;

        if (m_interest < 0 || m_interest > 1) {
            cout << m_interest << " is not a valid selection! Please try again!" << endl;
            cin.clear();
            cin.ignore();
            continue;
        }
        else {
            valid = true;
        }
    }
    return m_interest;
}

/*********************
* Set Account status *
*********************/
int Account::setAccStatus() {
    bool valid = false;

    while (!valid) {
        cout << "Is account active?" << endl;
        cout << "1. Yes  0. No: ";
        cin >> m_active;

        if (m_active < 0 || m_active > 1) {
            cout << m_active << " is not a valid selection! Please try again!" << endl;
            cin.clear();
            cin.ignore();
            continue;
        }
        else {
            valid = true;
        }
    }
    return m_active;
}

/******************************
* Set Balance for new Account *
******************************/
long double Account::setAccBal() {
    cout << "Enter Account balance: ";
    cin >> m_balance;

    return m_balance;
}

/*********************************************
* Set date for when Account was first opened *
*********************************************/
void Account::setOpenDate(vector<int>& dateVec) {
    cout << "Enter the date your Account was first opened: [DD/MM/YYYY] ";
    int day = 0;
    int month = 0;
    int year = 0;

    // Instantiate class objects
    Date* date = new Date();

    cin >> day;
    cin >> month;
    cin >> year;
    dateVec.push_back(year);
    dateVec.push_back(month);
    dateVec.push_back(day);
}

/********************************************
* Formatted header for viewing Account data *
********************************************/
void Account::viewAccHeader() {
    cout << "|----------------------------------------------------------------------------------------------------------|" << endl;
    cout << "| # ";
    cout << "| " << setw(12) << left << "Opened ";
    cout << "| " << setw(16) << "Institution ";
    cout << "| Account Name ";
    cout << "| Account Type ";
    cout << "| Active ";
    cout << "| " << setw(10) << "Balance ";
    cout << "| " << setw(10) << "Projected ";
    cout << "| " << setw(5) << "APY   |" << endl;
    cout << "|----------------------------------------------------------------------------------------------------------|" << endl;
}

/**********************************************
* Displays a formatted account list (verbose) *
**********************************************/
void Account::viewAccList() {
    int num = 1;
    vector<Account*> acct;

    // Instantiate class objects
    SQL* sql = new SQL();
    Account* Acct;
    Transaction* Trans = new Transaction();
    Date* date = new Date();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select data from accounts and order by id in ascending order
        pstmt = sql->con->prepareStatement("SELECT accounts.*, acc_type.type FROM accounts LEFT JOIN(acc_type) ON (acc_type.id = accounts.type) ORDER BY accounts.id ASC");
        res = pstmt->executeQuery();
        while (res->next()) {
            // Add all DB data to private class members
            m_id = res->getInt("id");
            m_instName = res->getString("institution");
            m_typeStr = res->getString("type");
            m_accName = res->getString("acc_id");
            m_active = res->getInt("active");
            // If active = 0, display No
            // Else, Yes
            if (m_active == 0) {
                m_activeStr = "No";
            }
            else {
                m_activeStr = "Yes";
            }
            m_balance = res->getDouble("balance");
            m_interest = res->getInt("interest");
            m_apy = res->getDouble("apy");
            m_openDate = res->getString("opened");
            m_rDate = date->rDateString(m_openDate);
            // Get a summary of all pending Transactions
            m_proj_bal = Trans->sumPTrans(m_id);
            m_proj_bal += m_balance;
            // Use secondary Account constructor for adding data to vector
            Acct = new Account(m_id, m_instName, m_typeStr, m_accName, m_active, m_activeStr, m_balance, m_proj_bal, m_interest, m_rDate);
            // Push back into vector
            acct.push_back(Acct);
        }
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    // Display formatted Account data (Verbose)
    for (size_t c = 0; c < acct.size(); c++) {
        cout << "| " << setw(2) << left << num++;
        cout << "| " << setw(12) << left << acct[c]->m_rDate;
        cout << "| " << setw(16) << left << acct[c]->m_instName;
        cout << "| " << setw(13) << acct[c]->m_accName;
        cout << "| " << setw(13) << acct[c]->m_typeStr;
        cout << "| " << setw(7) << acct[c]->m_activeStr;
        cout << "|$ " << setw(9) << right << acct[c]->m_balance;
        cout << "|$ " << setw(9) << right << acct[c]->m_proj_bal;
        if (acct[c]->m_interest == 0) {
            cout << "| " << setw(5) << "  " << " |" << endl;
        }
        else {
            cout << "| " << setw(5) << acct[c]->m_apy << "%|" << endl;
        }
    }
}

/******************************************************
* Displays a formatted Accounts total for all acounts *
******************************************************/
void Account::viewAccTotals() {
    int w = 0;
    int digits = 0;
    ostringstream oss;

    // Instantiate class object
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select the sum of balance of all Accounts
        pstmt = sql->con->prepareStatement("SELECT SUM(balance) AS total FROM accounts");
        res = pstmt->executeQuery();
        while (res->next()) {
            m_balance = res->getDouble("total");
        }

        // Delete SQL objects and close connection
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    cout << "|----------------------------------------------------------------------------------------------------------|" << endl;
    cout << "| " << left << "Total";

    // Displays totals according to the number of
    // digits used in order to keep the formatting consistent
    oss << fixed << setprecision(2) << m_balance;
    digits = static_cast<int>(oss.str().size());
    if (digits == 4) {w = 96;}
    else if (digits == 5) {w = 95;}
    else if (digits == 6) {w = 94;}
    else if (digits == 7) {w = 93;}
    else if (digits == 8) {w = 92;}
    else if (digits == 9) {w = 91;}
    else if (digits == 10) {w = 90;}
    cout << setw(w) << right << "$" << m_balance << right << "|" << endl;

    cout << "|----------------------------------------------------------------------------------------------------------|" << endl;
}

/****************************************
* Formatted header for Account Overview *
****************************************/
void Account::accPreviewHeader() {
    cout << "|------------------------------------------------------------------------|" << endl;
    cout << "| " << setw(16) << left << "Institution " << "|" << setw(2);
    cout << " Account Name " << "|" << setw(13);
    cout << " Account Type " << "|" << setw(7);
    cout << " Balance " << "  |" << setw(9);
    cout << " Projected " << " |" << setw(9) << endl;
    cout << "|------------------------------------------------------------------------|" << endl;
}

/*********************************************
* Displays a formatted account overview list *
*********************************************/
void Account::accPreviewList() {
    vector<Account*> acct;
    vector<int> dateVec;

    // Instantiate class onjects
    SQL* sql = new SQL();
    Account* Acct;
    Transaction* Trans = new Transaction();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select data from accounts and order by id in ascending order
        pstmt = sql->con->prepareStatement("SELECT accounts.*, acc_type.type FROM accounts LEFT JOIN(acc_type) ON (acc_type.id = accounts.type) ORDER BY accounts.id ASC");
        res = pstmt->executeQuery();
        while (res->next()) {
            // Add data to private class memebers
            m_accType = 0;
            m_active = 0;
            m_interest = 0;
            m_apy = 0.0;
            m_openDate = "";
            m_id = res->getInt("id");
            m_instName = res->getString("institution");
            m_typeStr = res->getString("type");
            m_accName = res->getString("acc_id");
            m_balance = res->getDouble("balance");
            // Get summary of all pending Transactions
            m_proj_bal = Trans->sumPTrans(m_id);
            m_proj_bal += m_balance;
            // Add all Transaction data to secondary Transaction constructor for adding to vector
            Acct = new Account(m_id, m_instName, m_typeStr, m_accName, m_active, m_activeStr, m_balance, m_proj_bal, m_interest, m_openDate);
            // Push back into vector
            acct.push_back(Acct);
        }

        // Delete SQL objects and close connection
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    // Display formatted Account data
    for (size_t c = 0; c < acct.size(); c++) {
        cout << "| " << setw(16) << left << acct[c]->m_instName;
        cout << "| " << setw(13) << acct[c]->m_accName;
        cout << "| " << setw(13) << acct[c]->m_typeStr;
        cout << "|$ " << setw(9) << right << acct[c]->m_balance;
        cout << "|$ " << setw(10) << right << acct[c]->m_proj_bal << "|" << endl;
    }
}

/******************************************************
* Displays a formatted Accounts total for all acounts *
******************************************************/
void Account::accPreviewTotals() {
    int w = 0;
    int digits = 0;
    ostringstream oss;

    // Instantiate Class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        pstmt = sql->con->prepareStatement("SELECT SUM(balance) AS total FROM accounts");
        res = pstmt->executeQuery();
        vector<long double> bal;
        while (res->next()) {
            m_balance = res->getDouble("total");
        }

        // Delete SQL objects and close connection
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    cout << "|------------------------------------------------------------------------|" << endl;
    cout << "| " << left << "Total";

    // Displays totals according to the number of
    // digits used in order to keep the formatting consistent
    oss << fixed << setprecision(2) << m_balance; // Convert m_balance to string
    digits = static_cast<int>(oss.str().size()); // Get number of digits of m_balance
    if (digits == 4) {w = 62;}
    else if (digits == 5) {w = 61;}
    else if (digits == 6) {w = 60;}
    else if (digits == 7) {w = 59;}
    else if (digits == 8) {w = 58;}
    else if (digits == 9) {w = 57;}
    else if (digits == 10) {w = 56;}
    cout << setw(w) << right << "$" << m_balance << right << "|" << endl;
    cout << "|------------------------------------------------------------------------|" << endl;
}

/*****************************************
* Edit individual details for an Account *
*****************************************/
void Account::editAcc() {
    bool complete = false;
    bool valid = false;
    int accID = 0;
    int intSel = 0;
    char charSel = ' ';
    int conf = 0;
    enum action { accDate=1, inst, accName, accType, active, interest, bal, quit }; // Enum for enum map menu
    map<action, string> menu{ // Map for displaying a menu
            {action::accDate, "Edit Opened Date"},
            {action::inst, "Change Institution"},
            {action::accName, "Change Account Name"},
            {action::accType,"Change Account Type"},
            {action::active,"Change active status"},
            {action::interest,"Edit interest"},
            {action::bal, "Edit Balance"},
            {action::quit, "Quit"}
    };
    map<action, string>::iterator i; // Iterator for enum map menu

    // Instantiate class objects
    SQL* sql = new SQL();

    cout << "--------------------------------------------------" << endl << endl;
    cout << "Edit Accounts" << endl << endl;

    // Select Account
    accID = accSel(1,0);

    // Select action to perform
    cout << "Select an action to perform:" << endl;
    for (i = menu.begin(); i != menu.end(); i++) {
        cout << i->first << " - " << i->second << endl;
    }
    cin >> intSel;

    // Switch Case for selection
    switch (intSel) {
        case action::accDate: DBUpdateOpenDate(accID);
            break;
        case action::inst: DBUpdateInstName(accID);
            break;
        case action::accName: DBUpdateAccName(accID);
            break;
        case action::accType: DBUpdateAccType(accID);
            break;
        case action::active: DBUpdateAccStatus(accID);
            break;
        case action::interest: DBUpdateAccInterest(accID);
            break;
        case action::bal:
            // Warns the user of changing the balance if they have been adding transactions to this account
            conf = warnConf();
            if (conf == 2) {
                break; // If the user decides to quit
            }
            else {
                // If the user agrees to continue
                cout << "Enter Balance: ";
                cin >> m_balance;
                DBUpdateAccBal(accID, m_balance);
            }
            break;
        case action::quit: cout << "Quit" << endl;
            break;
        default:
            cout << "Incorrect Selection!" << endl;
    }
}

/********************
* Select an Account *
********************/
int Account::accSel(int action, int accID) {
    bool valid = false;
    int id = 0;
    int num = 1;
    string stmt = "";
    vector<Account*> acct;

    // Instantiate class objects
    SQL* sql = new SQL();
    Transaction* Trans = new Transaction();
    Date* date = new Date();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        /**********************************
        * Action Code 1: Normal Access     *
        * Action Code 2: Interest Priority *
        * Action Code 3: Transfer Priority *
        ***********************************/

        if (action == 1) {
            // Select Account data for normal use
            stmt = "SELECT accounts.* FROM accounts LEFT JOIN(acc_type) ON (accounts.type = acc_type.id) ORDER BY accounts.id ASC";
        }
        else if (action == 2) {
            // Select Account data for use for interest data
            stmt = "SELECT accounts.* FROM accounts LEFT JOIN(acc_type) ON (accounts.type = acc_type.id) WHERE accounts.interest = 1 ORDER BY accounts.id ASC";
        }
        else if (action == 3) {
            // Select Account data for a transfer
            stmt = "SELECT accounts.* FROM accounts LEFT JOIN(acc_type) ON (accounts.type = acc_type.id) WHERE accounts.id <> ? ORDER BY accounts.id ASC";
        }

        // Select Accounts from DB
        pstmt = sql->con->prepareStatement(stmt);
        if (action == 3) {
            pstmt->setInt(1, accID);
        }
        res = pstmt->executeQuery();

        // Save Account data to vector
        while (res->next()) {
            m_id = res->getInt("id");
            m_instName = res->getString("institution");
            m_typeStr = res->getString("type");
            m_accName = res->getString("acc_id");
            m_active = res->getInt("active");
            if (m_active == 1) {
                m_activeStr = "Active";
            }
            else {
                m_activeStr = "Inactive";
            }
            m_balance = res->getDouble("balance");
            m_interest = res->getInt("interest");
            m_openDate = res->getString("opened");
            m_rDate = date->rDateString(m_openDate);
            Trans->sumPTrans(m_id);
            Account* Acct = new Account(m_id, m_instName, m_typeStr, m_accName, m_active, m_activeStr, m_balance, m_proj_bal, m_interest, m_rDate);
            acct.push_back(Acct);
        }

        // Delete SQL objects and close connection
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    while (!valid) {
        // Display Accounts for selection
        cout << "Select an Account:" << endl;
        for (size_t c = 0; c < acct.size(); c++) {
            cout << num++ << " - " << acct[c]->m_rDate << " - " << acct[c]->m_instName << " - " << acct[c]->m_accName << " - " << acct[c]->m_typeStr << " - " << acct[c]->m_activeStr << " - " << acct[c]->m_balance << endl;
        }
        cin >> accID;
        if (accID < 1 || accID > acct.size()) {
            cin.clear();
            cin.ignore();
            cout << accID << " Is an incorrect selection! Please try again!" << endl << endl;
        }
        else {
            // Use the user input to set the Account id
            --accID;
            id = acct[accID]->m_id;
            valid = true;
        }
    }
    return id;
}

/*****************************
* Update Open Date in the DB *
*****************************/
void Account::DBUpdateOpenDate(int id) {
    int day = 0;
    int month = 0;
    int year = 0;
    vector<int> dateVec;

    // Instantiate class objects
    SQL* sql = new SQL();
    Date* date = new Date();

    cout << "Enter new date [MM DD YYYY]: ";
    cin >> month;
    cin >> day;
    cin >> year;
    dateVec.push_back(year);
    dateVec.push_back(month);
    dateVec.push_back(day);
    m_openDate = date->dateToString(dateVec);

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Update the opened date for a selected Account in the DB
        pstmt = sql->con->prepareStatement("UPDATE accounts SET opened = ? WHERE id = ?");
        pstmt->setString(1, m_openDate);
        pstmt->setInt(2, id);

        pstmt->executeUpdate();

        // Delete SQL objects and close connection
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/************************************
* Update Institution Name in the DB *
************************************/
void Account::DBUpdateInstName(int id) {
    SQL* sql = new SQL();

    cin.ignore();
    cout << "Enter Institution name: ";
    getline(cin, m_instName);

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Update the Institution name of a selected Account in the DB
        pstmt = sql->con->prepareStatement("UPDATE accounts SET institution = ? WHERE id = ?");
        pstmt->setString(1, m_instName);
        pstmt->setInt(2, id);

        pstmt->executeUpdate();

        // Delete SQL objects and close connection
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/********************************
* Update Account Name in the DB *
********************************/
void Account::DBUpdateAccName(int id) {
    SQL* sql = new SQL();

    cin.ignore();
    cout << "Enter Account Name: ";
    getline(cin, m_accName);

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        pstmt = sql->con->prepareStatement("UPDATE accounts SET acc_id = ? WHERE id = ?");
        pstmt->setString(1, m_accName);
        pstmt->setInt(2, id);

        pstmt->executeUpdate();

        // Delete SQL objects and close connection
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/********************************
* Update Account Type in the DB *
********************************/
void Account::DBUpdateAccType(int id) {
    bool valid = false;
    SQL* sql = new SQL();

    cout << "Select Account Type:" << endl;
    while (!valid) {
        try {
            // Instantiate SQL objects
            sql->connect();
            PreparedStatement* pstmt;
            ResultSet* res;

            // Select data from acc_type
            pstmt = sql->con->prepareStatement("SELECT * FROM acc_type");
            res = pstmt->executeQuery();

            // Display data from DB for user selection
            cout << "Select an Account type:" << endl;
            while (res->next()) {
                cout << res->getInt("id") << " - " << res->getString("type") << endl;
            }
            cin >> m_accType;

            if (m_accType < 1 || m_accType > res->rowsCount()) {
                cout << m_accType << " is not a valid selection! Please try again!" << endl;
                cin.clear();
                cin.ignore();
                continue;
            }
            else {
                // Delete SQL objects and close connection
                delete res;
                delete pstmt;
                sql->con->close();
                delete sql->con;

                valid = true;
            }
        }
        catch (SQLException & e) {
            string file = __FILE__;
            string func = __func__;
            int line = __LINE__;
            sql->catchSQLE(e, file, func, line);
        }
        catch (std::runtime_error & e) {
            string func = __func__;
            int line = __LINE__;
            sql->catchRuntimeE(e, func, line);
        }
    }

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        pstmt = sql->con->prepareStatement("UPDATE accounts SET type = ? WHERE id = ?");
        pstmt->setInt(1, m_accType);
        pstmt->setInt(2, id);

        pstmt->executeUpdate();

        // Delete SQL objects and close connection
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/**********************************
* Update Account Status in the DB *
**********************************/
void Account::DBUpdateAccStatus(int id) {
    SQL* sql = new SQL();

    cout << "Select Account status: " << endl;
    cout << "1. Active   0. Inactive" << endl;
    cin >> m_active;

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        pstmt = sql->con->prepareStatement("UPDATE accounts SET active = ? WHERE id = ?");
        pstmt->setInt(1, m_active);
        pstmt->setInt(2, id);

        pstmt->executeUpdate();

        // Delete SQL objects and close connection
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/***********************************
* Update Account Balance in the DB *
***********************************/
void Account::DBUpdateAccBal(int id, long double balance) {
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Commit changes to DB
        pstmt = sql->con->prepareStatement("UPDATE accounts SET balance = ? WHERE id = ?");
        pstmt->setDouble(1, balance);
        pstmt->setInt(2, id);

        pstmt->executeUpdate();

        // Delete SQL objects and close connection
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/**************************
* Update Account Interest *
**************************/
void Account::DBUpdateAccInterest(int id) {
    SQL* sql = new SQL();

    cout << "Does this Account accrue interest?" << endl;
    cout << "1. Yes   0. No: ";
    cin >> m_interest;

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        pstmt = sql->con->prepareStatement("UPDATE accounts SET interest = ? WHERE id = ?");
        pstmt->setInt(1, m_interest);
        pstmt->setInt(2, id);

        pstmt->executeUpdate();

        // Delete SQL objects and close connection
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/********************************************
* Displays a warning for ANY/ALL users to   *
* use the edit Account balance with caution *
********************************************/
int Account::warnConf() {
    int conf = 0;
    bool valid = false;

    while (!valid) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 4);
        cout << "WARNING! You should ONLY use this if you entered the wrong balance when adding a new account." << endl
             << "Do NOT use this if you have already been using the program with the selected account and have" << endl
             << "ANY transactions already in the ledger!" << endl;
        SetConsoleTextAttribute(hConsole, 7);
        cout << "Select 'YES' to continue..." << endl
             << "or select 'NO' to return to menu..." << endl
             << "[1. YES, I UNDERSTAND THE RISK] [2. NO, I WANT TO EXIT]";
        cin.ignore();
        cin >> conf;
        if (conf < 1 || conf > 2) {
            cout << "ERROR - Incorrect input! Please try again!" << endl;
            continue;
        }
        else if (conf == 1){
            cout << endl << endl << "Access granted! Enter at your own risk!" << endl;
            valid = true;
        }
        else if (conf == 2) {
            break;
        }
    }
    return conf;
}

/***********************************
* Get Account Name Accessor Method *
***********************************/
string Account::getAccName(int accID) {
    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Account name from DB
        pstmt = sql->con->prepareStatement("SELECT acc_id FROM accounts WHERE id = ?");
        pstmt->setInt(1, accID);

        res = pstmt->executeQuery();

        while (res->next()) {
            m_accName = res->getString("acc_id");
        }

        // Delete SQL objects and close connection
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return m_accName;
}

/***********************************
* Get Account Type Accessor Method *
***********************************/
string Account::getAccType(int type) {
    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Account name from DB
        pstmt = sql->con->prepareStatement("SELECT type FROM acc_type WHERE id = ?");
        pstmt->setInt(1, type);

        res = pstmt->executeQuery();

        while (res->next()) {
            m_typeStr = res->getString("type");
        }

        // Delete SQL objects and close connection
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return m_typeStr;
}

/*************************************************
* Get the Account balance for a selected Account *
*************************************************/
long double Account::getBalance(int accID) {
    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Get Account Projected Balance
        pstmt = sql->con->prepareStatement("SELECT balance FROM accounts WHERE id = ?");
        pstmt->setInt(1, accID);

        res = pstmt->executeQuery();

        while (res->next()) {
            m_balance = res->getDouble("balance");
        }

        // Delete SQL objects and close connection
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return m_balance;
}

/*************************************
* Pre program check for Account data *
*************************************/
bool Account::checkAcc() {
    bool acct = false;

    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL instances
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        pstmt = sql->con->prepareStatement("SELECT * FROM accounts");
        res = pstmt->executeQuery();
        if (res->rowsCount() > 0) {
            acct = true;
        }

        // Delete pointers and close connection
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return acct;
}

/*********************************
* Pre program check for APY data *
*********************************/
bool Account::checkAPY() {
    bool apy = false;
    int interest = 0;
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        pstmt = sql->con->prepareStatement("SELECT * FROM acc_apy");
        res = pstmt->executeQuery();

        if (res->rowsCount() > 0) {
            apy = true;
        }

        // Delete SQL objects and close connection
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return apy;
}

/*************************************************
* Set APY rates and amounts for a select Account *
*************************************************/
void Account::setAPY() {
    bool complete = false;
    int accID = 0;
    long double amount = 0.0;
    long double rate = 0.0;
    char charSel = ' ';

    // Select Account
    accID = accSel(2,0);

    while (!complete) {

        // Set APY amount
        cout << "Enter minimum amount: ";
        cin >> amount;

        // Set APY rate
        cout << "Enter APY rate: ";
        cin >> rate;

        // Instantiate class objects
        SQL* sql = new SQL();

        try {
            // Instantiate SQL objects
            sql->connect();
            PreparedStatement* pstmt;

            // Set new values for APY
            pstmt = sql->con->prepareStatement("INSERT INTO acc_apy(acc_id,rate,amount) VALUES(?,?,?)");
            pstmt->setInt(1, accID);
            pstmt->setDouble(2, rate);
            pstmt->setDouble(3, amount);

            // Commit input to the DB
            pstmt->execute();

            // Delete SQL objects and close connection
            delete pstmt;
            sql->con->close();
            delete sql->con;
        }
        catch (SQLException & e) {
            string file = __FILE__;
            string func = __func__;
            int line = __LINE__;
            sql->catchSQLE(e, file, func, line);
        }
        catch (std::runtime_error & e) {
            string func = __func__;
            int line = __LINE__;
            sql->catchRuntimeE(e, func, line);
        }

        cout << "Another? [Y/N] ";
        cin >> charSel;
        if (charSel == 'y' || charSel == 'Y') {
            continue;
        }
        else if (charSel == 'n' || charSel == 'N') {
            break;
        }
        else {
            cout << charSel << " is not a valid selection! Please try again...";
            continue;
        }
    }
}

/****************************************
* Formatted header for viewing APY data *
****************************************/
void Account::viewAPYHeader() {
    cout << "--------------------------------------------------" << endl << endl;
    cout << "View APY Data" << endl << endl;

    cout << "|--------------------------------------|" << endl;
    cout << "| " << setw(15) << left << "Account";
    cout << "| " << setw(6) << "Rate";
    cout << "| " << setw(12) << "Amount" << "|" << endl;
    cout << "|--------------------------------------|" << endl;
}

/************************************************************
* View a formatted list of APY data for a select ed Account *
************************************************************/
void Account::viewAPY(int accID) {
    int num = 1;
    struct APY {
        string accName;
        long double rate;
        long double amount;
    };
    APY* apy;
    vector<APY*> a;

    // Instantiate class objects
    SQL* sql = new SQL();

    // Select APY from DB
    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Interest data from DB
        pstmt = sql->con->prepareStatement("SELECT * FROM acc_apy WHERE acc_id = ? ORDER BY id ASC");
        pstmt->setInt(1, accID);

        res = pstmt->executeQuery();

        while (res->next()) {
            // Get Account name
            apy = new APY;
            apy->accName = getAccName(accID);

            apy->rate = res->getDouble("rate");
            apy->amount = res->getDouble("amount");
            a.push_back(apy);
        }

        // Display Interest Data
        for (size_t c = 0; c < a.size(); c++) {
            cout << "| " << setw(15) << left << a[c]->accName;
            cout << "| " << setw(5) << right << a[c]->rate << "%";
            cout << "|$ " << setw(10) << right << a[c]->amount << " |" << endl;
        }
        cout << "|--------------------------------------|" << endl << endl << endl;

        // Delete SQL objects and close connection
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/************************************
* Edit APY Account, Amount, or Rate *
************************************/
void Account::editAPY(int accID) {
    bool complete = false;
    bool valid = false;
    char charSel = ' ';
    int num = 1;
    int intSel = 0;
    int id = 0;
    enum action { acct=1, apy, min, quit };
    map<action, string> menu{
            {action::acct, "Change Account"},
            {action::apy, "Change Rate"},
            {action::min, "Change Minimum Amount"},
            {action::quit, "Quit"},
    };
    map<action, string>::iterator it;
    struct Interest {
        int id;
        int acc_id;
        string acct;
        long double rate;
        long double amount;
    };
    vector<Interest*> apy_i;

    // Instantiate class objects
    SQL* sql = new SQL();

    cout << "--------------------------------------------------" << endl << endl;
    cout << "Edit APY Data" << endl << endl;

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select APY data from DB
        pstmt = sql->con->prepareStatement("SELECT * FROM acc_apy WHERE acc_id = ?");
        pstmt->setInt(1, accID);

        res = pstmt->executeQuery();

        // Retrieve data
        while (res->next()) {
            Interest* i = new Interest;
            i->id = res->getInt("id");
            i->acc_id = res->getInt("acc_id");
            i->acct = getAccName(i->acc_id);
            i->rate = res->getDouble("rate");
            i->amount = res->getDouble("amount");
            apy_i.push_back(i);
        }

        // Delete SQL objects and close connection
        delete res;
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    while (!complete) {
        // Display APY data and select a record to modify
        num = 1;
        for (size_t c = 0; c < apy_i.size(); c++) {
            cout << num++ << " - " << apy_i[c]->acct << " - " << apy_i[c]->rate << " - " << apy_i[c]->amount << endl;
        }
        cin >> intSel;
        intSel--;
        id = apy_i[intSel]->id;

        // Display Menu
        cout << "Select an action to perform:" << endl;
        for (it = menu.begin(); it != menu.end(); it++) {
            cout << it->first << " - " << it->second << endl;
        }
        cin >> intSel;

        // Perform Action
        switch (intSel) {
            case action::acct: setAPYAcc(id);
                break;
            case action::apy: setAccAPY(id);
                break;
            case action::min: setAPYAmount(id);
                break;
            case action::quit: cout << "Goodbye!" << endl;
                break;
            default: cout << "Incorrect Selection!";
        }

        // Ask User if another Account is needed
        valid = false;
        while (!valid) {
            cout << "Another?: [Y/N]";
            cin >> charSel;
            if (charSel == 'y' || charSel == 'Y' || charSel == 'n' || charSel == 'N') {
                valid = true;
            }
            else {
                cout << charSel << " is not a valid selection! Please try again!" << endl;
                continue;
            }
        }
        if (charSel == 'y' || charSel == 'Y') {
            continue;
        }
        else if (charSel == 'n' || charSel == 'N') {
            complete = true;
        }

        cout << endl << endl;
    }
}

/********************************
* Set new APY Account in the DB *
********************************/
void Account::setAPYAcc(int id) {
    int accID = 0;

    // Instantiate class objects
    SQL* sql = new SQL();

    // Select the new interest account
    accID = accSel(1,0);

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Commit change to DB
        pstmt = sql->con->prepareStatement("UPDATE acc_apy SET acc_id = ? WHERE id = ?");
        pstmt->setInt(1, accID);
        pstmt->setInt(2, id);

        pstmt->executeUpdate();

        // Delete SQL objects and close connection
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/**************************************************
* Set new APY rate for selected Account in the DB *
**************************************************/
void Account::setAccAPY(int id) {
    long double rate = 0.0;

    // Instantiate class objects
    SQL* sql = new SQL();

    // Get new APY rate
    cout << "Enter new Account APY: ";
    cin >> rate;

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Commit change to DB
        pstmt = sql->con->prepareStatement("UPDATE acc_apy SET rate = ? WHERE id = ?");
        pstmt->setDouble(1, rate);
        pstmt->setInt(2, id);

        pstmt->executeUpdate();

        // Delete SQL objects and close connection
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/****************************************************
* Set new APY Amount for selected account in the DB *
****************************************************/
void Account::setAPYAmount(int id) {
    long double amount = 0.0;

    // Instantiate class objects
    SQL* sql = new SQL();

    // Get new APY amount
    cout << "Enter new minimum APY Amount: ";
    cin >> amount;

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Commit change to DB
        pstmt = sql->con->prepareStatement("UPDATE acc_apy SET amount = ? WHERE id = ?");
        pstmt->setDouble(1, amount);
        pstmt->setInt(2, id);

        pstmt->executeUpdate();

        // Delete SQL objects and close connection
        delete pstmt;
        sql->con->close();
        delete sql->con;
    }
    catch (SQLException & e) {
        string file = __FILE__;
        string func = __func__;
        int line = __LINE__;
        sql->catchSQLE(e, file, func, line);
    }
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/********************************
* Calculate new Account balance *
********************************/
long double Account::calcBal(long double amount, long double balance) {
    // Calculate new balance
    if (amount < 0.0) {
        m_balance += abs(amount);
    }
    else {
        m_balance -= amount;
    }
    return m_balance;
}