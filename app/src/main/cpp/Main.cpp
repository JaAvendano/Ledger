
#include <iomanip>
#include <windows.h>

#include "boost/smart_ptr.hpp"
#include "mysql/jdbc.h"
#include "Menu.h"
#include "MenuCommand.h"
#include "Account.h"
#include "Transaction.h"
#include "SQL.h"

using namespace std;

// Pre-Program checks
bool accCheck();
bool apyCheck();
bool transCheck();

void setConsoleTextColor(int);

int addAcc();    // Add an Account
int viewAcc();   // View Accounts
int editAcc();   // Edit Account
int addAPY();    // Add APY rates for an Account that accrues interest
int viewAPY();   // View APY data
int editAPY();   // Edit APY data
int addTrans();  // Add a new Transaction
int viewTrans(); // View a fromatted list of Transactions
int recTrans();  // Reconcile pending Transactions
int editTrans(); // Edit a Transaction

int main(int argc, char** argv) {
    string complete = "";
    bool acct = false;
    bool apy = false;
    bool trans = false;

    // Instantiate class objects
    Account* Acct = new Account();

    // Set precision for monetary values
    cout << setprecision(2) << showpoint << fixed;

    while (complete != "Q") {
        acct = accCheck();    // Check DB for any existing Account data
        apy = apyCheck();     // Check DB for Accounts that accrue interest
        trans = transCheck(); // Check DB for any existing Transaction Data

        if (acct == true) {
            cout << endl << endl;
            Acct->accPreviewHeader();
            Acct->accPreviewList();
            Acct->accPreviewTotals();
        }

        // Menu object calls
        Menu* mainMenu; // Displays the Main Menu
        Menu* accMenu; // Displays the Account Menu
        Menu* accAPY; // Displays Interest Menu in Accounts
        Menu* transMenu; // Displays Transaction Menu

        /*********************************
        * Menu Options
        * 1 Accounts
        * - 1 Add Account
        * - 2 View Accounts
        * - 3 Edit Account
        * - 4 Interest
        * - - 1 Add APY
        * - - 2 View APY
        * - - 3 Edit APY
        * 2 Transactions
        * - 1 Add Transactions
        * - 2 View Transactions
        * - 3 Reconcile Transactions
        * - 4 Edit a Transaction
        *********************************/

        // Menu Initialization
        mainMenu = new Menu("1", "Main Menu", "Q");
        accMenu = new Menu("1", "Accounts", "Q");
        mainMenu->addMenuItem(accMenu);
        accMenu->addMenuItem(new MenuCommand("1", "Add Accounts", addAcc));
        if (acct == true) {
            accMenu->addMenuItem(new MenuCommand("2", "View Accounts", viewAcc));
            accMenu->addMenuItem(new MenuCommand("3", "Edit Account", editAcc));
            accMenu->addMenuItem(accAPY = new Menu("4", "Interest", "Q"));
            accMenu->addMenuItem(accAPY);
            accAPY->addMenuItem(new MenuCommand("1", "Add APY", addAPY));
            if (apy == true) {
                accAPY->addMenuItem(new MenuCommand("2", "View APY Data", viewAPY));
                accAPY->addMenuItem(new MenuCommand("3", "Edit APY Data", editAPY));
            }
            transMenu = new Menu("2", "Transactions", "Q");
            mainMenu->addMenuItem(transMenu);
            transMenu->addMenuItem(new MenuCommand("1", "Add Transaction", addTrans));
            if (trans == true) {
                transMenu->addMenuItem(new MenuCommand("2", "View Transactions", viewTrans));
                transMenu->addMenuItem(new MenuCommand("3", "Reconcile Pending Transactions", recTrans));
                transMenu->addMenuItem(new MenuCommand("4", "Edit a Transaction", editTrans));
            }
        }

        //Menu execute
        complete = mainMenu->execute();
    }

    return EXIT_SUCCESS;
}

/*********************************************************************
* Count number of existing Accounts in db                            *
* If there are 0, then only the Add Accounts menu option will appear *
* If there are 1 or more, then more menu options are shown           *
*********************************************************************/
bool accCheck() {
    bool acct = false;

    // Instantiate class objects
    Account* Acct = new Account();

    cout << "Checking for Account data.......";

    // Check for Account data
    acct = Acct->checkAcc();

    if (acct == true) {
        setConsoleTextColor(1);
    }
    else {
        setConsoleTextColor(0);
    }

    return acct;
}

