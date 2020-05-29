#include "BPT.h"
#include "pch.h"
/*
2020/5/28
欠完成：
	1、Delete_Key
	2、Delete_Adjust
*/

/*------------------Node--------------------*/
template <typename T>
Node<T>::Node(int rank, bool isleaf) :
	rank(rank),
	isleaf(isleaf),
	key_num(0),
	father(NULL),
	last_leaf(NULL),
	next_leaf(NULL)
{
	/*两个vector初始化, key容量为rank, pointer的容量为rank + 1*/
	for (int i = 0; i < rank; i++) {
		key.push_back(T());//key: rank-1
		pointer.push_back(NULL);//pointer: rank
	}
	pointer.push_back(NULL);

	if (isleaf) {
		min_key = ceil((rank - 1) / 2);
		min_pointer = ceil((rank - 1) / 2);
	}else{
		min_key = ceil(rank / 2) - 1;
		min_pointer = ceil(rank / 2);
	}
}

template <typename T>
Node<T>::~Node() {

}

template <typename T>
bool Node<T>::isroot() {
	return (this->father == NULL);
}

template<typename T>
Node<T>* Node<T>::split(T& up_key) {
	/*
		up_key:
		1、叶节点：key[left_key_num]
		2、中间节点：key[left_pointer_num - 1]
	*/
	Node* nnode = new Node(rank, isleaf);
	//叶节点
	if (isleaf) {
		unsigned left_key_num = ceil(rank / 2);//old node
		unsigned right_key_num = rank - left_key_num;//new node
		up_key = key[left_key_num];//新节点的第一个元素需要上移
		for (unsigned int i = left_key_num; i < rank; i++) {
			nnode->key[i - left_key_num] = key[i];
			nnode->value[i - left_key_num] = pointer[i];
			key[i] = T();
			value[i] = int();
		}
		//调整指针
		nnode->last_leaf = this;
		nnode->next_leaf = this->next_leaf;
		this->next_leaf = nnode;
		nnode->father = this->father;
		//调整key的个数
		nnode->key_num = right_key_num;
		key_num = left_key_num;
	}
	//中间节点
	else {
		unsigned left_pointer_num = rank + 1 - ceil(rank / 2);//old node
		unsigned right_pointer_num = ceil(rank / 2);//new node
		up_key = key[left_pointer_num - 1];//夹在两部分指针的中间的key需要上移
		/*pointer transfer*/
		for (unsigned int i = left_pointer_num; i <= rank; i++) {
			nnode->pointer[i - left_pointer_num] = pointer[i];
			nnode->pointer[i - left_pointer_num]->father = nnode;
			pointer[i] = NULL;
		}
		/*key transfer*/
		for (unsigned int i = left_pointer_num; i < rank; i++) {
			nnode->key[i - left_pointer_num] = key[i];
			key[i] = T();
		}
		nnode->father = this->father;//连接父节点
		key[left_pointer_num - 1] = T();//把需要提上去的key的位置置空
		//修改前后两个节点的key_num
		nnode->key_num = left_pointer_num - 1;
		key_num = right_pointer_num - 1;
	}
}

template<typename T>
unsigned int Node<T>::insert_key(T v) {
	if (key_num == 0) {
		key[0] = v;
		key_num++;
		return 0;
	}
	else {
		unsigned int index = 0;
		bool if_exist = search_key(v, index);
		if (if_exist) {
			cout << "In insert_key(T v), v is already exist in position: " << index << endl;
			return index;
		}
		else {
			for (unsigned int i = key_num; i > index; i--) {
				key[i] = key[i - 1];
			}
			key[index] = v;
			for (unsigned int i = key_num + 1; i > index + 1; i--) {
				pointer[i] = pointer[i - 1];
			}
			pointer[index + 1] = NULL;
			key_num++;
			return index;//返回插入的位置
		}
	}
}

