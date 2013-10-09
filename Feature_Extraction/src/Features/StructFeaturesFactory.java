package Features;

import java.util.ArrayList;

import Analyze.Struct;
import Features.StructFeatures.StructFeature;

public class StructFeaturesFactory {
	public static StructFeatures getStructFeatures(Struct myStruct) {
		
		StructFeatures structFeatures = new StructFeatures();
		ArrayList<StructFeature> structFeatureArray = new ArrayList<StructFeature>();
		for(int structFeatureIndex = 0; 
				structFeatureIndex < myStruct.fields.size(); 
				structFeatureIndex++) { 
			structFeatures.addStructFeature(myStruct.fields.get(structFeatureIndex).size, FeaturesFactory.getFeature(myStruct.fields.
					get(structFeatureIndex).type));

		}

		structFeatures._StructListOfFeatures = structFeatureArray;
		return structFeatures;
	}
}
