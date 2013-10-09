package Analyze;

import java.util.ArrayList;

public class Struct {
	public String tag;
	public ArrayList<StructField> fields = new ArrayList<StructField>();
	
	
	public Struct(String tag, ArrayList<StructField> fields) {
		this.tag = tag;
		this.fields = fields;
	}


	public Struct() {
		// TODO Auto-generated constructor stub
	}
}
