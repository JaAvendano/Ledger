package com.finance.rest;

import com.finance.model.Transactions;

import java.math.BigDecimal;
import java.util.List;

import retrofit2.Call;
import retrofit2.http.Field;
import retrofit2.http.FormUrlEncoded;
import retrofit2.http.GET;
import retrofit2.http.POST;
import retrofit2.http.Query;

public interface TransactionService {


    @GET("/transactions.php")
    Call<List<Transactions>> getAllAccTrans(@Query("acc_id") int accId);

    @FormUrlEncoded
    @POST("/transactions.php")
    Call<Void> addTransaction(@Field("acc_id") int accId,
                              @Field("status") String status,
                              @Field("day") int day,
                              @Field("month") int month,
                              @Field("year") int year,
                              @Field("name") String name,
                              @Field("method") int method,
                              @Field("cat") int cat,
                              @Field("amount") BigDecimal amount,
                              @Field("balance") BigDecimal balance);
}
