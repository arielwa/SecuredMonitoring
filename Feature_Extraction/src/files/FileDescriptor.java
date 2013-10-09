package files;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class FileDescriptor {
		
	private final static int READ_BUFFER_SIZE = 0x20bb;
	String _filePath;
	String _fileData;
	
	public FileDescriptor(String filePath) throws IOException {
		_filePath = new String(filePath);
		StringBuffer fileData = new StringBuffer();
		BufferedReader reader = new BufferedReader(new FileReader(filePath));
		char[] buf = new char[READ_BUFFER_SIZE];
		int numRead = 0;
		while ((numRead = reader.read(buf)) != -1) {
			String readData = String.valueOf(buf, 0, numRead);
			fileData.append(readData);
		}
		reader.close();
		_fileData = new String(fileData.toString());
	}

	public String getHexString(long hstart, int hLength) throws IOException {
		int fStart = (int) (hstart/2);
		int fEnd = fStart + (hLength/2);
		String hexStr;
		//if (hstart % 2 == 0){
			hexStr = _fileData.substring(fStart, fEnd);
		//}else{
			//fEnd=fEnd+1;
			//hexStr = _fileData.substring(fStart, fEnd);
		//}	
		
	    StringBuilder sb = new StringBuilder();
	    for (byte b : hexStr.getBytes()) {
	        sb.append(String.format("%02X", b));
	    }
	    hexStr = sb.toString();
	    //if (hstart % 2 != 0){
	    	//hexStr = hexStr.substring(1, hexStr.length()-1);	
	    //}
	    return hexStr;
	}

	public long length() {
		return _fileData.length() * 2;
	}

	public int getHexInt(long hCurrTagIndex) {
		int start = (int) (hCurrTagIndex/2);
		int first = _fileData.charAt(start);
		int second = _fileData.charAt(start+1);
		return (first<<8)|second;
		
	}


}
