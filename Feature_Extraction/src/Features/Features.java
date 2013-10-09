package Features;

import java.util.ArrayList;


public interface Features {

	public	ArrayList<Feature> calculateFeature(byte[] value);
	public String getFeatures();
	public String getArffAttributeDeclarations();
}
