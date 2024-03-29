#include <iostream>

using namespace std;

template <typename Key, typename Value>
class Map {
private:
	struct Node {
		pair<Key, Value> data;
		Node* pLeft;
		Node* pRight;
		Node* Parent;
		Node() {
			pLeft = nullptr;
			pRight = nullptr;
			Parent = nullptr;
		}
		Node(pair<Key, Value> _data) {
			data = _data;
			pLeft = nullptr;
			pRight = nullptr;
			Parent = nullptr;
		}
	};
	Node* tree;
	size_t size;

	void clear(Node* _node) {
		if (_node == nullptr) {
			return;
		}
		clear(_node->pLeft);
		clear(_node->pRight);
		delete _node;
	}
	Node* insert(Node* _node, pair<Key, Value> _data) {
		if (_node == nullptr) {
			size++;
			_node = new Node(_data);
			return _node;
		}
		if (size == 0) {
			size++;
			_node->data = _data;
			return _node;
		}
		if (_node->data.first > _data.first) {
			_node->pLeft = insert(_node->pLeft, _data);
			if (_node->pLeft != nullptr) {
				_node->pLeft->Parent = _node;
			}
		}
		else if (_node->data.first < _data.first) {
			_node->pRight = insert(_node->pRight, _data);
			if (_node->pRight != nullptr) {
				_node->pRight->Parent = _node;
			}
		}
		else {
			_node->data = _data;
		}
		return _node;
	}
	Node* find(Node* _node, Key _key) {
		if (_node == nullptr) {
			cout << "No such element!\n";
			return nullptr;
		}
		Node* tempNode;
		if (_node->data.first > _key) {
			tempNode = find(_node->pLeft, _key);
		}
		else if (_node->data.first < _key) {
			tempNode = find(_node->pRight, _key);
		}
		else {
			tempNode = _node;
		}
		return tempNode;
	}
	Node* erase(Node* _node, Key _key) {
		if (_node == nullptr) {
			cout << "No such element!\n";
			return 0;
		}
		if (_node->data.first > _key) {
			_node->pLeft = erase(_node->pLeft, _key);
		}
		else if (_node->data.first < _key) {
			_node->pRight = erase(_node->pRight, _key);
		}
		else if (_node->pLeft != nullptr && _node->pRight != nullptr) {
			_node->data = min_node(_node->pRight)->data;
			_node->pRight = erase(_node->pRight, _node->data.first);
		}
		else {
			if (_node->pLeft != nullptr) {
				_node = _node->pLeft;
			}
			else if (_node->pRight != nullptr) {
				_node = _node->pRight;
			}
			else {
				_node = nullptr;
			}
			size--;
		}
		return _node;
	}
	Node* max_node(Node* _node) {
		return _node->pRight ? max_node(_node->pRight) : _node;
	}
	Node* min_node(Node* _node) {
		return _node->pLeft ? min_node(_node->pLeft) : _node;
	}
	Node* next(Node* _node, Key _key) {
		if (_node == nullptr) {
			return nullptr;
		}
		Node* temp_node;
		if (_key >= _node->data.first) {
			temp_node = next(_node->pRight, _key);
		}
		else {
			temp_node = next(_node->pLeft, _key);
			if (temp_node == nullptr || temp_node->data.first <= _key) {
				temp_node = _node;
			}
		}
		return temp_node;
	}
	Node* prev(Node* _node, Key _key) {
		if (_node == nullptr) {
			return nullptr;
		}
		Node* temp_node;
		if (_key <= _node->data.first) {
			temp_node = prev(_node->pLeft, _key);
		}
		else {
			temp_node = prev(_node->pRight, _key);
			if (temp_node == nullptr || temp_node->data.first >= _key) {
				temp_node = _node;
			}
		}
		return temp_node;
	}
public:
	Map() {
		tree = new Node();
		size = 0;
	}
	~Map() {
		clear(tree);
	}
	size_t Size() {
		return size;
	}
	bool Empty() {
		return size == 0;
	}
	void Insert(pair<Key, Value> _data) {
		insert(tree, _data);
	}
	void Erase(Key _key) {
		erase(tree, _key);
	}
	Value& Find(Key _key) {
		Node* temp_node = find(tree, _key);
		if (temp_node != nullptr) {
			return temp_node->data.second;
		}
	}
	bool FindBool(Key _key) {
		return find(tree, _key) != nullptr;
	}
	void Clear() {
		clear(tree);
		size = 0;
		tree = new Node();
	}
	Value& operator[](Key _key) {
		Node* temp_node = find(tree, _key);
		if (temp_node != nullptr) {
			return temp_node->data.second;
		}
		else {
			exit(1);
		}
	}
	Key& Next(Key _key) {
		Node* temp_node = next(tree, _key);
		if (temp_node == nullptr || temp_node->data.first <= _key) {
			cout << "No such element!\n";
			exit(1);
		}
		else {
			return temp_node->data.first;
		}
	}
	Key& Prev(Key _key) {
		Node* temp_node = prev(tree, _key);
		if (temp_node == nullptr || temp_node->data.first >= _key) {
			cout << "No such element!\n";
			exit(1);
		}
		else {
			return temp_node->data.first;
		}
	}

