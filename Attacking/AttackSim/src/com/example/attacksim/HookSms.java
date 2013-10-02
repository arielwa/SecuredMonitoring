package com.example.attacksim;

import java.util.List;

import android.app.PendingIntent;
import android.os.RemoteException;

public class HookSms extends ISms.Stub {
	
	ISms oISms;
	
	public HookSms(ISms oISms) {
		this.oISms = oISms;
	}
	
	public List<SmsRawData> getAllMessagesFromIccEf() throws RemoteException {
		// TODO what?
		return null;
	}
		
	@Override
	public boolean updateMessageOnIccEf(int messageIndex, int newStatus,
			byte[] pdu) throws RemoteException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean copyMessageToIccEf(int status, byte[] pdu, byte[] smsc)
			throws RemoteException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void sendData(String destAddr, String scAddr, int destPort,
			byte[] data, PendingIntent sentIntent, PendingIntent deliveryIntent)
			throws RemoteException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void sendText(String destAddr, String scAddr, String text,
			PendingIntent sentIntent, PendingIntent deliveryIntent)
			throws RemoteException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void sendMultipartText(String destinationAddress, String scAddress,
			List<String> parts, List<PendingIntent> sentIntents,
			List<PendingIntent> deliveryIntents) throws RemoteException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public boolean enableCellBroadcast(int messageIdentifier)
			throws RemoteException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean disableCellBroadcast(int messageIdentifier)
			throws RemoteException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean enableCellBroadcastRange(int startMessageId, int endMessageId)
			throws RemoteException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean disableCellBroadcastRange(int startMessageId,
			int endMessageId) throws RemoteException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public int getPremiumSmsPermission(String packageName)
			throws RemoteException {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void setPremiumSmsPermission(String packageName, int permission)
			throws RemoteException {
		// TODO Auto-generated method stub
		
	}

}
