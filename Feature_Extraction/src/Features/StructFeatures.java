package Features;

import java.util.ArrayList;

public class StructFeatures implements Features {
	
	public ArrayList<StructFeature> _StructListOfFeatures = new ArrayList<StructFeature>();
	public ArrayList<Feature> 	features;
	@Override
	public ArrayList<Feature> calculateFeature(byte[] value) {
		int currIndexInValue = 0;
		byte fieldValue[]; 
		features.clear();
		features = new ArrayList<Feature>();
		for(int currField = 0; currField<_StructListOfFeatures.size(); currField++)  {
			int currFieldsize = _StructListOfFeatures.get(currField).size;
			fieldValue = new byte[currFieldsize];
			for(int currIndexInField = 0;
			    currIndexInField<currFieldsize && currField + currIndexInField < value.length; 
				currIndexInField++) {
				fieldValue[currIndexInField] = value[currIndexInValue + currIndexInField];
				
			}
			features.addAll( _StructListOfFeatures.get(currField)._feature.calculateFeature(fieldValue));
			currIndexInValue += currFieldsize;
		}
		
		return features;
	}

	@Override
	public String getFeatures() {
		// TODO Auto-generated method stub
		return null;
	}
	public class StructFeature {
		public Features _feature;
		public int      size;
	}
	@Override
	public String getArffAttributeDeclarations() {
		String declarations = "";
		for(int i = 0; i< features.size(); i++)
			declarations = declarations + features.get(i)._AttributeDeclaration + "\n";
		
		return declarations;
	}

	public void addStructFeature(int size, Features feature) {
		
		StructFeature structFeature = new StructFeature();
		structFeature._feature = feature;
		structFeature.size = size;
		_StructListOfFeatures.add(structFeature);
	}
	


}
