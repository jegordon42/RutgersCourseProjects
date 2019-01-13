package apps;

import java.io.*;
import java.util.*;

public class Expression {

	String expr;                
    
	ArrayList<ScalarSymbol> scalars;   
	
	ArrayList<ArraySymbol> arrays;
    
    public static final String delims = " \t*+-/()[]";
    
    public Expression(String expr) {
        this.expr = expr;
    }

    public void buildSymbols(){
		arrays = new ArrayList<ArraySymbol>();
    	scalars = new ArrayList<ScalarSymbol>();
		for(int i = 0; i < expr.length(); i = i + 1)
		{
			if(Character.isLetter(expr.charAt(i)))
			{
				String Symbol="";
				while(i < expr.length() && Character.isLetter(expr.charAt(i)))
				{
					Symbol = Symbol + expr.charAt(i);
					i = i + 1;
				}
				if(i < expr.length() && expr.charAt(i) == '[')
				{
					arrays.add(new ArraySymbol(Symbol));
				}
				else
				{
					scalars.add(new ScalarSymbol(Symbol));
				}
			}
		}
    }
    
    public void loadSymbolValues(Scanner sc) 
    throws IOException {
        while (sc.hasNextLine()) {
            StringTokenizer st = new StringTokenizer(sc.nextLine().trim());
            int numTokens = st.countTokens();
            String sym = st.nextToken();
            ScalarSymbol ssymbol = new ScalarSymbol(sym);
            ArraySymbol asymbol = new ArraySymbol(sym);
            int ssi = scalars.indexOf(ssymbol);
            int asi = arrays.indexOf(asymbol);
            if (ssi == -1 && asi == -1) {
            	continue;
            }
            int num = Integer.parseInt(st.nextToken());
            if (numTokens == 2) { // scalar symbol
                scalars.get(ssi).value = num;
            } else { // array symbol
            	asymbol = arrays.get(asi);
            	asymbol.values = new int[num];
                // following are (index,val) pairs
                while (st.hasMoreTokens()) {
                    String tok = st.nextToken();
                    StringTokenizer stt = new StringTokenizer(tok," (,)");
                    int index = Integer.parseInt(stt.nextToken());
                    int val = Integer.parseInt(stt.nextToken());
                    asymbol.values[index] = val;              
                }
            }
        }
    }
    
    public float evaluate(){
    		String theExpr = expr;
    		for(int i = 0; i < scalars.size(); i = i + 1){
    			theExpr = theExpr.replace(scalars.get(i).name, "" + scalars.get(i).value);
    		}
    		String answer = evaluate(theExpr);
    		try
    		{
    			return Float.parseFloat(answer);
    		}
    		catch (Exception e)
    		{
    			return 0;
    		}
    }

    private String evaluate(String theExpr){
	    	if (theExpr == null || theExpr.length() == 0)
	    	{
	    		return "0";
	    	}
	    	
	    	if(theExpr.indexOf('[') > -1 )
	    	{
	    		return EvaluateArrays(theExpr);
	    	}
	    	else	 if(theExpr.indexOf('(') > -1)
	    	{
	    		return EvaluateParentheses(theExpr);
	    	}
	    	else
	    	{
	    		theExpr = theExpr.replace(" ", "");
	    		theExpr = " " + theExpr;
	    			
	    		int operations = 0;
	    		for(int i = 2; i < theExpr.length(); i = i + 1)
	    		{
	    			if(theExpr.charAt(i) == '+' || theExpr.charAt(i) == '-' || theExpr.charAt(i) == '/' || theExpr.charAt(i) == '*')
	    				operations = operations + 1;
	    		}
	    		if(operations==0)
	    			return theExpr;
	    			
		    	theExpr = EvaluateMultiplicationAndDivision(theExpr);
		    	theExpr = EvaluateAdditionAndSubtraction(theExpr);
		}
	    	return theExpr;
    }
    
    private String EvaluateArrays(String theExpr){
	    	for(int i = 0; i < theExpr.length(); i = i + 1)
			{
				if(Character.isLetter(theExpr.charAt(i))) 
				{
					String newSym = "";
					boolean isArray = false;
					int beginningIndex = i;
					while(i < theExpr.length() && Character.isLetter(theExpr.charAt(i)))
					{
						newSym = newSym + theExpr.charAt(i);
						i = i + 1;
					}
					if (i < theExpr.length() && theExpr.charAt(i) == '[')
						isArray = true;
					if (isArray) 
					{
						int num = 1;
						i = i + 1;
						int endIndex = i;9	
						for (; endIndex<theExpr.length(); endIndex = endIndex + 1) 
						{
							if (theExpr.charAt(endIndex) == '[')
								num = num + 1;
							if (theExpr.charAt(endIndex) == ']')
								num = num - 1;
			
							if (num == 0 && theExpr.charAt(endIndex) == ']')
								break;
						}
						String beforeArray = theExpr.substring(0, beginningIndex);
						String afterArray = theExpr.substring(endIndex + 1);
		
						String ArrayIndex = evaluate(theExpr.substring(i, endIndex));
						int count = 0;
						for(int j = 0; j < arrays.size(); j = j + 1)
						{
							if(arrays.get(j).name.equals(newSym))
							{
								count = j;
								break;
							}
						}
						String evalArray = "" + arrays.get(count).values[(int)Float.parseFloat(ArrayIndex)];
						return evaluate(beforeArray + evalArray + afterArray);
					}
				}
			}
	    		return "";
	}
    
