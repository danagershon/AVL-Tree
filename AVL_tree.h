#ifndef WET1CPP_AVL_TREE_H
#define WET1CPP_AVL_TREE_H

#include <iostream>

template <class KeyType, class DataType>
class AVL_tree{
    class AVLvertex{
    public:
        KeyType key;
        DataType* data;
        AVLvertex *left, *right;
        int height;

        explicit AVLvertex(KeyType key, DataType* data = nullptr)
                : key(key), data(data), left(nullptr), right(nullptr), height(1) {}
    };

    AVLvertex *root;

    /* the recursive method to traverse the tree in an inorder manner, while
     * applying the user supplied function to a vertex's key when visiting it */
    template <class Func>
    void inorderAux(AVLvertex* curr_root, Func& doSomething){
        if(curr_root == nullptr){
            return;
        }
        inorderAux(curr_root->left, doSomething);
        doSomething(curr_root->key);
        inorderAux(curr_root->right, doSomething);
    }

    /* calculate the height of the given vertex */
    int getHeight(AVLvertex* v);

    /* calculate and update the height of the given vertex */
    int updateHeight(AVLvertex* v);

    /* calculate the balance factor of the given vertex */
    int getBF(AVLvertex* v);

    /* search a vertex with a matching key in the tree in a recursive manner.
     * return a pointer to the vertex if found or nullptr otherwise */
    AVLvertex* searchVertexRecursive(AVLvertex* curr_root, KeyType& key);

    /* calculate the maximum of two integers */
    int max(int h1, int h2);

    /* preform a right rotation to the given vertex. The method returns the
     * root of the new subtree */
    AVLvertex* rotateRight(AVLvertex* v);

    /* preform a left rotation to the given vertex. The method returns the
     * root of the new subtree */
    AVLvertex* rotateLeft(AVLvertex* v);

    /* insert a vertex to the tree using recursive calls in order to have a
     * track of the vertexes we visited, so we can rebalance them when the
     * recursive calls unwind. The method returns the root of the modified
     * subtree */
    AVLvertex* insertVertexRecursive(AVLvertex* curr_root, KeyType& key,
            DataType* data);

    /* delete a vertex from the tree using recursive calls in order to have a
     * track of the vertexes we visited, so we can rebalance them when the
     * recursive calls unwind. The method returns the root of the modified
     * subtree */
    AVLvertex* deleteVertexRecursive(AVLvertex* curr_root, KeyType& key);

    /* deallocate every vertex and it's data in the tree which it's root is
     * curr_root, using a recursive postorder traversal */
    void deleteTree(AVLvertex* curr_root);

    /* rebalance the given vertex if it's balance factor is not between -1 and
     * 1 */
    AVLvertex* rebalanceVertex(AVLvertex* curr_root);

public:

    /* constructor  */
    AVL_tree();

    /* destructor  */
    ~AVL_tree();

    AVL_tree(const AVL_tree& tree) = delete;
    AVL_tree& operator=(const AVL_tree& tree);

    /* return true if a vertex with a matching key exists int the tree, false
     * otherwise  */
    bool keyExists(KeyType key);

    /* the interface method to insert a vertex with a "key" and "data" to the
     * tree */
    void insertKey(KeyType key,DataType* data);

    /* the interface method to delete a vertex with the matching key from the
     * tree */
    void deleteKey(KeyType key);

    /* return the pointer to the data that the vertex with the matching key
     * holds */
    DataType* getData(KeyType key);

    /* the interface method to traverse the tree in an inorder manner, while
     * applying the user supplied function to a vertex's key when visiting it */
    template <class Func>
    void inorder(Func& doSomething){inorderAux(root, doSomething);}
};

template<class KeyType, class DataType>
AVL_tree<KeyType, DataType>::AVL_tree()  : root(nullptr) {}

template<class KeyType, class DataType>
AVL_tree<KeyType, DataType>::~AVL_tree() {
    /* call the recursive method that deletes every vertex and it's data from
     * the tree whilst preforming a postorder traversal */
    deleteTree(root);
}

