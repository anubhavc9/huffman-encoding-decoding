#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

// function to allocate a new tree node
Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* newnode = new Node();

	newnode->ch = ch;
	newnode->freq = freq;
	newnode->left = left;
	newnode->right = right;

	return newnode;
}

// comparator to be used for ordering the heap
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// lowest frequency character has the highest priority
		return l->freq > r->freq;
	}
};

void encode(Node* root, string str, unordered_map<char,string>&huffmanCode)
{
	if(root == nullptr)
		return;
	// found a leaf node
	if(root->left == nullptr && root->right == nullptr)
	{
		huffmanCode[root->ch] = str;
	}
	encode(root->left, str+"0", huffmanCode);
	encode(root->right, str+"1", huffmanCode);
}

// traverse the Huffman tree & decode the encoded string
void decode(Node* root, int &top_index, string str)
{
	if(root == nullptr)
		return;
	// found a leaf node
	if(!root->left && !root->right)
	{
		cout << root->ch;
		return;
	}
	top_index++;

	if(str[top_index] == '0')
		decode(root->left, top_index, str);
	else
		decode(root->right, top_index, str);
}

void buildHuffmanTree(string text)
{
	// count frequency of appearance of each character & store it in a map
	unordered_map <char,int> freq;
	for(char ch : text)
	{
		freq[ch]++;
	}

	// create a priority queue to store leaf node of the Huffman tree
	// by passing in the comparator, we're making it a min heap
	// Refer this for more clarity -> https://www.geeksforgeeks.org/priority-queue-in-cpp-stl/
	priority_queue <Node*, vector<Node*>, comp> pq;

	// create a leaf node for each character & add it to the priority queue
	for(auto it : freq)
	{
		pq.push(getNode(it.first,it.second,nullptr,nullptr));
	}

	// do while there is more than 1 node in the queue
	while(pq.size() != 1)
	{
		// remove two nodes of lowest frequency (highest priority) from the priority queue
		Node* left = pq.top();
		pq.pop();
		Node* right = pq.top();
		pq.pop();

		// create a new internal node with these two nodes as children and with frequency
		// equal to the sum of the two nodes' frequencies. Add the new node to the priority queue.
		int sum = left->freq + right->freq;
		pq.push(getNode('\0',sum,left,right));
	}

	// now only 1 node remains in the priority queue which will be the root of the Huffman tree
	Node* root = pq.top();

	// traverse the Huffman tree & store the Huffman codes in a map
	unordered_map <char,string> huffmanCode;

	// call the function to do the encoding
	encode(root,"",huffmanCode);

	cout << "\nHuffman codes are:\n" << endl;
	cout << "\nCharacter\tCode\n\n";
	for(auto ppair : huffmanCode)
	{
		cout << ppair.first << "\t\t" << ppair.second << endl;
	}

	cout << "\nOriginal string was:\n" << text << endl;

	// build the encoded string from the codes
	string str = "";
	for(char ch : text)
	{
		str += huffmanCode[ch];
	}

	cout << "\nEncoded string is : \n" << str << endl;

	// traverse the Huffman tree again and this time decode the encoded string
	int top_index = -1;
	cout << "\nDecoded string is:\n";
	while(top_index < (int)str.size()-2)
	{
		decode(root, top_index, str);
	}

	cout << endl;
}

int main()
{
	//string text = "Huffman coding is a data compression algorithm";
    string text = "BCCABBDDAECCBBAEDDCC";
	buildHuffmanTree(text);

    return 0;
}
