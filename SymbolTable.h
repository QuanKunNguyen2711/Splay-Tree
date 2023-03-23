#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Object;
template<class T> class Node;
template<class T> class LinkedList;
template<class T> class St;
template<class T> class Qeue;

class Object {
private:
    string identifier;
    int level;
    string type;
    bool is_func;
    LinkedList<string>* params;
public:
    Object() {
        identifier = "";
        level = -1;
        type = "";
        params = NULL;
        is_func = false;
    }
    Object(string identifier, int level, string type, bool is_func, LinkedList<string>* params) {
        this->identifier = identifier;
        this->level = level;
        this->type = type;
        this->params = params;
        this->is_func = is_func;
    }
    Object(string identifier, int level) {
        this->identifier = identifier;
        this->level = level;
        this->type = "";
        this->params = NULL;
        this->is_func = false;
    }
    friend bool operator< (const Object&, const Object&);
    friend bool operator> (const Object&, const Object&);
    friend bool operator== (const Object&, const Object&);

    friend class Node<Object>;
    friend class SymbolTable;
    friend class LinkedList<string>;
};

template<class T>
class Node {
private:
    T data;
    Node<T>* parent;
    Node<T>* left;
    Node<T>* right;
public:
    Node<T>() {
        parent = NULL;
        left = NULL;
        right = NULL;
    }
    Node<T>(T data) {
        this->data = data;
        parent = NULL;
        left = NULL;
        right = NULL;
    }
    friend class SymbolTable;
    friend class LinkedList<T>;
    friend class St<T>;
    friend class Qeue<T>;
};

template<class T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int count;
public:
    LinkedList<T>() {
        head = NULL;
        tail = NULL;
        count = 0;
    }
    void push_back(Node<T>*);
    void clear();
    friend class SymbolTable;
};

template<class T>
class St {
private:
    Node<T>* top;
    int count;
public:
    St<T>() {
        count = 0;
        top = nullptr;
    }
    ~St<T>() {
        while (top != NULL) {
            Node<T>* temp = top;
            top = top->right;
            delete temp;
        }
    }
    void push(T);
    T get_top();
    int pop();
    bool isEmpty();
};

template<class T>
class Qeue {
private:
    Node<T>* front;
    Node<T>* tail;
    int count;
public:
    Qeue<T>() {
        count = 0;
        front = nullptr;
        tail = nullptr;
    }
    ~Qeue<T>() {
        while (front != NULL) {
            Node<T>* temp = front;
            front = front->right;
            delete temp;
        }
    }
    void enqeue(T);
    void deqeue();
    Node<T>* get_front();
    void st_tail(Node<T>*);
    void st_front(Node<T>*);
};

class SymbolTable
{
    Node<Object>* root;
public:
    SymbolTable() {
        root = NULL;
    }
    ~SymbolTable() {
        deleteSplayTree(this->root);
    }
    void run(string filename);
    void deleteSplayTree(Node<Object>*);
    void right_rotate(Node<Object>*);
    void left_rotate(Node<Object>*);
    void splay(Node<Object>*, int&);
    void insert(Node<Object>*, int&, int&, string);
    void check_assign(string, string, int, string, int&, int&);
    Node<Object>* search(Node<Object>*, Node<Object>*, int&, int&);
    Node<Object>* look_up(string, int, int&, int&);
    Node<Object>* max_left(Node<Object>*);
    void delete_node(Node<Object>*, int&);
    void print(Node<Object>*, string&);
};
#endif