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
	if (root->left == NULL && root->right == NULL) {
		code[root->ch] = text;
		return;
	}
	encode(root->right, text + "1", code);
	encode(root->left, text + "0", code);
}

void decode(Node *root, string &text, int &i) {
	if (root == nullptr) return;
	if (root->left == NULL && root->right == NULL) {
		cout << root->ch;
		return;
	}
	if (text[i] == '0') {
		decode(root->left, text, ++i);
	}
	else {
		decode(root->right, text, ++i);
	}
}

void Tree::buildTree(string text) {
	unordered_map<char, int> map;
	for (auto ch : text) {	
		map[ch]++;
	}
	priority_queue<Node*, vector<Node*>, compare> queue;

	for (auto leaf : map) {	
		queue.push(createNode(leaf.first, leaf.second, nullptr, nullptr));
	}

	while (queue.size() != 1) {
		Node *left = queue.top();
		queue.pop();
		Node *right = queue.top();
		queue.pop();
		int sumFreq = left->freq + right->freq;

		queue.push(createNode(' ', sumFreq, left, right)); 
	}

	Node *root = queue.top(); 

	unordered_map<char, string> codes; 
	encode(root, "", codes);

	for (auto text : codes) {
		cout << text.first << " " << text.second << endl;
	}

	string str = "";
	for (char c : text) {	
		str += codes[c];
	}
	int i = 0;
	while (i < str.length()) {
		decode(root, str, i);
	}
}