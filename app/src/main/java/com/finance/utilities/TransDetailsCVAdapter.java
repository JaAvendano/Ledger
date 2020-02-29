package com.finance.utilities;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.finance.ledger.R;
import com.finance.model.Transactions;

public class TransDetailsCVAdapter extends RecyclerView.Adapter<TransDetailsCVAdapter.TransDetailViewHolder>{

    private Transactions transaction;

    public TransDetailsCVAdapter(Transactions targetTransaction){
        transaction = targetTransaction;
    }

    public static class TransDetailViewHolder extends RecyclerView.ViewHolder {

        private TextView date;
        private TextView accId;
        private TextView id;
        private TextView name;
        private TextView cat;
        private TextView amount;
        private TextView bal;
        private TextView method;
        private TextView status;

        public TransDetailViewHolder(@NonNull View v) {
            super(v);

            date = v.findViewById(R.id.trans_details_date);
            accId = v.findViewById(R.id.trans_details_acc_id);
            id = v.findViewById(R.id.trans_details_id);
            name = v.findViewById(R.id.trans_details_name);
            cat = v.findViewById(R.id.trans_details_cat);
            amount = v.findViewById(R.id.trans_details_amount);
            bal = v.findViewById(R.id.trans_details_bal);
            method = v.findViewById(R.id.trans_details_method);
            status = v.findViewById(R.id.trans_details_status);
        }
    }

    @NonNull
    @Override
    public TransDetailViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.trans_detail_view, parent, false);
        TransDetailViewHolder holder = new TransDetailViewHolder(v);
        return holder;
    }

    @Override
    public void onBindViewHolder(@NonNull TransDetailViewHolder holder, int position) {

        Transactions currentTrans = transaction;

        holder.date.setText(sortDate(currentTrans.getDay(), currentTrans.getMonth(), currentTrans.getYear()));
        holder.accId.setText(String.valueOf(currentTrans.getAcc_id()));
        holder.id.setText(String.valueOf(currentTrans.getId()));
        holder.name.setText(currentTrans.getName());
        holder.cat.setText(String.valueOf(currentTrans.getCat()));
        holder.amount.setText(String.valueOf(currentTrans.getAmount()));
        holder.bal.setText(String.valueOf(currentTrans.getBalance()));
        holder.method.setText(String.valueOf(currentTrans.getMethod()));
        holder.status.setText(currentTrans.getStatus());

    }

    @Override
    public int getItemCount() {
        return 1;
    }

    public String sortDate(int day, int month, int year){
        String date = day + "/" + month + "/" + year;
        return date;
    }
}
