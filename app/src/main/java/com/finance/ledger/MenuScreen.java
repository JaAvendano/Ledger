package com.finance.ledger;

import android.os.Bundle;
import android.view.MenuItem;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModel;
import androidx.lifecycle.ViewModelProvider;

import com.finance.model.Accounts;
import com.finance.utilities.DataManagement;
import com.google.android.material.navigation.NavigationView;

import java.util.List;

public class MenuScreen extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener{

    private DrawerLayout drawerLayout;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_menu_screen);

        Toolbar toolbar = findViewById(R.id.toolBar);
        setSupportActionBar(toolbar);

        drawerLayout = findViewById(R.id.overViewScreen);
        NavigationView navigationView = findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawerLayout, toolbar,
                R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawerLayout.addDrawerListener(toggle);
        toggle.syncState();



        if(savedInstanceState == null) {

            OverViewScreen overViewScreen = new OverViewScreen();

            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                                overViewScreen).commit();

            navigationView.setCheckedItem(R.id.main_menu_account);
        }

    }

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
        switch(item.getItemId()){
            case R.id.main_menu_account:

                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new OverViewScreen()).commit();
                break;
            case R.id.main_menu_deposits:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new DepositScreen()).commit();
                break;

            case R.id.main_menu_transactions:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new AddTransScreen()).commit();
                break;
            case R.id.main_menu_add_account:
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        new AddAccountScreen()).commit();
                break;
        }
        drawerLayout.closeDrawer(GravityCompat.START);
        return true;
    }

    @Override
    public void onBackPressed() {
        if(drawerLayout.isDrawerOpen(GravityCompat.START)){
            drawerLayout.closeDrawer(GravityCompat.START);
        }else{
            super.onBackPressed();
        }
    }

    public void toast(String message){
        Toast.makeText(getApplicationContext(), message, Toast.LENGTH_LONG);
    }
}
