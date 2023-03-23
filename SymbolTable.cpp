#include "SymbolTable.h"

inline bool operator< (const Object& lhs, const Object& rhs) {
    if (lhs.level < rhs.level) {
        return true;
    }
    else if (lhs.level == rhs.level) {
        if (lhs.identifier.compare(rhs.identifier) < 0) {
            return true;
        }
    }
    return false;
}

inline bool operator> (const Object& lhs, const Object& rhs) {
    if (lhs.level > rhs.level) {
        return true;
    }
    else if (lhs.level == rhs.level) {
        if (lhs.identifier.compare(rhs.identifier) > 0) {
            return true;
        }
    }
    return false;
}

inline bool operator== (const Object& lhs, const Object& rhs) {
    if (lhs.level == rhs.level && lhs.identifier == rhs.identifier) return true;
    return false;
}

inline void SymbolTable::left_rotate(Node<Object>* n) {
    Node<Object>* temp = n->right;
    n->right = temp->left;
    if (temp->left != NULL) {
        temp->left->parent = n; // liên kết lại parent của temp->left là node n
    }
    temp->parent = n->parent; // đưa node lên root, liên kết với parent của node n ban đầu
    if (n->parent == NULL) {
        this->root = temp; // nếu n là root, root bây h là temp
    }
    else if (n == n->parent->left) { // n là con của node parent
        n->parent->left = temp; // node con của parent bây h là temp
    }
    else {
        n->parent->right = temp;
    }
    temp->left = n; 
    n->parent = temp; // liên kết lại parent của n bây h là temp
}

inline void SymbolTable::right_rotate(Node<Object>* n) {
    Node<Object>* temp = n->left;
    n->left = temp->right;
    if (temp->right != NULL) {
        temp->right->parent = n;
    }
    temp->parent = n->parent;
    if (n->parent == NULL) {
        this->root = temp;
    }
    else if (n == n->parent->right) {
        n->parent->right = temp;
    }
    else {
        n->parent->left = temp;
    }
    temp->right = n;
    n->parent = temp;
}

inline void SymbolTable::splay(Node<Object>* n, int& num_splay) {
    if (n->parent == NULL) return;
    while (n->parent != NULL) {
        if (n->parent == this->root) {
            if (n == n->parent->left) {
                right_rotate(n->parent);
            }
            else {
                left_rotate(n->parent);
            }
        }
        else {
            Node<Object>* p = n->parent; // node parent
            Node<Object>* g = p->parent; // node grandparent

            if (n->parent->left == n && p->parent->left == p) { // zig-zig
                right_rotate(g);
                right_rotate(p);
            }
            else if (n->parent->right == n && p->parent->right == p) { // zag-zag
                left_rotate(g);
                left_rotate(p);
            }
            else if (n->parent->right == n && p->parent->left == p) { // zag-zig
                left_rotate(p);
                right_rotate(g);
            }
            else if (n->parent->left == n && p->parent->right == p) { // zig-zag
                right_rotate(p);
                left_rotate(g);
            }
        }
    }
    num_splay++;
}

inline void SymbolTable::insert(Node<Object>* n, int& num_compare, int& num_splay, string instruction) {
    Node<Object>* prev = NULL;
    Node<Object>* temp = this->root;
    while (temp != NULL) {
        prev = temp;
        if (n->data < temp->data) {
            temp = temp->left;
            num_compare++;
        }
        else if (n->data > temp->data) {
            temp = temp->right;
            num_compare++;
        }
        else {
            throw Redeclared(instruction);
        }
    }
    n->parent = prev;

    if (prev == NULL) {
        this->root = n;
    } 
    else if (n->data < prev->data) {
        prev->left = n;
    }
    else if (n->data > prev->data) {
        prev->right = n;
    }
    else {
        throw Redeclared(instruction);
    }
    splay(n, num_splay);
}

inline void SymbolTable::deleteSplayTree(Node<Object>* t) {
    if (t == NULL) return;
    if (t->data.params != NULL) {
        t->data.params->clear();
        delete t->data.params;
        t->data.params = nullptr;
    }
    deleteSplayTree(t->left);
    deleteSplayTree(t->right);
    delete t;
    t = nullptr;
}

template<class T> inline void LinkedList<T>::push_back(Node<T>* n) {
    if (head == NULL) {
        head = n;
        tail = head;
    }
    else { // add at the end
        tail->right = n;
        tail = n;
    }
    count++;
}

