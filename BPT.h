#ifndef _BPT_H
#define _BPT_H
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
#include<queue>
using namespace std;
enum node_type {root, branch, leaf, any};//节点类型的声明

template<typename T>
class Node {
public:
	Node(int rank, node_type type);//———— √
	~Node();//———— √
	bool add_key_branch(T& key);//———— √
	bool add_key_leaf(T& key, int val);//———— √
	bool delete_key(int index);//———— √
	int search_key(T key);//返回key的index，如果没有，则返回-1

private:
	int rank;//阶, 表示孩子的最大数量
	int key_cap;//rank-1,表示key的最大数量
	int key_count;//节点当中key的数量
	vector<T> keys;//存放key
	vector<Node*> children;//指向孩子的指针
	vector<int> vals;//values
	Node* last;//上一个叶节点
	Node* next;//叶节点中指向下一个叶节点
	Node* father;//上一层的节点
	node_type type;//节点的类型
};


/*-------------分割线---------------*/
template<typename T>
class BPT {
public:
	BPT(int n);
	~BPT();
	bool insert_key(T key);//插入
	bool delete_key(T key);//删除
	pair<Node*, T> find(T key);//查询
	bool drop_tree();
private:
	int rank;//阶
	int level;//树的层数
	Node* root;//根节点
};

#endif