	class IteratorMap : iterator<bidirectional_iterator_tag, pair<Key, Value>> {
	private:
		Node* ptr;
	public:
		Node* max_node(Node* _node) {
			return _node->pRight ? max_node(_node->pRight) : _node;
		}
		Node* min_node(Node* _node) {
			return _node->pLeft ? min_node(_node->pLeft) : _node;
		}
		Node* next(Node* _node) {
			if (_node->pRight != nullptr) {
				return min_node(_node->pRight);
			}
			Node* parent = _node->Parent;
			while (parent != nullptr && _node == parent->pRight) {
				_node = parent;
				parent = _node->Parent;
			}
			return parent;
		}
		Node* prev(Node* _node) {
			if (_node->pLeft != nullptr) {
				return max_node(_node->pLeft);
			}
			Node* parent = _node->Parent;
			while (parent != nullptr && _node == parent->pLeft) {
				_node = parent;
				parent = _node->Parent;
			}
			return parent;
		}
		IteratorMap() {
			ptr = nullptr;
		}
		IteratorMap(Node* _node) {
			ptr = _node;
		}
		IteratorMap& operator=(const IteratorMap& it) {
			this->ptr = it.ptr;
			return *this;
		}
		bool operator==(const IteratorMap& it) const {
			return this->ptr == it.ptr;
		}
		bool operator!=(const IteratorMap& it) const {
			return this->ptr != it.ptr;
		}
		IteratorMap& operator++() {
			ptr = next(ptr);
			return *this;
		}
		IteratorMap& operator++(Key) {
			auto it = this;
			ptr = next(ptr);
			return *it;
		}
		IteratorMap& operator--() {
			ptr = prev(ptr);
			return *this;
		}
		IteratorMap& operator--(Key) {
			auto it = this;
			ptr = prev(ptr);
			return *it;
		}
		pair<Key, Value>& operator*() {
			return ptr->data;
		}
		pair<Key, Value> operator*() const {
			return ptr->data;
		}
		bool operator>(const IteratorMap& it)
		{
			return ptr->data.first > it.ptr->data.first;
		}
		bool operator<(const IteratorMap& it)
		{
			return ptr->data.first < it.ptr->data.first;
		}
		bool operator<=(const IteratorMap& it)
		{
			return ptr->data.first <= it.ptr->data.first;
		}
		bool operator>=(const IteratorMap& it)
		{
			return ptr->data.first >= it.ptr->data.first;
		}
	};
	IteratorMap begin() {
		return IteratorMap(min_node(tree));
	}
	IteratorMap end() {
		Node* temp_node = new Node();
		max_node(tree)->pRight = temp_node;
		temp_node->Parent = max_node(tree);
		return IteratorMap(temp_node);
	}
};
template <typename Key>
class Set {
private:
	Map <Key, char>* tree;
public:
	Set() {
		tree = new Map<Key, char>();
	}
	~Set() {
		Clear();
	}
	void Insert(Key _key) {
		(*tree).Insert(make_pair(_key, 0));
	}
	void Erase(Key _key) {
		(*tree).Erase(_key);
	}
	size_t Size() {
		return (*tree).Size();
	}
	bool Empty() {
		return (*tree).Size() == 0;
	}
	bool Find(Key _key) {
		return (*tree).FindBool(_key);
	}
	void Clear() {
		(*tree).Clear();
	}
	Key& Next(Key _key) {
		return (*tree).Next(_key);
	}
	Key& Prev(Key _key) {
		return (*tree).Prev(_key);
	}
	class IteratorSet : iterator<bidirectional_iterator_tag, Key> {
	private:
		typename Map<Key, char>::IteratorMap  ptr;
	public:
		IteratorSet(typename Map<Key, char>::IteratorMap it) {
			ptr = it;
		}
		IteratorSet& operator=(const IteratorSet& it) {
			this->ptr = it.ptr;
			return *this;
		}
		bool operator==(const IteratorSet& it) const {
			return this->ptr == it.ptr;
		}
		bool operator!=(const IteratorSet& it) const {
			return this->ptr != it.ptr;
		}
		IteratorSet& operator++() {
			ptr++;
			return *this;
		}
		IteratorSet& operator++(Key) {
			auto it = this;
			ptr++;
			return *it;
		}
		IteratorSet& operator--() {
			ptr--;
			return *this;
		}
		IteratorSet& operator--(Key) {
			auto it = this;
			ptr--;
			return *it;
		}
		Key& operator*() {
			return (*ptr).first;
		}
		Key operator*() const {
			return (*ptr).first;
		}
		bool operator>(const IteratorSet& it) const {
			return this->ptr > it.ptr;
		}
		bool operator>=(const IteratorSet& it) const {
			return this->ptr >= it.ptr;
		}
		bool operator<(const IteratorSet& it) const {
			return this->ptr < it.ptr;
		}
		bool operator<=(const IteratorSet& it) const {
			return this->ptr <= it.ptr;
		}
	};
	IteratorSet begin() {
		return IteratorSet((*tree).begin());
	}
	IteratorSet end() {
		return IteratorSet((*tree).end());
	}
};

