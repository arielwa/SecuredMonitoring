package Features;

import Analyze.Struct;

public class FeaturesFactory {

	public static Features getFeature(int featureCode) {
	
		switch (featureCode) {
		
		case 0x5711:
			return new NumberFeatures();
		case 0x5712:
			return new NumberFeatures();
		case 0x5713:
			return new CharFeatures();
		case 0x5714:
			return new PointerFeatures();
		case 0x5715:
			return new ArrayFeatures();
			
		}
		
		
		return null;
	}
}
