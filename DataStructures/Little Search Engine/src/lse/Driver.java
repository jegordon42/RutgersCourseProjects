package lse;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class Driver
{
	static Scanner stdin = new Scanner(System.in);

	public static void main(String[] args)	throws IOException {
		LittleSearchEngine lse = new LittleSearchEngine();
		lse.makeIndex("docs.txt","noisewords.txt");
		System.out.println(lse.getKeyword("paraphrase;"));
		ArrayList<String> retrnd = lse.top5search("stupidd", "stupid");
		System.out.println("Docs:");
		try{
			for(String doc: retrnd)
				System.out.println(doc);
		}catch(NullPointerException ex){
			System.out.println("no docs containing these keywords.");
		}
		//System.out.println(lse.keywordsIndex);
		System.out.println("done");


	}
}