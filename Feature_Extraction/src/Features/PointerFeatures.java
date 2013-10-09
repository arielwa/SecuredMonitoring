package Features;

import java.util.ArrayList;

public class PointerFeatures implements Features{
	public ArrayList<Feature> features=new ArrayList<Feature>();
	private String _val;



	@Override
	public ArrayList<Feature> calculateFeature(byte[] value) {
		features.add(new Feature("Value", "NUMERIC", getValue(value)));

		_val = changeToString(value);
		return features;
	}

	@Override
	public String getFeatures() {
		String ans = "";
		for (int i = 0; i < features.size(); i++)
			ans += features.get(i) + ",";
		//ans += "POINTER";
		return ans;
	}

	
	
	public String changeToString(byte[] value) {
		String d = new String(value);
		StringBuilder output = new StringBuilder();
		for (int i = 0; i < d.length(); i += 2) {
			String str = d.substring(i, i + 2);
			output.append((char) Integer.parseInt(str, 16));
		}
		//return output.toString();
		return "0x"+d;
	}

	@Override
	public String getArffAttributeDeclarations() {
		// TODO Auto-generated method stub
		return null;
	}
	
	public int getValue(byte[] value) {
		int intValue = 0;
		
		for(int i = 0; i < value.length; i++)
			intValue = intValue * 10 + value[i];
		
		return intValue;
	}
	
	
	
}
