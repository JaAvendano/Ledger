//
// Created by mojoncaliente on 2/14/20.
//

#include <jni.h>
#include <string>
#include "Menu.h"
#include "Account.h"
#include "Transaction.h"
#include "SQL.h"

JNIEXPORT jboolean JNICALL
Java_com_finance_utilities_JNIImp_accCheck(JNIEnv *env, jobject obj){

    bool acct = false;

    Account* acct = new Account();

    acct = Acct->checkAcc();

    return num *num;

}

JNIEXPORT jboolean JNICALL
Java_com_finance_utilities_JNIImp_transCheck(JNIEnv *env, jobject obj){
    int status = 0;
    int accID = 0;

    Account* acct = new Account();
    Transaction* trans = new Transaction();
    accID = Acct->accSel(1, 0);


}

