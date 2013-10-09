package il.ac.bgu.wekawrapper;

/**
 * @author Tomer Godinger
 * @since 2013.09.18
 */
public interface ObserverActivity
{
	public void onFileObserved(int Event, String path);
}
