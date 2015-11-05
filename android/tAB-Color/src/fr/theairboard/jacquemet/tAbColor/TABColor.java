/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package fr.theairboard.jacquemet.tAbColor;

/**
 *  @author XXXX
 *  
 *  Modification to BLE by JulienJ (jacquemet.julien@gmail.com)
 *  
 */



import android.app.ActionBar;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.TextView;
import android.widget.Toast;

/**
 * This is the main Activity that displays the current chat session.
 */
public class TABColor extends  BlunoLibrary {
    // Debugging
    private static final String TAG = "TABColor";
    private static final boolean D = true;
 
    // Message types sent from the BTService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    
    // the color...
    // private static final char COL_WHITE = 'a';
    // private static final char COL_YELLOW = 'b';
    // private static final char COL_PINK = 'c';
    // private static final char COL_RED = 'd';
    // private static final char COL_CYAN = 'e';
    // private static final char COL_GREEN = 'f';
    // private static final char COL_BLUE = 'g';
    // private static final char COL_OFF = 'h';
    
    private ColorCircle mColorCircle;

    private CompoundButton mToggleButton;

    // Key names received from the BTService Handler
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";

    // Intent request codes
    //private static final int REQUEST_CONNECT_DEVICE_SECURE = 1;
    //private static final int REQUEST_CONNECT_DEVICE_INSECURE = 2;
    //private static final int REQUEST_ENABLE_BT = 3;
    private static final int REQUEST_CONNECT_DEVICE_RFCOMM = 4;

    // Name of the connected device
    private String mConnectedDeviceName = null;
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;
    // Member object for the chat services
        
    private PowerManager.WakeLock mWakeLock = null;

    private boolean mBeginBTState;
    
    private int mRed;
    private int mGreen;
    private int mBlue;
    private int mPower = 255;

    private Button buttonScan;
    
    private TextView tvState;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(D) Log.e(TAG, "+++ ON CREATE +++");

        // Set up the window layout
        setContentView(R.layout.main);

