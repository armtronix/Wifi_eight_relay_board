package net.armtronix.wifieightrelay.http;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicHttpResponse;
import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.speech.RecognizerIntent;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;

import android.widget.Toast;
import android.widget.ToggleButton;

public class Client extends Activity {
	 private static final String TAG = "Client";
	private Socket socket;
    public SeekBar seekBar;
	private static int SERVERPORT_MAIN = 0;
	private static String SERVER_MAIN = "";
	private String serverinputdata = null;
	private String serveroutputdata = null;
	private String temp = null;
	public EditText et;
	private Handler mHandler;
	public static final int VOICE_RECOGNITION_REQUEST_CODE = 1234;
	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);		
		Intent intent = getIntent();
		SERVERPORT_MAIN = Integer.valueOf(intent.getStringExtra("SERVERPORT"));
	    SERVER_MAIN = intent.getStringExtra("SERVER_IP");
		
//        et =(EditText) findViewById(R.id.EditText01);
//		Button send=(Button) findViewById(R.id.myButton);
//        send.setOnClickListener(new OnClickListener()
//        {
//    	public void onClick(View view) 
//    	{
//    		try {
//    			
//    			PrintWriter out = new PrintWriter(new BufferedWriter(
//    					new OutputStreamWriter(socket.getOutputStream())),
//    					true);
//    			//et=(EditText) findViewById(R.id.EditText01);
//    			serveroutputdata = et.getText().toString();
//    			out.println(serveroutputdata);
//    			et.setText("");
//
//    		} catch (UnknownHostException e) {
//    			e.printStackTrace();
//    		} catch (IOException e) {
//    			e.printStackTrace();
//    		} catch (Exception e) {
//    			e.printStackTrace();
//    		}
//    	 }
//    	});
//	    /*WIFI EIGHT RELAY BOARD VIRTUAL SWITCH CODE
//	     * 
//	     * Physial Switch                     Relay           MQTT ON      MQTT OFF            HTTP ON                         HTTP OFF 
//	     *   switch 1(GPIO7)                  Relay1(GPIO4)   R1_ON         R1_OFF       http://ip/gpio?state_05=1         http://ip/gpio?state_05=0
//	     *   switch 2( GPIO11)                Relay2(GPIO5)   R2_ON         R2_OFF       http://ip/gpio?state_09=1         http://ip/gpio?state_09=0
//	     *   switch 3(GPIO12)                 Relay3(GPIO6)   R3_ON         R3_OFF       http://ip/gpio?state_06=1         http://ip/gpio?state_06=0
//	     *   switch 4(GPIO13)                 Relay4(GPIO8)   R4_ON         R4_OFF       http://ip/gpio?state_08=1         http://ip/gpio?state_08=0
//	     *   switch 5(GPIOA0)                 Relay5(GPIO9)   R5_ON         R5_OFF       http://ip/gpio?state_02=1         http://ip/gpio?state_02=0
//	     *   switch 6(GPIOA1)                 Relay6(GPIO10)  R6_ON         R6_OFF       http://ip/gpio?state_04=1         http://ip/gpio?state_04=0
//	     *   switch 7(GPIOA2)                 Relay7(GPIO2)   R7_ON         R7_OFF       http://ip/gpio?state_10=1         http://ip/gpio?state_10=0
//	     *   switch 8(GPIOA3)                 Relay8(GPIO3)   R8_ON         R8_OFF       http://ip/gpio?state_03=1         http://ip/gpio?state_03=0
//	     *        
        
        
        final ToggleButton onoffbutton1=(ToggleButton) findViewById(R.id.onoffbutton1);
        onoffbutton1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            	 temp="/gpio?state_05=1";
            	 new HTTPdemo().execute("");
            	 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	 //onoffbutton.setText("ON");
            	}
            	else
            	{
                 temp="/gpio?state_05=0";
                 new HTTPdemo().execute("");
                 //onoffbutton.setText("OFF");
                 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	}
            		
            
    			}
            	}
            );
		
        
      final ToggleButton onoffbutton2=(ToggleButton) findViewById(R.id.onoffbutton2);
        onoffbutton2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            	 temp="/gpio?state_09=1";
            	 new HTTPdemo().execute("");
            	 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	 //onoffbutton.setText("ON");
            	}
            	else
            	{
                 temp="/gpio?state_09=0";
                 new HTTPdemo().execute("");
                 //onoffbutton.setText("OFF");
                 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	}
            		
    			}
            	}
            );
        final ToggleButton onoffbutton3=(ToggleButton) findViewById(R.id.onoffbutton3);
        onoffbutton3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            	 temp="/gpio?state_06=1";
            	 new HTTPdemo().execute("");
            	 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	 //onoffbutton.setText("ON");
            	}
            	else
            	{
                 temp="/gpio?state_06=0";
                 new HTTPdemo().execute("");
                 //onoffbutton.setText("OFF");
                 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	}
            		
            	
    			}
            	}
            );
        
        final ToggleButton onoffbuttonbutton4=(ToggleButton) findViewById(R.id.onoffbutton4);
        onoffbuttonbutton4.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            		temp="/gpio?state_08=1";
            		 new HTTPdemo().execute("");

            	}
            	else
            	{
            		temp="/gpio?state_08=0";
            		 new HTTPdemo().execute("");

            	}
            		
            
    			}
            	}
            );
        
        final ToggleButton onoffbuttonbutton5=(ToggleButton) findViewById(R.id.onoffbutton5);
        onoffbuttonbutton5.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            		temp="/gpio?state_02=1";
            		 new HTTPdemo().execute("");

            	}
            	else
            	{
            		temp="/gpio?state_02=0";
            		 new HTTPdemo().execute("");

            	}
            		
            
    			}
            	}
            );
        
        final ToggleButton onoffbuttonbutton6=(ToggleButton) findViewById(R.id.onoffbutton6);
        onoffbuttonbutton6.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            		temp="/gpio?state_04=1";
            		 new HTTPdemo().execute("");

            	}
            	else
            	{
            		temp="/gpio?state_04=0";
            		 new HTTPdemo().execute("");

            	}
            		
            
    			}
            	}
            );
        
        final ToggleButton onoffbuttonbutton7=(ToggleButton) findViewById(R.id.onoffbutton7);
        onoffbuttonbutton7.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            		temp="/gpio?state_10=1";
            		 new HTTPdemo().execute("");

            	}
            	else
            	{
            		temp="/gpio?state_10=0";
            		 new HTTPdemo().execute("");

            	}
            		
            
    			}
            	}
            );
        
        final ToggleButton onoffbuttonbutton8=(ToggleButton) findViewById(R.id.onoffbutton8);
        onoffbuttonbutton8.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            		temp="/gpio?state_03=1";
            		 new HTTPdemo().execute("");

            	}
            	else
            	{
            		temp="/gpio?state_03=0";
            		 new HTTPdemo().execute("");

            	}
            		
            
    			}
            	}
            );
        
        

	}
	
	public void informationMenu() 
	{
	    startActivity(new Intent("android.intent.action.INFOSCREEN"));
	}


	private String SendDataFromAndroidDevice() {
	    String result = "";
        String u="http://"+SERVER_MAIN+temp;
	    try {
	        HttpClient httpclient = new DefaultHttpClient();
	        HttpGet getMethod = new HttpGet(u);

	        BufferedReader in = null;
	        BasicHttpResponse httpResponse = (BasicHttpResponse) httpclient
	                .execute(getMethod);
	        in = new BufferedReader(new InputStreamReader(httpResponse
	                .getEntity().getContent()));

	        StringBuffer sb = new StringBuffer("");
	        String line = "";
	        while ((line = in.readLine()) != null) {
	            sb.append(line);
	        }
	        in.close();
	        result = sb.toString(); 


	    } catch (Exception e) {
	        e.printStackTrace();
	    }
	    return result;
	}
	
private class HTTPdemo extends
    AsyncTask<String, Void, String> {

@Override
protected void onPreExecute() {}

@Override
protected String doInBackground(String... params) {
    String result = SendDataFromAndroidDevice();
    return result;
}

@Override
protected void onProgressUpdate(Void... values) {}

@Override
protected void onPostExecute(String result) {

    if (result != null && !result.equals("")) {
        try {
            JSONObject resObject = new JSONObject(result);

            } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
}
	


	

}