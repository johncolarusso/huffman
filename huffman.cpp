/*
John Colarusso

Use: huffman [-d] < inputFileName > outputFileName
	-d specifies whether the input will be decompressed from an encoded file to a plain text file.
	Not specifying -d will compress the input plain text file to an encoded file.

Sample use/input files:
	huffman < hugeinput.txt > compressed1
	huffman -d < compressed1 > output

*/
//#define debug
#include <iostream>
#include <ostream>
#include <cstdlib>
#include <string>
#include <climits>
#include <cmath>
#define MAX_ASCII 256
using namespace std;

struct Node {
	Node() { }
	Node(int myValue, char myC, int weight)
	{
		c = myC;
		value = myValue;
		w = weight;
		left = NULL;
		right = NULL;
		smallest = myValue;
	}
	Node( char myC, int weight, Node* leftnode, Node* rightnode )
	{
		c = myC;
		value = c;
		w = weight;
		left = leftnode;
		right = rightnode;
		smallest = minChar(left,right);
	}
	bool operator<( Node& n ) {
		if( w < n.w ) {
			return true;
		} else if( w > n.w ) {
			return false;
		} else {
			return smallest < n.smallest;
		}
	}
	int minChar( Node* n1, Node* n2 ) 
	{
		if( n1->smallest <  n2->smallest ) {
			return n1->smallest;
		} else {
			return n2->smallest;
		}
	}
	char c;
	int w;
	int value;
	int smallest;
	Node* left;
	Node* right;
};

void swap( Node &e1, Node &e2 )
{
	Node temp;
	temp = e1;
	e1 = e2;
	e2 = temp;
}
Node* min( Node& n1, Node& n2 ) {
	if( n1 < n2 ) {
		return &n1;
	} else {
		return &n2;
	}
}
Node* max( Node& n1, Node& n2 ) {
	if( n2 < n1 ) {
		return &n1;
	} else {
		return &n2;
	}
}
class MinHeap
{
	public:
		MinHeap() { 
			size = 0;
			capacity = 256;
		};
		void init()
		{
			arr = new Node[capacity];
		}
		void insert(Node e)
		{
			arr[size] = e;
			int pos = size;
			int parent;
			while(pos > 0)
			{
				parent = (pos-1) / 2;
				if( arr[pos] < arr[parent] ) {
					swap(arr[pos], arr[parent]);
				} else {
					break;
				}
				pos = parent;
			}
			size++;
		}
		int getSize()
		{
			return size;
		}
		Node deleteMin()
		{
			Node rtn = arr[0];
			arr[0] = arr[size-1];
			size--;
			int root = 0;
			int left = 1;
			int right = 2;
			while( root < size ) {
				if( left < size ) {
					if( arr[left] < arr[root] ) {
						if( right < size ) {
							if( arr[right] < arr[left] ) {
								swap( arr[root], arr[right] );
								root = right;
							} else {
								swap( arr[root], arr[left] );
								root = left;
							}
						} else {
							swap( arr[root], arr[left] );
							root = left;
						}
						left = 2*root+1;
						right = 2*root+2;
						continue;
					} else if( right < size ) {
						if( arr[right] < arr[root] ) {
							swap( arr[root], arr[right] );
							root = right;
							left = 2*root+1;
							right = 2*root+2;
							continue;
						}
					} else {
						break;
					}
				}
				break;
			}
			return rtn;
		}
		void printHeap() // debug 
		{
			cout << "size " <<  size << ": ";
			if( size == 0 )
				cout << "empty" << endl;
			else {
				for(int i = 0; i < size; i++)
					cout << arr[i].w << " ";
				cout << endl;
			}
		
		}
		Node* arr;
		int size;		// size is the index after last element of heap 
						// and # of elements
		unsigned long int capacity;
};
MinHeap h;

