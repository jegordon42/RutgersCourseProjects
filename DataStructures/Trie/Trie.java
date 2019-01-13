package trie;

import java.util.ArrayList;

/**
 * This class implements a Trie. 
 * 
 * @author Sesh Venugopal
 *
 */
public class Trie {
	
	// prevent instantiation
	private Trie() { }
	
	/**
	 * Builds a trie by inserting all words in the input array, one at a time,
	 * in sequence FROM FIRST TO LAST. (The sequence is IMPORTANT!)
	 * The words in the input array are all lower case.
	 * 
	 * @param allWords Input array of words (lowercase) to be inserted.
	 * @return Root of trie with all words inserted from the input array
	 */
	public static TrieNode buildTrie(String[] allWords) {
		
		TrieNode root = new TrieNode(null, null, null);
		
		for(int i = 0; i < allWords.length; i++)
		{
			if(i == 0)
			{
				Indexes index = new Indexes(i,(short)0,(short) (allWords[i].length() - 1));
				TrieNode child = new TrieNode(index, null, null);
				root.firstChild = child;
			}
			else
			{
				TrieNode curNode = root.firstChild;
				String commonPrefix;
				boolean keepGoing = true;
				while(keepGoing)
				{
					String curWord = allWords[curNode.substr.wordIndex].substring(curNode.substr.startIndex);
					String wordToAdd = allWords[i].substring(curNode.substr.startIndex);
					commonPrefix = FindCommonPrefix(curWord,wordToAdd);
					if(commonPrefix == null)
					{
						if(curNode.sibling == null)
						{
							Indexes siblingIndex = new Indexes(i, (short) curNode.substr.startIndex, (short) (allWords[i].length() - 1));
							TrieNode sibling = new TrieNode(siblingIndex, null, null);
							curNode.sibling = sibling;
							keepGoing = false;
						}
						else
						{
							curNode = curNode.sibling;
						}
					}
					else
					{
						String curPrefix = allWords[curNode.substr.wordIndex].substring(curNode.substr.startIndex, curNode.substr.endIndex + 1);
						if(!curPrefix.equals(commonPrefix) && curNode.firstChild != null)
						{
							if(curPrefix.length() > commonPrefix.length())
							{
								curNode.substr.endIndex = (short) (commonPrefix.length() - 1);
								TrieNode child = curNode.firstChild;
								if(child.sibling == null)
								{
									child.substr.startIndex = (short) commonPrefix.length();
								}
								else
								{
									Indexes parentIndex = new Indexes(child.substr.wordIndex, (short) commonPrefix.length(), (short) (curPrefix.length() - 1));
									TrieNode parent = new TrieNode(parentIndex, child, null);
									curNode.firstChild = parent;
									child.substr.startIndex = (short) curPrefix.length();
									child.sibling.substr.startIndex = (short) curPrefix.length();
								}
							}
							else
							{
								TrieNode child = curNode.firstChild;
								Indexes parentIndex = new Indexes(child.substr.wordIndex, (short) curPrefix.length(), (short) (commonPrefix.length() - 1));
								TrieNode parent = new TrieNode(parentIndex, child, child.sibling);
								child.sibling = null;
								curNode.firstChild = parent;
								child.substr.startIndex = (short) commonPrefix.length();
							}
							
							
						}
						
						commonPrefix = FindCommonPrefix(allWords[curNode.substr.wordIndex].substring(0, curNode.substr.endIndex + 1),allWords[i]);
						if(curNode.firstChild == null)
						{
							Indexes childSiblingIndex = new Indexes(i, (short) commonPrefix.length(), (short) (allWords[i].length() - 1));
							TrieNode childSibling = new TrieNode(childSiblingIndex, null, null);
							Indexes childIndex = new Indexes(curNode.substr.wordIndex, (short) commonPrefix.length(), (short) (curNode.substr.endIndex));
							TrieNode child = new TrieNode(childIndex, null, childSibling);
							curNode.substr.endIndex = (short) (commonPrefix.length() - 1);
							curNode.firstChild = child;
							keepGoing = false;
						}
						else
						{
							curNode = curNode.firstChild;
						}
					}
				}
			}
		}
		return root;
	}
	
