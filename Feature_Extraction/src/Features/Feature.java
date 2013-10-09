package Features;

import java.io.BufferedWriter;
import java.io.IOException;

import Analyze.AnalyzeByFunctions;

public class Feature {
	String _AttributeDeclaration;
	String _type;
	public int    _value;
	public Feature(String attribute, String type, int value) {
		_AttributeDeclaration = attribute;
		_value = value;
		_type = type;
	}
	public void getAttributeDeclaration(BufferedWriter out) throws IOException {
		out.append("@ATTRIBUTE " + _AttributeDeclaration + AnalyzeByFunctions.currentFeatureNumber() + " " + _type + "\n");

	}
}