template<class T> inline void LinkedList<T>::clear() {
    while (head != NULL) {
        Node<T>* temp = head;
        head = head->right;
        delete temp;
        count--;
    }
    tail = NULL;
}

inline bool check_number(string str) {
    int len = str.length();
    for (int i = 0; i <= len / 2; i++) {
        if (str[i] < 48 || str[i] > 57 || str[len - i - 1] < 48 || str[len - i - 1] > 57)
        {
            return false;
        }
    }
    return true;
}

inline bool check_string(string str) {
    unsigned int len = str.length();
    if (str[0] == 39 && str[len - 1] == 39) {
        for (unsigned int j = 1; j <= (len - 1) / 2; j++) {
            if (((str[j] >= 97 && str[j] <= 122) ||
                (str[j] >= 65 && str[j] <= 90) ||
                (str[j] >= 48 && str[j] <= 57) ||
                 str[j] == 32
                ) &&
                ((str[len - j - 1] >= 97 && str[len - j - 1] <= 122) ||
                 (str[len - j - 1] >= 65 && str[len - j - 1] <= 90) ||
                 (str[len - j - 1] >= 48 && str[len - j - 1] <= 57) ||
                  str[len - j - 1] == 32
                ))
            {
                continue;
            }
            else {
                return false;
            }
        }
    }
    else {
        return false;
    }
    return true;
}

inline Node<Object>* SymbolTable::search(Node<Object>* root, Node<Object>* n, int& num_compare, int& num_splay) {
    if (root == NULL) return NULL;

    if (root->data == n->data) {
        num_compare++;
        splay(root, num_splay);
        return root;
    }
    else if (n->data < root->data) {
        num_compare++;
        return search(root->left, n, num_compare, num_splay);
    }
    else if (n->data > root->data) {
        num_compare++;
        return search(root->right, n, num_compare, num_splay);
    }
    else {
        return NULL;
    }
}

inline void SymbolTable::check_assign(string identifier, string value, int scope, string instruction, int& num_compare, int& num_splay) {
    regex format("[a-z]+[\\w]*");
    if (value == "number" || value == "string") {
        Node<Object>* n = look_up(identifier, scope, num_compare, num_splay);
        if (n == NULL) {
            throw Undeclared(instruction);
        }
        else if (this->root->data.type != value || this->root->data.is_func) { // cùng type và identifier ko phải là hàm
            throw TypeMismatch(instruction);
        }
    }
    else { // nếu là gán biến
        Node<Object>* val = look_up(value, scope, num_compare, num_splay); // splay biến
        if (val == NULL) {
            throw Undeclared(instruction);
        }
        Node<Object>* iden = look_up(identifier, scope, num_compare, num_splay); // tìm identifier
        if (iden == NULL) {
            throw Undeclared(instruction);
        }
        else if (this->root->data.is_func || val->data.is_func || this->root->data.type != val->data.type) { // cùng type và cả 2 ko phải hàm
            throw TypeMismatch(instruction);
        }
    }
    cout << num_compare << " " << num_splay << endl;
}

inline Node<Object>* SymbolTable::look_up(string identifier, int scope, int& num_compare, int& num_splay) {
    if (scope < 0) return NULL;
    Object obj = Object(identifier, scope);
    Node<Object> n = Node<Object>(obj);
    int temp_compare = num_compare;
    int temp_splay = num_splay;
    Node<Object>* find_n = search(this->root, &n, num_compare, num_splay);
    if (find_n == NULL && scope >= 0) {
        scope--;
        num_compare = temp_compare;
        num_splay = temp_splay;
        return look_up(identifier, scope, num_compare, num_splay);
    }
    return find_n;
}

inline Node<Object>* SymbolTable::max_left(Node<Object>* subtree) {
    Node<Object>* p = subtree;
    while (p->right != NULL) {
        p = p->right;
    }
    return p;
}

