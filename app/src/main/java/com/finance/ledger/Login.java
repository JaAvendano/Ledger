package com.finance.ledger;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

public class Login {//extends AppCompatActivity implements View.OnClickListener{

    /*
    Button loginBtn, registerBtn;
    EditText email, password;

    FirebaseAuth auth;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        email = (EditText) findViewById(R.id.loginScreen);
        password = (EditText) findViewById(R.id.pswScreen);

        loginBtn = (Button) findViewById(R.id.loginBtn);
        registerBtn = (Button) findViewById(R.id.registerBtn);
        auth = FirebaseAuth.getInstance();





        loginBtn.setOnClickListener(this);
        registerBtn.setOnClickListener(this);

    }

    @Override
    public void onStart(){
        super.onStart();
        FirebaseUser currentUser = auth.getCurrentUser();
        updateCurrentUser(currentUser);

    }

    public void updateCurrentUser(FirebaseUser user){

    }

    @Override
    public void onStop(){
        super.onStop();
        if(auth.getCurrentUser() != null) {
            auth.signOut();
        }

    }
    @Override
    public void onClick(View v)
    {
        int id = v.getId();

        String user = this.email.getText().toString();
        String psw = this.password.getText().toString();



        switch(id)
        {
            case R.id.loginBtn:
               checkCredentials(user, psw);

            break;

            case R.id.registerBtn:
                toast("Registration simulation");
        }

    }

    public void checkCredentials(String user, String psw){

        String username = this.email.getText().toString();
        String password = this.password.getText().toString();

        if( user.equals("") & psw.equals("")){
            toast("You need to fill out both fields!");
        }else{

            auth.signInWithEmailAndPassword(user, psw)
                    .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                        @Override
                        public void onComplete(@NonNull Task<AuthResult> task) {
                            if(task.isSuccessful()){
                                Intent intent = new Intent(getApplicationContext(), OverViewScreen.class);
                                intent.putExtra("user", auth.getCurrentUser().toString());
                                startActivity(intent);
                            }else{
                                toast("One of the fields does not match");
                            }
                        }
                    });
        }

    }

    public void toast(String message){
        Toast.makeText(getApplicationContext(),message,Toast.LENGTH_LONG).show();

    }

     */
}
