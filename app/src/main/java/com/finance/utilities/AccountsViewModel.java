package com.finance.utilities;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.finance.model.Accounts;
import com.finance.rest.RestRepo;

import java.util.List;

public class AccountsViewModel extends ViewModel {
    private final MutableLiveData<List<Accounts>> accountsList = new MutableLiveData<>();
    private final MutableLiveData<Accounts> selectedAcc = new MutableLiveData<>();

    public MutableLiveData<List<Accounts>> getAccList(List<Accounts> itemList){
        return accountsList;
    }
    public MutableLiveData<Accounts> getSelectedAcc(){
        return selectedAcc;
    }
    public void setAccList(List<Accounts> accList){
        accountsList.setValue(accList);
    }
    public void setAccount(Accounts account){
        selectedAcc.setValue(account);
    }

    /*
    public Accounts fetchAccById(int accId){

    }
    public List<Accounts> fetchAllAccounts(){

        RestRepo repo = new RestRepo();

    }
    */

}
