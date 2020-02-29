package com.finance.ledger;


import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.finance.model.Accounts;
import com.finance.rest.AccountService;
import com.finance.rest.ApiClient;
import com.finance.rest.TransactionService;
import com.finance.utilities.DataManagement;
import com.finance.utilities.DepositsScreenAdaptor;
import com.finance.utilities.LoadingScreenAdapter;

import org.jetbrains.annotations.NotNull;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.LinkedList;
import java.util.List;
import java.util.Observable;
import java.util.Queue;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class DepositScreen extends Fragment {

    private EditText acc, amount;
    private RecyclerView accounts;
    private Button cancel, submit;
    private View currentView;
    private List<Accounts> accList;
    private Accounts selectedAccount;
    private Queue<Integer> selectionSorter = new LinkedList<>();

    private RecyclerView.LayoutManager layoutManager;
    private DepositsScreenAdaptor adaptor;
    private LoadingScreenAdapter loadingAdapter;


    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        currentView = inflater.inflate(R.layout.deposit_screen, container, false);
        getActivity().setTitle("Deposit");

        accounts = currentView.findViewById(R.id.deposit_account);
        accounts.setHasFixedSize(true);
        amount = currentView.findViewById(R.id.deposit_amount);

        cancel = currentView.findViewById(R.id.btn_deposit_cancel);
        submit = currentView.findViewById(R.id.btn_add_deposit_submit);

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
           displayScreenInfo(accList);
       }

    }

    private void loadingScreen(){
        layoutManager = new LinearLayoutManager(getContext());
        loadingAdapter = new LoadingScreenAdapter();

        accounts.setLayoutManager(layoutManager);
        accounts.setAdapter(loadingAdapter);
    }


    private void displayScreenInfo(List<Accounts> list){
        layoutManager = new LinearLayoutManager(getContext());
        adaptor = new DepositsScreenAdaptor(list);

        accounts.setLayoutManager(layoutManager);
        accounts.setAdapter(adaptor);
        adaptor.getClickedItem(position -> {

            selectedAccount = list.get(position);
            if(!selectionSorter.isEmpty()){
                int oldPosition = selectionSorter.element();
                accounts.getChildAt(oldPosition).setBackgroundColor(Color.WHITE);
                selectionSorter.remove();
            }
            selectionSorter.add(position);
            selectedAccount = accList.get(position);

        });
        submit.setOnClickListener(v -> submitFunction());

        cancel.setOnClickListener(v -> {
            assert getFragmentManager() != null;
            getFragmentManager()
                    .beginTransaction()
                    .replace(R.id.fragment_container, new OverViewScreen())
                    .commit();
        });
    }
    private void submitFunction(){
        TransactionService service = ApiClient.getRetrofit().create(TransactionService.class);
        Calendar calendar = Calendar.getInstance();
        int day = calendar.get(Calendar.DATE) + 1;
        int month = calendar.get(Calendar.MONTH);
        int year = calendar.get(Calendar.YEAR);
        BigDecimal am = new BigDecimal(this.amount.getText().toString());

        Call call = service.addTransaction(selectedAccount.getId(),"", day, month, year,"deposit", 1, 1, am, selectedAccount.getBalance().add(am));
        call.enqueue(new Callback() {
            @Override
            public void onResponse(Call call, Response response) {
                if(response.isSuccessful()){

                    assert getFragmentManager() != null;
                    getFragmentManager()
                            .beginTransaction()
                            .replace(R.id.fragment_container, new OverViewScreen())
                            .commit();
                }

            }

            @Override
            public void onFailure(Call call, Throwable t) {
                call.cancel();
                toast("Something went wrong");

            }
        });

    }

    private void toast(String message){
        Toast.makeText(getContext(), message, Toast.LENGTH_LONG).show();
    }
}
