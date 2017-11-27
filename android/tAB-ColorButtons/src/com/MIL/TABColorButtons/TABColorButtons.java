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

package com.MIL.TABColorButtons;


import android.app.ActionBar;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;

/**
 * This is the main Activity that displays the current chat session.
 */
public class TABColorButtons extends Activity {
    // Debugging
    private static final String TAG = "TABColorButtons";
    private static final boolean D = true;

    // Message types sent from the BTService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    
    // the color...
    private static final char COL_WHITE = 'a';
    private static final char COL_YELLOW = 'b';
    private static final char COL_PINK = 'c';
    private static final char COL_RED = 'd';
    private static final char COL_CYAN = 'e';
    private static final char COL_GREEN = 'f';
    private static final char COL_BLUE = 'g';
    private static final char COL_OFF = 'h';

    // Key names received from the BTService Handler
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";

    // Intent request codes
    private static final int REQUEST_CONNECT_DEVICE_SECURE = 1;
    private static final int REQUEST_CONNECT_DEVICE_INSECURE = 2;
    //private static final int REQUEST_ENABLE_BT = 3;
    private static final int REQUEST_CONNECT_DEVICE_RFCOMM = 4;

    // Name of the connected device
    private String mConnectedDeviceName = null;
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;
    // Member object for the chat services
    private BTService mChatService = null;
    
    private PowerManager.WakeLock mWakeLock = null;

    private boolean mBeginBTState;


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