//	traverse( root, table, length_table, 0, 0 );
void traverse( Node* root,  unsigned long * table, int* len_table, unsigned long code, int codeLength )
{
	if( root != NULL ) {
		if( root->left == NULL && root->right == NULL ) {
			table[root->value] = code;
			len_table[root->value] = codeLength;
		} else {
			traverse( root->left, table, len_table, (code << 1), codeLength+1);
			traverse( root->right, table, len_table, (code << 1) | 0x01, codeLength+1);
		}
	}
}
void printZero()
{
	unsigned char zero = '\0';
	cout << zero; 
	cout << zero; 
	cout << zero; 
	cout << zero;
}
int getIndex(char* table, char c)
{
	for(int i = 0; i < 256; i++)
	{
		if( table[i] == c )
		{
			return i;
		}
	}
}
void compress()
{
	unsigned long freqs[MAX_ASCII];	
	Node arr[10000];
	char ascii[MAX_ASCII];	
	unsigned long table[MAX_ASCII];
	int length_table[MAX_ASCII];
	int arr_count = 0;
	string input;
	Node* n, *root;
	
	for(int i = 0; i < 256; i++) {
		freqs[i] = 0;
		table[i] = -1;
		length_table[i] = 0;
		ascii[i] = (char)i;
	}

	int temp = cin.get();
	while( (int)temp != 0xffffffff ) {	// Append Char From Input to an Input String
		input+=temp;
		freqs[temp]++;
		temp = cin.get();
	}
	
	h.init();
	for( int i = 0; i < 256; i++ ) {	// Insert into Min Heap
		if(freqs[i] != 0) {
			n = new Node(i, (char)i, freqs[i]);
			h.insert(*n);
	//		h.printHeap();
		}
	}
	
	// Delete from a minheap and store nodes into a list
	while(h.getSize() > 1) {
		arr[arr_count] = h.deleteMin();
		arr[arr_count+1] = h.deleteMin();
		n = new Node( '\0', 
					arr[arr_count].w + arr[arr_count+1].w, 
					max(arr[arr_count], arr[arr_count+1]),
					min(arr[arr_count], arr[arr_count+1]) );
		h.insert(*n);
	//	h.printHeap();
		arr_count += 2;
	}
	arr[arr_count] = h.deleteMin();
	
	
	/* Traverse the tree to find codes */
	root = &arr[arr_count];
	traverse( root, table, length_table, 0, 0 );
	#ifdef debug
	for(int i = 0; i < 256; i++) {		
		if(freqs[i] != 0)	
			cout << "  " << (char)i <<  " ASCII = " << i  		
			<< "| freq = " << freqs[i] 
			<< " | " << table[i] 
			<< ", length "			<< length_table[i] 
			<< endl;	
	}
	#endif
	/* Use code table to write frequencies */
	unsigned long num;
	unsigned char bit;
	unsigned char byte;
	int bytes_printed = 0;
	
	cout << "HUFFMA1" << '\0';
	for(int i = 0; i < 256; i++)
	{
		if(freqs[i] != 0) {
			num = freqs[i];
			bytes_printed = 0;
			byte = 0;
			while(bytes_printed < 4)
			{
				byte = num & 0xFF;
				cout << byte;
				bytes_printed++;
				num = num >> 8;
			}
			while(bytes_printed < 4) {
				cout << '\0';
				bytes_printed++;
			}
		} else {
			printZero();
		}
	}
	
	/* use code table to write input */
	unsigned char x = 0;
	int bitsleft = 0;
	string output;
	int o_count = 0;
	int code_length;
	char current_char;
	int index;
	unsigned long huffman_tree_code;
	int bit_mask = 0;
	
	for(int i = 0; i < input.length(); i++)
	{
		current_char = input[i];
		index = getIndex(ascii, current_char);
		huffman_tree_code = table[index];  
		code_length = length_table[index]; 
			#ifdef debug
			cout << "\n\n\nChar  " << current_char << " ascii value " << index << "  (length " << code_length << "):";
			#endif
		bit_mask = 0x01 << (code_length - 1);
		while( code_length > 0 )
		{
			bit = (huffman_tree_code & bit_mask) >> (code_length - 1);
			#ifdef debug
				cout << endl << "Added bit " <<(int)bit;
				cout << " code left: " << huffman_tree_code;
			#endif
			code_length--;
			bit_mask = bit_mask >> 1;
			x += bit * pow(2, bitsleft);
			bitsleft++;
			if(bitsleft == 8) {
			#ifndef debug
				cout << x; 
			#endif
				#ifdef debug
				cout << endl << "new byte: " << x;
				#endif
				o_count++;
				x = 0;
				bitsleft = 0;
			}
			#ifdef debug
			cout << " byte: " << (int)x;
			#endif
		}
	}
	if(bitsleft != 0) {
		#ifndef debug
		cout << x;
		#endif
		o_count++;
	}
	

}
void decompress()
{
	unsigned long freqs[MAX_ASCII];
	unsigned long total = 0;
	Node arr[10000];
	char ascii[MAX_ASCII];	
	unsigned long table[MAX_ASCII];
	int length_table[MAX_ASCII];
	int arr_count = 0;
	Node* n, *root;
	
	for(int i = 0; i < 256; i++) {
		freqs[i] = 0;
		table[i] = -1;
		length_table[i] = 0;
		ascii[i] = (char)i;
	}
	
	int temp;
	unsigned long f_temp;
	for(int i = 0; i < 8; i++) {
		temp = cin.get();
	}
	for(int i = 0; i < 256; i++)
	{
		f_temp = 0;
		for(int j = 0; j <= 3; j++)
		{
			temp = cin.get();
			temp = temp << 8*j;
			f_temp = f_temp | temp;
		}
		freqs[i] = f_temp;
		total += f_temp;
	}

	h.init();
	for( int i = 0; i < 256; i++ ) {	// Insert into Min Heap
		if(freqs[i] != 0) {
			n = new Node(i, (char)i, freqs[i]);
			h.insert(*n);
	//		h.printHeap();
		}
	}
	
	// Delete from a minheap and store nodes into a list
	while(h.getSize() > 1) {
		arr[arr_count] = h.deleteMin();
		arr[arr_count+1] = h.deleteMin();
		n = new Node( '\0', 
					arr[arr_count].w + arr[arr_count+1].w, 
					max(arr[arr_count], arr[arr_count+1]),
					min(arr[arr_count], arr[arr_count+1]) );
		h.insert(*n);
	//	h.printHeap();
		arr_count += 2;
	}
	arr[arr_count] = h.deleteMin();
	
	
	// Traverse the tree: For each 0, traverse left, For each 1, traverse right
	// Print Character when at leaf node, Reset ptr to root
	root = &arr[arr_count];
	
	temp = cin.get();
	
	int compare;
	int t = 0;
	Node* ptr = root;
	while( temp != 0xffffffff ) {	// Get Char From Input -> Input (String)
		for(int i = 0; i < 8; i++)
		{
			if(t == total)
				break;
			compare = temp & 0x01;
			if(compare == 0) {
				ptr = ptr->left;
			} else {
				ptr = ptr->right;
			}
			if(ptr->left == NULL && ptr->right == NULL)
			{
				cout << ptr->c;
				t++;
				ptr = root;
			}
			temp = temp >> 1;
		}
		temp = cin.get();
	}
}
int main( int argc, char* argv[] )
{
	string decmp( "-d" );		// option to decompress
	if( argc == 2 && decmp.compare(argv[1]) == 0 )
	{
		decompress();
	}
	else
		compress();
	return 0;
}