inline void SymbolTable::delete_node(Node<Object>* n, int& num_splay) {
    Node<Object>* left_subtree = this->root->left;
    if (left_subtree != NULL) {
        left_subtree->parent = NULL;
    }
    Node<Object>* right_subtree = this->root->right;
    if (right_subtree != NULL) {
        right_subtree->parent = NULL;
    }
    delete n;
    if (left_subtree != NULL && right_subtree != NULL) {
        Node<Object>* m = max_left(left_subtree);
        splay(m, num_splay);
        m->right = right_subtree;
        right_subtree->parent = m;
        this->root = m;
    }
    else if (left_subtree != NULL && right_subtree == NULL) {
        this->root = left_subtree;
    }
    else {
        this->root = right_subtree;
    }
}

inline void SymbolTable::print(Node<Object>* n, string& res) {
    if (n == NULL) return;
    char s = n->data.level + '0';
    string temp(1, s);
    res.append(n->data.identifier + "//" + temp + " ");
    print(n->left, res);
    print(n->right, res);
}

template<class T> void St<T>::push(T var) {
    Node<T>* temp = new Node<T>(var);
    temp->right = this->top;
    this->top = temp;
    this->count++;
}

template<class T> int St<T>::pop() {
    if (this->count == 0) return 0;
    Node<T>* temp = this->top;
    this->top = temp->right;
    this->count--;
    delete temp;
    return 1;
}

template<class T> T St<T>::get_top() {
    if (this->count == 0) return -1;
    return this->top->data;
}

template<class T> bool St<T>::isEmpty() {
    if (this->count == 0) return true;
    return false;
}

template<class T> void Qeue<T>::enqeue(T n) {
    Node<T>* newNode = new Node<T>(n);
    if (this->count == 0) {
        this->front = newNode;
    }
    else {
        this->tail->right = newNode;
    }
    this->tail = newNode;
    this->count++;
}

template<class T> void Qeue<T>::deqeue() {
    if (this->count == 0) return;
    Node<Object>* temp = this->front;
    if (this->count == 1) {
        this->tail = NULL;
    }
    this->front = this->front->right;
    this->count--;
    delete temp;
}

template<class T> Node<T>* Qeue<T>::get_front() {
    if (this->count == 0) return NULL;
    return this->front;
}

template<class T> void Qeue<T>::st_tail(Node<T>* n) {
    this->tail = n;
}

template<class T> void Qeue<T>::st_front(Node<T>* n) {
    this->front = n;
}

