package Analyze;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Vector;
import java.net.*;

import files.FileDescriptor;
import Features.ArrayFeatures;
import Features.CharFeatures;
import Features.Feature;
import Features.Features;
import Features.NumberFeatures;
import Features.PointerFeatures;
import Features.StringFeatures;
import Features.StructFeaturesFactory;

public class AnalyzeByFunctions {


	// tags helpers
	private final static int NAME_SIZE = 4;
	private final static int FUNC_SIZE = 4;
	private final static int LENGTH_SIZE = 4;
	private final static int TAG_SIZE = NAME_SIZE + FUNC_SIZE + LENGTH_SIZE  + NAME_SIZE;

	private static HashMap<Integer,Features> _TagToFeatures = new HashMap<Integer,Features>();
	private static HashMap<Integer, ArrayList<Feature>> _functionsToFeatures = new HashMap<Integer, ArrayList<Feature>>();
	private static HashMap<Integer, BufferedWriter> _funcAttList = new HashMap<Integer, BufferedWriter>(); 
	private static int _filesNumber =0;
	private static int _currFeatureNumber = 0;
	private final static String snapshotName = "snapshot";
	private static  int snapshotNumber = 0;

	public static void main(String[] args) throws Exception {
		Runtime.getRuntime().exec("/home/eyalshem/Android/emulatorScript &");

		init();
		String snapshotDirectory = "/home/eyalshem/Android/AndroidSDK/sdk/tools";	
		File dir = new File("/home/eyalshem/Android/Snapshots");
		String filePath = "";
		while(true) {
			
			String[] files = dir.list();
			System.out.println("Files: " + files.length);

			filePath = snapshotDirectory+"/"+snapshotName + snapshotNumber;
			System.out.println(filePath);
			//_filesNumber++;

			File currentSnapshot = new File(filePath);
			while(!currentSnapshot.exists())
			{}	
		
			ExtractFeaturesFromSnapshot(filePath);
			System.out.println("done "+_filesNumber);
			
			
			outputFeaturesToFunctionFiles();
			closeAtt();
			snapshotNumber++;
			
			
		}
	}



	private static void init() throws IOException {
		

		takeSnapshot(snapshotName,  snapshotNumber);
		

		_TagToFeatures.put(0x5711,new NumberFeatures());
		_TagToFeatures.put(0x5712, new NumberFeatures());
		_TagToFeatures.put(0x5713, new CharFeatures());
		_TagToFeatures.put(0x5714, new PointerFeatures());
		_TagToFeatures.put(0x5715, new ArrayFeatures());

		// Converts annotation of types in struct files to tags
		HashMap<Integer,Integer> numberToTag = new HashMap<Integer,Integer>();
		numberToTag.put(1,0x5711);
		numberToTag.put(2,0x5712);
		numberToTag.put(3,0x5713);
		numberToTag.put(4, 0x5714);
		numberToTag.put(5,0x5715);
		File dir = new File("/home/eyalshem/Android/structs");
		String[] files = dir.list();	
		String tag = "";
		for(int i = 0; i<files.length; i++) {
			System.out.println(files[i]);
			BufferedReader br = new BufferedReader(new FileReader("/home/eyalshem/Android/structs/"+ files[i]));
			String line = br.readLine();
			line = br.readLine();
			ArrayList<StructField> fields = new ArrayList<StructField>();
			if(line != null  && line.length()  > 6) { //Second line: obtain tag
				String[] words = line.split("\\s+");
				tag = words[1];
			}

			line = br.readLine();
			while (line != null && line.length()  > 2) {
				String[] words = line.split("\\s+");
				if(words.length > 1)
					fields.add(new StructField(numberToTag.get(Integer.parseInt(words[0])), Integer.parseInt(words[1])));
				line = br.readLine();
			}
			//TODO: change "tag" from hex string to number
			// Put struct features in tags to features
			System.out.println("TAG: " + tag);
			_TagToFeatures.put(Integer.valueOf(tag),StructFeaturesFactory.getStructFeatures(new Struct(tag, fields)));	        	        	      
		} 

	}
	public static  int currentFeatureNumber() {
		return _currFeatureNumber++;
	}


	private static void closeAtt() throws IOException{
		for (BufferedWriter writer : _funcAttList.values()) {
			writer.close();
		}
	}



