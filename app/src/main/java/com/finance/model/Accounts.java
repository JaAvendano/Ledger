package com.finance.model;

import com.google.gson.annotations.SerializedName;

import java.math.BigDecimal;
import java.sql.Date;

public class Accounts {

    @SerializedName("id")
    private int id;
    @SerializedName("institution")
    private String institution;
    @SerializedName("type")
    private String type;
    @SerializedName("name")
    private String name;
    @SerializedName("active")
    private int active;
    @SerializedName("balance")
    private BigDecimal balance;


    public Accounts() {
    }


    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getInstitution() {
        return institution;
    }

    public void setInstitution(String institution) {
        this.institution = institution;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getActive() {
        return active;
    }

    public void setActive(int active) {
        this.active = active;
    }

    public BigDecimal getBalance() {
        return balance;
    }

    public void setBalance(BigDecimal balance) {
        this.balance = balance;
    }

    @Override
    public String toString() {
        return "Accounts{" +
                "id=" + id +
                ", institution='" + institution + '\'' +
                ", type='" + type + '\'' +
                ", name='" + name + '\'' +
                ", active=" + active +
                ", balance=" + balance +
                '}';
    }
}