    private String EvaluateParentheses(String theExpr){
	    	for(int i = 0; i < theExpr.length(); i = i + 1)
			{
			if(theExpr.charAt(i) == '(')
			{
				int beginningIndex = i;
				int num = 1;
				i = i + 1;
				int endIndex = i;
				for (; endIndex < theExpr.length(); endIndex = endIndex + 1)
				{
					if (theExpr.charAt(endIndex)=='(')
						num = num + 1;
					if (theExpr.charAt(endIndex)==')')
						num = num - 1;
					if (num == 0 && theExpr.charAt(endIndex) == ')')
						break;
				}
				String paren = evaluate(theExpr.substring(i, endIndex));
				return evaluate(theExpr.substring(0, beginningIndex) + paren + theExpr.substring(endIndex + 1));
			}
		}
	    	return "";
    }
    
    private String EvaluateMultiplicationAndDivision(String theExpr){
		float firstNum;
		float secondNum;
		int indexFirst;
		int indexSecond;
		boolean negative = false;
		
	    	for(int i = 0; i < theExpr.length(); i = i + 1)
			{
				if(i == 1 && theExpr.charAt(i) == '-')
					negative = true;
				else if(theExpr.charAt(i) == '*' || theExpr.charAt(i) == '/')
				{
					indexFirst = i;
					indexSecond = i;
					
					while(indexFirst > 0 && (Character.isDigit(theExpr.charAt(indexFirst - 1)) || theExpr.charAt(indexFirst - 1) == '.'))
					{
						indexFirst = indexFirst - 1;
					}
					while(indexSecond < theExpr.length() - 1 && (Character.isDigit(theExpr.charAt(indexSecond + 1)) || theExpr.charAt(indexSecond + 1) == '.'))
					{
						indexSecond = indexSecond + 1;
					}
					
				try 
				{
					firstNum = Float.parseFloat(theExpr.substring(indexFirst, i));
				} 
				catch (Exception e)
				{
					firstNum = 0;
				}
				
				boolean secondZero = true;
				try 
				{
					secondNum = Float.parseFloat(theExpr.substring(i + 1, indexSecond + 1));
				}
				catch (Exception e) 
				{
					secondNum = 0;
					secondZero = false;
				}
				
				if(negative)
				{
					firstNum = firstNum * -1;
				}
				String returned= "";
				if(theExpr.charAt(i) == '*')
				{
					returned= "" + (firstNum * secondNum);
				}
				else
				{
					if(secondNum == 0)
					{
						throw new IllegalArgumentException("Argument 'divisor' is 0");
					}
					else if(secondNum == 0 && secondZero)
						secondNum = 1;
					
					returned= "" + (firstNum / secondNum);
				}
				int indexStart = 0;
				if(negative)
					indexStart = 2;
				theExpr = theExpr.substring(indexStart,indexFirst) + returned + theExpr.substring(indexSecond + 1);
				i = 0;
				negative = false;
				}	
			}
    		return theExpr;
    }
    
    private String EvaluateAdditionAndSubtraction(String theExpr){
    		float firstNum;
		float secondNum;
		int indexFirst;
		int indexSecond;
		boolean negative = false;
		
		for(int i = 0; i < theExpr.length(); i = i + 1)
		{
			if(i == 1 && theExpr.charAt(i) == '-')
				negative = true;
			else if(theExpr.charAt(i) == '+' || theExpr.charAt(i) == '-')
			{
				indexFirst = i;
				indexSecond = i;

				while(indexFirst > 0 && (Character.isDigit(theExpr.charAt(indexFirst - 1)) || theExpr.charAt(indexFirst - 1) == '.') )
				{
					indexFirst = indexFirst - 1;
				}
				while(indexSecond < theExpr.length() - 1 && (Character.isDigit(theExpr.charAt(indexSecond + 1)) || theExpr.charAt(indexSecond + 1) == '.'))
				{
					indexSecond = indexSecond + 1;
				}
				
				try 
				{
					firstNum = Float.parseFloat(theExpr.substring(indexFirst, i));
				} 
				catch (Exception e) 
				{
					firstNum = 0;
				}
					
				try 
				{
					secondNum = Float.parseFloat(theExpr.substring(i + 1, indexSecond + 1));
				} 
				catch (Exception e) 
				{
					secondNum = 0;
				}
				
				if(negative)
					firstNum = firstNum * -1;
				
				String returned = "";
				if(theExpr.charAt(i) == '+')
				{
					returned= "" + (firstNum + secondNum);
				}
				else
					returned= "" + (firstNum - secondNum);
				
				int indexStart = 0;
				if(negative)
					indexStart = 2;
				theExpr = theExpr.substring(indexStart, indexFirst) + returned + theExpr.substring(indexSecond + 1);
				i = 0;
				negative = false;
			}
		}
		theExpr = theExpr.replace(" ", "");
		return theExpr;
    }
    
    public void printScalars() {
        for (ScalarSymbol ss: scalars) {
            System.out.println(ss);
        }
    }
    
    public void printArrays() {
    		for (ArraySymbol as: arrays) {
    			System.out.println(as);
    		}
    }

}