template<typename T>
unsigned int Node<T>::insert_key(T k, int val) {
	if (!isleaf) {
		return -1;
	}
	if (key_num == 0) {
		key[0] = k;
		value[0] = val;
		key_num++;
		return 0;
	}
	else {
		unsigned int index = 0;
		for (unsigned int i = key_num; i > index; i--) {
			key[i] = key[i - 1];
			value[i] = value[i - 1];
		}
		key[index] = k;
		value[index] = val;
		key_num++;
		return index;
	}
}
/*删除之后调整节点内元素的位置*/
template<typename T>
bool Node<T>::delete_key(unsigned int index) {
	if (index > key_num - 1 || index < 0) {
		return false;
	}
	else {
		//leaf node
		if (isleaf) {
			for (unsigned int i = index; i < key_num; i++) {
				key[i] = key[i + 1];
				value[i] = value[i + 1];
			}
			key[key_num - 1] = T();
			value[key_num - 1] = int();
			key_num--;
		}
		//branch node
		else {
			for (unsigned int i = index; i < key_num - 1; i++) {
				key[i] = key[i + 1];
			}
			for (unsigned int i = index + 1; i < key_num; i++) {
				pointer[i] = pointer[i + 1];
			}
			key[key_num - 1] = T();
			pointer[key_num] = NULL;
			key_num--;
		}
		return true;
	}
}

template<typename T>
bool Node<T>::search_key(T k, unsigned int &index) {
	if (key_num == 0) {
		index = 0;
		return false;
	}
	else {
		//比该节点最小的值还要小
		if (k < key[0]) {
			index = 0;
			return false;
		}
		//比该节点最大的值还要大
		else if (k > key[key_num - 1]) {
			index = key_num;
			return false;
		}
		else {
			//k介于该节点最大最小key之间，二分法找到大于等于k的最小index
			unsigned int left = 0;
			unsigned int right = key_num - 1;
			unsigned int mid;
			while (left < right) {
				mid = (left + right) / 2;
				if (key[mid] >= k) {
					right = k;
				}
				else {
					left = mid + 1;
				}
			}
			index = right;
			return (key[index] == k);
		}
	}
}

template <typename T>
bool Node<T>::find_range(unsigned int index, T& k, vector<int>& ans) {
	unsigned int i;
	for (i = index; i < key_num && key[i] <= k; i++) {
		ans.push_back(value[i]);
	}
	if (key[i] >= k) {
		return true;
	}
	return false;
}

template <typename T>
bool Node<T>::find_range(unsigned int index, vector<int>& ans) {
	unsigned int i;
	for (i = index; i < key_num; i++) {
		ans.push_back(value[i]);
	}
	return false;
}

template<typename T>
void Node<T>::print() {
	for (unsigned int i = 0; i < key_num; i++) {
		cout << key[i] << "-";
	}
}

/*-------------------BPT-------------------*/
template <typename T>
BPT<T>::BPT(string File, int Rank) :
	File(File),
	Rank(Rank),
	Node_num(1)
{
	Root = new Node<T>(Rank, true);
	MostLeftLeaf = Root;
}

template <typename T>
BPT<T>::~BPT() {
	Delete_Tree(Root);
	Root = NULL;
}

template<typename T>
void BPT<T>::Delete_Tree(Node<T>* node) {
	while (node) {
		if (!node->isleaf) {
			for (unsigned int i = 0; i <= node->key_num; i++) {
				Delete_Tree(node->pointer[i]);
				node->pointer[i] = NULL;
			}
		}
	}
	delete node;
}
/*-----Insertion: Search_Node + Insert_Key + Insert_Adjust-----*/
template <typename T>
Node<T>* BPT<T>::Search_Node(T k)
{
	Node<T>* node = Root;
	while (!node->isleaf) {
		for (unsigned int i = 0; i <= node->key_num; i++) {
			if (i < node->key_num && k == node->key[i]) {
				node = node->pointer[i + 1];
				break;
			}
			if (i == node->key_num || k < node->key[i]) {
				node = node->pointer[i];
				break;
			}
		}
	}
	return node;
}

template <typename T>
bool BPT<T>::Insert_Key(T k, int val) {
	if (!Root) {
		Root = new Node<T>(Rank, true);
		Node_num = 1;
		MostLeftLeaf = Root;
	}

	Node<T>* node = Search_Node(k);
	unsigned int index = 0;
	bool is_exist = node->search_key(k, index);
	//存在
	if (is_exist) {
		return false;
	}
	//不存在，则执行插入操作
	else {
		node->insert_key(k, val);
		if (node->key_num == Rank) {
			Insert_Adjust(node);//插入之后的节点调整
		}
		return true;
	}
}

