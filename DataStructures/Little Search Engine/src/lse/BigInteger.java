package math;

/**
 * This class encapsulates a BigInteger, i.e. a positive or negative integer with 
 * any number of digits, which overcomes the computer storage length limitation of 
 * an integer.
 * 
 */
public class BigInteger {

	/**
	 * True if this is a negative integer
	 */
	boolean negative;
	
	/**
	 * Number of digits in this integer
	 */
	int numDigits;
	
	/**
	 * Reference to the first node of this integer's linked list representation
	 * NOTE: The linked list stores the Least Significant Digit in the FIRST node.
	 * For instance, the integer 235 would be stored as:
	 *    5 --> 3  --> 2
	 */
	DigitNode front;
	
	/**
	 * Initializes this integer to a positive number with zero digits, in other
	 * words this is the 0 (zero) valued integer.
	 */
	public BigInteger() {
		negative = false;
		numDigits = 0;
		front = null;
	}
	
	/**
	 * Parses an input integer string into a corresponding BigInteger instance.
	 * A correctly formatted integer would have an optional sign as the first 
	 * character (no sign means positive), and at least one digit character
	 * (including zero). 
	 * Examples of correct format, with corresponding values
	 *      Format     Value
	 *       +0            0
	 *       -0            0
	 *       +123        123
	 *       1023       1023
	 *       0012         12  
	 *       0             0
	 *       -123       -123
	 *       -001         -1
	 *       +000          0
	 *       
	 * 
	 * @param integer Integer string that is to be parsed
	 * @return BigInteger instance that stores the input integer
	 * @throws IllegalArgumentException If input is incorrectly formatted
	 */
	public static BigInteger parse(String integer) 
	throws IllegalArgumentException {
		//Declare the BigInteger
		BigInteger bigInt = new BigInteger();
		integer = integer.trim();
		//If integer starts with a negative sign assign the negative variable of BigInteger to true
		if(integer.charAt(0) == '-')
		{
			bigInt.negative = true;
		}
		//Get rid of negative/positive sign
		if(integer.charAt(0) == '-' || integer.charAt(0) == '+')
		{
			integer = integer.substring(1);
		}
		//Get rid of leading zeros
		while(integer.charAt(0) == '0' && integer.length() > 1)
		{
			integer = integer.substring(1);
		}
		//parse the integer
		int numDigits = 0;
		DigitNode digit = null;
		for(int digits = integer.length(); digits > numDigits; numDigits = numDigits + 1) 
		{
			if(Character.isDigit(integer.charAt(numDigits)))
			{
				digit = new DigitNode(Character.getNumericValue(integer.charAt(numDigits)), digit);
			}
			else
			{
				throw new IllegalArgumentException();
			}
		}
		bigInt.numDigits = numDigits;
		bigInt.front = digit;
		
		return bigInt;
	}
	
	/**
	 * Adds an integer to this integer, and returns the result in a NEW BigInteger object. 
	 * DOES NOT MODIFY this integer.
	 * NOTE that either or both of the integers involved could be negative.
	 * (Which means this method can effectively subtract as well.)
	 * 
	 * @param other Other integer to be added to this integer
	 * @return Result integer
	 */
	public BigInteger add(BigInteger other) {
		//Figure out which BigInteger is bigger
		BigInteger biggerInt = null;
		BigInteger smallerInt = null;
		if(FirstIsBigger(this, other))
		{
			biggerInt = this;
			smallerInt = other;
		}
		else
		{
			biggerInt = other;
			smallerInt = this;
		}
		//add if both are positive/negative and subtract is only one is negative
		if((!biggerInt.negative && !smallerInt.negative) || (biggerInt.negative && smallerInt.negative))
		{
			return addInts(biggerInt, smallerInt);
		}
		else
		{	
			return subtractInts(biggerInt, smallerInt);
			
		}
	}
	
	private BigInteger addInts(BigInteger biggerInt, BigInteger smallerInt)
	{
		//add digits and put sum into new node
		DigitNode bigInt = biggerInt.front;
		DigitNode smallInt = smallerInt.front;
		DigitNode sum = null;
		boolean carryTheOne = false;
		
		for(int digitCount = 1; digitCount <= biggerInt.numDigits; digitCount = digitCount + 1)
		{
			if (digitCount <= smallerInt.numDigits)//if there are still digits in the small number add them
			{
				int digitSum = bigInt.digit + smallInt.digit;
				if(carryTheOne)
				{
					digitSum = digitSum + 1;
				}
				if(digitSum >= 10)
				{
					digitSum = digitSum - 10;
					carryTheOne = true;
				}
				else
				{
					carryTheOne = false;
				}
				sum = new DigitNode(digitSum, sum);
				
				bigInt = bigInt.next;
				smallInt = smallInt.next;
			}
			else //if there are only digits in the bigger integer 
			{
				int digitSum = bigInt.digit;
				if(carryTheOne)
				{
					digitSum = digitSum + 1;
				}
				if(digitSum >= 10)
				{
					digitSum = digitSum - 10;
					carryTheOne = true;
				}
				else
				{
					carryTheOne = false;
				}
				sum = new DigitNode(digitSum, sum);
				bigInt = bigInt.next;
			}
		}
		if(carryTheOne)
		{
			sum = new DigitNode(1, sum);
		}
		BigInteger finalSum = new BigInteger();
		finalSum.front = Reverse(sum);
		finalSum.numDigits = getDigits(finalSum.front);
		if(biggerInt.negative || smallerInt.negative)
		{
			finalSum.negative = true;
		}
		return finalSum;
	}
	
