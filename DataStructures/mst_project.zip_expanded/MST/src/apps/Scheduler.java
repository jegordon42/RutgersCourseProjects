package apps;
import java.util.*;

public class Scheduler
{
	public static ArrayList<Calendar> TheCalendar = new ArrayList<Calendar>();

	public static void main(String[] args) 
	{
		String choice = getOption();
		
		while(!choice.equals("6"))
		{
			if(choice.equals("1"))
				ViewMonthlyCalendar();
			else if(choice.equals("2"))
				ViewWeeklyCalendar();
			else if(choice.equals("3"))
				ViewDailyCalendar();
			else if(choice.equals("4"))
				AddEvent();
			else if(choice.equals("5"))
				DeleteEvent();
			
			choice = getOption();
		}
		
	}
	
	public static String getOption()
	{
		Scanner console = new Scanner(System.in);
		System.out.println("Scheduler");
		System.out.println("1. View Monthly Calendar");
		System.out.println("2. View Weekly Calendar");
		System.out.println("3. View Daily Calendar");
		System.out.println("4. Add Event");
		System.out.println("5. Delete Event");
		System.out.println("6. Quit");
		System.out.println("Select an option. (Enter 1, 2, 3, 4, 5 or 6)");
		String choice = console.nextLine();
		
		while(!choice.equals("1") && !choice.equals("2") && !choice.equals("3") && !choice.equals("4") && !choice.equals("5") && !choice.equals("6"))
		{
			
			System.out.println();
			System.out.println("Invalid Choice, Try Again.");
			System.out.println("Scheduler");
			System.out.println("1. View Monthly Calendar");
			System.out.println("2. View Weekly Calendar");
			System.out.println("3. View Daily Calendar");
			System.out.println("4. Add Event");
			System.out.println("5. Delete Event");
			System.out.println("6. Quit");
			System.out.println("Select an option. (Enter 1, 2, 3, 4, 5 or 6)");
			choice = console.nextLine();
		}
		
		return choice;
	}
	
	public static int getYear()
	{
		Scanner console = new Scanner(System.in);
		boolean isValid = true;
		int intYear = 0;
		
		do
		{
			if(!isValid)
				System.out.println("Invalid Year, Try Again.");
			System.out.println("Please select a year.");
			String stringYear = console.next();
			isValid = true;
			
			try
		    {
				intYear = Integer.parseInt(stringYear);
				if(intYear <= 0)
					isValid = false;		
		    } 
			catch (NumberFormatException ex)
		    {
		        isValid = false;
		    }
			
		}while(!isValid);
		
		return intYear;
	}
	
	public static int getMonth()
	{
		Scanner console = new Scanner(System.in);
		boolean isValid = true;
		int intMonth = 0;
		
		do
		{
			if(!isValid)
				System.out.println("Invalid Month, Try Again.");
			System.out.println("Please select a month. (Enter a number between 1 and 12)");
			String stringMonth = console.next();
			isValid = true;
			
			try
		    {
				intMonth = Integer.parseInt(stringMonth);
				if(intMonth <= 0 || intMonth > 12)
					isValid = false;		
		    } 
			catch (NumberFormatException ex)
		    {
		        isValid = false;
		    }
			
		}while(!isValid);
		
		return intMonth;
	}
	
	public static int getDay()
	{
		Scanner console = new Scanner(System.in);
		boolean isValid = true;
		int intDay = 0;
		
		do
		{
			if(!isValid)
				System.out.println("Invalid Day, Try Again.");
			System.out.println("Please select a day. (Enter a number between 1 and 31)");
			String stringDay = console.next();
			isValid = true;
			
			try
		    {
				intDay = Integer.parseInt(stringDay);
				if(intDay <= 0 || intDay > 31)
					isValid = false;		
		    } 
			catch (NumberFormatException ex)
		    {
		        isValid = false;
		    }
			
		}while(!isValid);
		
		return intDay;
	}
	
	public static int getWeek()
	{
		Scanner console = new Scanner(System.in);
		
		System.out.println("Please select a week.");
		System.out.println("1. Days 1-7");
		System.out.println("2. Days 8-14");
		System.out.println("3. Days 15-21");
		System.out.println("4. Days 22-28");
		System.out.println("5. Days 29-31");
		System.out.println("Select an option. (Enter 1, 2, 3, 4 or 5)");
		String week = console.next();
		
		while(!week.equals("1") && !week.equals("2") && !week.equals("3") && !week.equals("4") && !week.equals("5"))
		{
			
			System.out.println();
			System.out.println("Invalid Choice, Try Again.");
			System.out.println("Please select a week.");
			System.out.println("1. Days 1-7");
			System.out.println("2. Days 8-14");
			System.out.println("3. Days 15-21");
			System.out.println("4. Days 22-28");
			System.out.println("5. Days 29-31");
			System.out.println("Select an option. (Enter 1, 2, 3, 4 or 5)");
			week = console.next();
		}
		
		return Integer.parseInt(week);
	}
	