        // Get local Bluetooth adapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        // If the adapter is null, then Bluetooth is not supported
        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth is not available", Toast.LENGTH_LONG).show();
            finish();
            return;
        }

    
                
        onCreateProcess();														//onCreate Process by BlunoLibrary
         
        serialBegin(115200);
        
        tvState = (TextView)findViewById(R.id.state);
        tvState.setText(R.string.title_not_connected);
        
        buttonScan = (Button) findViewById(R.id.buttonScan);					//initial the button for scanning the BLE device
        buttonScan.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub

				buttonScanOnClickProcess();										//Alert Dialog for selecting the BLE device
			}
		});
    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.option_menu, menu);
        return true;
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.connect_scan:
                buttonScanOnClickProcess();										//Alert Dialog for selecting the BLE device
                break;
            case R.id.discoverable:
                ensureDiscoverable();
                break;
        }
        return false;
    }


    @Override
    public void onStart() {
        super.onStart();
        if(D) Log.e(TAG, "++ ON START ++");

        // If BT is not on, request that it be enabled.
        // setupChat() will then be called during onActivityResult
        mBeginBTState = mBluetoothAdapter.isEnabled(); 
        if (!mBeginBTState) {
        	mBluetoothAdapter.enable();
        	while(!mBluetoothAdapter.isEnabled());
        // Otherwise, setup the chat session
        }
        
        setupChat();
     
    }
    
	protected void onResume(){
		super.onResume();
		onResumeProcess();	
		
		 setupChat();//onResume Process by BlunoLibrary
	}



    private void setupChat() {
        Log.d(TAG, "setupChat()");

        // Initialize the send button with a listener that for click events
        mColorCircle = (ColorCircle) findViewById(R.id.colorcircle);
        mColorCircle.setFunction((Activity)this, "setRGB");
        mColorCircle.setColor(Color.WHITE);
        
        mToggleButton = ((CompoundButton)findViewById(R.id.switchOnOff));
        mToggleButton.setOnCheckedChangeListener(new ToggleListener());
        
        setColorButtonsEnabled(true);
        
        //buttonScanOnClickProcess();	 
 


    }

    // onClickListener for color switch
    private class ToggleListener implements CompoundButton.OnCheckedChangeListener {
        public ToggleListener() {
        }
 
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
            mColorCircle.setEnabled(isChecked);
            mColorCircle.setClickable(isChecked);
            
            //mToggleButton.setEnabled(isChecked);
         
            if (isChecked) {
                mColorCircle.setColor(Color.WHITE);
                sendMessage("a");
            } else {
                mColorCircle.setColor(Color.BLACK);
                sendMessage("h");
            }
        }
    }
    
    private void setColorButtonsEnabled(boolean en) {
    	mToggleButton.setEnabled(en);
    	if ((mToggleButton.isChecked() && en) || !en) {
    	 
            mColorCircle.setClickable(en);
        
    	}
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        onPauseProcess();														//onPause Process by BlunoLibrary
    }
	
	protected void onStop() {
		super.onStop();
		onStopProcess();														//onStop Process by BlunoLibrary
	}
    
	@Override
    protected void onDestroy() {
        super.onDestroy();	
        onDestroyProcess();	
        //onDestroy Process by BlunoLibrary
        if (!mBeginBTState) mBluetoothAdapter.disable();
    }





    /**
     * Sends a message.
     * @param message  A string of text to send.
     */
    private void sendMessage(String message) {
    	if(D) Log.d(TAG, "Send "+message);
    	if(tvState.getText().toString().contains("Connected") && message!="")
    		serialSend(message);
    
    }

  




    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(D) Log.d(TAG, "onActivityResult " + resultCode);
        onActivityResultProcess(requestCode, resultCode, data);					//onActivityResult Process by BlunoLibrary
		super.onActivityResult(requestCode, resultCode, data);
    }


    private String getColor() {
    	int r = mRed * mPower / 255;
    	int g = mGreen * mPower / 255;
    	int b = mBlue * mPower / 255;
        // int a = (color>>24)&0xFF;
    	if(D) Log.e(TAG, "r" + r + ",g" + g + ",b" + b +",;");
        
    	//return "r" + r + ",g" + g + ",b" + b +",;";
    	
    	
    	if(r> 200 && b>200 && g>200)
    		return "a";
    	else if(r> 200 && b<100 && g>200)
    		return "b";
    	else if(r> 200 && b>200 && g<100)
    		return "c";
    	else if(r> 200 && b<100 && g<100)
    		return "d";
    	else if(r< 100 && b>200 && g>200)
    		return "e";
    	else if(r< 100 && b<100 && g>200)
    		return "f";
    	else if(r< 200 && b>200 && g<100)
    		return "g";
    	else if(r< 100 && b<100 && g<100)
    		return "h";
    	else
    		return "";
    
    }
    
    public void setRGB(int color) {
        mBlue  = (color)&0xFF;
        mGreen = (color>>8)&0xFF;
        mRed   = (color>>16)&0xFF;
    	if(D) Log.e(TAG, "!!! COLOR : " + Integer.toHexString(color));
    	//mColorPower.setColors(color, Color.BLACK);
   
        sendMessage(getColor());
    }

    public void setSaturation(int sat) {
    	mBlue  = (sat)&0xFF;
        mGreen = (sat>>8)&0xFF;
        mRed   = (sat>>16)&0xFF;
    	if(D) Log.e(TAG, "!!! SATURATION : " + Integer.toHexString(sat));
        sendMessage(getColor());
    }

    public void setPower(int power) {
    	mPower = (power)&0xFF;
    	if(D) Log.e(TAG, "!!! POWER : " + Integer.toHexString(power));
        sendMessage(getColor());
    }
 
	
	 @Override
	    public void onConectionStateChange(connectionStateEnum theConnectionState) {//Once connection state changes, this function will be called
	        switch (theConnectionState) {											//Four connection state
	            case isConnected:
	                setColorButtonsEnabled(true);
	                tvState.setText(R.string.title_connected);
	                break;
	            case isConnecting:
	                setColorButtonsEnabled(false);
	                tvState.setText(R.string.title_connecting);
	                break;
	            case isToScan:
	                setColorButtonsEnabled(false);
	                //tvState.setText(R.string.title_scan);
	                tvState.setText(R.string.title_not_connected);
	                break;
	            case isScanning:
	                setColorButtonsEnabled(false);
	                tvState.setText(R.string.title_scanning);
	                break;
	            case isDisconnecting:
	                setColorButtonsEnabled(false);
	                tvState.setText(R.string.title_not_connected);
	                break;
	            default:
	                break;
	        }
	    }

	@Override
	public void onSerialReceived(String theString) {
		// TODO Auto-generated method stub
		
	}
	
	  private void ensureDiscoverable() {
	        if(D) Log.d(TAG, "ensure discoverable");
	        Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
	        discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
	        startActivity(discoverableIntent);
	    }
}
