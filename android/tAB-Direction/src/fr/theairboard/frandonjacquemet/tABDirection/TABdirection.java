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


/**
 * @author jacquemet.julien@gmail.com
 *
 * 
 */

package fr.theairboard.frandonjacquemet.tABDirection;



import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;
import android.widget.TextView;

/**
 * This is the main Activity that displays the current chat session.
 */
public class TABdirection extends BlunoLibrary {
    // Debugging
    private static final String TAG = "TABdirection";
    private static final boolean D = true;

    // the directions...
    private static final char DIR_RIGHT = 'r';
    private static final char DIR_LEFT = 'l';
    private static final char STOP_RIGHT = 't';
    private static final char STOP_LEFT = 'm';

    TextView tvState;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(D) Log.e(TAG, "+++ ON CREATE +++");

        // Set up the window layout
        setContentView(R.layout.main);
        onCreateProcess();

        setupChat();
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

    protected void onResume(){
        super.onResume();
        onResumeProcess();														//onResume Process by BlunoLibrary
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        onActivityResultProcess(requestCode, resultCode, data);					//onActivityResult Process by BlunoLibrary
        super.onActivityResult(requestCode, resultCode, data);
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
        onDestroyProcess();														//onDestroy Process by BlunoLibrary
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
    public void onSerialReceived(String theString) {							//Once connection data received, this function will be called
        // TODO Auto-generated method stub
        //serialReceivedText.append(theString);							//append the text into the EditText
        //The Serial data from the BLUNO may be sub-packaged, so using a buffer to hold the String is a good choice.
    }

    private void setupChat() {
        Log.d(TAG, "setupChat()");

        tvState = (TextView)findViewById(R.id.state);
        tvState.setText(R.string.title_not_connected);


        findViewById(R.id.left).setOnTouchListener(new DirTouchListener(DIR_LEFT, STOP_LEFT));
        findViewById(R.id.right).setOnTouchListener(new DirTouchListener(DIR_RIGHT, STOP_RIGHT));

        setColorButtonsEnabled(false);

        serialBegin(115200);
    }

    private void setColorButtonsEnabled(boolean en) {
        findViewById(R.id.left).setClickable(en);
        findViewById(R.id.right).setClickable(en);
    }

    // onClickListener for color buttons
    private class DirTouchListener implements View.OnTouchListener {
        private char mDir = '0';
        private char mStop = '0';
        public DirTouchListener(char dir, char stop) { mDir = dir; mStop = stop; }
        public boolean onTouch(View v, MotionEvent event)
        {
            if (v.isClickable()) {
                if (event.getAction() == MotionEvent.ACTION_DOWN)
                    sendMessage(mDir);
                else if (event.getAction() == MotionEvent.ACTION_UP)
                    sendMessage(mStop);
            }
            return false;
        }
    }

    private void sendMessage(char message) {
        // Check that we're actually connected before trying anything
        if (tvState.getText().toString().equals(R.string.title_connected)) {
            Toast.makeText(this, R.string.title_not_connected, Toast.LENGTH_SHORT).show();
            return;
        }
        //Toast.makeText(this, Character.toString(message), Toast.LENGTH_SHORT).show();
        serialSend(Character.toString(message));

    }

    private void ensureDiscoverable() {
        if(D) Log.d(TAG, "ensure discoverable");
        Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
        discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
        startActivity(discoverableIntent);
    }

}
