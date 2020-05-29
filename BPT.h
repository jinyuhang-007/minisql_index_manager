#ifndef _BPT_H
#define _BPT_H
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

/*----------Node----------*/
template<typename T>
class Node {
public:
	friend class BPT;//声明友元类
	Node(int rank, bool isleaf);	//finished
	~Node();						//finished
	bool isroot();					//finished
	Node<T>* split(T& up_key);		//finished
	unsigned int insert_key(T k);	//finished
	unsigned int insert_key(T k, int val);//finished
	bool delete_key(unsigned int index);//finished
	bool search_key(T k, unsigned int &index);//finished
	bool find_range(unsigned int index, T& k, vector<int>& ans);//finished
	bool find_range(unsigned int index, vector<int>& ans);//finished
public:
	void print();//debug
private:
	unsigned int rank;//阶
	bool isleaf;
	unsigned int key_num;//key的数量
	vector<T> key;
	vector<Node<T>*> pointer;
	vector<int> value;
	Node<T>* father;//父亲节点
	Node<T>* last_leaf;//上一个叶节点
	Node<T>* next_leaf;//下一个叶节点
private:
	unsigned int min_key;//节点中最少的key的数量
	unsigned int min_pointer;//节点中最少的pointer的数量
};

/*-------------BPT------------*/
template<typename T>
class BPT {
public:
	BPT(string File, int Rank);
	~BPT();
	void Delete_Tree(Node<T>* node);//被析构函数调用
	Node<T>* Search_Node(T k);//便于插入操作
	bool Insert_Adjust(Node<T>* node);//便于插入操作
	bool Delete_Adjust(Node<T>* node);
public:
	bool Insert_Key(T k, int val);//插入
	bool Delete_Key(T k);//删除
	bool Search_Key(T k);//查询
	void Print();//debug
private:
	string File;
	int Rank;
	int Node_num;
	Node<T>* Root;
	Node<T>* MostLeftLeaf;
};
#endif
