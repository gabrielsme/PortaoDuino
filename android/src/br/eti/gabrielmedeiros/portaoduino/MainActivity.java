package br.eti.gabrielmedeiros.portaoduino;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import at.abraxas.amarino.Amarino;
import at.abraxas.amarino.AmarinoIntent;

public class MainActivity extends Activity {
	private EditText edtSenha;
	private TextView txtAvisos;
	//private String macBluetooth = "00:11:12:01:04:86";//Juscilesio
	private String macBluetooth = "20:13:08:06:08:36";//Gabriel
	private RecebeArduino br;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		edtSenha = (EditText)findViewById(R.id.edtSenha);
		txtAvisos = (TextView)findViewById(R.id.txtAvisos);
		
		br = new RecebeArduino();
		
		Amarino.connect(this, macBluetooth);
		registerReceiver(br, new IntentFilter(AmarinoIntent.ACTION_RECEIVED));
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	@Override
	protected void onStop() {
		super.onStop();
		
		Amarino.disconnect(this, macBluetooth);
	}
	
	public void abrir_click(View view){
		Amarino.sendDataToArduino(getApplicationContext(), macBluetooth, 'A', edtSenha.getText().toString());
	}
	
	public void fechar_click(View view){
		Amarino.sendDataToArduino(getApplicationContext(), macBluetooth, 'F', edtSenha.getText().toString());
	}
	
	public class RecebeArduino extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {  
			int dataType = intent.getIntExtra(AmarinoIntent.EXTRA_DATA_TYPE, -1);

			if(dataType == AmarinoIntent.STRING_EXTRA){
				String data = intent.getStringExtra(AmarinoIntent.EXTRA_DATA);
				if(data != null){
					txtAvisos.setText(String.valueOf(data));
				}
			}
		}
	}
}