	public static void ViewMonthlyCalendar()
	{
		int year = getYear();
		int month = getMonth();
		
		ArrayList<Calendar> MonthlyCalendar = new ArrayList<Calendar>();
		
		for(int i = 0; i < TheCalendar.size(); i++)
		{
			Calendar event = TheCalendar.get(i);
			if(event.year == year && event.month == month)
				MonthlyCalendar.add(event);
		}
		
		System.out.println();
		System.out.println("There are " + MonthlyCalendar.size() + " events in " + month + "/" + year);
		
		for(int i = 0; i < MonthlyCalendar.size(); i++)
		{
			Calendar event = MonthlyCalendar.get(i);
			System.out.println("Day " + event.day + ":");
			System.out.println("-" + event.description);
		}
		
		System.out.println();
	}
	
	public static void ViewWeeklyCalendar()
	{
		int year = getYear();
		int month = getMonth();
		int week = getWeek();
		int lowWeekday = 0;
		int highWeekday = 0;
		if(week == 1)
		{
			lowWeekday = 1;
			highWeekday = 7;
		}
		if(week == 2)
		{
			lowWeekday = 8;
			highWeekday = 14;
		}
		if(week == 3)
		{
			lowWeekday = 15;
			highWeekday = 21;
		}
		if(week == 4)
		{
			lowWeekday = 22;
			highWeekday = 28;
		}
		if(week == 5)
		{
			lowWeekday = 29;
			highWeekday = 31;
		}
		
		ArrayList<Calendar> WeeklyCalendar = new ArrayList<Calendar>();
		
		for(int i = 0; i < TheCalendar.size(); i++)
		{
			Calendar event = TheCalendar.get(i);
			if(event.year == year && event.month == month && event.day >= lowWeekday && event.day <= highWeekday)
				WeeklyCalendar.add(event);
		}
		
		System.out.println();
		System.out.println("There are " + WeeklyCalendar.size() + " events in " + month + "/" + year + " days " + lowWeekday + "-" + highWeekday);
		
		for(int i = 0; i < WeeklyCalendar.size(); i++)
		{
			Calendar event = WeeklyCalendar.get(i);
			System.out.println("Day " + event.day + ":");
			System.out.println("-" + event.description);
		}
		
		System.out.println();
	}
	
	public static void ViewDailyCalendar()
	{
		int year = getYear();
		int month = getMonth();
		int day = getDay();
		
		ArrayList<Calendar> DailyCalendar = new ArrayList<Calendar>();
		
		for(int i = 0; i < TheCalendar.size(); i++)
		{
			Calendar event = TheCalendar.get(i);
			if(event.year == year && event.month == month && event.day == day)
				DailyCalendar.add(event);
		}
		
		System.out.println();
		System.out.println("There are " + DailyCalendar.size() + " events on " + day + "/" + month + "/" + year);
		
		for(int i = 0; i < DailyCalendar.size(); i++)
		{
			Calendar event = DailyCalendar.get(i);
			System.out.println("-" + event.description);
		}
		
		System.out.println();
	}
	
	public static void AddEvent()
	{
		int year = getYear();
		int month = getMonth();
		int day = getDay();
		
		System.out.println("Please enter Event Description:");
		
		Scanner console = new Scanner(System.in);
		String description = console.nextLine();
		Calendar event = new Calendar(year, month, day, description);
		TheCalendar.add(event);
		System.out.println();
	}
	
	public static void DeleteEvent()
	{
		int year = getYear();
		int month = getMonth();
		int day = getDay();
		
		ArrayList<Calendar> daysToRemove = new ArrayList<Calendar>();
		
		for(int i = 0; i < TheCalendar.size(); i++)
		{
			Calendar event  = TheCalendar.get(i);
			if(event.year == year && event.month == month && event.day == day)
				daysToRemove.add(event);
		}
		
		for(int i = 0; i < daysToRemove.size(); i++)
		{
			TheCalendar.remove(daysToRemove.get(i));
		}
		
		System.out.println();
	}
}