#include <iostream>
#include <vector>

template <typename T>
class Tree {
private:
    struct Node {
        Node(T value, bool color,  Node* parent,
             Node* left,
             Node* right);
        Node* left;
        Node* right;
        Node* parent;
        bool color;
        T data;
    };
    const bool BLACK = 1;
    const bool RED = 0;
    Node *Nil;
    static bool DefaultCmp(const T &a, const T &b);
    bool Eq(const T &a, const T &b);
    Node* root;
    bool (*cmp) (const T& a, const T& b);
    int size_tree;
    size_t size();
    void Destroy(Node* cur);
    void FixInsert(Node* cur);
    void RightRotate(Node*);
    void LeftRotate(Node*);
    void FixDelete(Node* cur, Node* parent);
    void fixRelation(Node *del_node, Node *son);
    Node* Find(T val);
public:
    explicit Tree(bool (*f) (const T&, const T&) = DefaultCmp);
    ~Tree();
    void insert(T elem);
    void erase(T elem);
    T next(T val);
    T GetMin();
};

template<typename T>
Tree<T>::Node::Node(T value, bool color, Tree::Node *parent, Tree::Node *left,Tree::Node *right)
        :data(value)
        ,color(color)
        ,parent(parent)
        ,left(left)
        ,right(right) {}

template<typename T>
size_t Tree<T>::size() {
    return this->size_tree;
}

template<typename T>
void Tree<T>::insert(T elem) {
    if (root == Nil) {
        root = new Node(elem, BLACK, Nil, Nil, Nil);
        return;
    }
    if (Find(elem) != Nil)
        return;

    Node* cur = root;
    while (cur != Nil) {
        if (cmp(elem, cur->data)) {
            if (cur->left != Nil) {
                cur = cur->left;
            }
            else {
                cur = cur->left = new Node(elem, RED, cur, Nil, Nil);
                break;
            }
        }
        else {
            if (cur->right != Nil) {
                cur = cur->right;
            }
            else {
                cur = cur->right = new Node(elem, RED, cur, Nil, Nil);
                break;
            }
        }
    }
    FixInsert(cur);
}

template <typename T>
void Tree<T>::fixRelation(Node* del_node, Node* son) {
    if (del_node == root) {
        root = son;
        son->parent = Nil;
        return;
    }
    if (del_node->parent->left == del_node) {
        del_node->parent->left = son;
    } else {
        del_node->parent->right = son;
    }
    if (son != Nil) {
        son->parent = del_node->parent;
    }
}

template <typename T>
void Tree<T>::erase(T val) {
    Node* elem = Find(val);
    Node* del = elem->right;
    Node* child = Nil;
    if (elem->right == Nil || elem->left == Nil) {
        del = elem;
    } else {
        del = elem->right;
        while (del->left != Nil)
            del = del->left;
        elem->data = del->data;
    }
    child = (del->left == Nil) ? del->right : del->left;
    fixRelation(del, child);

    if (del->color == BLACK)
        FixDelete(child, del);
    delete del;
}

template <typename T>
bool Tree<T>::DefaultCmp(const T &a, const T &b) {
    return a < b;
}

template<typename T>
Tree<T>::Tree(bool (*f)(const T &, const T&))
        :size_tree(0)
        ,cmp(f) {
    Nil = nullptr;
    Nil = new Node(T(), BLACK, nullptr, nullptr, nullptr);
    Nil->parent = Nil;
    Nil->left = Nil;
    Nil->right = Nil;
    root = Nil;
}

template<typename T>
void Tree<T>::FixInsert(Tree::Node* cur) {
    if (cur == root) {
        cur->color = BLACK;
        return;
    }
    Node* par = cur->parent;
    while (cur != root && par->color == RED) {
        if (par->parent->left == par) {
            if (par->parent->right == Nil) {
                if (cur->parent->right == cur) {
                    cur = cur->parent;
                    LeftRotate(cur);
                    par = cur->parent;
                }
                par->color = BLACK;
                par->parent->color = RED;
                RightRotate(par->parent);
            } else if (par->parent->right->color == RED) {
                par->parent->color = RED;
                par->parent->left->color = BLACK;
                par->parent->right->color = BLACK;
                cur = par->parent;
                par = cur->parent;
            } else {
                if (cur->parent->right == cur) {
                    cur = cur->parent;
                    LeftRotate(cur);
                    par = cur->parent;
                }
                RightRotate(par->parent);
                par->color = BLACK;
                par->left->color = RED;
                par->right->color = RED;
            }
        } else {
            if (par->parent->left == Nil) {
                if (cur->parent->left == cur) {
                    cur = cur->parent;
                    RightRotate(cur);
                    par = cur->parent;
                }
                par->color = BLACK;
                par->parent->color = RED;
                LeftRotate(par->parent);
            } else if (par->parent->left->color == RED) {
                par->parent->color = RED;
                par->parent->right->color = BLACK;
                par->parent->left->color = BLACK;
                cur = par->parent;
                par = cur->parent;
            } else {
                if (cur->parent->left == cur) {
                    cur = cur->parent;
                    RightRotate(cur);
                    par = cur->parent;
                }
                LeftRotate(par->parent);
                par->color = BLACK;
                par->left->color = RED;
                par->right->color = RED;
            }
        }
    }
}

