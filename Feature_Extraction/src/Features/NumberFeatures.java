package Features;

import java.util.ArrayList;

public class NumberFeatures implements Features {

	ArrayList<Feature> features = new ArrayList<Feature>();


	@Override
	public ArrayList<Feature> calculateFeature(byte[] value) {
		features.clear();
		int num = changeToInt(value);
		//features.add(num);
		features.add(new Feature("Value", "NUMERIC", num));
		features.add(new Feature("Positive", "{0,1}", isPositive(num)));
		features.add(new Feature("Prime", "{0,1}", isPrime(num)));
		features.add(new Feature("IntValue", "NUMERIC", intValue(value)));
		features.add(new Feature("Odd", "{0,1}", isOdd(num)));
		features.add(new Feature("Negative", "{0,1}", isNegative(num)));
		return features;
	}

	private Integer isNegative(int num) {
		if (num < 0)
			return 1;
		return 0;
	}

	private int isOdd(int num) {
		return num % 2;
		/* if (num % 2 == 1)
			return 1;
		return 0; */
	}

	private int intValue(byte[] value) {
		if (value.length > 4)
			return 1; // long
		return 0;
	}

	private int isPrime(int num) {
		System.out.println("For prime: " + num);
		if ((num < 0)||(num == 0) || (num == 1))
			return 0;
		for (int i = 2; i < (num / 2); i++)
			if (num % i == 0)
				return 0;
		return 1;
	}

	private int isPositive(int num) {
		if (num > 0)
			return 1;
		return 0;
	}

	private int changeToInt(byte[] val) {
		int ans = 0;
		//System.out.println("Val: " + val.toString());
		for (int i = 0; i < val.length; i++) {
			if (val[i] >= 'a')
				ans += val[i] - 'a' + 10;
			else
				ans = ans + val[i] - '0';
			if (ans != 0)
				ans *= Math.pow(10, val.length - i - 1);
		//System.out.println(ans);	
		//System.out.println("Finished going over: " + val[i]);
		}
		return ans;
	}

	@Override
	public String getFeatures() {
		String ans = "" + features.get(0);
		for (int i = 1; i < features.size(); i++)
			ans += ", " + features.get(i);
		//ans += ", NUMBER";
		return ans;
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