template<typename T>
bool BPT<T>::Insert_Adjust(Node<T>* node) {
	T up_key;
	Node<T>* nnode = node->split(up_key);
	Node_num++;
	//如果要分裂的节点是根节点
	if (node->isroot()) {
		Node<T>* Root = new Node<T>(Rank, false);//新的根节点，且该节点一定不是叶节点
		this->Root = Root;
		Node_num++;//节点数增加1
		node->father = Root;
		nnode->father = Root;
		Root->insert_key(up_key);
		Root->pointer[0] = node;
		Root->pointer[1] = nnode;
		return true;
	}
	else {
		unsigned int index = node->father->insert_key(up_key);
		node->father->pointer[index + 1] = nnode;
		nnode->father = node->father;
		if (node->father->key_num == Rank) {
			Insert_Adjust(node->father);
		}
		return true;
	}
}

/*--------Deletion: Search_Node + Delete_Key + Delete_Adjust---------------*/
template<typename T>
bool BPT<T>::Delete_Key(T k) {
	if (!Root) {
		return false;
	}
	unsigned int index = 0;
	Node<T>* node = Search_Node(k);//找到k可能存在的叶节点
	bool is_exist = node->search_key(k, index);//判断k是否存在，并返回最接近k的index
	if (!is_exist) {
		cout << k << " dosen't exist. Can't delete it" << endl;
		return false;
	}
	else {
		//如果该结点同时是根节点
		if (node->isroot()) {
			node->delete_key(index);
			return Delete_Adjust(node);
		}
		
		else {
			if (index == 0 && node != MostLeftLeaf) {
				unsigned int index = 0;
				Node<T>* nfather = node->father;
				bool is_exist = nfather->search_key(k, index);
				while (!is_exist) {
					if (nfather->father) {
						nfather = nfather->father;
						is_exist = nfather->search_key(k, index);
					}
					else {
						break;
					}
				}
				nfather->key[index] = node->key[1];
				node->delete_key(index);
				return Delete_Adjust(node);
			}
			else {
				node->delete_key(index);
				return Delete_Adjust(node);
			}
		}
	}
}


