package com.finance.ledger;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.finance.model.Accounts;
import com.finance.model.Transactions;
import com.finance.utilities.DataManagement;
import com.finance.utilities.LoadingScreenAdapter;
import com.finance.utilities.TransactionsCardViewAdaptor;

import java.util.List;


public class TransactionsScreen extends Fragment {

    private List<Transactions> transList;

    private View currentView;
    private RecyclerView recyclerView;
    private RecyclerView.LayoutManager layoutManager;
    private TransactionsCardViewAdaptor adaptor;
    private LoadingScreenAdapter loadingAdapter;



    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        getActivity().setTitle("Transactions");
        currentView = inflater.inflate(R.layout.overview_screen, container, false);
        recyclerView = currentView.findViewById(R.id.overview_recyclerView);
        recyclerView.setHasFixedSize(true);
        loadingScreen();

        return currentView;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {

        Accounts selectedAcc = DataManagement.getSelectedAcc().getValue();

        if(transList == null) {
            DataManagement.getAllTransactions(selectedAcc.getId()).observe(this, new Observer<List<Transactions>>() {
                @Override
                public void onChanged(List<Transactions> transactions) {
                    transList = transactions;
                    displayScreenInfo(transList);

                }
            });
        }else {
            transList = DataManagement.getAllTransactions(selectedAcc.getId()).getValue();
            displayScreenInfo(transList);
        }
    }

    private void loadingScreen(){
        layoutManager = new LinearLayoutManager(getContext());
        loadingAdapter = new LoadingScreenAdapter();

        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(adaptor);
    }
    private void displayScreenInfo(List<Transactions> itemsList){

        layoutManager = new LinearLayoutManager(getContext());
        adaptor = new TransactionsCardViewAdaptor(itemsList);

        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(adaptor);
        adaptor.getClickedItem(position -> {
            Transactions selectedTrans = transList.get(position);
            DataManagement.setSelectedTrans(selectedTrans);
            DataManagement.setTransId(selectedTrans.getId());
            assert getFragmentManager() != null;
            getFragmentManager()
                    .beginTransaction()
                    .replace(R.id.fragment_container, new TransactionsDetailsScreen())
                    .commit();

        });
    }

    private void toast(String message){
        Toast.makeText(getContext(),message,Toast.LENGTH_LONG).show();

    }


}
