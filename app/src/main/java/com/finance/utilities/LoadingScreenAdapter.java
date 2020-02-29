package com.finance.utilities;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.finance.ledger.R;

public class LoadingScreenAdapter extends RecyclerView.Adapter<LoadingScreenAdapter.LoadScreenHolder>{

    @NonNull
    @Override
    public LoadScreenHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.loading_screen, parent, false);
        LoadScreenHolder holder = new LoadScreenHolder(v);
        return holder;
    }

    @Override
    public void onBindViewHolder(@NonNull LoadScreenHolder holder, int position) {


    }

    @Override
    public int getItemCount() {
        return 0;
    }

    public static class LoadScreenHolder extends RecyclerView.ViewHolder{

        public LoadScreenHolder(@NonNull View itemView) {
            super(itemView);
        }
    }
}
