#include "BPT.h"
/*构造函数*/
template<class T>
Node<T>::Node(int rank, node_type type):
	rank(rank), type(type), key_count(0), key_cap(rank-1), last(NULL), next(NULL), father(NULL)
{
	keys.resize(rank - 1);
	children.resize(rank);
}
/*析构函数*/
template<class T>
Node<T>::~Node() {

}

/*向中间节点中增加key*/
template<class T>
bool Node<T>::add_key_branch(T& key) {
	/*已经存在这样的节点*/
	if (search_key(key) >= 0) {
		return ture;
	}
	/*节点还没有满*/
	else if (key_count < rank - 1) {
		int i, j;
		for (i = key_count; i >= 1 && keys[i - 1] > key; i--) {
			keys[i] = keys[i - 1];
		}
		keys[i] = key;
		for (j = key_count + 1; j > i + 1; j--) {
			children[j] = children[j - 1];
		}
		children[i + 1] = NULL;//新插入的节点的右边的指针设置为NULL
		key_count++;
		return ture;
	}
	/*刚好满了，节点需要分裂成两个节点*/
	else if(key_count == rank - 1){
		return false;
	}
}

/*向叶子节点中插入数据*/
template<class T>
bool Node<T>::add_key_leaf(T& key, int val) {
	/*已经存在这样的节点*/
	if (search_key(key) >= 0) {
		return ture;
	}
	/*节点还没有满*/
	else if (key_count < rank - 1) {
		int i, j;
		for (i = key_count; i >= 1 && keys[i - 1] > key; i--) {
			keys[i] = keys[i - 1];
			vals[i] = vals[i - 1];
		}
		key_count++;
		keys[i] = key;
		vals[i] = val;
		return ture;
	}
	/*刚好满了，节点需要分裂成两个节点*/
	else if (key_count == rank - 1) {
		return false;
	}
}


/*删除节点中指定index的key*/
template<class T>
bool Node<T>::delete_key(int index) {
	if (index >= key_count || index < 0) {
		return false;
	}
	else {
		if (type == leaf) {
			for (int i = index; i < key_count - 1; i++) {
				keys[i] = keys[i + 1];
				vals[i] = vals[i + 1];
			}
		}
		else {
			for (int i = index; i < key_count - 1; i++) {
				keys[i] = keys[i + 1];
			}
			/*保留节点左边的指针，删除右边的指针*/
			for (int i = index + 1; i < key_count; i++) {
				children[i] = children[i + 1];
			}
			children[key_count - 1] = NULL;//所有指针向左移动之后最后一个空位的指针设为NULL
		}
		key_count--;
		return true;
	}
}

/*	
	在节点中搜索key
	如果无，返回-1；有，返回下标
*/
template<class T>
int Node<T>::search_key(T key) {
	/*该节点为空*/
	if (key_count == 0) {
		return -1;
	}
	/*该节点的最小元素大于key或最大元素小于key*/
	else if (key < keys[0] || key > keys[key_count - 1]) {
		return -1;
	}
	/*二分法查找*/
	else {
		int l = 0, r = key_count - 1, m = 0;
		m = (l + r) / 2;
		while (l < r && keys[m] != key){
			if (keys[m] < key) {
				l = m + 1;
			}
			else if (keys[mid] > key) {
				r = m;
			}
			m = (l + r) / 2;
		}
		if (keys[m] == key) {
			return m;
		}
		return -1;
	}
}

/*---------------------分割线---------------------*/
/*构造函数*/
template<class T>
BPT<T>::BPT(int n):rank(n) {
	root = NULL;
}
/*析构函数*/
template<class T>
BPT<T>::~BPT() {

}
/*插入*/
template<class T>
bool BPT<T>::insert_key(T key) {

}
/*删除*/
template<class T>
bool BPT<T>::delete_key(T key) {

}
/*查询*/
template<class T>
pair<Node<T>*, int> BPT<T>::find(T key) {

}
