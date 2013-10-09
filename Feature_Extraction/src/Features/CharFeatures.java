package Features;

import java.util.ArrayList;

public class CharFeatures implements Features  {

	public ArrayList<Feature> features=new ArrayList<Feature>();
	public char _val;

	@Override
	public ArrayList<Feature> calculateFeature(byte[] value) {
		features.clear();

		//_val = changeToChar(value);
		features.add(new Feature("Value", "NUMERIC", changeToChar(value)));

		return features;
	}

	public char changeToChar(byte[] value) {
		String d = new String(value);
		StringBuilder output = new StringBuilder();
		for (int i = 0; i < d.length(); i += 2) {
			String str = d.substring(i, i + 2);
			output.append((char) Integer.parseInt(str, 16));
		}
		return output.charAt(0);
	}
	
	@Override
	public String getFeatures() {
		//String ans="Char" + features.get(0);
		String ans="";
		for(int i=0; i< features.size(); i++)
			ans+=features.get(i) + ",";
		//ans+="CHAR";
		return ans;
	}

	public   String getArffAttributeDeclarations() {
		String declaration = "% Char \n@ATTRIBUTE charValue   CHAR";
		return declaration;

	}

	
}
