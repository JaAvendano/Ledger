//
// Created by mojoncaliente on 2/13/20.
//

#ifndef SQL_h
#define SQL_h

#include <mysql/jdbc.h>

using namespace std;
using namespace sql;

class SQL {
private:
    string url = "tcp://127.0.0.1:3306";
    string user = "mcdz";
    string pass = "ihPyN8&l[qo!fchN]4YeULM9,KOEy.FeSh0${[8@W6v=ZhYOeC,dodvn8@{a8t9j";
public:
    SQL() {}
    ~SQL() {
        con->close();
        delete con;
    }

    /**************************************
    * Used for connecting to MySQL Server *
    **************************************/
    void connect(){
        // Get Class instances
        driver = get_driver_instance();
        con = driver->connect(url, user, pass);
        con->setSchema("my_ledger_dev");
    }

    /*************************
    * Catches SQL Exceptions *
    *************************/
    void catchSQLE(SQLException& e, string file, string func, int line){
        cout << "ERROR: SQLException in " << file;
        cout << " (" << func << ") on line " << line << endl;
        cout << "ERROR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << ")" << endl;

        if (e.getErrorCode() == 1047) {
            /*
            Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
            Message: Unknown command
            */
            cout << "\nYour server does not seem to support Prepared Statements at all. ";
            cout << "Perhaps MYSQL < 4.1?" << endl;
        }
    }

    /*************************
    * Catches Runtime Errors *
    *************************/
    void catchRuntimeE(runtime_error& e, string func, int line) {
        cout << "ERROR: runtime_error in " << line;
        cout << " (" << func << ") on line " << line << endl;
        cout << "ERROR: " << e.what() << endl;
    }

    // Instantiates SQL class instances
    Driver* driver;
    Connection* con;
};

#endif
