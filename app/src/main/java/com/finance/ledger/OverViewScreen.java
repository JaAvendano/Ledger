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
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.finance.model.Accounts;
import com.finance.utilities.CardViewAdapter;
import com.finance.utilities.DataManagement;
import com.finance.utilities.LoadingScreenAdapter;

import java.util.List;

public class OverViewScreen extends Fragment
{

    private List<Accounts> accList;
    private Accounts selectedAccount;

    private View currentView;
    private RecyclerView recyclerView;
    private RecyclerView.LayoutManager layoutManager;
    private CardViewAdapter adapter;
    private LoadingScreenAdapter loadingAdapter;

    @Override
    public View onCreateView(@Nullable LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        getActivity().setTitle("Accounts");

        assert inflater != null;
        currentView = inflater.inflate( R.layout.overview_screen, container, false);
        recyclerView = currentView.findViewById(R.id.overview_recyclerView);
        recyclerView.setHasFixedSize(true);

        loadingScreen();

        return currentView;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {

        if(accList == null){
            DataManagement.getAllAccounts().observe(this, new Observer<List<Accounts>>() {
                @Override
                public void onChanged(List<Accounts> accountsList) {
                    accList = accountsList;
                    displayScreenInfo(accList);
                }
            });
        }else {
            accList = DataManagement.getAllAccounts().getValue();
            displayScreenInfo(accList);
        }

    }

    private void loadingScreen(){
        loadingAdapter = new LoadingScreenAdapter();
        layoutManager = new LinearLayoutManager(getContext());
        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(loadingAdapter);
    }
    private void displayScreenInfo(List<Accounts> accounts){

        layoutManager = new LinearLayoutManager(getContext());
        adapter = new CardViewAdapter(accounts);

        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(adapter);
        adapter.getClickedItem(position -> {
            Accounts selectedAcc = accounts.get(position);
            int id = selectedAcc.getId();
            DataManagement.setSelectedAcc(selectedAcc);

            TransactionsScreen transScreen = new TransactionsScreen();

            assert getFragmentManager() != null;
            getFragmentManager()
                    .beginTransaction()
                    .replace(R.id.fragment_container, transScreen)
                    .commit();
        });

    }

    private void toast(String message){
        Toast.makeText(getContext(), message, Toast.LENGTH_LONG).show();
    }

}