	private BigInteger subtractInts(BigInteger biggerInt, BigInteger smallerInt)
	{
		//subtract digits and put difference into new node
		DigitNode bigInt = biggerInt.front;
		DigitNode smallInt = smallerInt.front;
		DigitNode sum = null;
		boolean carryTheOne = false;
		
		for(int digitCount = 1; digitCount <= biggerInt.numDigits; digitCount = digitCount + 1)
		{
			if (digitCount <= smallerInt.numDigits)//if there are still digits in the small number subtract them
			{
				int bigDigit = bigInt.digit;
				int smallDigit = smallInt.digit;
				int digitSum;
				if(carryTheOne)
				{
					bigDigit = bigDigit - 1;
				}
				if (bigDigit >= smallDigit)
				{
					digitSum = bigDigit - smallDigit;
					carryTheOne = false;
				}
				else
				{
					digitSum = (bigDigit + 10) - smallDigit;
					carryTheOne = true;
				}
				
				sum = new DigitNode(digitSum, sum);
				
				bigInt = bigInt.next;
				smallInt = smallInt.next;
			}
			else //if there are only digits in the bigger integer 
			{
				int digitSum = bigInt.digit;
				if(carryTheOne)
				{
					digitSum = digitSum - 1;
				}
				if(digitSum < 0)
				{
					digitSum = digitSum + 10;
					carryTheOne = true;
				}
				else
				{
					carryTheOne = false;
				}
				sum = new DigitNode(digitSum, sum);
				bigInt = bigInt.next;
			}
		}
		BigInteger finalSum = new BigInteger();
		finalSum.front = RemoveLeadingZeros(sum);
		finalSum.front = Reverse(finalSum.front);
		finalSum.numDigits = getDigits(finalSum.front);
		if(biggerInt.negative)
		{
			finalSum.negative = true;
		}
		return finalSum;
	}
	
	/**
	 * Returns the BigInteger obtained by multiplying the given BigInteger
	 * with this BigInteger - DOES NOT MODIFY this BigInteger
	 * 
	 * @param other BigInteger to be multiplied
	 * @return A new BigInteger which is the product of this BigInteger and other.
	 */
	public BigInteger multiply(BigInteger other) {
		DigitNode first = this.front;
		DigitNode second = other.front;
		int firstDigit = 1;
		int secondDigit = 1;
		BigInteger product = new BigInteger();
		product.front = new DigitNode(0, null);
		BigInteger number = new BigInteger();
		while(firstDigit <= this.numDigits)
		{
			while(secondDigit <= other.numDigits)
			{
				int digitProduct = first.digit * second.digit;
				String integer = "" + digitProduct;
				int zerosToAdd = firstDigit + secondDigit - 2;
				for(int i = 0; i < zerosToAdd; i = i + 1)
				{
					integer = integer + "0";
				}
				number = parse(integer);
				product = product.add(number);
				second = second.next;
				secondDigit = secondDigit + 1;
			}
			second = other.front;
			secondDigit = 1;
			first = first.next;
			firstDigit = firstDigit + 1;
		}
		if((!this.negative && other.negative) || (this.negative && !other.negative))
		{
			product.negative = true;
		}
		product.numDigits = getDigits(product.front);
		return product;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		if (front == null) {
			return "0";
		}
		
		String retval = front.digit + "";
		for (DigitNode curr = front.next; curr != null; curr = curr.next) {
				retval = curr.digit + retval;
		}
		
		if (negative) {
			retval = '-' + retval;
		}
		retval = retval + " digits: " + numDigits;
		return retval;
	}
	
	private DigitNode Reverse(DigitNode original)
	{
		DigitNode reversed = null;
		while(original != null)
		{
			reversed = new DigitNode(original.digit, reversed);
			original = original.next;
		}
		return reversed;
	}
	
	private boolean FirstIsBigger(BigInteger first, BigInteger second)
	{
		DigitNode originalFirst = first.front;
		DigitNode originalSecond = second.front;
		if(first.numDigits > second.numDigits)
		{
			return true;
		}
		else if(first.numDigits < second.numDigits)
		{
			return false;
		}
		else
		{
			first.front = Reverse(first.front);
			second.front = Reverse(second.front);
			while(first.front != null)
			{
				if(first.front.digit > second.front.digit)
				{
					first.front = originalFirst;
					second.front = originalSecond;
					return true;
				}
				else if(first.front.digit < second.front.digit)
				{
					first.front = originalFirst;
					second.front = originalSecond;
					return false;
				}
				else
				{
					first.front = first.front.next;
					second.front = second.front.next;
				}
			}
		}
		first.front = originalFirst;
		second.front = originalSecond;
		return false;
	}
	
	private DigitNode RemoveLeadingZeros(DigitNode original)
	{
		while(original != null && original.digit == 0 )
		{
			original = original.next;
		}
		if(original == null)
		{
			original = new DigitNode(0,original);
		}
		return original;
	}
	
	private int getDigits(DigitNode number)
	{
		DigitNode original = number;
		int digits = 0;
		while(number != null)
		{
			digits = digits + 1;
			number = number.next;
		}
		number = original;
		return digits;
	}
	
}
