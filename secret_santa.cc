/*
Input to the algorithm: 1) Set of pairings that should not be allowed. 
						2) Number of people
Output:	A Hamiltonian cycle if it exists or display no solution

Approach: Initilaize all the edges in the adjacency matrix to be true and set those entries false which appear in the given set of disallowed pairings
			Initialize with a node and try to complete a cycle within the constraints : 
			1) The pairings must satify the given set, and
			2) No element is visited more than once (except the first element which is visited again at the last to complete the cycle)
			
			For 1) We check if connection to previous to current node in the path is permissible
			For 2) We keep track of the visited nodes
*/


#include<iostream>
#include<vector>
#include<set>

using namespace std;

class Graph{
private:
	int v;
	std::vector<std::vector<bool> > *adj;
public:
	Graph(int x) : v(x)
	{
		adj= new std::vector<std::vector<bool> > (v, std::vector<bool> (v, true));
	}

	friend void createPairing(set<pair<int, int> >, int);
	friend bool createPairingUtil(std::vector<int>&, int, Graph*, std::vector<bool> &);
};

bool createPairingUtil(std::vector<int> &path, int pos, Graph *g, std::vector<bool> &visited)
{
	if(pos == g->v)
	{
		if((*(g->adj))[path[pos-1]][0])
		{
			path.push_back(0);
			return true;
		}

		return false;
	}

	for(int i = 1; i < g->v; i++)
	{
		if((*(g->adj))[path[pos-1]][i] && !visited[i])
		{
			//*(g->adj)[path[pos-1]][i] = false;
			visited[i] = true;
			path.push_back(i);
			if (createPairingUtil(path, pos+1, g, visited))
				return true;
			
			else{
				visited[i] = false;
				path.pop_back();
			}
			
		}
	}

	return false;

}

void createPairing(set<pair<int, int> > no_pair, int n)
{
	Graph *g = new Graph(5);
	std::vector<bool> visited(n, false);
	vector<int> path;

	//Set the adj entries to false for each entry in no_pair
	for(auto it = no_pair.begin(); it!= no_pair.end(); it++)
		(*(g->adj))[it->first][it->second] = false;
	
	path.push_back(0);

	if (createPairingUtil(path, 1, g, visited))
	{
		cout << "Secret Santa Cycle Exists :" << endl;
		for(int i = 0; i < path.size(); i++)
			cout << path[i];
	}

	else
		cout << "No path exists" << endl;
	
}

int main()
{
	//This set contains the pairings that are not allowed.
	set<pair<int, int> > s;
	s.insert(make_pair(0,4));
	s.insert(make_pair(0,2));
	s.insert(make_pair(2,0));
	s.insert(make_pair(2,3));
	s.insert(make_pair(3,2));
	s.insert(make_pair(4,0));

	/* Following constraints makes it impossible to find a path. Uncomment and see.
	s.insert(make_pair(3,4));
	s.insert(make_pair(4,3));
	*/

	createPairing(s, 5);

	return 0;
}
