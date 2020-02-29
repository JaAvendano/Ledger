package com.finance.rest;

import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class ApiClient {


    private static Retrofit retrofit;
    private static String BASE_URL = "https://Ledger.avsnotes.com";

    public static Retrofit getRetrofit(){

       retrofit = new Retrofit.Builder()
               .baseUrl(BASE_URL)
               .addConverterFactory(GsonConverterFactory.create())
               .build();

       return retrofit;
    }





}
