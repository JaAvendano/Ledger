package com.finance.rest;

import com.finance.model.Accounts;

import org.jetbrains.annotations.NotNull;

import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class RestRepo {

    private List<Accounts> accList;
    private AccountService service;
    private Call<List<Accounts>> call;
    private RestCallback callback;

    public RestRepo(){
        callback = new RestCallback();
    }

    public void makeVoidCall(){
        service = ApiClient.getRetrofit().create(AccountService.class);
        call = service.getAllAccounts();
        call.enqueue(callback);
    }


}
