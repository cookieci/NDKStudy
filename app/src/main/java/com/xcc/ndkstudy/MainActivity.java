package com.xcc.ndkstudy;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

//javah -d jni -classpath E:\AndroidStudioWork\NDKStudy\app\build\intermediates\classes\debug com.xcc.ndkstudy.MainActivity
public class MainActivity extends AppCompatActivity {
    @BindView(R.id.text)
    TextView text;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
    }

    @OnClick(R.id.text)
    public void onViewClicked() {
        for (int i = 0; i < 100; i++) {
            String s = new GetString().stringToJni();
            text.setText(s);
        }
    }
}
