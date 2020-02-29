package com.finance.ledger;

import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
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

import org.jetbrains.annotations.NotNull;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class AddTransScreen extends Fragment {

    private Accounts selectedAccount;
    private List<Accounts> accList;
    private Queue<Integer> selectionSorter = new LinkedList<>();
    private View currentView;
    private EditText name, amount;
    private RecyclerView acc;
    private Button cancel, submit;
    private TextView bal;
    private RecyclerView.LayoutManager layoutManager;
    private DepositsScreenAdaptor adaptor;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        currentView = inflater.inflate(R.layout.add_tans_screen, container, false);

        getActivity().setTitle("Add Transaction");



        acc = currentView.findViewById(R.id.add_trans_acc);
        acc.setHasFixedSize(true);
        name = currentView.findViewById(R.id.add_trans_name);
        amount = currentView.findViewById(R.id.add_trans_amount);

        bal = currentView.findViewById(R.id.add_trans_bal);

        cancel = currentView.findViewById(R.id.btn_add_trans_cancel);
        submit = currentView.findViewById(R.id.btn_add_trans_submit);

        return currentView;
    }


    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        if(accList == null) {
            DataManagement.getAllAccounts().observe(this, new Observer<List<Accounts>>() {
                @Override
                public void onChanged(List<Accounts> accountsList) {
                    accList = accountsList;
                    displayScreenInfo(accList);
                }
            });
        }else{
            displayScreenInfo(accList);
        }

        submit.setOnClickListener(v -> submitBtnFunction());

        cancel.setOnClickListener(v -> cancelBtnFunction());
    }

    private void displayScreenInfo(List<Accounts> list){
        acc = currentView.findViewById(R.id.add_trans_acc);
        acc.setHasFixedSize(true);
        layoutManager = new LinearLayoutManager(getContext());
        adaptor = new DepositsScreenAdaptor(list);

        acc.setLayoutManager(layoutManager);
        acc.setAdapter(adaptor);

        adaptor.getClickedItem(position -> {

            if(!selectionSorter.isEmpty()){
                int oldPosition = selectionSorter.element();
                acc.getChildAt(oldPosition).setBackgroundColor(Color.WHITE);
                selectionSorter.remove();
            }

            selectionSorter.add(position);
            selectedAccount = list.get(position);

        });

    }


    private void submitBtnFunction(){
        int accId = selectedAccount.getId();
        String status = "Status filler";
        Calendar calendar = Calendar.getInstance();
        int day = calendar.get(Calendar.DATE) + 1;
        int month = calendar.get(Calendar.MONTH);
        int year = calendar.get(Calendar.YEAR);
        String transName = name.getText().toString();
        int meth = 1;
        int category = 1;
        BigDecimal quantity = new BigDecimal(amount.getText().toString());
        BigDecimal balance = new BigDecimal(bal.getText().toString());

        TransactionService service = ApiClient.getRetrofit().create(TransactionService.class);

        Call<Void> call = service.addTransaction(accId, status, day, month, year, transName, meth, category, quantity, balance);
        call.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(@NotNull Call<Void> call, @NotNull Response<Void> response) {
                if(!response.isSuccessful()){
                    toast("Response was no good");
                }else{
                    OverViewScreen overViewScreen = new OverViewScreen();
                    assert getFragmentManager() != null;
                    getFragmentManager()
                            .beginTransaction()
                            .replace(R.id.fragment_container, overViewScreen)
                            .commit();
                }

            }

            @Override
            public void onFailure(@NotNull Call<Void> call, @NotNull Throwable t) {
                call.cancel();
                toast("Call was canceled");
            }
        });
    }


    private void cancelBtnFunction(){
        assert getFragmentManager() != null;
        getFragmentManager()
                .beginTransaction()
                .replace(R.id.fragment_container, new OverViewScreen())
                .commit();
    }

    private void toast(String message){
        Toast.makeText(getContext(), message, Toast.LENGTH_LONG).show();
    }
}
