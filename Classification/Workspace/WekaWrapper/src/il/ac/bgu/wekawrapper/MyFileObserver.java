package il.ac.bgu.wekawrapper;

import java.util.HashSet;
import android.os.FileObserver;

/**
 * @author Tomer Godinger
 * @since 2013.09.18
 */
public class MyFileObserver extends FileObserver{
	  private HashSet<ObserverActivity> registeredObservers;

	public MyFileObserver(String path) {
	    super(path, FileObserver.ALL_EVENTS);
	    registeredObservers = new HashSet<ObserverActivity>();
	}

	public void registerObserver(ObserverActivity toRegister){
	  registeredObservers.add(toRegister);
	}

	public void unregisterObserver(ObserverActivity toUnregister){
	  registeredObservers.remove(toUnregister);
	}

	public void onEvent(int Event, String path){
	  for(ObserverActivity o: this.registeredObservers){
	    o.onFileObserved(Event, path);
	  }
	}
}
