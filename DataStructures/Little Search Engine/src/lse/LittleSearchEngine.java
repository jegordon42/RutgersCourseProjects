package lse;

import java.io.*;
import java.util.*;

/**
 * This class builds an index of keywords. Each keyword maps to a set of pages in
 * which it occurs, with frequency of occurrence in each page.
 *
 */
public class LittleSearchEngine {
	
	/**
	 * This is a hash table of all keywords. The key is the actual keyword, and the associated value is
	 * an array list of all occurrences of the keyword in documents. The array list is maintained in 
	 * DESCENDING order of frequencies.
	 */
	HashMap<String,ArrayList<Occurrence>> keywordsIndex;
	
	/**
	 * The hash set of all noise words.
	 */
	HashSet<String> noiseWords;
	
	/**
	 * Creates the keyWordsIndex and noiseWords hash tables.
	 */
	public LittleSearchEngine() {
		keywordsIndex = new HashMap<String,ArrayList<Occurrence>>(1000,2.0f);
		noiseWords = new HashSet<String>(100,2.0f);
	}
	
	/**
	 * Scans a document, and loads all keywords found into a hash table of keyword occurrences
	 * in the document. Uses the getKeyWord method to separate keywords from other words.
	 * 
	 * @param docFile Name of the document file to be scanned and loaded
	 * @return Hash table of keywords in the given document, each associated with an Occurrence object
	 * @throws FileNotFoundException If the document file is not found on disk
	 */
	public HashMap<String,Occurrence> loadKeywordsFromDocument(String docFile) 
	throws FileNotFoundException {
		HashMap<String,Occurrence> kws = new HashMap<String,Occurrence>(1000,2.0f);;
		Scanner sc = new Scanner(new File(docFile));
		while (sc.hasNext()) {
			String word = sc.next();
			if(getKeyword(word) != null)
			{
				if(kws.containsKey(getKeyword(word)))
					kws.get(getKeyword(word)).frequency++;
				else
					kws.put(getKeyword(word), new Occurrence(docFile, 1));
			}
		}
		sc.close();
		return kws;
	}
	
	/**
	 * Merges the keywords for a single document into the master keywordsIndex
	 * hash table. For each keyword, its Occurrence in the current document
	 * must be inserted in the correct place (according to descending order of
	 * frequency) in the same keyword's Occurrence list in the master hash table. 
	 * This is done by calling the insertLastOccurrence method.
	 * 
	 * @param kws Keywords hash table for a document
	 */

	public void mergeKeywords(HashMap<String,Occurrence> kws) {
		Object[] keys = kws.keySet().toArray();
		for(int i = 0; i < keys.length - 1; i++)
		{
			String key = keys[i].toString();
			if(keywordsIndex.containsKey(keys[i]))
			{
				ArrayList<Occurrence> arr = keywordsIndex.get(key);
				arr.add(kws.get(key));
				insertLastOccurrence(arr);
				keywordsIndex.put(key, arr);
			}
			else{
				ArrayList<Occurrence> arr = new ArrayList<Occurrence>();
				arr.add(kws.get(key));
				keywordsIndex.put(key, arr);
			}
		}
	}
	
	/**
	 * Given a word, returns it as a keyword if it passes the keyword test,
	 * otherwise returns null. A keyword is any word that, after being stripped of any
	 * trailing punctuation, consists only of alphabetic letters, and is not
	 * a noise word. All words are treated in a case-INsensitive manner.
	 * 
	 * Punctuation characters are the following: '.', ',', '?', ':', ';' and '!'
	 * 
	 * @param word Candidate word
	 * @return Keyword (word without trailing punctuation, LOWER CASE)
	 */

	public String getKeyword(String word) {
		boolean hasPunctuation = false;
		int IndexOfPunctuation = -1;
		for(int i = 0; i < word.length() - 1; i++)
		{
			char curChar = word.charAt(i);
			if(Character.isLetter(curChar))
			{
				if(hasPunctuation)
					return null;
			}
			else if(curChar == '.' || curChar == ',' || curChar == '?' || curChar == ':' || curChar == ';' || curChar == '!' )
			{
				if(IndexOfPunctuation == -1)
					IndexOfPunctuation = i;
				hasPunctuation = true;
			}
			else
				return null;
		}
		System.out.println(IndexOfPunctuation);
		if(IndexOfPunctuation != -1)
			word = word.substring(0, IndexOfPunctuation-3);
		
		word = word.toLowerCase();
		
		if(noiseWords.contains(word))
			return null;
		
		return word;
	}
	
	/**
	 * Inserts the last occurrence in the parameter list in the correct position in the
	 * list, based on ordering occurrences on descending frequencies. The elements
	 * 0..n-2 in the list are already in the correct order. Insertion is done by
	 * first finding the correct spot using binary search, then inserting at that spot.
	 * 
	 * @param occs List of Occurrences
	 * @return Sequence of mid point indexes in the input list checked by the binary search process,
	 *         null if the size of the input list is 1. This returned array list is only used to test
	 *         your code - it is not used elsewhere in the program.
	 */

