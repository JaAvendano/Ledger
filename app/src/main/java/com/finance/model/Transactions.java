package com.finance.model;

import com.google.gson.annotations.SerializedName;

import org.jetbrains.annotations.NotNull;

import java.math.BigDecimal;

public class Transactions {

    @SerializedName("id")
    private int id;
    @SerializedName("acc_id")
    private int acc_id;
    @SerializedName("status")
    private String status;
    @SerializedName("day")
    private int day;
    @SerializedName("month")
    private int month;
    @SerializedName("year")
    private int year;
    @SerializedName("name")
    private String name;
    @SerializedName("method")
    private int method;
    @SerializedName("cat")
    private int cat;
    @SerializedName("amount")
    private BigDecimal amount;
    @SerializedName("balance")
    private BigDecimal balance;

    public Transactions() {
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getAcc_id() {
        return acc_id;
    }

    public void setAcc_id(int acc_id) {
        this.acc_id = acc_id;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public int getDay() {
        return day;
    }

    public void setDay(int day) {
        this.day = day;
    }

    public int getMonth() {
        return month;
    }

    public void setMonth(int month) {
        this.month = month;
    }

    public int getYear() {
        return year;
    }

    public void setYear(int year) {
        this.year = year;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getMethod() {
        return method;
    }

    public void setMethod(int method) {
        this.method = method;
    }

    public int getCat() {
        return cat;
    }

    public void setCat(int cat) {
        this.cat = cat;
    }

    public BigDecimal getAmount() {
        return amount;
    }

    public void setAmount(BigDecimal amount) {
        this.amount = amount;
    }

    public BigDecimal getBalance() {
        return balance;
    }

    public void setBalance(BigDecimal balance) {
        this.balance = balance;
    }

    @NotNull
    @Override
    public String toString() {
        return "Transactions{" +
                "id=" + id +
                ", acc_id=" + acc_id +
                ", status='" + status + '\'' +
                ", day=" + day +
                ", month=" + month +
                ", year=" + year +
                ", name='" + name + '\'' +
                ", method=" + method +
                ", cat=" + cat +
                ", amount=" + amount +
                ", balance=" + balance +
                '}';
    }
}
