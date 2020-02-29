package com.finance.utilities;

import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.finance.ledger.R;
import com.finance.model.Accounts;

import java.util.List;

public class DepositsScreenAdaptor extends RecyclerView.Adapter<DepositsScreenAdaptor.DepositScreenViewHolder>{

    private List<Accounts> accountsList;
    private ClickedItemListener itemListener;

    public DepositsScreenAdaptor(List<Accounts> accList){
        accountsList = accList;
    }
    public interface ClickedItemListener{
        void clickedItem(int position);
    }

    public void getClickedItem(ClickedItemListener clickedItem){
        itemListener = clickedItem;
    }

    static class DepositScreenViewHolder extends RecyclerView.ViewHolder {

        private TextView accName;
        private TextView accBalText;
        private TextView institution;

        private DepositScreenViewHolder(@NonNull View v, ClickedItemListener listener) {
            super(v);
            accName = v.findViewById(R.id.cardView_accName);
            accBalText = v.findViewById(R.id.cardView_Bal);
            institution = v.findViewById(R.id.cardView_institution);

            v.setOnClickListener(v1 -> {
                if(listener != null){
                    int position = getAdapterPosition();
                    if(position != RecyclerView.NO_POSITION){
                        listener.clickedItem(position);
                    }
                }
                v1.setBackgroundColor(Color.GRAY);

            });
        }
    }
    @NonNull
    @Override
    public DepositScreenViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.overview_cardview, parent, false);
        DepositsScreenAdaptor.DepositScreenViewHolder holder = new DepositsScreenAdaptor.DepositScreenViewHolder(v, itemListener);
        return holder;
    }

    @Override
    public void onBindViewHolder(@NonNull DepositScreenViewHolder holder, int position) {
        Accounts currentItem = accountsList.get(position);

        holder.accName.setText(currentItem.getName());
        holder.accBalText.setText(String.valueOf(currentItem.getBalance()));
        holder.institution.setText(currentItem.getInstitution());
    }

    @Override
    public int getItemCount() {
        return accountsList.size();
    }



}
