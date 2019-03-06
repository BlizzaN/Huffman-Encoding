#pragma once
#include "pch.h"
#include "Tree.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

//Node structure
struct Node {
	char ch;
	int freq;
	Node *left;
	Node *right;
};

Node *createNode(char ch, int freq, Node *left, Node *right) {
	Node *node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

struct compare {
	bool operator()(Node *left, Node *right) {
		return left->freq > right->freq;
	}
};

void encode(Node *root, string text, unordered_map<char, string> &code) {
	if (root == nullptr) return;
	//Found a leaf, set the bitstring to correct char
	if (root->left == NULL && root->right == NULL) {
		code[root->ch] = text;
		return;
	}
	encode(root->right, text + "1", code);	
	encode(root->left, text + "0", code);
}

void decode(Node *root, string &text, int &i) {
	if (root == nullptr) return;
		//Found a leaf
		if (root->left == NULL && root->right == NULL) {
			cout << root->ch;
			return;
		}		
		if (text[i] == '0') {
			decode(root->left, text, ++i);
		}else {
			decode(root->right, text, ++i);
		}
}

void Tree::buildTree(string text) {
	unordered_map<char, int> map;
	//Increment frequence for every char and add it to map
	for (auto ch : text) {	
		map[ch]++;
	}

	//Priorityqueue heap sort, sort all elements
	priority_queue<Node*, vector<Node*>, compare> queue;

	//Create leaf node with character and frequence of it
	for (auto leaf : map) {	
		queue.push(createNode(leaf.first, leaf.second, nullptr, nullptr));
	}

	//Create nodes as long as there exists more than one tree
	while (queue.size() != 1) {	
		Node *left = queue.top();
		queue.pop();
		Node *right = queue.top();
		queue.pop();
		cout << "LEFT: " << left->freq << " RIGHT: " << right->freq << endl;
		int sumFreq = left->freq + right->freq;

		//Create new node with the new freq
		queue.push(createNode(' ', sumFreq, left, right)); //log(n)
	}

	//Store root node
	Node *root = queue.top(); //1

	//Traverse the tree and store the codes
	unordered_map<char, string> codes; 
	encode(root, "", codes);	


	for (auto text : codes) {
		cout << text.first << " " << text.second << endl;
	}

	string str = "";
	//Loop through the text and store all bits in str
	for (char c : text) {	
		str += codes[c];
	}
	cout << "Bits: " << str << endl;
	//Traverse the tree and decode the codes
	int i = 0;
	while(i < str.length()) {	
		decode(root, str, i);
	}
}