package apps;

import structures.*;
import java.util.ArrayList;

import apps.PartialTree.Arc;

public class MST {
	
	/**
	 * Initializes the algorithm by building single-vertex partial trees
	 * 
	 * @param graph Graph for which the MST is to be found
	 * @return The initial partial tree list
	 */
	public static PartialTreeList initialize(Graph graph) {
		PartialTreeList L = new PartialTreeList();
		for(Vertex v : graph.vertices){
			PartialTree T = new PartialTree(v);
			MinHeap<PartialTree.Arc> P = new MinHeap<PartialTree.Arc>();
			Vertex.Neighbor tmp = v.neighbors;
			while(tmp != null){
				P.insert(new PartialTree.Arc(v, tmp.vertex, tmp.weight));
				tmp = tmp.next;
			}
			T.getArcs().merge(P);
			L.append(T);
		}
		return L;
	}

	/**
	 * Executes the algorithm on a graph, starting with the initial partial tree list
	 * 
	 * @param ptlist Initial partial tree list
	 * @return Array list of all arcs that are in the MST - sequence of arcs is irrelevant
	 */
	public static ArrayList<PartialTree.Arc> execute(PartialTreeList ptlist) {
		ArrayList<PartialTree.Arc> result = new ArrayList<PartialTree.Arc>();
		while(ptlist.size() != 1){	
			PartialTree added = ptlist.remove();
			PartialTree.Arc a = null;
			for(int i=0;i<added.getArcs().size();i++){
				boolean cont = true;
				a = added.getArcs().getMin();
				added.getArcs().deleteMin();
				MinHeap<PartialTree.Arc> deleted = new MinHeap<PartialTree.Arc>();
				while(!added.getArcs().isEmpty()){
					Arc b = added.getArcs().getMin();
					added.getArcs().deleteMin();
					deleted.insert(b);
					if(a.v2.name.equals(b.v1.name)){
						cont = false;
						break;
					}
				}
				added.getArcs().merge(deleted);
				if(cont == true)
					break;
			}

			if(a == null){
				System.out.println("There are no arcs to this element. It cannot be part of the MST.");
				continue;
			}
			result.add(a);
			PartialTree merged = ptlist.removeTreeContaining(a.v2);	
			added.merge(merged);
			ptlist.append(added);
		}
		return result;
	}
}