	private static String FindCommonPrefix(String firstWord, String secondWord)
	{
		int index;
		if(firstWord.length() > secondWord.length())
			index = secondWord.length();
		else
			index = firstWord.length();
		
		for(int i = index; i > 0; i--)
		{
			if(firstWord.substring(0, i).equals(secondWord.substring(0, i)))
			{
				return firstWord.substring(0,i);
			}
				
		}
		return null;
	}
	
	/**
	 * Given a trie, returns the "completion list" for a prefix, i.e. all the leaf nodes in the 
	 * trie whose words start with this prefix. 
	 * For instance, if the trie had the words "bear", "bull", "stock", and "bell",
	 * the completion list for prefix "b" would be the leaf nodes that hold "bear", "bull", and "bell"; 
	 * for prefix "be", the completion would be the leaf nodes that hold "bear" and "bell", 
	 * and for prefix "bell", completion would be the leaf node that holds "bell". 
	 * (The last example shows that an input prefix can be an entire word.) 
	 * The order of returned leaf nodes DOES NOT MATTER. So, for prefix "be",
	 * the returned list of leaf nodes can be either hold [bear,bell] or [bell,bear].
	 *
	 * @param root Root of Trie that stores all words to search on for completion lists
	 * @param allWords Array of words that have been inserted into the trie
	 * @param prefix Prefix to be completed with words in trie
	 * @return List of all leaf nodes in trie that hold words that start with the prefix, 
	 * 			order of leaf nodes does not matter.
	 *         If there is no word in the tree that has this prefix, null is returned.
	 */
	public static ArrayList<TrieNode> completionList(TrieNode root, String[] allWords, String prefix) {
		ArrayList<TrieNode> list = new ArrayList<TrieNode>();
		if(root.substr == null)
		{
			try
			{
				list.addAll(completionList(root.firstChild, allWords, prefix));
			}
			catch(Exception e)
			{
				return null;
			}
			return list;
		}
		String commonPrefix = FindCommonPrefix(allWords[root.substr.wordIndex], prefix);
		if(commonPrefix == null)
		{
			if(root.sibling == null)
				return null;
			else
			{
				try
				{
					list.addAll(completionList(root.sibling, allWords, prefix));
				}
				catch(Exception e)
				{
					return null;
				}
			}
			
			return list;
		}
		else
		{
			if(root.sibling == null && root.firstChild == null && commonPrefix.equals(prefix))
				list.add(root);
			else 
			{
				if(root.sibling != null)
				{
					try
					{
						list.addAll(completionList(root.sibling, allWords, prefix));
					}
					catch(Exception e)
					{
						//continue
					}
				}
				if(root.firstChild != null)
				{
					try
					{
						list.addAll(completionList(root.firstChild, allWords, prefix));
					}
					catch(Exception e)
					{
						return null;
					}
				}
				else if(commonPrefix.equals(prefix))
					list.add(root);
			}
			
			return list;
		}
	}
	
	public static void print(TrieNode root, String[] allWords) {
		System.out.println("\nTRIE\n");
		print(root, 1, allWords);
	}
	
	private static void print(TrieNode root, int indent, String[] words) {
		if (root == null) {
			return;
		}
		for (int i=0; i < indent-1; i++) {
			System.out.print("    ");
		}
		
		if (root.substr != null) {
			String pre = words[root.substr.wordIndex]
							.substring(0, root.substr.endIndex+1);
			System.out.println("      " + pre);
		}
		
		for (int i=0; i < indent-1; i++) {
			System.out.print("    ");
		}
		System.out.print(" ---");
		if (root.substr == null) {
			System.out.println("root");
		} else {
			System.out.println(root.substr);
		}
		
		for (TrieNode ptr=root.firstChild; ptr != null; ptr=ptr.sibling) {
			for (int i=0; i < indent-1; i++) {
				System.out.print("    ");
			}
			System.out.println("     |");
			print(ptr, indent+1, words);
		}
	}
 }
