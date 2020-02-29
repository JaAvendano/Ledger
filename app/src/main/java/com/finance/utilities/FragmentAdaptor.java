package com.finance.utilities;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentStatePagerAdapter;

import java.util.ArrayList;
import java.util.List;

public class FragmentAdaptor extends FragmentStatePagerAdapter {

    private final List<Fragment> fragList = new ArrayList<>();
    private final List<String> fragTitleList = new ArrayList<>();

    public FragmentAdaptor(@NonNull FragmentManager fm, int behavior) {
        super(fm, behavior);
    }

    private void addFragment(Fragment fragment, String title){
        fragList.add(fragment);
        fragTitleList.add(title);
    }
    @NonNull
    @Override
    public Fragment getItem(int position) {
        return fragList.get(position);
    }

    @Override
    public int getCount() {
        return fragList.size();
    }
}
