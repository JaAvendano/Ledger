package com.finance.ledger;

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

import com.finance.model.Accounts;
import com.finance.rest.AccountService;
import com.finance.rest.ApiClient;

import org.jetbrains.annotations.NotNull;

import java.math.BigDecimal;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class AddAccountScreen extends Fragment {

    private Accounts accounts;


    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.add_acc_screen, container, false);
        getActivity().setTitle("Add Account");
        //edit text
        EditText institution, type, name, balance;
        Button cancel, submit;

        institution = v.findViewById(R.id.add_acc_institution);
        type = v.findViewById(R.id.add_acc_type);
        name = v.findViewById(R.id.add_acc_name);
        balance = v.findViewById(R.id.add_acc_bal);
        //buttons
        cancel = v.findViewById(R.id.btn_add_acc_cancel);
        submit = v.findViewById(R.id.btn_add_acc_submit);


        submit.setOnClickListener(v1 -> {
            accounts = new Accounts();

            accounts.setInstitution(institution.getText().toString());
            accounts.setType(type.getText().toString());
            accounts.setName(name.getText().toString());
            accounts.setBalance(new BigDecimal(balance.getText().toString()));
            accounts.setActive(1);
            AccountService service = ApiClient.getRetrofit().create(AccountService.class);
            Call<Void> call = service.addAccount(accounts.getInstitution(),
                    accounts.getType(),
                    accounts.getName(),
                    accounts.getActive(),
                    accounts.getBalance());
            call.enqueue(new Callback<Void>() {
                @Override
                public void onResponse(@NotNull Call<Void> call, @NotNull Response<Void> response) {
                    if(!response.isSuccessful()){
                        toast("Response failed");
                    }else{
                        OverViewScreen overViewScreen = new OverViewScreen();
                        assert getFragmentManager() != null;
                        getFragmentManager()
                                .beginTransaction()
                                .replace(R.id.fragment_container, overViewScreen)
                                .commit();
                        toast("Response was good");
                    }
                }

                @Override
                public void onFailure(@NotNull Call<Void> call, @NotNull Throwable t) {

                    call.cancel();
                    toast("call was canceled");
                }
            });

            toast("new account input: " + accounts);
        });


        cancel.setOnClickListener(v12 -> {
            assert getFragmentManager() != null;
            getFragmentManager()
                    .beginTransaction()
                    .replace(R.id.fragment_container, new OverViewScreen())
                    .commit();
        });


        return v;
    }

    private void toast(String message){
        Toast.makeText(getContext(), message, Toast.LENGTH_LONG).show();
    }
}
