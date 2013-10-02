package com.example.attacksim;

import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.net.ConnectivityManager;
import android.os.Bundle;
import android.os.Process;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.telephony.SmsManager;
import android.view.Menu;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;

public class AttackSim extends Activity {

	public int myUid;
	public TextView tv;
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_attack_sim);
		
		myUid = Process.myUid();
		tv = (TextView)findViewById(R.id.textView);

		final Button rootButton = (Button) findViewById(R.id.root_button);
        rootButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View arg0) {
				changeUid(0);		
				tv.setText("Changed uid to root.\n");
			}
		});
        
        final Button unrootButton = (Button) findViewById(R.id.unroot_button);
        unrootButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View arg0) {
				changeUid(myUid);
				tv.setText("Changed uid to user.\n");
			}
		});
        
        final Button rootOpButton = (Button) findViewById(R.id.root_op_button);
        rootOpButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View arg0) {
				tv.setText("UID: " + Integer.toString(Process.myUid()) + "\n");
		        tv.append("Perform root operation: " + isNetworkAvailable() + "\n");
			}
		});
        
        final Button hookSyscallButton = (Button) findViewById(R.id.hook_syscall_button);
        final Spinner spinner = (Spinner) findViewById(R.id.spinner);
        hookSyscallButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View arg0) {
				if (((String)spinner.getSelectedItem()).equals("Ioctl"))
					hookSyscall(54);
				else
					hookSyscall(spinner.getSelectedItemPosition() + 3);
				
				tv.setText("Hooked system call " + spinner.getSelectedItem().toString() + "\n");
				tv.append("Look at dmesg in adb to see the effects.\n");
			}
		});
        
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
                R.array.syscalls, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);
        
        final Button contextManagerButton = (Button) findViewById(R.id.context_manager_button);
        contextManagerButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View arg0) {
				changeContextManager();		
				tv.setText("Became the Context Manager,\n" +
						"Prepare for chaos.\n");
			}
		});
	}
	
	private native void changeUid(int uid);
	private native void hookSyscall(int syscall);
	private native void changeContextManager();
	
	private boolean isNetworkAvailable() {
		try {
			//ConnectivityManager cm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
			//cm.getActiveNetworkInfo();
			AlarmManager am = (AlarmManager) getSystemService(Context.ALARM_SERVICE);
			am.setTime(999999999);
			//SmsManager sms = SmsManager.getDefault();
		    //sms.sendTextMessage("0544444444", null, "yo", null, null);
			//LocationManager lm = (LocationManager)getSystemService(Context.LOCATION_SERVICE);
			//Location loc = lm.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
			return true;
		} catch (SecurityException e) {
			e.printStackTrace();
			return false;
		}
	}
	
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.attack_sim, menu);
		return true;
	}

	static {
        System.loadLibrary("attack_sim");
	}
}