template<class KeyType, class DataType>
AVL_tree<KeyType, DataType>& AVL_tree<KeyType, DataType>::operator=
        (const AVL_tree & tree) {
    return *this;
}

template<class KeyType, class DataType>
bool AVL_tree<KeyType, DataType>::keyExists(KeyType key){
    AVLvertex* v = searchVertexRecursive(root, key);
    if(v == nullptr) {
        return false;
    } else {
        return true;
    }
}

template<class KeyType, class DataType>
DataType* AVL_tree<KeyType, DataType>::getData(KeyType key){
    AVLvertex* v = searchVertexRecursive(root, key);
    if(v == nullptr){
        return nullptr;
    }

    return v->data;
}

template<class KeyType, class DataType>
typename AVL_tree<KeyType, DataType>::AVLvertex*
AVL_tree<KeyType, DataType>::searchVertexRecursive
(AVL_tree::AVLvertex* curr_root, KeyType& key){
    if (curr_root == nullptr) {
        return nullptr;
    } else if (curr_root->key == key) {
        return curr_root;
    } else if (curr_root->key < key) {
        return searchVertexRecursive(curr_root->right, key);
    } else {
        return searchVertexRecursive(curr_root->left, key);
    }
}

template<class KeyType, class DataType>
int AVL_tree<KeyType, DataType>::getHeight(AVL_tree::AVLvertex *v) {
    if(v == nullptr){
        return 0;
    } else {
        return v->height;
    }
}

template<class KeyType, class DataType>
int AVL_tree<KeyType, DataType>::updateHeight(AVL_tree::AVLvertex *v) {
    if(v == nullptr){
        return 0;
    } else{
        v->height = 1 + max(getHeight(v->left),getHeight(v->right));
        return v->height;
    }
}

template<class KeyType, class DataType>
int AVL_tree<KeyType, DataType>::max(int h1, int h2) {
    return h1 > h2 ? h1 : h2;
}

template<class KeyType, class DataType>
int AVL_tree<KeyType, DataType>::getBF(AVL_tree::AVLvertex *v) {
    if(v == nullptr){
        return 0;
    } else{
        return getHeight(v->left) - getHeight(v->right);
    }
}

template<class KeyType, class DataType>
typename AVL_tree<KeyType, DataType>::AVLvertex*
AVL_tree<KeyType, DataType>::rotateRight(AVL_tree::AVLvertex *v) {
    AVLvertex* to_rotate = v;
    AVLvertex* to_rotate_left_child = to_rotate->left;
    AVLvertex* right_subtree = to_rotate_left_child->right;

    /* preform rotation */
    to_rotate_left_child->right = to_rotate;
    to_rotate->left = right_subtree;

    /* update the height of the vertexes that their subtree changed */
    updateHeight(to_rotate);
    updateHeight(to_rotate_left_child);

    /* return the root of the new subtree */
    return to_rotate_left_child;
}

template<class KeyType, class DataType>
typename AVL_tree<KeyType, DataType>::AVLvertex*
AVL_tree<KeyType, DataType>::rotateLeft(AVL_tree::AVLvertex *v) {
    AVLvertex* to_rotate = v;
    AVLvertex* to_rotate_right_child = to_rotate->right;
    AVLvertex* left_subtree = to_rotate_right_child->left;

    /* preform rotation */
    to_rotate_right_child->left = to_rotate;
    to_rotate->right = left_subtree;

    /* update the height of the vertexes that their subtree changed */
    updateHeight(to_rotate);
    updateHeight(to_rotate_right_child);

    /* return the root of the new subtree */
    return to_rotate_right_child;
}

template<class KeyType, class DataType>
void AVL_tree<KeyType, DataType>::insertKey(KeyType key, DataType *data) {
    root = insertVertexRecursive(root, key, data);
}