inline void SymbolTable::run(string filename)
{
    ifstream fs;
    fs.open(filename);
    char buf[100];
    int scope = 0;
    St<int> scope_st = St<int>();
    Qeue<Object> variable_qeue = Qeue<Object>();
    while (!fs.eof()) {
        fs.getline(buf, 100);
        string instruction = buf;
        string str = instruction;
        int pos = -1;
        if (str == "BEGIN") {
            scope++;
            scope_st.push(scope);
        }
        else if (str == "END") {
            if (scope_st.isEmpty()) {
                throw UnknownBlock();
            }
            else {
                int current_scope = scope_st.get_top();
                Node<Object>* p = variable_qeue.get_front();
                Node<Object>* p_pre = NULL;
                int num_compare = 0;
                int num_splay = 0;
                while (p != NULL && p->data.level != current_scope) {
                    p_pre = p;
                    p = p->right;
                }
                while (p != NULL && p->data.level == current_scope) {
                    Node<Object> n = Node<Object>(Object(p->data.identifier, current_scope));
                    search(this->root, &n, num_compare, num_splay);
                    delete_node(this->root, num_splay);
                    Node<Object>* temp = p;
                    p = p->right;
                    delete temp;
                }
                if (p_pre != NULL) {
                    p_pre->right = NULL;
                    variable_qeue.st_tail(p_pre);
                }
                else {
                    variable_qeue.st_front(NULL);
                }
                scope_st.pop();
                scope--;
            }
        }
        else if (str == "PRINT") {
            string res = "";
            print(this->root, res);
            if (!res.empty()) {
                res.pop_back();
                cout << res << endl;
            }
        }
        else if ((pos = str.find(' ')) != -1) {
            string ACTION = str.substr(0, pos);
            if (ACTION == "INSERT") {
                str.erase(0, pos + 1);
                string identifier = str.find(' ') != string::npos ? str.substr(0, str.find(' ')) : "";
                regex format("[a-z]+[\\w]*");
                if (!identifier.empty() && regex_match(identifier, format)) {
                    str.erase(0, str.find(' ') + 1);
                    string type = str.find(' ') != string::npos ? str.substr(0, str.find(' ')) : "";
                    if (!type.empty()) {
                        str.erase(0, str.find(' ') + 1);
                        string isStatic = str;
                        if (!isStatic.empty() && str.find(' ') == string::npos) {
                            if (isStatic != "true" && isStatic != "false") {
                                throw InvalidInstruction(instruction);
                            }
                            int num_compare = 0;
                            int num_splay = 0;
                            // Xử lý type trc khi insert
                            if (type == "number" || type == "string") {
                                int level = isStatic == "true" ? 0 : scope;
                                Object newObj = Object(identifier, level, type, false, NULL);
                                Node<Object>* newNode = new Node<Object>(newObj);
                                insert(newNode, num_compare, num_splay, instruction);
                                if (level > 0) {
                                    variable_qeue.enqeue(newObj);
                                }
                                cout << num_compare << " " << num_splay << endl;
                            }
                            else if (type[0] == '(' && (pos = type.find(')')) != -1 && type[pos + 1] == '-' && type[pos + 2] == '>') {
                                int level = isStatic == "true" ? 0 : scope;
                                if (level != 0) { // Hàm chỉ được khai báo ở toàn cục
                                    throw InvalidDeclaration(instruction);
                                }
                                if (pos == 1) { // Hàm rỗng
                                    if (type.substr(4) == "number" || type.substr(4) == "string") {
                                        type = type.substr(4);
                                        Object newObj = Object(identifier, 0, type, true, NULL);
                                        Node<Object>* newNode = new Node<Object>(newObj);
                                        insert(newNode, num_compare, num_splay, instruction);
                                        cout << num_compare << " " << num_splay << endl;
                                    }
                                    else {
                                        throw InvalidInstruction(instruction);
                                    }
                                }
                                else { // Xử lý params
                                    int comma = -1;
                                    int start = 1;
                                    if (type.find(',', 1) == string::npos && (pos - start) > 6) { // no separator ','
                                        throw InvalidInstruction(instruction);
                                    }
                                    LinkedList<string>* l = new LinkedList<string>();
                                    while (pos - start > 0) {
                                        comma = type.find(',', start);
                                        string sub = "";
                                        if (comma != -1) {
                                            sub = type.substr(start, comma - start);
                                        }
                                        else {
                                            sub = type.substr(start, pos - start);
                                        }
                                        if (sub == "number" || sub == "string") {
                                            Node<string>* newNode = new Node<string>(sub);
                                            l->push_back(newNode);
                                        }
                                        else {
                                            l->clear();
                                            delete l;
                                            throw InvalidInstruction(instruction);
                                        }
                                        if (comma != -1) {
                                            start = comma + 1;
                                        }
                                        else {
                                            start = pos;
                                        }
                                    }
                                    type = type.substr(pos + 3);
                                    if (type != "number" && type != "string") {
                                        throw InvalidInstruction(instruction);
                                    }
                                    Object newObj = Object(identifier, 0, type, true ,l);
                                    Node<Object>* newNode = new Node<Object>(newObj);
                                    insert(newNode, num_compare, num_splay, instruction);
                                    cout << num_compare << " " << num_splay << endl;
                                }
                            }
                            else {
                                throw InvalidInstruction(instruction);
                            }
                        }
                        else {
                            throw InvalidInstruction(instruction);
                        }
                    }
                    else {
                        throw InvalidInstruction(instruction);
                    }
                }
                else {
                    throw InvalidInstruction(instruction);
                }
            }
            else if (ACTION == "ASSIGN") {
                str = str.erase(0, pos + 1);
                string identifier = str.find(' ') != string::npos ? str.substr(0, str.find(' ')) : "";
                // Kiểm tra value trc
                if (!identifier.empty()) {
                    str = str.erase(0, str.find(' ') + 1);
                    int num_splay = 0;
                    int num_compare = 0;
                    regex format("[a-z]+[\\w]*");
                    if (str.find('(') == string::npos && str.find(')') == string::npos) { // Nếu ko có ()
                        if (regex_match(str, format)) {
                            check_assign(identifier, str, scope, instruction, num_compare, num_splay);
                        }
                        else if (check_string(str)) {
                            check_assign(identifier, "string", scope, instruction, num_compare, num_splay);
                        }
                        else if (check_number(str)) {
                            check_assign(identifier, "number", scope, instruction, num_compare, num_splay);
                        }
                        else {
                            throw InvalidInstruction(instruction);
                        }
                    } else if (str.find('(') != string::npos && str.find(')') != string::npos) {
                        int start = str.find('(');
                        int end = str.find(')');
                        if (str.find('(', start+1) != string::npos || end != (int)str.length()-1) {
                            throw InvalidInstruction(instruction);
                        }
                        string func_name = str.substr(0, start);
                        str.erase(0, start);
                        Node<Object>* func = look_up(func_name, scope, num_compare, num_splay); // tìm danh hiệu hàm
                        if (func == NULL) {
                            throw Undeclared(instruction);
                        }
                        else if (!func->data.is_func) { // nếu danh hiệu được gọi ko phải hàm
                            throw TypeMismatch(instruction);
                        }
                        else if (str == "()") { // hàm rỗng
                            if (func->data.params == NULL) {
                                if (!regex_match(identifier, format)) {
                                    throw InvalidInstruction(instruction);
                                }
                                Node<Object>* iden = look_up(identifier, scope, num_compare, num_splay);
                                if (iden == NULL) {
                                    throw Undeclared(instruction);
                                }
                                else if (this->root->data.is_func || this->root->data.type != func->data.type) {
                                    throw TypeMismatch(instruction);
                                }
                                cout << num_compare << " " << num_splay << endl;
                            }
                            else {
                                throw TypeMismatch(instruction);
                            }
                        }
                        else { // hàm ko rỗng tham số
                            start = 1;
                            end = str.length() - 1;
                            int separator = -1;
                            Node<string>* temp = this->root->data.params->head;
                            while (end - start > 0) {
                                separator = str.find(',', start);
                                string param = "";
                                if (separator != -1) {
                                    param = str.substr(start, separator - start);
                                }
                                else {
                                    param = str.substr(start, end - start);
                                }
                                if (regex_match(param, format)) {
                                    Node<Object>* n = look_up(param, scope, num_compare, num_splay);
                                    if (n == NULL) {
                                        throw Undeclared(instruction);
                                    }
                                    else if (this->root->data.type != temp->data || this->root->data.is_func) {
                                        throw TypeMismatch(instruction);
                                    }
                                }
                                else {
                                    if (temp->data == "number") {
                                        if (!check_number(param)) {
                                            if (!check_string(param)) {
                                                throw InvalidInstruction(instruction);
                                            }
                                            throw TypeMismatch(instruction);
                                        }
                                    }
                                    else {
                                        if (!check_string(param)) {
                                            if (!check_number(param)) {
                                                throw InvalidInstruction(instruction);
                                            }
                                            throw TypeMismatch(instruction);
                                        }
                                    }
                                }
                                if (separator != -1) {
                                    start = separator + 1;
                                }
                                else {
                                    start = end;
                                }
                                temp = temp->right;
                            }
                            if (temp != NULL) {
                                throw TypeMismatch(instruction);
                            }
                            else if (!regex_match(identifier, format)) {
                                throw InvalidInstruction(instruction);
                            }
                            Node<Object>* iden = look_up(identifier, scope, num_compare, num_splay); // root bây h là identifier
                            if (iden == NULL) {
                                throw Undeclared(instruction);
                            }
                            else if (this->root->data.is_func || this->root->data.type != func->data.type) {
                                throw TypeMismatch(instruction);
                            }
                            cout << num_compare << " " << num_splay << endl;
                        }
                    }
                    else {
                        throw InvalidInstruction(instruction);
                    }
                }
                else {
                    throw InvalidInstruction(instruction);
                }
            }
            else if (ACTION == "LOOKUP") {
                string identifier = str.erase(0, pos + 1);
                regex format("[a-z]+[\\w]*");
                if (!regex_match(identifier, format)) {
                    throw InvalidInstruction(instruction);
                }
                int num_compare = 0;
                int num_splay = 0;
                Node<Object>* n = look_up(identifier, scope, num_compare, num_splay);
                if (n == NULL) {
                    throw Undeclared(instruction);
                }
                cout << n->data.level << endl;
            }
            else {
                throw InvalidInstruction(instruction);
            }
        }
        else {
            throw InvalidInstruction(instruction);
        }
    }
    if (!scope_st.isEmpty()) {
        throw UnclosedBlock(scope_st.get_top());
    }
}