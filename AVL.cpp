#include <iostream>

template <typename type> class AVL;

template <typename type> class AVL_node
{
    type val;
    int height = 1;
    AVL_node<type>* left = nullptr;
    AVL_node<type>* right = nullptr;

    AVL_node(type value)
    {
        val = value;
    }

    int diff()
    {
        int lheight = left ? left->height : 0;
        int rheight = right ? right->height : 0;
        return lheight - rheight;
    }

    friend class AVL<type>;
};

template <typename type> class AVL
{
  private:
    AVL_node<type>* head = nullptr;
    unsigned int size = 0;

    static bool default_comp(type first, type second)
    {
        return first > second;
    }

    bool (*comp)(type first, type second);

    AVL_node<type>* insert(AVL_node<type>* node, type value)
    {
        if (!node) {
            ++size;
            return new AVL_node<type>(value);
        }
        if (value < node->val)
            node->left = insert(node->left, value);
        else
            node->right = insert(node->right, value);

        return balance(node);
    }

    AVL_node<type>* remove(AVL_node<type>* node, type value)
    {
        if (!node)
            return node;

        if (value < node->val)
            node->left = remove(node->left, value);

        else if (value > node->val)
            node->right = remove(node->right, value);

        else {
            size--;
            if (!node->right)
                return node->left;

            node = remove_rmin(node);

            //node->val = val;
        }
        return balance(node);
    }

    type get_min(AVL_node<type>* curr_node)
    {
        if (curr_node->left == 0)
            return curr_node->val;
        return get_min(curr_node->left);
    }

    AVL_node<type>* remove_rmin(AVL_node<type>* curr_node)
    {
        AVL_node<type>*parent = curr_node, *root = curr_node;
        curr_node = parent->right;
        type diff = get_min(curr_node);
        while (curr_node->left) {
            curr_node->sum -= diff;
            parent = curr_node;
            curr_node = curr_node->left;
        }

        root->val = curr_node->val;

        if (curr_node == root->right)
            root->right = root->right->right;
        else
            parent->left = curr_node->right;

        delete (curr_node);

        return root;
    }

    AVL_node<type>* rot_right(AVL_node<type>* node)
    {
        AVL_node<type>* root = node->left;
        node->left = root->right;
        root->right = node;

        height_upd(node);
        height_upd(root);
        return root;
    }

    AVL_node<type>* rot_left(AVL_node<type>* node)
    {
        AVL_node<type>* root = node->right;
        node->right = root->left;
        root->left = node;

        height_upd(node);
        height_upd(root);
        return root;
    }

    void height_upd(AVL_node<type>* node)
    {
        if (!node)
            return;

        int lheight = node->left ? node->left->height : 0;
        int rheight = node->right ? node->right->height : 0;
        node->height = std::max(lheight, rheight) + 1;
    }

    AVL_node<type>* balance(AVL_node<type>* node)
    {
        height_upd(node);

        if (node->diff() == -2) {
            if (node->right->diff() == 1)
                node->right = rot_right(node->right);

            return rot_left(node);
        }

        if (node->diff() == 2) {
            if (node->left->diff() == -1)
                node->left = rot_left(node->left);

            return rot_right(node);
        }
        return node;
    }

    void print(AVL_node<type>* node)
    {
        if (!node)
            return;
        print(node->left);
        std::cout << node->val << ' ';
        print(node->right);
    }

  public:
    AVL<type>()
    {
        comp = default_comp;
    }
    AVL<type>(bool (*other_comp)(type first, type second))
    {
        comp = other_comp;
    }
    void insert(type value)
    {
        if (!find(value))
            head = insert(head, value);
    }

    void remove(type value)
    {
        head = remove(head, value);
    }

    bool find(type value)
    {
        AVL_node<type>* curr_node = head;
        while (curr_node && curr_node->val != value) {
            if (value < curr_node->val)
                curr_node = curr_node->left;
            else
                curr_node = curr_node->right;
        }
        return curr_node != nullptr;
    }

    type get_min()
    {
        AVL_node<type>* curr_node = head;
        while (curr_node->left)
            curr_node = curr_node->left;

        return curr_node->val;
    }

    type get_max()
    {
        AVL_node<type>* curr_node = head;
        while (curr_node->right)
            curr_node = curr_node->right;

        return curr_node->val;
    }

    bool empty()
    {
        return size == 0;
    }
    void print()
    {
        print(head);
    }

};