template <typename Key, typename Value>
class Map2 {
private:
	struct Node {
		pair<Key, Value> data;
		Node* pLeft;
		Node* pRight;
		Node* Parent;
		Node* pNext;
		Node* pPrev;

		Node() {
			pLeft = nullptr;
			pRight = nullptr;
			Parent = nullptr;
			pNext = nullptr;
			pPrev = nullptr;
		}
		Node(pair<Key, Value> _data) {
			data = _data;
			pLeft = nullptr;
			pRight = nullptr;
			Parent = nullptr;
			pNext = nullptr;
			pPrev = nullptr;
		}
	};
	Node* tree;
	size_t size;

	void clear(Node* _node) {
		if (_node == nullptr) {
			return;
		}
		clear(_node->pLeft);
		clear(_node->pRight);
		delete _node;
	}
	Node* insert(Node* _node, pair<Key, Value> _data) {
		if (_node == nullptr) {
			size++;
			_node = new Node(_data);
			Node* next_node = next(tree, _data.first);
			_node->pNext = next_node;
			if (next_node != nullptr) {
				next_node->pPrev = _node;
			}
			Node* prev_node = prev(tree, _data.first);
			_node->pPrev = prev_node;
			if (prev_node != nullptr) {
				prev_node->pNext = _node;
			}
			return _node;
		}
		if (size == 0) {
			size++;
			_node->data = _data;
			return _node;
		}
		if (_node->data.first > _data.first) {
			_node->pLeft = insert(_node->pLeft, _data);
			if (_node->pLeft != nullptr) {
				_node->pLeft->Parent = _node;
			}
		}
		else if (_node->data.first < _data.first) {
			_node->pRight = insert(_node->pRight, _data);
			if (_node->pRight != nullptr) {
				_node->pRight->Parent = _node;
			}
		}
		else {
			_node->data = _data;
		}
		return _node;
	}
	Node* find(Node* _node, Key _key) {
		if (_node == nullptr) {
			return nullptr;
		}
		Node* tempNode;
		if (_node->data.first > _key) {
			tempNode = find(_node->pLeft, _key);
		}
		else if (_node->data.first < _key) {
			tempNode = find(_node->pRight, _key);
		}
		else {
			tempNode = _node;
		}
		return tempNode;
	}
	Node* erase(Node* _node, Key _key) {
		if (_node == nullptr) {
			return 0;
		}
		if (_node->data.first > _key) {
			_node->pLeft = erase(_node->pLeft, _key);
		}
		else if (_node->data.first < _key) {
			_node->pRight = erase(_node->pRight, _key);
		}
		else if (_node->pLeft != nullptr && _node->pRight != nullptr) {
			_node->data = min_node(_node->pRight)->data;
			_node->pNext = min_node(_node->pRight)->pNext;
			_node->pRight = erase(_node->pRight, _node->data.first);
		}
		else {
			if (_node->pLeft != nullptr) {
				_node = _node->pLeft;
			}
			else if (_node->pRight != nullptr) {
				_node = _node->pRight;
			}
			else {
				_node = nullptr;
			}
			size--;
		}
		return _node;
	}
	Node* max_node(Node* _node) {
		return _node->pRight ? max_node(_node->pRight) : _node;
	}
	Node* min_node(Node* _node) {
		return _node->pLeft ? min_node(_node->pLeft) : _node;
	}
	Node* next(Node* _node, Key _key) {
		if (_node == nullptr) {
			return nullptr;
		}
		Node* temp_node;
		if (_key >= _node->data.first) {
			temp_node = next(_node->pRight, _key);
		}
		else {
			temp_node = next(_node->pLeft, _key);
			if (temp_node == nullptr || temp_node->data.first <= _key) {
				temp_node = _node;
			}
		}
		return temp_node;
	}
	Node* prev(Node* _node, Key _key) {
		if (_node == nullptr) {
			return nullptr;
		}
		Node* temp_node;
		if (_key <= _node->data.first) {
			temp_node = prev(_node->pLeft, _key);
		}
		else {
			temp_node = prev(_node->pRight, _key);
			if (temp_node == nullptr || temp_node->data.first >= _key) {
				temp_node = _node;
			}
		}
		return temp_node;
	}
public:
	Map2() {
		tree = new Node();
		size = 0;
	}
	~Map2() {
		clear(tree);
	}
	size_t Size() {
		return size;
	}
	bool Empty() {
		return size == 0;
	}
	void Insert(pair<Key, Value> _data) {
		insert(tree, _data);
	}
	void Erase(Key _key) {
		erase(tree, _key);
	}
	Value& Find(Key _key) {
		Node* temp_node = find(tree, _key);
		if (temp_node != nullptr) {
			return temp_node->data.second;
		}
	}
	bool FindBool(Key _key) {
		return find(tree, _key) != nullptr;
	}
	void Clear() {
		clear(tree);
		size = 0;
		tree = new Node();
	}
	Value& operator[](Key _key) {
		Node* temp_node = find(tree, _key);
		if (temp_node != nullptr) {
			return temp_node->data.second;
		}
		else {
			exit(1);
		}
	}
	Key& Next(Key _key) {
		Node* temp_node = next(tree, _key);
		if (temp_node == nullptr || temp_node->data.first <= _key) {
			cout << "No such element!\n";
			exit(1);
		}
		else {
			return temp_node->data.first;
		}
	}
	Key& Prev(Key _key) {
		Node* temp_node = prev(tree, _key);
		if (temp_node == nullptr || temp_node->data.first >= _key) {
			cout << "No such element!\n";
			exit(1);
		}
		else {
			return temp_node->data.first;
		}
	}

