package apps;
import java.io.IOException;
import java.util.*;

import structures.Graph;

public class Driver
{
	public static void main(String[] args) throws IOException{
		Graph g = new Graph("graph1.txt");
		g.print();
		PartialTreeList ptList = MST.initialize(g);
		ArrayList<PartialTree.Arc> results = MST.execute(ptList);
		for(PartialTree.Arc a:results)
			System.out.println(a);
	}
}