template<typename T>
void Tree<T>::LeftRotate(Tree::Node* cur) {
    Node* tmp = cur->right;
    cur->right = tmp->left;
    if (tmp->left != Nil)
        tmp->left->parent = cur;
    tmp->left = cur;
    if (tmp != Nil)
        tmp->parent = cur->parent;
    if (cur == root)
        root = tmp;
    else {
        if (cur == cur->parent->left)
            cur->parent->left = tmp;
        else
            cur->parent->right = tmp;
    }
    tmp->left = cur;
    if (cur != Nil) cur->parent = tmp;
}

template<typename T>
void Tree<T>::RightRotate(Tree::Node* cur) {
    Node* tmp = cur->left;
    cur->left = tmp->right;
    if (tmp->right != Nil)
        tmp->right->parent = cur;
    tmp->right = cur;
    if (tmp != Nil)
        tmp->parent = cur->parent;
    if (cur == root)
        root = tmp;
    else {
        if (cur == cur->parent->right)
            cur->parent->right = tmp;
        else
            cur->parent->left = tmp;
    }
    tmp->right = cur;
    if (cur != Nil) cur->parent = tmp;
}

template<typename T>
typename Tree<T>::Node *Tree<T>::Find(T val) {
    Node* tmp = root;
    if (Eq(root->data, val))
        return root;
    while (tmp != Nil && !Eq(tmp->data, val))
        if (cmp(val, tmp->data))
            tmp = tmp->left;
        else
            tmp = tmp->right;

    return tmp;
}


template<typename T>
T Tree<T>::GetMin() {
    Node* tmp = root;
    while (tmp->left != Nil)
        tmp = tmp->left;
    return tmp->data;
}

template<typename T>
void Tree<T>::FixDelete(Tree::Node *cur, Tree::Node *parent) {
    Node* brother = Nil;
    Node* child = cur->left == Nil ? cur->right : cur->left;
    if (child != Nil) {
        child->color = BLACK;
        return;
    }

    while (cur->color == BLACK && cur != root) {
        if (parent->left == cur) {
            brother = parent->right;
            if (brother->color == RED) {
                parent->color = RED;
                brother->color = BLACK;
                cur = brother;
                parent = cur->parent;
                LeftRotate(parent);
            }
            if (brother->color == BLACK) {
                if (brother->left->color == BLACK && brother->right->color == BLACK) {
                    brother->color = RED;
                    cur = parent;
                    parent = cur->parent;
                } else {
                    if (brother->right->color == BLACK && brother->left->color == RED) {
                        brother->color = RED;
                        brother->left->color = BLACK;
                        RightRotate(brother);
                    }
                    if (brother->right->color == RED) {
                        brother->color = parent->color;
                        parent->color = BLACK;
                        brother->left->color = BLACK;
                        LeftRotate(parent);
                        cur = root;
                    }
                }
            }
        } else {
            brother = parent->left;
            if (brother->color == RED) {
                parent->color = RED;
                brother->color = BLACK;
                cur = brother;
                parent = cur->parent;
                RightRotate(parent);
            }
            if (brother->color == BLACK) {
                if (brother->right->color == BLACK && brother->left->color == BLACK) {
                    brother->color = RED;
                    cur = parent;
                    parent = cur->parent;
                } else {
                    if (brother->left->color == BLACK && brother->right->color == RED) {
                        brother->color = RED;
                        brother->right->color = BLACK;
                        LeftRotate(brother);
                    }
                    if (brother->left->color == RED) {
                        brother->color = parent->color;
                        parent->color = BLACK;
                        brother->right->color = BLACK;
                        RightRotate(parent);
                        cur = root;
                    }
                }
            }
        }
    }
    cur->color = BLACK;
    root->color = BLACK;
}

template<typename T>
bool Tree<T>::Eq(const T &a, const T &b) {
    return !(cmp(a, b) || cmp(b, a));
}

template<typename T>
T Tree<T>::next(T val) {
    Node* tmp = root;
    while (tmp != Nil && !Eq(tmp->data, val)) {
        if (cmp(tmp->data, val))
            if (tmp->right != Nil)
                tmp = tmp->right;
            else break;
        else
        if (tmp->left != Nil)
            tmp = tmp->left;
        else break;
    }
    while (tmp->parent != Nil && cmp(tmp->data, val))
        tmp = tmp->parent;
    if (cmp(tmp->data, val)) {
        return -1;
    } else {
        return tmp->data;
    }
}

template<typename T>
Tree<T>::~Tree() {
    Destroy(root);
    delete Nil;
}

template<typename T>
void Tree<T>::Destroy(Tree::Node *cur) {
    if (cur != Nil) {
        Destroy(cur->left);
        Destroy(cur->right);
        delete cur;
    }
}

bool cmp (const int &a, const int &b) {
    return a < b;
}


int main() {
    std::ios_base::sync_with_stdio();
    Tree<int> a(cmp);
    int n;
    std::cin >> n;
    int last = 0;
    for (int i = 0; i < n; i++) {
        char op;
        int value;
        std::cin >> op >> value;
        if (op == '+') {
            a.insert((value + last) % 1000000000);
            last = 0;
        }
        else {
            last = a.next(value);
            std::cout << last << "\n";
        }
    }
    return 0;
}