template<class KeyType, class DataType>
typename AVL_tree<KeyType, DataType>::AVLvertex*
AVL_tree<KeyType, DataType>::insertVertexRecursive
(AVL_tree::AVLvertex *curr_root, KeyType &key, DataType *data) {

    /* preform the usual insertion like in a regular binary search tree */
    if(curr_root == nullptr){
        return new AVLvertex(key, data);
    } else if(curr_root->key < key){
        curr_root->right = insertVertexRecursive(curr_root->right, key, data);
    } else {
        curr_root->left = insertVertexRecursive(curr_root->left, key, data);
    }

    updateHeight(curr_root);

    /* rebalance the current root if needed */
    return rebalanceVertex(curr_root);
}

template<class KeyType, class DataType>
void AVL_tree<KeyType, DataType>::deleteKey(KeyType key) {
    root = deleteVertexRecursive(root, key);
}

template<class KeyType, class DataType>
typename AVL_tree<KeyType, DataType>::AVLvertex*
AVL_tree<KeyType, DataType>::deleteVertexRecursive
(AVL_tree::AVLvertex *curr_root, KeyType &key) {

    /* preform the usual deletion like in a regular binary search tree */
    if(curr_root == nullptr){
        return curr_root;
    }

    if(curr_root->key < key){
        curr_root->right = deleteVertexRecursive(curr_root->right, key);
    } else if (key < curr_root->key){
        curr_root->left = deleteVertexRecursive(curr_root->left, key);
    } else { /* means that curr_root is the vertex to delete */
        if(curr_root->left == nullptr && curr_root->right == nullptr){
            /* no children case */
            AVLvertex* temp = curr_root;
            curr_root = nullptr;
            delete temp;
        } else if(curr_root->left == nullptr || curr_root->right == nullptr){
            /* one child case */
            AVLvertex* existing_child;
            if(curr_root->left != nullptr){
                existing_child = curr_root->left;
            } else {
                existing_child = curr_root->right;
            }
            *curr_root = *existing_child;
            delete existing_child;
        } else {
            /* 2 children case */
            AVLvertex* successor = curr_root->right;
            while (successor->left != nullptr){
                successor = successor->left;
            }
            curr_root->key = successor->key;
            curr_root->data = successor->data;
            curr_root->right = deleteVertexRecursive(curr_root->right,
                    successor->key);
        }
    }

    if(curr_root == nullptr){
        /* means the deleted vertex was the only vertex in the tree */
        return curr_root;
    }

    updateHeight(curr_root);

    /* rebalance the current root if needed */
    return rebalanceVertex(curr_root);
}

template<class KeyType, class DataType>
void AVL_tree<KeyType, DataType>::deleteTree(AVL_tree::AVLvertex *curr_root) {

    /* base case */
    if(curr_root == nullptr){
        return;
    }

    deleteTree(curr_root->left);
    deleteTree(curr_root->right);
    delete curr_root->data;
    delete curr_root;
}

template<class KeyType, class DataType>
typename AVL_tree<KeyType, DataType>::AVLvertex*
AVL_tree<KeyType, DataType>::rebalanceVertex(AVLvertex* curr_root){

    int BF = getBF(curr_root);

    if(BF >= -1 && BF <= 1) {
        /* balance factor is in bound therefore no rotations are needed */
        return curr_root;
    }

    if(BF == 2){
        if(getBF(curr_root->left) >= 0){
            /* LL rotation */
            return rotateRight(curr_root);
        } else {
            /* LR rotation */
            curr_root->left = rotateLeft(curr_root->left);
            return rotateRight(curr_root);
        }
    }

    if(BF == -2){
        if(getBF(curr_root->right) <= 0){
            /* RR rotation */
            return rotateLeft(curr_root);
        } else {
            /* RL rotation */
            curr_root->right = rotateRight(curr_root->right);
            return rotateLeft(curr_root);
        }
    }

    return curr_root;
}


#endif //WET1CPP_AVL_TREE_H
