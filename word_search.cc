/*
	Input: 1) Vocabulary of Words
		   2) 2-d matrix representing formed of the characters in which we want to search the words

	Output: Words in the matrix by going horizontal/vertical/zig-zag that are present in the vocabulary

	Approach: I've create a trie to store all the possible words in the vocabulary and then traverse this trie as we explore the word maze.

*/

#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<queue>
#include<stack>
#include<set>

using namespace std;

struct trieNode
{
	//We use frequency instead of a boolean value becuase a word can be the beginning of another word e.g pen and penance
	int freq;
	bool isLeaf;
	map<char, trieNode*> charMap;
	trieNode() : freq(0), isLeaf(false) {}
};


trieNode* insert_word(trieNode* root, string s)
{
	if(!root)
		root = new trieNode();

	trieNode *cur = root;
	for(int i =0; i < s.length(); i++)
	{
		//cout << "Inserting " << s[i];
		auto it = cur->charMap.find(s[i]);
	
		if(it == cur->charMap.end())
			cur->charMap[s[i]] = new trieNode();
		
		cur->charMap[s[i]]->freq++;
		cur = cur->charMap[s[i]];
	}

	cur->isLeaf = true;
	return root;
}

/*
	I'm using a queue to do a bfs traversal. Each entry in the queue stores the next node and the string so far
*/
void get_shortest_unique_prefix(trieNode *root, std::vector<string> &res)
{
	//If the trie is empty
	if(!root)
		return;

	queue<pair<trieNode*, string> > my_queue;
	string dummy = "";
	my_queue.push(make_pair(root, dummy));
	auto cur = my_queue.front();

	while(!my_queue.empty())
	{
		my_queue.pop();
		
		if(cur.first->freq == 1)
		{
			res.push_back(cur.second);
		}

		else{

			for(auto it = cur.first->charMap.begin(); it!= cur.first->charMap.end(); it++)
			{
				//Important: Conversion of a single character to a string is: string(size_t, char)
				my_queue.push(make_pair(it->second, cur.second + string(1, it->first)));
			}

		}

		if(!my_queue.empty())
			cur = my_queue.front();
	}
}

trieNode* is_present(trieNode *root, string s)
{
	if(!root)
		return NULL;

	trieNode *cur = root;
	for(int i = 0; i < s.size(); i++)
	{
		auto it = cur->charMap.find(s[i]);
		if(it == cur->charMap.end())
			return NULL;

		cur = cur->charMap[s[i]];
	}

	return cur;
}

bool is_word(trieNode* candidate)
{
	//Here we use another booleanfield in the trieNode. Alternatively, we can check if the frequency if a node > sum of frequency of outbound nodes
	if(!candidate)
		return false;

	return candidate->isLeaf;
}

bool isValid(int x, int y, std::vector<std::vector<char> > &crossword)
{
	return (x < crossword.size() && y < crossword[0].size());
}

void solve_crossword(trieNode *root, std::vector<std::vector<char> > &crossword, std::vector<string> &res, int x, int y, string &so_far, \
						std::vector<std::vector<bool> > &visited)
{
	if(!isValid(x, y, crossword) || visited[x][y])
		return;

	so_far+= crossword[x][y];
	trieNode *find_res = is_present(root, so_far);
	if(!find_res)
	{
		so_far.pop_back();
		return;
	}

	visited[x][y] = 1;
	
	if (is_word(find_res))
	{
		res.push_back(so_far);
	}
	
	solve_crossword(root, crossword, res, x+1, y, so_far, visited);
	solve_crossword(root, crossword, res, x-1, y, so_far, visited);
	solve_crossword(root, crossword, res, x, y+1, so_far, visited);
	solve_crossword(root, crossword, res, x, y-1, so_far, visited);
	

	so_far.pop_back();
	visited[x][y] = 0;
	
}

int main()
{
	trieNode *root = NULL;
	std::vector<string> res;
	root = insert_word(root, "oath");
	root = insert_word(root, "oaths");
	root = insert_word(root, "eat");
	root = insert_word(root, "rain");
	root = insert_word(root, "peace");
	

	std::vector<std::vector<char> > crossword = {
  													{'o','a','a','n'},
  													{'e','t','a','e'},
  													{'i','h','s','r'},
  													{'i','f','l','v'}
												};

	/*
	//Shortest unique prefixes
	get_shortest_unique_prefix(root, res);
	for(int i =0 ; i < res.size(); i++)
		cout << res[i] << endl;
	

	//Search in a trie
	trieNode *find1 = is_present(root, "peak");
	cout << (find1 ? is_word(find1) : 0);
	
	*/

	//Find dict words in the crossword by starting from each point in the matrix
	for(int i = 0; i < crossword.size(); i++)
	{
		for(int j = 0; j < crossword[0].size(); j++)
		{
			string so_far = "";
			std::vector<std::vector<bool> > visited(crossword.size(), std::vector<bool> (crossword[0].size(), 0)); 
			solve_crossword(root, crossword, res, i, j, so_far, visited);	
		}
	}
	
	for(int i =0 ; i < res.size(); i++)
		cout << res[i] << endl;

	return 0;
}