	private static void outputFeaturesToFunctionFiles() throws IOException {
		

		for (Integer functionID : _functionsToFeatures.keySet()) {
			File functionFile = new File(getFunctionFileName(functionID.intValue()));

			ArrayList<Feature> currentFunctionFeatures = _functionsToFeatures.get(functionID);
			BufferedWriter currentFunctionFileBuffer = null;
			//while(qul == 1);
			
			if (!functionFile.exists())
			{
				currentFunctionFileBuffer = new BufferedWriter(new FileWriter(getFunctionFileName(functionID)));

				System.out.println("File doesn't exist...yet ;) \n\n");

				currentFunctionFileBuffer.write("@RELATION Function"+functionID);
				currentFunctionFileBuffer.newLine();
				currentFunctionFileBuffer.newLine();
				currentFunctionFileBuffer.write("@ATTRIBUTE snapshotNumber NUMERIC");
				currentFunctionFileBuffer.newLine();
				for (Feature currentFunctionFeature : currentFunctionFeatures) {
					currentFunctionFeature.getAttributeDeclaration(currentFunctionFileBuffer);
				}  
				currentFunctionFileBuffer.newLine();	
				currentFunctionFileBuffer.write("@ATTRIBUTE class {benign, attack}");
				currentFunctionFileBuffer.newLine();
				currentFunctionFileBuffer.write("@DATA");
				currentFunctionFileBuffer.newLine(); 
				currentFunctionFileBuffer.close();
			}
			System.out.println("Let's just say...file exists now ;) \n\n");
			FileWriter fileWriter = new FileWriter(functionFile, true);
			//Function file already exists
			fileWriter.append("\n");

			Feature currentFeature  = null ;// = currentFunctionFeatures.get(0);
			System.out.println(currentFunctionFeatures.size());
			fileWriter.append(snapshotNumber + ", ");
			for(int i = 0; i < currentFunctionFeatures.size(); i++) {
				currentFeature = currentFunctionFeatures.get(i);
				fileWriter.append(currentFeature._value + ", ");
			}
			fileWriter.append("benign\n");
			fileWriter.close();
		


		}
	}

	static String getFunctionFileName(int functionID)
	{
		return "Function_" + functionID + ".arff";
	}

	/*At a file, go over it and check for every current string if it's in the format of a tag. 
	check the function and add it to the function has (if it exists create new list)
	 */
	static void ExtractFeaturesFromSnapshot(String filePath) throws Exception {

		System.out.println("Finding atts for: " + filePath);
		FileDescriptor currFile = new FileDescriptor(filePath);		
		for (Integer funcID : _functionsToFeatures.keySet()) {
			_functionsToFeatures.get(funcID).clear();
		}
		//f for file. h for hex
		long hFileLength = currFile.length();		
		
		for ( long hCurrTagIndex = 0; hCurrTagIndex < hFileLength - (TAG_SIZE * 2); hCurrTagIndex++) {

			int currTagName = currFile.getHexInt(hCurrTagIndex);
			if (!_TagToFeatures.keySet().contains(currTagName)) continue;
			String currFullTag = currFile.getHexString(hCurrTagIndex, TAG_SIZE);
			if (isTag(currFullTag)) {
				// CASE: the first 16 chars tag has same start and finish
				System.out.println("currFullTag "+hCurrTagIndex+" "+currFullTag+ " "+isTag(currFullTag));
				Tag currTag = new Tag(currFullTag);									
				int hTagSize = currTag.getSize();
				hTagSize = round4Up(hTagSize/2)*2;

				// check tag at the end of the value
				String currData = currFile.getHexString(hCurrTagIndex + TAG_SIZE, hTagSize);									
				String currNextTag = currFile.getHexString(hCurrTagIndex + TAG_SIZE + hTagSize, TAG_SIZE);
				currTag.setValue(_filesNumber,currData);
				if (currNextTag.equals(currTag.getString())) {	
					// CASE: Found complete tag
					if (_functionsToFeatures.get(currTag.getFuncInfo()) == null) {												
						_functionsToFeatures.put(currTag.getFuncInfo(), new ArrayList<Feature>());
					} 
					Features currFeatures = _TagToFeatures.get(currTagName);
					//System.out.println(currTag.getName());
					//System.out.println(currFeatures == null);
					ArrayList<Feature> allFeatures = currFeatures.calculateFeature(currData.getBytes());
					System.out.println("Adding " + allFeatures.size() + " features");
					_functionsToFeatures.get(currTag.getFuncInfo()).addAll(allFeatures);										
					hCurrTagIndex += TAG_SIZE*2 + hTagSize - 1;
				}
				// /CASE: the firs t 16 chars tag has same start and finish
			}
			else{
				hCurrTagIndex=hCurrTagIndex+4; 
			}
		}


	}

	private static int round4Up(int hTagSize) {
		if (hTagSize % 4 == 0)
			return hTagSize;
		if ((hTagSize + 1) % 4 == 0)
			return (hTagSize + 1);
		if ((hTagSize + 2) % 4 == 0)
			return (hTagSize + 2);
		return (hTagSize + 3);
	}

	static String readFile(String path, Charset encoding) throws IOException {
		byte[] encoded = Files.readAllBytes(Paths.get(path));
		return encoding.decode(ByteBuffer.wrap(encoded)).toString();
	}

	private static boolean isTag(String tag) {
		String temp = tag.substring(0, NAME_SIZE);
		if (tag.substring(TAG_SIZE - NAME_SIZE).equals(temp))
			return true; // found a tag
		return false;// there is no tag found
	}

	private static void takeSnapshot(String snapshotName, int snapshotCount) throws IOException {
		
		String snapshotCommand = "/home/eyalshem/Android/takeSnapshot.sh " + snapshotName ;
		System.out.println("Attempting to perform: " + snapshotCommand);
		Runtime.getRuntime().exec(snapshotCommand);		
	}

}