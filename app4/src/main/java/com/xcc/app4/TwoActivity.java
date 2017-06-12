package com.xcc.app4;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

/**
 * Created by Administrator on 2017/4/25.
 */
public class TwoActivity extends AppCompatActivity {
    @BindView(R.id.text)
    TextView text;
    @BindView(R.id.button)
    Button button;
    @BindView(R.id.button2)
    Button button2;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);

        button.setText("多线程资源抢占-模拟1");
        button2.setText("多线程资源抢占-模拟2");
    }

    @OnClick({R.id.text, R.id.button, R.id.button2})
    public void onViewClicked(View view) {
        switch (view.getId()) {
            case R.id.text://同时调用
//                MyJniThread2.startThread();
//                MyJniThread2.startThread2();
                break;
            case R.id.button:
                MyJniThread2.startThread();
                break;
            case R.id.button2:
                MyJniThread2.startThread2();
                break;
        }
    }
}