        // L'application bloque la mise en veille (voir onDestroy pour la relacher
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, TAG);
        mWakeLock.acquire();
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
        if (mChatService == null) setupChat();
    }

    @Override
    public synchronized void onResume() {
        super.onResume();
        if(D) Log.e(TAG, "+ ON RESUME +");

        // Performing this check in onResume() covers the case in which BT was
        // not enabled during onStart(), so we were paused to enable it...
        // onResume() will be called when ACTION_REQUEST_ENABLE activity returns.
        if (mChatService != null) {
            // Only if the state is STATE_NONE, do we know that we haven't started already
            if (mChatService.getState() == BTService.STATE_NONE) {
              // Start the Bluetooth chat services
              mChatService.start();
            }
        }
    }

    private void setupChat() {
        Log.d(TAG, "setupChat()");

        // Initialize the send button with a listener that for click events
        findViewById(R.id.buttonWhite).setOnClickListener(new ColorClickListener(COL_WHITE));
        findViewById(R.id.buttonYellow).setOnClickListener(new ColorClickListener(COL_YELLOW));
        findViewById(R.id.buttonPink).setOnClickListener(new ColorClickListener(COL_PINK));
        findViewById(R.id.buttonRed).setOnClickListener(new ColorClickListener(COL_RED));
        findViewById(R.id.buttonCyan).setOnClickListener(new ColorClickListener(COL_CYAN));
        findViewById(R.id.buttonGreen).setOnClickListener(new ColorClickListener(COL_GREEN));
        findViewById(R.id.buttonBlue).setOnClickListener(new ColorClickListener(COL_BLUE));
        findViewById(R.id.buttonOff).setOnClickListener(new ColorClickListener(COL_OFF));
        
        setColorButtonsEnabled(false);

        // Initialize the BTService to perform bluetooth connections
        mChatService = new BTService(this, mHandler);

    }
    
    private void setColorReturn(int size, byte[] buffer) {
    	int i;
    	switch(buffer[0]) {
    	case COL_WHITE:
    		i = 0xffffffff;
    		break;
    	case COL_YELLOW:
    		i = 0xffffff00;
    		break;
    	case COL_PINK:
    		i = 0xffff00ff;
    		break;
    	case COL_RED:
    		i = 0xffff0000;
    		break;
    	case COL_CYAN:
    		i = 0xff00ffff;
    		break;
    	case COL_GREEN:
    		i = 0xff00ff00;
    		break;
    	case COL_BLUE:
    		i = 0xff0000ff;
    		break;
    	default:
    		i = 0xff000000;
    		break;
    	}
    	findViewById(R.id.buttonReturn).setBackgroundColor(i);
    }
    
    // onClickListener for color buttons
    private class ColorClickListener implements View.OnClickListener {
    	private char mColor = '9';    	
    	public ColorClickListener(char color) { mColor = color; }
    	public void onClick(View v) {
    		sendMessage(mColor);
    	}
    }
    
    private void setColorButtonsEnabled(boolean en) {
        findViewById(R.id.buttonWhite).setClickable(en);
        findViewById(R.id.buttonYellow).setClickable(en);
        findViewById(R.id.buttonPink).setClickable(en);
        findViewById(R.id.buttonRed).setClickable(en);
        findViewById(R.id.buttonCyan).setClickable(en);
        findViewById(R.id.buttonGreen).setClickable(en);
        findViewById(R.id.buttonBlue).setClickable(en);
        findViewById(R.id.buttonOff).setClickable(en);
    }

    @Override
    public synchronized void onPause() {
        super.onPause();
        if(D) Log.e(TAG, "- ON PAUSE -");
    }

    @Override
    public void onStop() {
        mWakeLock.release();
        super.onStop();
        if(D) Log.e(TAG, "-- ON STOP --");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        // Stop the Bluetooth chat services
        if (mChatService != null) mChatService.stop();
		if (!mBeginBTState) mBluetoothAdapter.disable();
        if(D) Log.e(TAG, "--- ON DESTROY ---");
    }

    private void ensureDiscoverable() {
        if(D) Log.d(TAG, "ensure discoverable");
        if (mBluetoothAdapter.getScanMode() !=
            BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {
            Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
            discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
            startActivity(discoverableIntent);
        }
    }

    /**
     * Sends a message.
     * @param message  A string of text to send.
     */
    /*private void sendMessage(String message) {
        // Check that we're actually connected before trying anything
        if (mChatService.getState() != BTService.STATE_CONNECTED) {
            Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the BTService to write
            byte[] send = message.getBytes();
            mChatService.write(send);

            // Reset out string buffer to zero and clear the edit text field
            //mOutStringBuffer.setLength(0);
        }
    }*/

    private void sendMessage(char message) {
        // Check that we're actually connected before trying anything
        if (mChatService.getState() != BTService.STATE_CONNECTED) {
            Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }
        
        byte[] send = new byte[1];
        send[0] = (byte) message;
        mChatService.write(send);
    	
    }

    private final void setStatus(int resId) {
        final ActionBar actionBar = getActionBar();
        actionBar.setSubtitle(resId);
    }

    private final void setStatus(CharSequence subTitle) {
        final ActionBar actionBar = getActionBar();
        actionBar.setSubtitle(subTitle);
    }

    // The Handler that gets information back from the BTService
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case MESSAGE_STATE_CHANGE:
                if(D) Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
                switch (msg.arg1) {
                case BTService.STATE_CONNECTED:
                    setStatus(getString(R.string.title_connected_to, mConnectedDeviceName));
                    setColorButtonsEnabled(true);
                    break;
                case BTService.STATE_CONNECTING:
                    setStatus(R.string.title_connecting);
                    break;
                case BTService.STATE_LISTEN:
                case BTService.STATE_NONE:
                    setStatus(R.string.title_not_connected);
                    setColorButtonsEnabled(false);
                    break;
                }
                break;
            case MESSAGE_WRITE:
                byte[] writeBuf = (byte[]) msg.obj;
                int size = (int) msg.arg1;
                setColorReturn(size, writeBuf);
                break;
            case MESSAGE_READ:
                //byte[] readBuf = (byte[]) msg.obj;
                break;
            case MESSAGE_DEVICE_NAME:
                // save the connected device's name
                mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
                Toast.makeText(getApplicationContext(), "Connected to " + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                break;
            case MESSAGE_TOAST:
                Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST), Toast.LENGTH_SHORT).show();
                break;
            }
        }
    };

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(D) Log.d(TAG, "onActivityResult " + resultCode);
        switch (requestCode) {
        case REQUEST_CONNECT_DEVICE_RFCOMM:
            // When DeviceListActivity returns with a device to connect
            if (resultCode == Activity.RESULT_OK) {
                connectDevice(data, 2);
            }
            break;
        /*case REQUEST_ENABLE_BT:
            // When the request to enable Bluetooth returns
            if (resultCode == Activity.RESULT_OK) {
                // Bluetooth is now enabled, so set up a chat session
                setupChat();
            } else {
                // User did not enable Bluetooth or an error occurred
                Log.d(TAG, "BT not enabled");
                Toast.makeText(this, R.string.bt_not_enabled_leaving, Toast.LENGTH_SHORT).show();
                finish();
            }
            break;*/
        }
    }

    private void connectDevice(Intent data, int secure) {
        // Get the device MAC address
        String address = data.getExtras().getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
        // Get the BluetoothDevice object
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        // Attempt to connect to the device
        mChatService.connect(device, secure);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.option_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Intent serverIntent = null;
        switch (item.getItemId()) {
        case R.id.rfcomm_connect_scan:
            // Launch the DeviceListActivity to see devices and do scan
            serverIntent = new Intent(this, DeviceListActivity.class);
            startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE_RFCOMM);
            return true;
        case R.id.discoverable:
            // Ensure this device is discoverable by others
            ensureDiscoverable();
            return true;
        }
        return false;
    }
}