bool apyCheck() {
    bool apy = false;

    // Instantiate class objects
    Account* Acct = new Account();

    cout << "Checking for Interest data......";

    // Find accounts that are eiligble for interest
    apy = Acct->checkAPY();

    if (apy == true) {
        setConsoleTextColor(1);
    }
    else {
        setConsoleTextColor(0);
    }

    return apy;
}

bool transCheck() {
    bool trans = false;

    // Instantiate class objects
    Transaction* Trans = new Transaction();

    cout << "Checking for Transaction data...";

    // Check for Transaction data in DB
    trans = Trans->checkTrans();

    if (trans == true) {
        setConsoleTextColor(1);
    }
    else {
        setConsoleTextColor(0);
    }
    return trans;
}

void setConsoleTextColor(int check) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (check == 0) {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "[NONE]" << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
    else if (check == 1) {
        SetConsoleTextAttribute(hConsole, 2);
        cout << "[OK]" << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
}

/****************************************************
* addAcc() allows the user to add an account        *
* Currently unable to use class object to call      *
* directly to class member functions in menu system *
****************************************************/

int addAcc() {
    int status = 0;

    // Instantiate class objects
    Account* Acct = new Account();

    Acct->addAcc();


    return status;
}

int viewAcc() {
    int status = 0;

    // Instantiate class objects
    Account* Acct = new Account();

    cout << endl << endl << "--------------------------------------------------" << endl << endl;
    cout << "View Accounts" << endl << endl;

    Acct->viewAccHeader();
    Acct->viewAccList();
    Acct->viewAccTotals();

    cin.ignore();
    cout << "Press [Enter] to continue...";
    cin.get();
    cout << endl << endl;

    return status;
}

int editAcc() {
    int status = 0;

    // Instantiate class objects
    Account* Acct = new Account();

    Acct->editAcc();

    return status;
}

int addAPY() {
    int status = 0;

    // Instantiate class objects
    Account* Acct = new Account();

    Acct->setAPY();

    return status;
}

int viewAPY() {
    int status = 0;
    int accID = 0;

    // Instantiate class objects
    Account* Acct = new Account();

    // Select Account
    accID = Acct->accSel(2,0);

    Acct->viewAPYHeader();
    Acct->viewAPY(accID);

    cin.ignore();
    cout << "Press [Enter] to continue...";
    cin.get();

    cout << endl << endl;

    return status;
}

int editAPY() {
    int accID = 0;
    int status = 0;

    // Instantiate class objects
    Account* Acct = new Account();

    // Select Account
    accID = Acct->accSel(2,0);

    // Edit APY data
    Acct->editAPY(accID);

    return status;
}

int addTrans() {
    int status = 0;

    // Instantiate class objects
    Transaction* Trans = new Transaction();

    Trans->addTrans();

    return status;
}

int viewTrans() {
    int status = 0;
    int accID = 0;

    // Instantiate class objects
    Account* Acct = new Account();
    Transaction* Trans = new Transaction();

    // Select an Account
    accID = Acct->accSel(1,0);

    Trans->viewTransHeader(1);
    Trans->viewTransList(accID);

    return status;
}

int recTrans() {
    int status = 0;
    int accID = 0;
    int transID = 0;

    // Instantiate class objects
    Account* Acct = new Account();
    Transaction* Trans = new Transaction();

    // Select an Account
    accID = Acct->accSel(1,0);

    // Select a Transaction to reconcile
    transID = Trans->selTrans(2, accID);

    // Reconcile Transaction
    if (transID > 0) {
        Trans->recTrans(accID, transID);
    }

    return status;
}

int editTrans() {
    int status = 0;
    int accID = 0;
    int transID = 0;

    // Instantiate class objects
    Account* Acct = new Account();
    Transaction* Trans = new Transaction();

    // Select an Account
    accID = Acct->accSel(1, 0);

    // Select a Transaction to edit
    transID = Trans->selTrans(1, accID);

    // Select an action to take
    Trans->editTrans(accID, transID);

    return status;
}

