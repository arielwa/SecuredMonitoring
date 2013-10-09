package Features;

import java.util.ArrayList;


public class StringFeatures implements Features {

	ArrayList<Feature> features=new ArrayList<Feature>(4);
	private String _val;
	@Override
	public ArrayList<Feature> calculateFeature(byte[] value) {
		features.clear();

		features.add(new Feature("Count", "NUMERIC", wordCount(value)));
		features.add(new Feature("Value", "NUMERIC", stringValue(value)));
		features.add(new Feature("Size", "NUMERIC", stringSize(value)));
		features.add(new Feature("Delimiter", "NUMERIC", DelimiterCount(value)));		
		_val = changeToString(value);
		return features;
	}

	public String changeToString(byte[] value) {
		String d = new String(value);
		return d;
	}
	
	private int DelimiterCount(byte[] value) {
		int count=0;
		for(int i=0; i< value.length; i++)
			if( value[i]==' ')
				count++;
		return count;
	}

	private int stringSize(byte[] value) {
		return value.length;
	}

	private int stringValue(byte[] value) { // if alpha nomery
		for(int i=0; i< value.length; i++)
			if( value[i]>='0' || value[i]<='9')
				return 1;
		return 0;
	}

	private int wordCount(byte[] value) {
		int count=0;
		for(int i=0; i< value.length-1; i++)
			if( value[i]==' '){
				i++;
				if( value[i]!=' ')
					count++;
			}
		return count;	
	}

	public String getFeatures() {
		String ans="" + features.get(0);
		for(int i=1; i< features.size(); i++)
			ans+=", "+ features.get(i);
		//ans+=", STRING";
		return ans;
	}

	@Override
	public String getArffAttributeDeclarations() {
		// TODO Auto-generated method stub
		return null;
	}
	
}
