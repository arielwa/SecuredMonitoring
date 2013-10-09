package Analyze;

import java.util.HashMap;

public class Tag {
	private String _name;
	private int _funcInfo;
	private int _size;
	private String _type;
	private String _string;
	//private byte[] _value;
	private HashMap<Integer, byte[]> _valuesHash = new HashMap<Integer, byte[]>();

	//tags helpers
	final static int NAME_SIZE=4;
	final static int FUNC_SIZE=4;
	final static int LENGTH_SIZE=4;
	final static int TAG_SIZE=NAME_SIZE+FUNC_SIZE+LENGTH_SIZE+NAME_SIZE;

	public Tag(String name, int funcInfo, int size) {
		super();
		this._name = name;
		this._funcInfo = funcInfo;
		this._size = size;
		this._type=getTypeFromName(name);
	}
	
	public Tag(String wantToBeTag) {		
		super(); 
		this._name=getNameFromTag(wantToBeTag);
		this._funcInfo=getFuncInfoFromTag(wantToBeTag);
		this._size=getSizeFromTag(wantToBeTag);
		this._string=wantToBeTag;
		this._type=getTypeFromName(_name);
	}

	public String getName() {
		return _name;
	}

	private String getNameFromTag(String tag) {
		return tag.substring(0,NAME_SIZE);
	}

	public int getFuncInfo() {
		return _funcInfo;
	}

	public int getFuncInfoFromTag(String tag) {
		int place=NAME_SIZE;
		return changeToInt(tag.substring(place, place+FUNC_SIZE)); 
	}

	public int getSize() {
		return _size;
	}

	public int getSizeFromTag(String tag) {
		int place=NAME_SIZE+FUNC_SIZE;
		String part1 = tag.substring(place, place+(LENGTH_SIZE/2));
		String part2 = tag.substring(place+(LENGTH_SIZE/2), place+LENGTH_SIZE);
		String sizeStr = part2+part1;
		int sizeNum=0;
		for (int i=sizeStr.length()-1; i>=0; i--){
			sizeNum+=(int) (changeToInt(sizeStr.substring(i, i+1))* Math.pow(16, sizeStr.length()-1-i));
		} 
		return sizeNum*2;
	}
	
	public String getType() {
		return _type;
	}

	private String getTypeFromName(String name) {
		char type=name.charAt(3);
		if(type=='1')
			return "int";
		if(type=='2')
			return "double";
		if(type=='3')
			return "char";
		if(type=='4')
			return "pointer";
		if(type=='5')
			return "array";
		return "string";
	}
	

	public byte[] getValue(Integer key) {
		return _valuesHash.get(key);
	}

	public void setValue(Integer key, String valueAsString) {
		this._valuesHash.put(key, valueAsString.getBytes());
	}
	
	public String getString() {
		return _string;
	}
	
	private static int changeToInt(String substring) {
		int ans=0;
		for(int i=0; i<substring.length(); i++){
			if(substring.charAt(i)>='a')
				ans+=substring.charAt(i)-'a'+10;
			else
				ans=ans+substring.charAt(i)-'0';
			if(ans!=0)
				ans*= Math.pow(10,substring.length()-i-1);
		}
		return ans;
	}
}