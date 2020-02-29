package com.finance.rest;

import com.finance.model.Accounts;

import java.math.BigDecimal;
import java.util.List;

import retrofit2.Call;
import retrofit2.http.Field;
import retrofit2.http.FormUrlEncoded;
import retrofit2.http.GET;
import retrofit2.http.POST;

public interface AccountService {




    @GET("/accounts.php")
    Call<List<Accounts>> getAllAccounts();

    @FormUrlEncoded
    @POST("/accounts.php")
    Call<Void> addAccount(@Field("institution") String institution,
                          @Field("type") String type,
                          @Field("name") String name,
                          @Field("active") int active,
                          @Field("balance")BigDecimal balance);




}