	class IteratorMap2 : iterator<bidirectional_iterator_tag, pair<Key, Value>> {
	private:
		Node* ptr;
	public:
		Node* max_node(Node* _node) {
			return _node->pRight ? max_node(_node->pRight) : _node;
		}
		Node* min_node(Node* _node) {
			return _node->pLeft ? min_node(_node->pLeft) : _node;
		}
		IteratorMap2() {
			ptr = nullptr;
		}
		IteratorMap2(Node* _node) {
			ptr = _node;
		}
		IteratorMap2& operator=(const IteratorMap2& it) {
			this->ptr = it.ptr;
			return *this;
		}
		bool operator==(const IteratorMap2& it) const {
			return this->ptr == it.ptr;
		}
		bool operator!=(const IteratorMap2& it) const {
			return this->ptr != it.ptr;
		}
		IteratorMap2& operator++() {
			ptr = ptr->pNext;
			return *this;
		}
		IteratorMap2& operator++(Key) {
			auto it = this;
			ptr = ptr->pNext;
			return *it;
		}
		IteratorMap2& operator--() {
			ptr = ptr->pPrev;
			return *this;
		}
		IteratorMap2& operator--(Key) {
			auto it = this;
			ptr = ptr->pPrev;
			return *it;
		}
		pair<Key, Value>& operator*() {
			return ptr->data;
		}
		pair<Key, Value> operator*() const {
			return ptr->data;
		}
		bool operator>(const IteratorMap2& it)
		{
			return ptr->data.first > it.ptr->data.first;
		}
		bool operator<(const IteratorMap2& it)
		{
			return ptr->data.first < it.ptr->data.first;
		}
		bool operator<=(const IteratorMap2& it)
		{
			return ptr->data.first <= it.ptr->data.first;
		}
		bool operator>=(const IteratorMap2& it)
		{
			return ptr->data.first >= it.ptr->data.first;
		}
	};
	IteratorMap2 begin() {
		return IteratorMap2(min_node(tree));
	}
	IteratorMap2 end() {
		Node* temp_node = new Node();
		Node* max = max_node(tree);
		max->pRight = temp_node;
		max->pNext = temp_node;
		temp_node->Parent = max;
		temp_node->pPrev = max;
		return IteratorMap2(temp_node);
	}
};
template <typename Key>
class Set2 {
private:
	Map2 <Key, char>* tree;
public:
	Set2() {
		tree = new Map<Key, char>();
	}
	~Set2() {
		Clear();
	}
	void Insert(Key _key) {
		(*tree).Insert(make_pair(_key, 0));
	}
	void Erase(Key _key) {
		(*tree).Erase(_key);
	}
	size_t Size() {
		return (*tree).Size();
	}
	bool Empty() {
		return (*tree).Size() == 0;
	}
	bool Find(Key _key) {
		return (*tree).FindBool(_key);
	}
	void Clear() {
		(*tree).Clear();
	}
	Key& Next(Key _key) {
		return (*tree).Next(_key);
	}
	Key& Prev(Key _key) {
		return (*tree).Prev(_key);
	}
	class IteratorSet2 : iterator<bidirectional_iterator_tag, Key> {
	private:
		typename Map2<Key, char>::IteratorMap  ptr;
	public:
		IteratorSet2(typename Map2<Key, char>::IteratorMap2 it) {
			ptr = it;
		}
		IteratorSet2& operator=(const IteratorSet2& it) {
			this->ptr = it.ptr;
			return *this;
		}
		bool operator==(const IteratorSet2& it) const {
			return this->ptr == it.ptr;
		}
		bool operator!=(const IteratorSet2& it) const {
			return this->ptr != it.ptr;
		}
		IteratorSet2& operator++() {
			ptr++;
			return *this;
		}
		IteratorSet2& operator++(Key) {
			auto it = this;
			ptr++;
			return *it;
		}
		IteratorSet2& operator--() {
			ptr--;
			return *this;
		}
		IteratorSet2& operator--(Key) {
			auto it = this;
			ptr--;
			return *it;
		}
		Key& operator*() {
			return (*ptr).first;
		}
		Key operator*() const {
			return (*ptr).first;
		}
		bool operator>(const IteratorSet2& it) const {
			return this->ptr > it.ptr;
		}
		bool operator>=(const IteratorSet2& it) const {
			return this->ptr >= it.ptr;
		}
		bool operator<(const IteratorSet2& it) const {
			return this->ptr < it.ptr;
		}
		bool operator<=(const IteratorSet2& it) const {
			return this->ptr <= it.ptr;
		}
	};
	IteratorSet2 begin() {
		return IteratorSet2((*tree).begin());
	}
	IteratorSet2 end() {
		return IteratorSet2((*tree).end());
	}
};
int main() {
	return 0;
}