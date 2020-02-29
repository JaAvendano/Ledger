package com.finance.rest;

import com.finance.model.Accounts;

import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class RestCallback implements Callback<List<Accounts>> {
    private List<Accounts> accountsList;

    private List<Accounts> getAccountsList(){
        return accountsList;
    }
    @Override
    public void onResponse(Call<List<Accounts>> call, Response<List<Accounts>> response) {

        if(response.isSuccessful()){
            accountsList = response.body();
        }
    }

    @Override
    public void onFailure(Call<List<Accounts>> call, Throwable t) {

        call.cancel();
    }
}