	public ArrayList<Integer> insertLastOccurrence(ArrayList<Occurrence> occs) {
		if(occs.size() == 1)
			 return null;
		
		Occurrence currOcc = occs.get(occs.size() - 1);
		occs.remove(currOcc);
		
		int l = 0;
		int r = occs.size() - 1;
		 
		 ArrayList<Integer> indexes = new ArrayList<Integer>();
		 int m = 0;
		 
	     while (l <= r)
	     {
	     	m = l + (r-l)/2;
	     	indexes.add(m);
	     	if (occs.get(m).frequency == currOcc.frequency)
	         	break;
	        if (occs.get(m).frequency > currOcc.frequency)
	        		l = m + 1;
			else
	        		r = m - 1;
	     }  
	     
	     if(occs.get(m).frequency > currOcc.frequency)
	    	 	m++;
	     
	     occs.add(occs.get(occs.size()-1));
	     for(int i = occs.size() - 2; i > m; i--)
	     {
	    	 	occs.set(i, occs.get(i-1));
	     }
	     occs.set(m, currOcc);
	     
		return indexes;
	}
	
	/**
	 * This method indexes all keywords found in all the input documents. When this
	 * method is done, the keywordsIndex hash table will be filled with all keywords,
	 * each of which is associated with an array list of Occurrence objects, arranged
	 * in decreasing frequencies of occurrence.
	 * 
	 * @param docsFile Name of file that has a list of all the document file names, one name per line
	 * @param noiseWordsFile Name of file that has a list of noise words, one noise word per line
	 * @throws FileNotFoundException If there is a problem locating any of the input files on disk
	 */
	public void makeIndex(String docsFile, String noiseWordsFile) 
	throws FileNotFoundException {
		// load noise words to hash table
		Scanner sc = new Scanner(new File(noiseWordsFile));
		while (sc.hasNext()) {
			String word = sc.next();
			noiseWords.add(word);
		}
		
		// index all keywords
		sc = new Scanner(new File(docsFile));
		while (sc.hasNext()) {
			String docFile = sc.next();
			HashMap<String,Occurrence> kws = loadKeywordsFromDocument(docFile);
			mergeKeywords(kws);
		}
		sc.close();
	}
	
	/**
	 * Search result for "kw1 or kw2". A document is in the result set if kw1 or kw2 occurs in that
	 * document. Result set is arranged in descending order of document frequencies. (Note that a
	 * matching document will only appear once in the result.) Ties in frequency values are broken
	 * in favor of the first keyword. (That is, if kw1 is in doc1 with frequency f1, and kw2 is in doc2
	 * also with the same frequency f1, then doc1 will take precedence over doc2 in the result. 
	 * The result set is limited to 5 entries. If there are no matches at all, result is null.
	 * 
	 * @param kw1 First keyword
	 * @param kw1 Second keyword
	 * @return List of documents in which either kw1 or kw2 occurs, arranged in descending order of
	 *         frequencies. The result size is limited to 5 documents. If there are no matches, returns null.
	 */

	public ArrayList<String> top5search(String kw1, String kw2) {
		ArrayList<String> top5 = new ArrayList<String>();
		ArrayList<Occurrence> kw1Occurences = keywordsIndex.get(kw1);
		ArrayList<Occurrence> kw2Occurences = keywordsIndex.get(kw2);
		if(kw1Occurences == null && kw2Occurences == null)
			return null;
		else if(kw1Occurences == null)
		{
			for(int i = 0; i < 5 && i < kw2Occurences.size(); i++)
				top5.add(kw2Occurences.get(i).document);
			return top5;
		}
		else if(kw2Occurences == null)
		{
			for(int i = 0; i < 5 && i < kw1Occurences.size(); i++)
				top5.add(kw1Occurences.get(i).document);
			return top5;
		}
		int count = 0;
		int index1 = 0;
		int index2 = 0;
		Occurrence Occ1 = null;
		Occurrence Occ2 = null;
		for(; index1 < kw1Occurences.size() && index2 < kw2Occurences.size() && count < 5; index1++)
		{
			Occ1 = kw1Occurences.get(index1);
			for( ;index2 < kw2Occurences.size() && count < 5; index2++)
			{
				Occ2 = kw2Occurences.get(index2);
				if(Occ1.frequency >= Occ2.frequency)
				{
					if(top5.contains(Occ1.document))
						break;
					top5.add(Occ1.document);
					count++;
					break;
				}
				else
				{
					if(top5.contains(Occ2.document))
						continue;
					top5.add(Occ2.document);
					count++;
					if(index2 == kw2Occurences.size() - 1)
						index1--;
				}	
			}
		}
		if (count < 5)
		{
			if(index1 == kw1Occurences.size())
			{
				while(count != 5 && index2 < kw2Occurences.size())
				{
					if(!top5.contains(kw2Occurences.get(index2).document))
						top5.add(kw2Occurences.get(index2).document);
					index2++;
					count++;
				}
			}
			else
			{
				while(count != 5 && index1 < kw1Occurences.size())
				{
					if(!top5.contains(kw1Occurences.get(index1).document))
						top5.add(kw1Occurences.get(index1).document);
					index1++;
					count++;
				}
			}
				
		}
		return top5;
	}
}
