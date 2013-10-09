import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;


public class Main {
	public static void main(String[] args) throws IOException {
		ServerSocket ss = new ServerSocket(6666);
	
		Socket client = ss.accept();
		
		System.out.println("Connection from ip " + client.getInetAddress());

		client.close();
		ss.close();
	}
}
