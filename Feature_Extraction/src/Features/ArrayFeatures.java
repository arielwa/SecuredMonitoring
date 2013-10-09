package Features;

import java.util.ArrayList;

public class ArrayFeatures implements Features  {

	ArrayList<Feature> features= new ArrayList<Feature>(2);
	private String _val;

	@Override
	public ArrayList<Feature> calculateFeature(byte[] value) {
		_val = changeToString(value);
		features.clear();
		features.add(new Feature("Value", "NUMERIC", getValue(value)));
		features.add(new Feature("Length","NUMERIC",value.length));		
		features.add(new Feature("Sorted","{0,1}",isSorted(value)));		

		return features;
	}
	
	private int getValue(byte[] value) {
		int intValue = 0;
		
		for(int i = 0; i < value.length; i++)
			intValue = intValue * 10 + value[i];
		
		return intValue;
	}

	public String changeToString(byte[] value) {
		String d = new String(value);
		return d;
	}

	private Integer isSorted(byte[] value) {
		for(int i=0; i< value.length-1; i++)
			if(value[i]>value[i+1])
				return 0;
		return 1;
	}

	private Integer arrayLength(byte[] value) {
		return value.length;
	}
	
	@Override
	public String getFeatures() {
		String ans="" + features.get(0);
		for(int i=1; i< features.size(); i++)
			ans+=", "+ features.get(i);
		//ans+=", ARRAY";
		return ans;
	}

	@Override
	public String getArffAttributeDeclarations() {
		// TODO Auto-generated method stub
		return null;
	}

	
}

