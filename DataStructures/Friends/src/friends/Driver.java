package friends;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

public class Driver
{
	public static void main(String[] args) throws IOException 
	{
		InputStreamReader reader = new InputStreamReader(System.in, StandardCharsets.UTF_8);
		BufferedReader in = new BufferedReader(reader);
		String line;
		while ((line = in.readLine()) != null) 
		{
			double loanAmount = Double.parseDouble(line.substring(0, line.indexOf('~')));
			System.out.println(loanAmount);
			line = line.substring(line.indexOf('~') + 1);
			double numYears = Double.parseDouble(line.substring(0, line.indexOf('~')));
			System.out.println(numYears);
			line = line.substring(line.indexOf('~') + 1);
			double interestRate = Double.parseDouble(line.substring(0, line.indexOf('~')));
			System.out.println(interestRate);
			line = line.substring(line.indexOf('~') + 1);
			double downPayment = Double.parseDouble(line);
			System.out.println(downPayment);
		}
	}
}