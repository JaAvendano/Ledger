package com.finance.ledger;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.finance.model.Transactions;
import com.finance.utilities.DataManagement;
import com.finance.utilities.LoadingScreenAdapter;
import com.finance.utilities.TransDetailsCVAdapter;


public class TransactionsDetailsScreen extends Fragment {

    private Transactions selectedTrans;
    private RecyclerView recyclerView;
    private RecyclerView.LayoutManager layoutManager;
    private TransDetailsCVAdapter adapter;
    private LoadingScreenAdapter loadingAdaptor;



    @Override
    public View onCreateView(@Nullable LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        getActivity().setTitle("Transaction Details");
        View currentView = inflater.inflate(R.layout.overview_screen, container, false);

        recyclerView = currentView.findViewById(R.id.overview_recyclerView);
        recyclerView.setHasFixedSize(true);
        loadingScreen();

        return currentView;

    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        selectedTrans = DataManagement.getSelectedTrans().getValue();
        displayScreenInfo(selectedTrans);
    }

    public void loadingScreen(){
        layoutManager = new LinearLayoutManager(getContext());
        loadingAdaptor = new LoadingScreenAdapter();

        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(loadingAdaptor);
    }

    public void displayScreenInfo(Transactions trans){
        layoutManager = new LinearLayoutManager(getContext());
        adapter = new TransDetailsCVAdapter(trans);

        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(adapter);
    }

}
