package com.example.Test;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;

public class MyActivity extends Activity {
    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

    }

    public void openLogWeight(View view)
    {
        Intent intent = new Intent(this, LogWeightActivity.class);
        startActivity(intent);
    }

    public void openViewHistory(View view)
    {
        Intent intent = new Intent(this, ViewHistoryActivity.class);
        startActivity(intent);
    }
}
