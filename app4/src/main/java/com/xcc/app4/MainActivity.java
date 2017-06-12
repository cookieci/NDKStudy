package com.xcc.app4;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class MainActivity extends AppCompatActivity {
    @BindView(R.id.text)
    TextView text;
    @BindView(R.id.button)
    Button button;
    @BindView(R.id.button2)
    Button button2;
    @BindView(R.id.activity_main)
    RelativeLayout activityMain;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
    }

    @OnClick({R.id.text, R.id.button, R.id.button2})
    public void onViewClicked(View view) {
        switch (view.getId()) {
            case R.id.text:
                startActivity(new Intent(this, TwoActivity.class));
                break;
            case R.id.button://开启线程
                MyJniThread.startThread();
                //仅让线程运行1毫秒
                button.postDelayed(new Runnable() {
                    public void run() {
                        button2.performClick();
                    }
                }, 1);
                break;
            case R.id.button2://关闭线程
                MyJniThread.stopThread();
                break;
        }
    }
}