template<typename T>
bool BPT<T>::Delete_Adjust(Node<T>* node) {
	/*两种情况不需要调整树的结构：
		1、为根节点且key的个数大于0
		2、为其他节点且key的个数大于min_key
	*/
	if ((node->isroot() && node->key_num > 0) || (node->key_num >= node->min_key)) {
		return true;
	}
	else {
		//根节点
		if (node->isroot()) {
			//成为空树
			if (node->isleaf) {
				delete node;
				Root = NULL;
				MostLeftLeaf = NULL;
				Node_num = 0;
			}
			else {
				Root = node->pointer[0];
				Root->father = NULL;
				delete node;
				Node_num--;
			}
		}
		//叶节点
		else if(node->isleaf){
			Node<T>* father = node->father;
			unsigned int index = 0;
			father->search_key(node->key[0], index);
			//如果该叶节点的父亲节点的第一个指针指向的不是该节点（也就是说该节点有左兄弟）；并且该节点的第一个元素在父节点中的index为key_num-1
			if (father->pointer[0] != node && index + 1 == father->key_num) {
				//左边的叶节点有多出来的key
				if (node->last_leaf->key_num > node->min_key) {
					for (unsigned int i = node->key_num; i > 0; i--) {
						node->key[i] = node->key[i - 1];
						node->value[i] = node->value[i - 1];
					}
					node->key[0] = node->last_leaf->key[node->last_leaf->key_num - 1];
					node->value[0] = node->last_leaf->value[node->last_leaf->key_num - 1];
					node->last_leaf->delete_key(node->last_leaf->key_num - 1);

					node->key_num++;
					father->key[index] = node->key[0];//干什么用的？
					return true;
				}
				//左边的叶节点没有多出来的key
				else {
					father->delete_key(index);
					for (unsigned int i = 0; i < node->key_num; i++) {
						node->last_leaf->key[i + node->last_leaf->key_num] = node->key[i];
						node->last_leaf->value[i + node->last_leaf->key_num] = node->value[i];
					}
					node->last_leaf->key_num += node->key_num;
					node->last_leaf->next_leaf = node->next_leaf;
					node->next_leaf->last_leaf = node->last_leaf;
					delete node;
					Node_num--;
					return Delete_Adjust(node->father);
				}
			}
			//右边的兄弟
			else {
				Node<T>* next_leaf = NULL;
				if (father->pointer[0] == node) {
					next_leaf = father->pointer[1];
				}
				else {
					next_leaf = father->pointer[index + 2];
				}
				//借值
				if (next_leaf->key_num > next_leaf->min_key) {
					node->key[node->key_num] = next_leaf->key[0];
					node->value[node->key_num] = next_leaf->value[0];
					node->key_num++;
					next_leaf->delete_key(0);
					if (father->pointer[0] == node) {
						father->key[0] = next_leaf->key[0];
					}
					else {
						father->key[index + 1] = next_leaf->key[0];
					}
					return true;
				}
				//合并节点到原节点
				else {
					for (unsigned int i = 0; i < next_leaf->key_num; i++) {
						node->key[node->key_num + i] = next_leaf->key[i];
						node->value[node->key_num + i] = next_leaf->value[i];
					}
					if (node == father->pointer[0]) {
						father->delete_key(0);
					}
					else {
						father->delete_key[index + 1];
					}
					node->key_num += next_leaf->key_num;
					node->next_leaf = next_leaf->last_leaf;
					next_leaf->next_leaf->last_leaf = node;
					delete next_leaf;//删除右节点
					Node_num--;
					return Delete_Adjust(father);
				}
			}
		}
		//枝干节点
		else {
			unsigned int index = 0;
			Node<T>* father = node->father;
			Node<T>* brother;//兄弟节点
			father->search_key(node->pointer[0]->key[0], index);
			//左节点
			if ((father->pointer[0] != node) && (index + 1 == father->key_num)) {
				brother = father->pointer[index];
				//借节点
				if (brother->key_num > brother->min_key) {
					node->pointer[node->key_num + 1] = node->pointer[node->key_num];
					for (unsigned int i = node->key_num; i > 0; i--) {
						node->pointer[i] = node->pointer[i - 1];
						node->key[i] = node->key[i - 1];
					}
					node->pointer[0] = brother->pointer[brother->key_num];
					node->key[0] = parent->keys[index];
					node->key_num++;
					father->key[index] = brother->key[brother->key_num - 1];
					if (brother->pointer[brother->key_num]) {
						brother->pointer[brother->key_num]->father = node;
					}
					brother->delete_key(brother->key_num - 1);
					return true;
				}
				//合并
				else {
					brother->key[brother->key_num] = parent->keys[index];
					father->delete_key(index);
					brother->key_num++;
					for (int i = 0; i < node->key_num; i++) {
						brother->pointer[brother->key_num + i] = node->pointer[i];
						brother->key[brother->key_num + i] = node->key[i];
						brother->pointer[brother->key_num + i]->father = brother;
					}
					brother->pointer[brother->key_num + pNode->num] = node->pointer[node->key_num];
					brother->pointer[brother->key_num + pNode->num]->father = brother;
					brother->key_num += node->key_num;
					delete node;
					Node_num--;
					return Delete_Adjust(father);
				}
			}
			//右节点
			else {
				if (father->pointer[0] == node) {
					brother = father->pointer[1];
				}
				else {
					brother = father->pointer[index + 2];
				}
				//借点
				if (brother->key_num > brother->min_key) {
					node->pointer[node->key_num + 1] = brother->pointer[0];
					node->key[node->key_num] = brother->key[0];
					node->pointer[node->key_num + 1]->father = node;
					node->key_num++;
					if (node == father->pointer[0]) {
						father->key[0] = brother->key[0];
					}
					else {
						father->key[index + 1] = brother->key[0];
					}
					brother->pointer[0] = brother->pointer[1];
					brother->delete_key(0);
					return true;
				}
				//合并
				else {
					node->key[pNode->num] = father->key[index];
					if (node == father->pointer[0]) {
						father->delete_key(0);
					}
					else {
						father->delete_key(index + 1);
					}
					node->key_num++;
					for (int i = 0; i < brother->key_num; i++) {
						node->pointer[node->key_num + i] = brother->pointer[i];
						node->key[node->key_num + i] = brother->key[i];
						node->pointer[node->key_num + i]->father = node;
					}
					node->pointer[node->key_num + brother->key_num] = brother->pointer[brother->key_num];
					node->pointer[node->key_num + brother->key_num]->father = node;
					node->key_num += brother->key_num;
					delete brother;
					Node_num--;
					return Delete_Adjust(father);
				}

			}

		}
	}
}


template<typename T>
bool BPT<T>::Search_Key(T k) {

}

template<typename T>
void BPT<T>::Print() {
	Node<T>* node = MostLeftLeaf;
	while (node != NULL) {
		node->print();
		node = node->next_leaf;
	}
}

