//
// Created by mojoncaliente on 2/13/20.
//
#include "Account.h"
#include "Transaction.h"
#include "SQL.h"
#include "Date.h"

#include <string>
#include <iomanip>

using namespace std;

/**********************************************************
* Transaction Constructor for adding new Tranaction to DB *
**********************************************************/
Transaction::Transaction(int acc, string transName, int method, int cat, long double amount, long double balance, string date) {
    // Instantiate class instance
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Add user input into DB
        pstmt = sql->con->prepareStatement("INSERT INTO transactions(acc_id, name, method, cat, amount, balance, date) VALUES (?,?,?,?,?,?,?)");
        pstmt->setInt(1, acc);
        pstmt->setString(2, transName);
        pstmt->setInt(3, method);
        pstmt->setInt(4, cat);
        pstmt->setDouble(5, amount);
        pstmt->setDouble(6, balance);
        pstmt->setString(7, date);

        // Commit data to DB
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
}

/*****************************
* Check for Transaction Data *
*****************************/
bool Transaction::checkTrans() {
    bool trans = false;

    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Check for Transaction data in DB
        pstmt = sql->con->prepareStatement("SELECT * FROM transactions");

        res = pstmt->executeQuery();

        if (res->rowsCount() > 0) {
            trans = true;
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
    return trans;
}

/***********************
*Add a new Transaction *
***********************/
void Transaction::addTrans() {
    bool complete = false;
    bool valid = false;
    char charSel = ' ';
    vector<int> dateVec;

    // Instantiate class objects
    Account* Acct = new Account();
    Date* date = new Date();

    cout << "--------------------------------------------------" << endl << endl;
    cout << "Add Transaction" << endl << endl;

    while (!complete) {
        // Select Transaction type
        m_type = setTransType();
        cout << endl << endl;

        // Select account
        m_acc = Acct->accSel(1,0);
        if (m_type == 3) {
            m_destAcc = Acct->accSel(3,m_acc);
        }
        cout << endl << endl;

        // Date info
        dateVec.clear();
        dateVec.erase(dateVec.begin(), dateVec.begin());
        date->getCurrentDate(dateVec);

        // Convert date to string
        m_date = date->dateToString(dateVec);

        // Get human readable date
        m_rDate = date->rDateString(m_date);

        // Get transaction name
        if (m_type == 1 || m_type == 2) {
            m_transName = setTransName();
        } else if (m_type == 3) {
            m_transName = Acct->getAccName(m_acc);
            m_destTransName = Acct->getAccName(m_destAcc);
        }
        cout << endl << endl;

        // Get Transaction Method of payment
        m_method = setTransMethod(m_type);
        cout << endl << endl;

        // Get Transaction Category
        m_cat = setTransCat(m_type);
        if (m_type == 3) {
            m_destCat = 28;
        }
        cout << endl << endl;

        // Get Transaction amount
        m_amount = setAmount(m_type);
        cout << "Amount: " << m_amount << endl;
        if (m_type == 3) {
            m_destAmount = m_amount;
            m_amount = -m_amount;
        }
        cout << endl << endl;

        // Set balance = 0.0
        m_balance = 0.0;

        // Verify input
        valid = false;
        while (!valid) {
            // Get string representation for method
            m_methodStr = getMethodName(m_method);

            // Get string representation for category
            m_catStr = getCatName(m_cat);

            // Get Account name
            if (m_type == 3) {
                m_transName = Acct->getAccName(m_acc);
                m_destTransName = Acct->getAccName(m_destAcc);
            }

            // Display the input for user verification
            if (m_type == 1 || m_type == 2) {
                cout << m_rDate << " - " << m_transName << " - " << m_methodStr << " - " << m_catStr << " - " << m_amount << endl;
            }
            else if (m_type == 3) {
                cout << m_rDate << " - " << m_destTransName << " - " << m_methodStr << " - " << m_catStr << " - " << m_amount << endl;
                cout << m_rDate << " - " << m_transName << " - " << m_methodStr << " - " << m_catStr << " - " << m_destAmount << endl;
            }
            cout << "Is this correct? [Y/N]";
            cin >> charSel;
            cout << endl << endl;
            if (charSel == 'y' || charSel == 'Y' || charSel == 'n' || charSel == 'N') {
                valid = true;
            }
            else {
                cout << charSel << " is not a valid selection! Please select Y or N.";
                cin.clear();
                cin.ignore();
                cout << endl << endl;
            }
        }
        if (charSel == 'n' || charSel == 'N') {
            continue;
        }

        // Instantiate class objects for adding Transactions to DB
        Transaction* Trans;
        Transaction* DestTrans;

        // Add Transaction to DB
        if (m_type == 1 || m_type == 2) {
            Trans = new Transaction(m_acc, m_transName, m_method, m_cat, m_amount, m_balance, m_date);
        }
        else if (m_type == 3) {
            Trans = new Transaction(m_acc, m_destTransName, m_method, m_cat, m_amount, m_balance, m_date);
            DestTrans = new Transaction(m_destAcc, m_transName, m_method, m_cat, m_destAmount, m_balance, m_date);
        }

        // Ask user if they want to enter another transaction
        valid = false;
        charSel = ' ';
        while (!valid) {
            cout << "Another? [Y/N]: ";
            cin >> charSel;
            cout << endl << endl;
            if (charSel == 'n' || charSel == 'N' || charSel == 'y' || charSel == 'Y') {
                valid = true;
            }
            else {
                cout << charSel << " is not a valid selection! Please select Y or N";
                cin.clear();
                cin.ignore();
                cout << endl << endl;
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

/********************************************************
* Set the Transaction Type for adding a new Transaction *
********************************************************/
int Transaction::setTransType() {
    bool valid = false;

    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select all data from trans_type
        pstmt = sql->con->prepareStatement("SELECT * FROM trans_type ORDER BY id ASC");
        res = pstmt->executeQuery();
        while (!valid) {
            cout << "Select Transaction type:" << endl;
            while (res->next()) {
                cout << res->getInt("id") << " - " << res->getString("type") << endl;
            }
            cin >> m_type;
            cout << endl;
            if (m_type < 1 || m_type > res->rowsCount()) {
                // If user selects a number outside of the range of selections
                cout << m_type << " is not a valid selection! Please select a number from 1 to " << res->rowsCount();
                cin.clear();
                cin.ignore();
                cout << endl << endl;
            }
            else {
                valid = true;
            }
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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    return m_type;
}

/****************************************************
* Set Transaction Name for adding a new Transaction *
****************************************************/
string Transaction::setTransName() {
    cin.ignore();
    cout << "Enter Transaction name: ";
    getline(cin, m_transName);

    return m_transName;
}

/******************************************************
* Set Transaction Method for adding a new Transaction *
******************************************************/
int Transaction::setTransMethod(int type) {
    bool valid = false;

    // Instantiate class objects
    SQL* sql = new SQL();

    if (type == 1) {
        valid = false;
        while (!valid) {
            cout << "Select method of payment:" << endl;
            try {
                // Instantiate SQL objects
                sql->connect();
                PreparedStatement* pstmt;
                ResultSet* res;

                // Select data from trans_method
                pstmt = sql->con->prepareStatement("SELECT * FROM trans_method");
                res = pstmt->executeQuery();
                while (res->next()) {
                    cout << res->getInt("id") << " - " << res->getString("method") << endl;
                }
                cin >> m_method;
                if (m_method < 1 || m_method > res->rowsCount()) {
                    cout << m_method << " is not a valid selection! Please try again!" << endl;
                    cin.clear();
                    cin.ignore();
                    continue;
                }
                else {
                    valid = true;
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
            catch (std::runtime_error & e) {
                string func = __func__;
                int line = __LINE__;
                sql->catchRuntimeE(e, func, line);
            }
        }
    }
    else if (type == 2) {
        m_method = 6; // N/A
    }
    else if (type == 3) {
        m_method = 5; // Transfer
    }
    return m_method;
}


/**********************************************************
* Set a Transaction category for adding a new Transaction *
**********************************************************/
int Transaction::setTransCat(int type) {
    bool valid = false;
    int num = 1;
    string stmt = "";
    struct Cat {
        int id;
        string cat;
    };
    Cat* cat;
    vector<Cat*> c;

    // Instantiate class objects
    SQL* sql = new SQL();

    if (type == 1 || type == 2) {
        while (!valid) {
            cout << "Select transaction category:" << endl;
            try {
                // Instantiate SQL objects
                sql->connect();
                PreparedStatement* pstmt;
                ResultSet* res;

                if (type == 1) {
                    stmt = "SELECT * FROM trans_cat WHERE id > 8";
                }
                else if (type == 2) {
                    stmt = "SELECT * FROM trans_cat WHERE id < 9";
                }

                // Select Transaction categories from DB
                pstmt = sql->con->prepareStatement(stmt);
                res = pstmt->executeQuery();

                c.clear();
                while (res->next()) {
                    cat = new Cat;
                    cat->id = res->getInt("id");
                    cat->cat = res->getString("cat");
                    c.push_back(cat);
                }

                num = 1;
                for (size_t i = 0; i < c.size(); i++) {
                    cout << num++ << " - " << c[i]->cat << endl;
                }
                cin >> m_cat;
                m_cat--;
                m_cat = c[m_cat]->id;

                // Check input
                if (m_cat < 1 || m_cat > 26) {
                    cout << m_cat << " is not a valid input! Please Try again!";
                    cin.clear();
                    cin.ignore();
                    cout << endl << endl;
                }
                else {
                    valid = true;
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
            catch (std::runtime_error & e) {
                string func = __func__;
                int line = __LINE__;
                sql->catchRuntimeE(e, func, line);
            }
        }
    }
    else if (type == 3) {
        m_cat = 26;
    }

    return m_cat;
}

/*******************************
* Set new date for Transaction *
*******************************/
string Transaction::setDate() {
    int day = 0;
    int month = 0;
    int year = 0;
    vector<int> dateVec;

    // Instantiate class objects
    Date* date = new Date();

    // Prompt user for new date
    cout << "Set date [MM DD YYYY]: ";
    cin >> month;
    cin >> day;
    cin >> year;

    // Insert into vector
    dateVec.push_back(year);
    dateVec.push_back(month);
    dateVec.push_back(day);

    // Convert from vector<int> to string
    m_date = date->dateToString(dateVec);

    cout << endl << endl;

    return m_date;
}

/********************************************************
* Set a Transaction Amount for adding a new Transaction *
********************************************************/
long double Transaction::setAmount(int type) {
    cout << "Enter transaction amount: ";
    cin >> m_amount;
    if (type == 1) {
        m_amount = -m_amount;
    }
    return m_amount;
}

/*************************
* Get Transaction status *
*************************/
int Transaction::getStatus(int transID) {
    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Transaction status from DB
        pstmt = sql->con->prepareStatement("SELECT pending FROM transactions WHERE id = ?");
        pstmt->setInt(1, transID);

        res = pstmt->executeQuery();

        while (res->next()) {
            m_status = res->getInt("pending");
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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return m_status;
}

/*************************
* Get Transaction Method *
*************************/
int Transaction::methodSel() {
    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Transaction method from DB
        pstmt = sql->con->prepareStatement("SELECT * FROM trans_method");
        res = pstmt->executeQuery();

        while (res->next()) {
            cout << res->getInt("id") << " - " << res->getString("method") << endl;
        }
        cin >> m_method;

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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return m_method;
}

/***************************
* Get Transaction Category *
***************************/
int Transaction::catSel() {
    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Transaction method from DB
        pstmt = sql->con->prepareStatement("SELECT * FROM trans_cat");
        res = pstmt->executeQuery();

        while (res->next()) {
            cout << res->getInt("id") << " - " << res->getString("cat") << endl;
        }
        cin >> m_cat;

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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return m_cat;
}

/********************************
* Get a Transaction method name *
********************************/
string Transaction::getMethodName(int method) {
    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Method name from DB
        pstmt = sql->con->prepareStatement("SELECT method FROM trans_method WHERE id = ?");
        pstmt->setInt(1, method);

        res = pstmt->executeQuery();

        while (res->next()) {
            m_methodStr = res->getString("method");
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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return m_methodStr;
}

/********************************
* Get Transaction category name *
********************************/
string Transaction::getCatName(int cat) {
    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Method name from DB
        pstmt = sql->con->prepareStatement("SELECT cat FROM trans_cat WHERE id = ?");
        pstmt->setInt(1, cat);

        res = pstmt->executeQuery();

        while (res->next()) {
            m_catStr = res->getString("cat");
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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return m_catStr;
}

/***********************************
* Get Transaction Amount           *
* Task 1: All Transactions         *
* Task 2: All Pending Transactions *
***********************************/
long double Transaction::getAmount(int transID) {
    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Transaction Amount
        pstmt = sql->con->prepareStatement("SELECT amount FROM transactions WHERE id = ?");
        pstmt->setInt(1, transID);

        res = pstmt->executeQuery();

        while (res->next()) {
            m_amount = res->getDouble("amount");
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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return m_amount;
}

/***************************************************
* View foratted Transaction Header for viewTrans() *
***************************************************/
void Transaction::viewTransHeader(int task) {
    cout << endl << endl;
    cout << "--------------------------------------------------" << endl << endl;
    cout << "View Transactions" << endl << endl;

    if (task == 1) {
        cout << "|--------------------------------------------------------------------------------------------|" << endl;
    }
    else if (task == 2) {
        cout << "|------------------------------------------------------------------------------------|" << endl;
        cout << "| # ";
    }
    cout << "|" << setw(12) << left << " Date " << "|";
    cout << setw(26) << left << " Name " << "|";
    cout << setw(11) << " Method " << "|";
    cout << setw(16) << " Category " << "|";
    cout << setw(11) << "  Amount " << "|";
    if (task != 2) {
        cout << setw(11) << " Balance " << "|" << endl;
    }
    if (task == 1) {
        cout << "|--------------------------------------------------------------------------------------------|" << endl;
    }
    else if (task == 2) {
        cout << endl << "|------------------------------------------------------------------------------------|" << endl;
    }
}

/**************************************************
* View formatted Transaction list for viewTrans() *
**************************************************/
void Transaction::viewTransList(int accID) {
    int num = 1;
    string stmt = "";
    vector<Transaction*> trans;
    vector<Transaction*>::iterator i;

    // Instantiate class objects
    SQL* sql = new SQL();
    Account* Acct = new Account();
    Transaction* Trans;
    Date* date = new Date();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select all transactions from transactions table that correspond with the user selected account
        pstmt = sql->con->prepareStatement("SELECT transactions.*, trans_method.method, trans_cat.cat FROM transactions "
                                           "LEFT JOIN (trans_method) ON (transactions.method = trans_method.id) "
                                           "LEFT JOIN (trans_cat) ON (transactions.cat = trans_cat.id) WHERE acc_id = ? "
                                           "ORDER BY transactions.pending, transactions.date, transactions.id ASC LIMIT 300");
        pstmt->setInt(1, accID);
        res = pstmt->executeQuery();

        // Insert selected data into a vector
        while (res->next()) {
            m_id = res->getInt("id");
            m_status = res->getInt("pending");
            m_date = res->getString("date");
            m_rDate = date->transRDateString(m_date);
            m_transName = res->getString("name");
            m_methodStr = res->getString("method");
            m_catStr = res->getString("cat");
            m_amount = res->getDouble("amount");
            m_acc = res->getInt("acc_id");
            m_accName = "";
            m_balance = res->getDouble("balance");
            Trans = new Transaction(m_id, m_acc, m_accName, m_status, m_transName, m_methodStr, m_catStr, m_amount, m_balance, m_rDate);
            trans.push_back(Trans);
        }

        // Calculate the Projected balance
        // based on the previous transaction's balance
        for (size_t c = 0; c < trans.size(); c++) {
            if (trans[c]->m_balance == 0.0) {
                if (c == 0) {
                    trans[c]->m_balance = Acct->getBalance(accID);
                    trans[c]->m_balance += trans[c]->m_amount;
                }
                else {
                    // If the iterator is != 0, decrement the
                    // iterator to get the last transaction balance
                    // and increment the iterator back to where it
                    // belongs to carry on the operation as normal
                    --c;
                    m_balance = trans[c]->m_balance;
                    ++c;
                    trans[c]->m_balance += (m_balance + trans[c]->m_amount);
                }
            }
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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    // Display Trasnaction data
    for (i = trans.begin(); i != trans.end(); i++) {
        if (i.operator*()->m_status == 1) {
            cout << "| " << setw(11) << left << "Pending" << "| ";
        }
        else if (i.operator*()->m_status == 0) {
            cout << "| " << setw(11) << left << i.operator*()->m_rDate << "| ";
        }
        cout << setw(25) << left << i.operator*()->m_transName << "| ";
        cout << setw(10) << left << i.operator*()->m_methodStr << "| ";
        cout << setw(15) << left << i.operator*()->m_catStr << "|$ ";
        cout << setw(9) << right << i.operator*()->m_amount << "|$ ";
        cout << setw(9) << right << i.operator*()->m_balance << "|";
        cout << endl;
    }

    cout << "|--------------------------------------------------------------------------------------------|" << endl;
    cout << "Press [Enter] to continue..." << endl << endl;
    cin.ignore();
    cin.get();
}

/***********************************************************************
* Sum the amounts of the peding transactions for the Projected Balance *
***********************************************************************/
long double Transaction::sumPTrans(int accID) {
    // Instantiate class objects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Pending Transactions for selected account
        pstmt = sql->con->prepareStatement("SELECT SUM(amount) AS total FROM transactions WHERE pending = 1 AND acc_id = ?");
        pstmt->setInt(1, accID);

        res = pstmt->executeQuery();

        while (res->next()) {
            m_amount = res->getDouble("total");
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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
    return m_amount;
}

/****************************************
* Select a Transaction                  *
* Task 1: Normal Transaction selection  *
* Task 2: Pending Transaction Selection *
****************************************/
int Transaction::selTrans(int task, int accID) {
    bool complete = false;
    int num = 1;
    int intSel = 0;
    string stmt = "";
    vector<Transaction*> trans;
    vector<Transaction*>::iterator i;

    // Instantiate class objects
    SQL* sql = new SQL();
    Transaction* Trans;
    Date* date = new Date();

    viewTransHeader(2);

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        if (task == 1) {
            stmt = "SELECT transactions.*, trans_method.method, trans_cat.cat FROM transactions "
                   "LEFT JOIN (trans_method) ON (transactions.method = trans_method.id) "
                   "LEFT JOIN (trans_cat) ON (transactions.cat = trans_cat.id) WHERE acc_id = ? "
                   "ORDER BY transactions.id ASC LIMIT 300";
        }
        else if (task == 2) {
            stmt = "SELECT transactions.*, trans_method.method, trans_cat.cat FROM transactions "
                   "LEFT JOIN (trans_method) ON (transactions.method = trans_method.id) "
                   "LEFT JOIN (trans_cat) ON (transactions.cat = trans_cat.id) WHERE acc_id = ? AND pending = 1 "
                   "ORDER BY transactions.id ASC LIMIT 300";
        }

        // Select Transaction data from DB
        pstmt = sql->con->prepareStatement(stmt);
        pstmt->setInt(1, accID);
        res = pstmt->executeQuery();

        // Insert selected data into a vector
        while (res->next()) {
            m_id = res->getInt("id");
            m_status = res->getInt("pending");
            m_date = res->getString("date");
            m_rDate = date->transRDateString(m_date);
            m_transName = res->getString("name");
            m_methodStr = res->getString("method");
            m_catStr = res->getString("cat");
            m_amount = res->getDouble("amount");
            m_accName = "";
            m_balance = 0.0;
            Trans = new Transaction(m_id, m_acc, m_accName, m_status, m_transName, m_methodStr, m_catStr, m_amount, m_balance, m_rDate);
            trans.push_back(Trans);
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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    if (trans.size() == 0) {
        cout << "There are no pending transactions at this time!" << endl << endl;
        cout << "Press [Enter] to continue..." << endl;
        cin.ignore();
        cin.get();
        m_id = 0;
        complete = true;
    }
    else {
        for (i = trans.begin(); i != trans.end(); i++) {
            cout << "| " << num++ << " ";
            if (m_status == 1) {
                cout << "| " << setw(11) << left << "Pending" << "| ";
            }
            else {
                cout << "| " << setw(11) << left << i.operator*()->m_rDate << "| ";
            }
            cout << setw(25) << left << i.operator*()->m_transName << "| ";
            cout << setw(10) << left << i.operator*()->m_methodStr << "| ";
            cout << setw(15) << left << i.operator*()->m_catStr << "|$ ";
            cout << setw(9) << right << i.operator*()->m_amount << "|";
            cout << endl;
        }
        cout << "|------------------------------------------------------------------------------------|" << endl << endl;
        cin >> intSel;
        intSel--;
        m_id = trans[intSel]->m_id;
    }
    cout << endl << endl;

    return m_id;
}

/**********************************
* Reconcile a pending Transaction *
**********************************/
bool Transaction::recTrans(int accID, int transID) {
    bool complete = false;
    char charSel = ' ';

    // Instantiate class onjects
    SQL* sql = new SQL();
    Account* Acct = new Account();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Update Transaction status
        cout << "Changing Pending Status..." << endl;
        pstmt = sql->con->prepareStatement("UPDATE transactions SET pending = 0 WHERE id = ?");
        pstmt->setInt(1, transID);

        pstmt->executeUpdate();

        // Delete pointers and close connection
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

    // Get Transaction amount
    m_amount = getAmount(transID);

    // Get Account Balance
    m_balance = Acct->getBalance(accID);

    // Calculate new balance
    m_balance += m_amount;

    // Update Account balacne
    Acct->DBUpdateAccBal(accID, m_balance);

    // Update Transaction balance
    DBUpdateBalance(transID, m_balance);

    // Set new date for Transaction
    m_date = setDate();

    // Update Transaction date
    DBUpdateTransDate(transID, m_date);

    // Prompt user if finished
    cout << "Another [Y/N]: ";
    cin >> charSel;
    if (charSel == 'n' || charSel == 'N') {
        complete = true;
    }

    cout << endl << endl;

    return complete;
}

/*******************************************************
* Updates the Transaction Balance through iteration    *
* so as to get the Transaction balance previous to the *
* currently selected Transaction                       *
*******************************************************/
void Transaction::updateBalance(int accID, int transID) {
    struct Trans {
        int id;
        long double amount;
        long double balance;
    };
    Trans* tdata;
    vector<Trans*> trans;

    // Instantiate class onjects
    SQL* sql = new SQL();
    Account* Acct = new Account();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select all Transaction Balances for an Account from DB
        pstmt = sql->con->prepareStatement("SELECT id, amount, balance FROM transactions WHERE acc_id = ?");
        pstmt->setInt(1, accID);

        res = pstmt->executeQuery();

        while (res->next()) {
            tdata = new Trans;
            tdata->id = res->getInt("id");
            tdata->amount = res->getDouble("amount");
            tdata->balance = res->getDouble("balance");
            trans.push_back(tdata);
        }

        // Calculate new balance
        for (size_t c = 0; c < trans.size(); c++) {
            if (trans[c]->id == transID) {
                c--;
                m_balance = trans[c]->balance;
                cout << "Balance: " << m_balance << endl;
                c++;
                m_balance += trans[c]->amount;
            }
        }

        // Update Transaction balance
        pstmt = sql->con->prepareStatement("UPDATE transactions SET balance = ? WHERE id = ?");
        pstmt->setDouble(1, m_balance);
        pstmt->setInt(2, transID);

        pstmt->executeUpdate();

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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/****************************
* Update Transaction Status *
****************************/
void Transaction::DBUpdateStatus(int transID) {
    // Instantiate class onjects
    SQL* sql = new SQL();
    Account* Acct = new Account();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Transaction status from DB
        pstmt = sql->con->prepareStatement("SELECT pending, acc_id, amount FROM transactions WHERE id = ?");
        pstmt->setInt(1, transID);

        res = pstmt->executeQuery();

        while (res->next()) {
            m_status = res->getInt("pending");
            m_acc = res->getInt("acc_id");
            m_amount = res->getDouble("amount");
        }

        // Change Transaction status to opposite
        if (m_status == 1) {
            m_status = 0;
        }
        else {
            m_status = 1;
        }

        // Update Transaction status in DB
        pstmt = sql->con->prepareStatement("UPDATE transactions SET pending = ? WHERE id = ?");
        pstmt->setInt(1, m_status);
        pstmt->setInt(2, transID);

        pstmt->executeUpdate();

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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }

    // Update Account Balance Accordingly

    // Get Account balance
    m_balance = Acct->getBalance(m_acc);

    // Get Transaction amount
    m_amount = getAmount(transID);

    // Calculate new Account Balance
    if (m_status == 0) {
        m_balance += m_amount;

        // Update Account balance
        Acct->DBUpdateAccBal(m_acc, m_balance);

        // Clear the Transaction Balance
        DBUpdateBalance(transID, m_balance);
    }
    else {
        Acct->calcBal(m_amount, m_balance);

        // Update Account balance
        Acct->DBUpdateAccBal(m_acc, m_balance);

        // Clear the Transaction Balance
        DBUpdateBalance(transID, 0.0);
    }
}

/********************************
* Update Transaction name in DB *
********************************/
void Transaction::DBUpdateName(int transID) {
    // Instantiate class onjects
    SQL* sql = new SQL();

    cin.ignore();
    // Prompt user for a new Transaction name
    cout << "Enter a new Transaction name: ";
    getline(cin, m_transName);

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Update Transaction balance
        pstmt = sql->con->prepareStatement("UPDATE transactions SET name = ? WHERE id = ?");
        pstmt->setString(1, m_transName);
        pstmt->setInt(2, transID);

        pstmt->executeUpdate();

        // Delete pointers and close connection
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

/**************************************
* Update Transaction method in the DB *
**************************************/
void Transaction::DBUpdateMethod(int transID) {
    // Instantiate class onjects
    SQL* sql = new SQL();

    cin.ignore();
    // Prompt user for a new Transaction method
    cout << "Select a new Transaction method: " << endl;
    m_method = methodSel();


    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Update Transaction balance
        pstmt = sql->con->prepareStatement("UPDATE transactions SET method = ? WHERE id = ?");
        pstmt->setInt(1, m_method);
        pstmt->setInt(2, transID);

        pstmt->executeUpdate();

        // Delete pointers and close connection
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

/**************************************
* Update Transaction method in the DB *
**************************************/
void Transaction::DBUpdateCat(int transID) {
    // Instantiate class onjects
    SQL* sql = new SQL();

    cin.ignore();
    // Prompt user for a new Transaction method
    cout << "Select a new Transaction category: " << endl;
    m_cat = catSel();


    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Update Transaction balance
        pstmt = sql->con->prepareStatement("UPDATE transactions SET cat = ? WHERE id = ?");
        pstmt->setInt(1, m_cat);
        pstmt->setInt(2, transID);

        pstmt->executeUpdate();

        // Delete pointers and close connection
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

/**************************************
* Update Transaction amount in the DB *
**************************************/
void Transaction::DBUpdateAmount(int accID, int transID, long double amount) {
    // Instantiate class onjects
    SQL* sql = new SQL();
    Account* Acct = new Account();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Update Transaction balance
        pstmt = sql->con->prepareStatement("UPDATE transactions SET amount = ? WHERE id = ?");
        pstmt->setDouble(1, amount);
        pstmt->setInt(2, transID);

        pstmt->executeUpdate();

        // Delete pointers and close connection
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

    // If the Transaction was not pending,
    // we will need to update the Account Balance
    m_status = getStatus(transID);
    if (m_status == 0) {
        // Get Account Balance
        m_balance = Acct->getBalance(accID);

        // calculate new balance
        m_balance += m_amount;

        // Update Account Balance
        Acct->DBUpdateAccBal(accID, m_balance);

        // Update Transaction balance
        DBUpdateBalance(transID, m_balance);
    }
}

/********************************************
* Update the Transaction balance in the     *
* DB when Reconciling a pending Transaction *
********************************************/
void Transaction::DBUpdateBalance(int transID, long double balance) {
    // Instantiate class onjects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Update Transaction balance
        pstmt = sql->con->prepareStatement("UPDATE transactions SET balance = ? WHERE id = ?");
        pstmt->setDouble(1, balance);
        pstmt->setInt(2, transID);

        pstmt->executeUpdate();

        // Delete pointers and close connection
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

/*****************************************
* Update the Transaction date in the DB  *
* when reconciling a pending Transaction *
*****************************************/
void Transaction::DBUpdateTransDate(int transID ,string date) {
    int day = 0;
    int month = 0;
    int year = 0;
    vector<int> dateVec;

    // Instantiate class onjects
    SQL* sql = new SQL();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Update Transaction balance
        pstmt = sql->con->prepareStatement("UPDATE transactions SET date = ? WHERE id = ?");
        pstmt->setString(1, date);
        pstmt->setInt(2, transID);

        pstmt->executeUpdate();

        // Delete pointers and close connection
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

/*****************************
* Update Transaction Account *
*****************************/
void Transaction::DBUpdateTransAcc(int oldAcc, int transID) {
    int newAcc = 0;
    long double old_acc = 0.0;
    long double new_acc = 0.0;

    // Instantiate class objects
    SQL* sql = new SQL();
    Account* Acct = new Account();

    // Prompt user for new Transaction Account
    newAcc = Acct->accSel(3, oldAcc);

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;

        // Update Transaction Account
        pstmt = sql->con->prepareStatement("UPDATE transactions SET acc_id = ? WHERE id = ?");
        pstmt->setInt(1, newAcc);
        pstmt->setInt(2, transID);

        pstmt->executeUpdate();

        // Delete pointers and close connection
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

    // Get Transaction status
    m_status = getStatus(transID);

    if (m_status == 0) {
        // Get the Account balance
        old_acc = Acct->getBalance(oldAcc);
        new_acc = Acct->getBalance(newAcc);

        // Get Transaction amount
        m_amount = getAmount(transID);

        // Calculate new Balances
        if (m_amount < 0) {
            old_acc += abs(m_amount);
            new_acc += m_amount;
        }
        else if (m_amount > 0) {
            old_acc += m_amount;
            new_acc += -m_amount;
        }

        // Update Transactioin Balance
        updateBalance(newAcc, transID);

        // Update Account balances
        Acct->DBUpdateAccBal(oldAcc, old_acc);
        Acct->DBUpdateAccBal(newAcc, new_acc);

        cout << endl << endl;
    }
}

/******************************
* Edit the individual details *
* of a Transaction            *
******************************/
void Transaction::editTrans(int accID, int transID) {
    bool valid = false;
    int intSel = 0;

    // Instantiate a new class object
    Account* Acct = new Account();

    // Enum Map for a menu
    enum action { acc = 1, status, name, method, cat, amount, date, quit };
    map<action, string> menu{
            {action::acc,"Change Transaction Account"},
            {action::status,"Change Transaction Status"},
            {action::name,"Change Transaction Name"},
            {action::method,"Change Transaction Method"},
            {action::cat,"Change Transaction Category"},
            {action::amount,"Change Transaction Amount"},
            {action::date,"Change Transaction Date"},
            {action::quit,"Quit"}
    };
    map<action, string>::iterator i;

    while (!valid) {
        // Display the menu and make a selection
        for (i = menu.begin(); i != menu.end(); i++) {
            cout << i->first << " - " << i->second << endl;
        }
        cin >> intSel;

        switch (intSel) {
            case action::acc: DBUpdateTransAcc(accID, transID);
                break;
            case action::status: DBUpdateStatus(transID);
                break;
            case action::name: DBUpdateName(transID);
                break;
            case action::method: DBUpdateMethod(transID);
                break;
            case action::cat: DBUpdateCat(transID);
                break;
            case action::amount: editAmount(accID, transID);
                break;
            case action::date: editDate(transID);
                break;
            case action::quit: cout << "Goodbye!" << endl << endl;
                valid = true;
                break;
            default: cout << intSel << " is an incorrect selection, please try again!" << endl;
                cin.clear();
                cin.get();
        }
    }
}

/**************************
* Edit Transaction amount *
**************************/
void Transaction::editAmount(int accID, int transID) {
    // Instantiate class objects
    Account* Acct = new Account();

    // Remove current Transaction amount and balance
    removeAmount(accID, transID);

    // Get new Transaction type
    m_type = setTransType();

    // Get new Transaction amount
    m_amount = setAmount(m_type);
    cout << "Amount1: " << m_amount << endl;

    // Update Transaction amount
    DBUpdateAmount(accID, transID, m_amount);
}

/******************************************************************
* Remove Trasnaction amount and balance to make room for new ones *
******************************************************************/
void Transaction::removeAmount(int accID, int transID) {
    // Instantiate class objects
    SQL* sql = new SQL();
    Account* Acct = new Account();

    try {
        // Instantiate SQL objects
        sql->connect();
        PreparedStatement* pstmt;
        ResultSet* res;

        // Select Transaction amount
        pstmt = sql->con->prepareStatement("SELECT amount FROM transactions WHERE id = ?");
        pstmt->setInt(1, transID);

        res = pstmt->executeQuery();

        while (res->next()) {
            m_amount = res->getDouble("amount");
        }

        // Get Account balance
        m_balance = Acct->getBalance(accID);

        // Calculate new balance
        m_balance = Acct->calcBal(m_amount, m_balance);

        // Update Account balance
        Acct->DBUpdateAccBal(accID, m_balance);

        pstmt = sql->con->prepareStatement("UPDATE transactions SET amount = 0.0, balance = 0.0 WHERE id = ?");
        pstmt->setInt(1, transID);

        pstmt->executeUpdate();

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
    catch (std::runtime_error & e) {
        string func = __func__;
        int line = __LINE__;
        sql->catchRuntimeE(e, func, line);
    }
}

/************************
* Edit Transaction date *
************************/
void Transaction::editDate(int transID) {
    // Get new date
    m_date = setDate();

    // Update Transaction date
    DBUpdateTransDate(transID, m_date);
}