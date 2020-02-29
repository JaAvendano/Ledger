package com.finance.utilities;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;


import com.finance.ledger.R;
import com.finance.model.Transactions;

import java.util.List;

public class TransactionsCardViewAdaptor extends RecyclerView.Adapter<TransactionsCardViewAdaptor.TransViewHolder>{

    private List<Transactions> transList;
    private ClickedItemListener itemListener;

    public TransactionsCardViewAdaptor(List<Transactions> itemList){
        this.transList = itemList;
    }

    public interface ClickedItemListener{

        void clickedItem(int position);
    }
    public void getClickedItem(ClickedItemListener clickedItem){
        itemListener = clickedItem;
    }

    static class TransViewHolder extends RecyclerView.ViewHolder{
        private TextView day;
        private TextView name;
        private TextView amount;
        private TextView balance;

        private TransViewHolder(@NonNull View v, ClickedItemListener listener) {
            super(v);

            day = v.findViewById(R.id.transDay);
            name = v.findViewById(R.id.transName);
            amount = v.findViewById(R.id.transAmount);
            balance = v.findViewById(R.id.transAccBal);
            v.setOnClickListener(v1 -> {
                if(listener != null){
                    int position = getAdapterPosition();
                    if(position != RecyclerView.NO_POSITION){
                        listener.clickedItem(position);
                    }
                }
            });
        }
    }

    @NonNull
    @Override
    public TransViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.transaction_cardview, parent, false);
        TransViewHolder holder = new TransViewHolder(v, itemListener);
        return holder;
    }

    @Override
    public void onBindViewHolder(@NonNull TransViewHolder holder, int position) {
        Transactions currentTrans = transList.get(position);

        holder.amount.setText(String.valueOf(currentTrans.getAmount()));
        holder.balance.setText(String.valueOf(currentTrans.getBalance()));
        holder.name.setText(currentTrans.getName());
        holder.day.setText(sortDay(currentTrans.getDay(), currentTrans.getMonth(), currentTrans.getYear()));

    }

    private String sortDay(int day, int month, int year){
        String date = day + "/" + month + "/" + year;

        return date;
    }
    @Override
    public int getItemCount() {
        return transList.size();
    }


}
