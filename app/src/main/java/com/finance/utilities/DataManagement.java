package com.finance.utilities;

import android.widget.Toast;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.finance.model.Accounts;
import com.finance.model.Transactions;
import com.finance.rest.AccountService;
import com.finance.rest.ApiClient;
import com.finance.rest.TransactionService;

import org.jetbrains.annotations.NotNull;

import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class DataManagement extends ViewModel {

    private static MutableLiveData<List<Accounts>> accList;
    private static MutableLiveData<Accounts> selectedAcc;
    private static MutableLiveData<Integer> transId;
    private static MutableLiveData<List<Transactions>> transList;
    private static MutableLiveData<Transactions> selectedTrans;


    public static LiveData<List<Accounts>> getAllAccounts(){
        if(accList == null){
            accList = new MutableLiveData<>();
            loadAllAccounts();
        }
        return accList;
    }
    private static void loadAllAccounts(){
        AccountService service = ApiClient.getRetrofit().create(AccountService.class);
        Call<List<Accounts>> call = service.getAllAccounts();
        call.enqueue(new Callback<List<Accounts>>() {
            @Override
            public void onResponse(@NotNull Call<List<Accounts>> call, @NotNull Response<List<Accounts>> response) {
                accList.setValue(response.body());
            }

            @Override
            public void onFailure(@NotNull Call<List<Accounts>> call, @NotNull Throwable t) {
                call.cancel();
            }
        });
    }
    public static LiveData<List<Transactions>> getAllTransactions(int accId){
        if(transList == null){
            transList = new MutableLiveData<>();
            loadAllAccTrans(accId);
        }
        return transList;
    }
    private static void loadAllAccTrans(int accId){
        TransactionService service = ApiClient.getRetrofit().create(TransactionService.class);
        Call<List<Transactions>> call = service.getAllAccTrans(accId);
        call.enqueue(new Callback<List<Transactions>>() {
            @Override
            public void onResponse(Call<List<Transactions>> call, Response<List<Transactions>> response) {
                transList.setValue(response.body());

            }

            @Override
            public void onFailure(Call<List<Transactions>> call, Throwable t) {
                call.cancel();
            }
        });
    }


    public static void setSelectedAcc(Accounts acc){
        selectedAcc = new MutableLiveData<>();
        selectedAcc.setValue(acc);
    }
    public static LiveData<Accounts> getSelectedAcc(){
        return selectedAcc;
    }
    public static void setTransId(int id){
        transId = new MutableLiveData<>();
        transId.setValue(id);
    }
    public static void setSelectedTrans(Transactions trans){
        selectedTrans = new MutableLiveData<>();
        selectedTrans.setValue(trans);
    }
    public static LiveData<Transactions> getSelectedTrans(){
        return selectedTrans;
    }

    public static LiveData<Integer> getTransId(){
        return transId;
    }
}
