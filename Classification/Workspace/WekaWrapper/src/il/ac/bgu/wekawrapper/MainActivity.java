package il.ac.bgu.wekawrapper;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.nio.ByteBuffer;
import java.util.Random;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity implements ObserverActivity
{
	private final String FILE = Environment.getExternalStorageDirectory().getPath() + "/Test.txt";
	public static MainActivity activity = null;
	
	static
	{
		System.loadLibrary("weka_jni");
	}
	
	private static native void init();
	private static native int readMemory(Object buffer, int bufferSize);
	private static native Object getMemoryBuffer(int bufferSize);
	
	private final static char[] hexArray = "0123456789ABCDEF".toCharArray();
	public static String bytesToHex(byte[] bytes) {
	    char[] hexChars = new char[bytes.length * 2];
	    int v;
	    for ( int j = 0; j < bytes.length; j++ ) {
	        v = bytes[j] & 0xFF;
	        hexChars[j * 2] = hexArray[v >>> 4];
	        hexChars[j * 2 + 1] = hexArray[v & 0x0F];
	    }
	    return new String(hexChars);
	}
	
	public static void showByteArray(final byte[] arBuffer)
	{
		activity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				activity.alertBox("Byte Array", bytesToHex(arBuffer));
			}
		});
	}
	
/*	FileObserver Sauron = new FileObserver(FILE)
	{
		@Override
		public void onEvent(int event, String path)
		{
			MainActivity.activity.alertBox("Finally", "File changed!");
		}
	};*/
	
	MyFileObserver Sauron = new MyFileObserver(FILE);
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		MainActivity.activity = this;
		setContentView(R.layout.activity_main);
		final Button btnChangeFile = (Button) findViewById(R.id.btnChangeFile);
		btnChangeFile.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				Random r = new Random();
				writeToFile(FILE, Integer.toString(r.nextInt()));
			}
		});
		
		final Button btnInit = (Button) findViewById(R.id.btnModuleInit);
		btnInit.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				init();
				byte[] arTest = new byte[]{0x11, 0x22, 0x33, 0x7F};
				showByteArray(arTest);
			}
		});
		
		final Button btnReadMemory = (Button) findViewById(R.id.btnModuleReadMemory);
		btnReadMemory.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				ByteBuffer bbBuffer= ByteBuffer.allocateDirect(16);
				byte[] arBuffer = bbBuffer.array();
				readMemory(bbBuffer, 16);
				showByteArray(arBuffer);
			}
		});
		
		final Button btnGetBuffer = (Button) findViewById(R.id.btnModuleGetBuffer);
		btnGetBuffer.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				ByteBuffer objBuffer = (ByteBuffer)getMemoryBuffer(16);
				//final byte[] arBuffer = objBuffer.array();
				byte[] arBuffer = new byte[16];
				objBuffer.get(arBuffer);
				showByteArray(arBuffer);
			}
		});
		
		Sauron.registerObserver(this);
		Sauron.startWatching();
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	public void alertBox(String title, String mymessage)
	{
		new AlertDialog.Builder(this)
				.setMessage(mymessage)
				.setTitle(title)
				.setCancelable(true)
				.setNeutralButton(android.R.string.cancel,
						new DialogInterface.OnClickListener()
						{
							public void onClick(DialogInterface dialog,
									int whichButton)
							{
							}
						}).show();
	}
	
	private void writeToFile(String path, String data)
	{
		OutputStreamWriter outputStreamWriter = null;
		try
		{
			outputStreamWriter = new OutputStreamWriter(new FileOutputStream(FILE));
			outputStreamWriter.write(data);
		}
		catch (IOException e)
		{
			Log.e("Exception", "File write failed: " + e.toString());
		}
		finally
		{
			if (outputStreamWriter != null)
			{
				try
				{
					outputStreamWriter.close();
				}
				catch (IOException e)
				{
					Log.e("Exception", "Close file failed: " + e.toString());
				}
			}
		}
	}
	
	@Override
	protected void onPause()
	{
		Sauron.stopWatching();
		Sauron.unregisterObserver(this);
		super.onPause();
	}
	
	@Override
	protected void onResume()
	{
		super.onResume();
		Sauron.registerObserver(this);
		Sauron.startWatching();
	}
	
	@Override
	public void onFileObserved(int Event, String path)
	{
		this.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				alertBox("Finally", "File changed!");
			}
		});
	}
}
