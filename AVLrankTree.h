#ifndef WET2CPP_AVLRANKTREE_H
#define WET2CPP_AVLRANKTREE_H

#include <iostream>

template <class KeyType>
class AVLrankTree{
    class AVLvertex{
    public:
        KeyType key;
        AVLvertex *left, *right;
        int height;
        int count;
        int sum;

        explicit AVLvertex(KeyType key)
                : key(key), left(nullptr), right(nullptr), height(1),
                  count(1), sum(key.getKey()) {}
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

    int getCount(AVLvertex* v);

    int getSum(AVLvertex* v);

    int updateCount(AVLvertex* v);

    int updateSum(AVLvertex* v);

    void updateSumAndCountAfterRotation(AVLvertex* v);

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
    AVLvertex* insertVertexRecursive(AVLvertex* curr_root, KeyType& key);

    /* delete a vertex from the tree using recursive calls in order to have a
     * track of the vertexes we visited, so we can rebalance them when the
     * recursive calls unwind. The method returns the root of the modified
     * subtree */
    AVLvertex* deleteVertexRecursive(AVLvertex* curr_root, KeyType& key);

    /* deallocate every vertex in the tree which it's root is
     * curr_root, using a recursive postorder traversal */
    void deleteTree(AVLvertex* curr_root);

    /* rebalance the given vertex if it's balance factor is not between -1 and
     * 1 */
    AVLvertex* rebalanceVertex(AVLvertex* curr_root);

    void sumOfkLargestKeysRec(AVLvertex* curr_root,
            int& remaining_elements_count, int& curr_sum);

    int getTreeSize(AVLvertex* curr_root);

    void mergeArrays(KeyType* arr1, KeyType* arr2, KeyType* merged_arr,
            int size1, int size2);

    void treeToSortedArray(AVLvertex* curr_root, KeyType* arr, int* curr_index);

    AVLvertex* sortedArrayToAVLtree(KeyType* arr, int start, int end);

    AVLvertex* mergeTrees(AVLvertex* this_root, AVLvertex* other_root,
            int this_tree_size, int other_tree_size);

    void updateNodeFieldsAfterMerge(AVLvertex* curr_root);

    void printTreeRec(AVLvertex* curr_root);

public:

    /* constructor  */
    AVLrankTree();

    /* destructor  */
    ~AVLrankTree();

    AVLrankTree(const AVLrankTree& tree) = delete;
    AVLrankTree& operator=(const AVLrankTree& tree);

    /* return true if a vertex with a matching key exists int the tree, false
     * otherwise  */
    bool keyExists(KeyType key);

    /* the interface method to insert a vertex with a "key" to the
     * tree */
    void insertKey(KeyType key);

    /* the interface method to delete a vertex with the matching key from the
     * tree */
    void deleteKey(KeyType key);

    /* the interface method to traverse the tree in an inorder manner, while
     * applying the user supplied function to a vertex's key when visiting it */
    template <class Func>
    void inorder(Func& doSomething){inorderAux(root, doSomething);}

    int sumOfkLargestKeys(int k);

    void mergeTrees(AVLrankTree& other_tree);

    void printTree();
};

template<class KeyType>
AVLrankTree<KeyType>::AVLrankTree()  : root(nullptr) {}

template<class KeyType>
AVLrankTree<KeyType>::~AVLrankTree() {
    /* call the recursive method that deletes every vertex from
     * the tree whilst preforming a postorder traversal */
    deleteTree(root);
}

template<class KeyType>
AVLrankTree<KeyType>& AVLrankTree<KeyType>::operator=
        (const AVLrankTree & tree) {
    return *this;
}

template<class KeyType>
bool AVLrankTree<KeyType>::keyExists(KeyType key){
    AVLvertex* v = searchVertexRecursive(root, key);
    if(v == nullptr) {
        return false;
    } else {
        return true;
    }
}

template<class KeyType>
typename AVLrankTree<KeyType>::AVLvertex*
AVLrankTree<KeyType>::searchVertexRecursive
        (AVLrankTree::AVLvertex* curr_root, KeyType& key){
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

template<class KeyType>
int AVLrankTree<KeyType>::getHeight(AVLrankTree::AVLvertex *v) {
    if(v == nullptr){
        return 0;
    } else {
        return v->height;
    }
}

template<class KeyType>
int AVLrankTree<KeyType>::updateHeight(AVLrankTree::AVLvertex *v) {
    if(v == nullptr){
        return 0;
    } else{
        v->height = 1 + max(getHeight(v->left),getHeight(v->right));
        return v->height;
    }
}

template<class KeyType>
int AVLrankTree<KeyType>::max(int h1, int h2) {
    return h1 > h2 ? h1 : h2;
}

template<class KeyType>
int AVLrankTree<KeyType>::getBF(AVLrankTree::AVLvertex *v) {
    if(v == nullptr){
        return 0;
    } else{
        return getHeight(v->left) - getHeight(v->right);
    }
}

template<class KeyType>
typename AVLrankTree<KeyType>::AVLvertex*
AVLrankTree<KeyType>::rotateRight(AVLrankTree::AVLvertex *v) {
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

template<class KeyType>
typename AVLrankTree<KeyType>::AVLvertex*
AVLrankTree<KeyType>::rotateLeft(AVLrankTree::AVLvertex *v) {
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

template<class KeyType>
void AVLrankTree<KeyType>::insertKey(KeyType key) {
    root = insertVertexRecursive(root, key);
}

template<class KeyType>
typename AVLrankTree<KeyType>::AVLvertex*
AVLrankTree<KeyType>::insertVertexRecursive(AVLrankTree::AVLvertex *curr_root,
        KeyType &key) {

    /* preform the usual insertion like in a regular binary search tree */
    if(curr_root == nullptr){
        return new AVLvertex(key);
    } else if(curr_root->key < key){
        curr_root->count++; // added
        curr_root->sum += key.getKey(); // added
        curr_root->right = insertVertexRecursive(curr_root->right, key);
    } else {
        curr_root->count++; // added
        curr_root->sum += key.getKey(); // added
        curr_root->left = insertVertexRecursive(curr_root->left, key);
    }

    updateHeight(curr_root);

    /* rebalance the current root if needed */
    return rebalanceVertex(curr_root);
}

template<class KeyType>
void AVLrankTree<KeyType>::deleteKey(KeyType key) {
    root = deleteVertexRecursive(root, key);
}

template<class KeyType>
typename AVLrankTree<KeyType>::AVLvertex*
AVLrankTree<KeyType>::deleteVertexRecursive
        (AVLrankTree::AVLvertex *curr_root, KeyType &key) {

    /* preform the usual deletion like in a regular binary search tree */
    if(curr_root == nullptr){
        return curr_root;
    }

    if(curr_root->key < key){
        curr_root->count--; // added
        curr_root->sum -= key.getKey(); // added
        curr_root->right = deleteVertexRecursive(curr_root->right, key);
    } else if (key < curr_root->key){
        curr_root->count--; // added
        curr_root->sum -= key.getKey(); // added
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

template<class KeyType>
void AVLrankTree<KeyType>::deleteTree(AVLrankTree::AVLvertex *curr_root) {

    /* base case */
    if(curr_root == nullptr){
        return;
    }

    deleteTree(curr_root->left);
    deleteTree(curr_root->right);
    delete curr_root;
}

template<class KeyType>
typename AVLrankTree<KeyType>::AVLvertex*
AVLrankTree<KeyType>::rebalanceVertex(AVLvertex* curr_root){

    int BF = getBF(curr_root);

    if(BF >= -1 && BF <= 1) {
        /* balance factor is in bound therefore no rotations are needed */
        updateSumAndCountAfterRotation(curr_root);
        return curr_root;
    }

    if(BF == 2){
        if(getBF(curr_root->left) >= 0){
            /* LL rotation */
            AVLvertex* new_root = rotateRight(curr_root);
            updateSumAndCountAfterRotation(new_root);
            return new_root;
        } else {
            /* LR rotation */
            curr_root->left = rotateLeft(curr_root->left);
            AVLvertex* new_root = rotateRight(curr_root);
            updateSumAndCountAfterRotation(new_root);
            return new_root;
        }
    }

    if(BF == -2){
        if(getBF(curr_root->right) <= 0){
            /* RR rotation */
            AVLvertex* new_root = rotateLeft(curr_root);
            updateSumAndCountAfterRotation(new_root);
            return new_root;
        } else {
            /* RL rotation */
            curr_root->right = rotateRight(curr_root->right);
            AVLvertex* new_root = rotateLeft(curr_root);
            updateSumAndCountAfterRotation(new_root);
            return new_root;
        }
    }

    return curr_root;
}

template<class KeyType>
int AVLrankTree<KeyType>::getCount(AVLrankTree::AVLvertex *v) {
    if(v == nullptr) {
        return 0;
    } else {
        return v->count;
    }
}

template<class KeyType>
int AVLrankTree<KeyType>::getSum(AVLrankTree::AVLvertex *v) {
    if(v == nullptr) {
        return 0;
    } else {
        return v->sum;
    }
}

template<class KeyType>
int AVLrankTree<KeyType>::updateCount(AVLrankTree::AVLvertex *v) {
    if(v == nullptr) {
        return 0;
    } else {
        v->count = 1 + getCount(v->left) + getCount(v->right);
        return v->count;
    }
}

template<class KeyType>
int AVLrankTree<KeyType>::updateSum(AVLrankTree::AVLvertex *v) {
    if(v == nullptr) {
        return 0;
    } else {
        v->sum = (v->key).getKey() + getSum(v->left) + getSum(v->right);
        return v->sum;
    }
}

template<class KeyType>
void AVLrankTree<KeyType>::updateSumAndCountAfterRotation(AVLrankTree::AVLvertex *v) {
    updateCount(v->left);
    updateSum(v->left);

    updateCount(v->right);
    updateSum(v->right);

    updateCount(v);
    updateSum(v);
}

template<class KeyType>
int AVLrankTree<KeyType>::sumOfkLargestKeys(int k) {
    int sum = 0;
    sumOfkLargestKeysRec(root, k, sum);
    return sum;
}

template<class KeyType>
void AVLrankTree<KeyType>::sumOfkLargestKeysRec(AVLrankTree::AVLvertex *curr_root,
        int &remaining_elements_count, int &curr_sum) {
    if (curr_root == nullptr) {
        return;
    }

    if(curr_root->count == remaining_elements_count) {
        remaining_elements_count = 0;
        curr_sum += curr_root->sum;
        return;
    }
    else if(curr_root->count < remaining_elements_count){
        remaining_elements_count -= curr_root->count;
        curr_sum += curr_root->sum;
        return;
    }
    else {
        sumOfkLargestKeysRec(curr_root->right, remaining_elements_count,
                curr_sum);
        if(remaining_elements_count == 0){
            return;
        }
        else{
            curr_sum += (curr_root->key).getKey();
            remaining_elements_count--;
            if(remaining_elements_count == 0){
                return;
            }
            else {
                sumOfkLargestKeysRec(curr_root->left, remaining_elements_count,
                        curr_sum);
            }
        }
    }
}

template<class KeyType>
int AVLrankTree<KeyType>::getTreeSize(AVLrankTree::AVLvertex *curr_root) {
    if(curr_root == nullptr){
        return 0;
    } else {
        return curr_root->count;
    }
}

template<class KeyType>
void AVLrankTree<KeyType>::mergeTrees(AVLrankTree &other_tree) {
    AVLvertex* new_root = mergeTrees(root, other_tree.root, getTreeSize(root),
            getTreeSize(other_tree.root));
    deleteTree(root);
    deleteTree(other_tree.root);
    other_tree.root = nullptr;
    root = new_root;
}

template<class KeyType>
typename AVLrankTree<KeyType>::AVLvertex*
AVLrankTree<KeyType>::mergeTrees(AVLrankTree::AVLvertex *this_root,
        AVLrankTree::AVLvertex *other_root, int this_tree_size,
        int other_tree_size) {
    auto * this_tree_arr = new KeyType[this_tree_size];
    auto * other_tree_arr = new KeyType[other_tree_size];

    int this_tree_index = 0;
    treeToSortedArray(this_root, this_tree_arr, &this_tree_index);
    int other_tree_index = 0;
    treeToSortedArray(other_root, other_tree_arr, &other_tree_index);

    auto * merged_arr = new KeyType[this_tree_size + other_tree_size];

    mergeArrays(this_tree_arr, other_tree_arr, merged_arr, this_tree_size,
            other_tree_size);

    delete[] this_tree_arr;
    delete[] other_tree_arr;

    AVLvertex* new_root = sortedArrayToAVLtree(merged_arr, 0,
            this_tree_size + other_tree_size - 1);

    delete[] merged_arr;

    updateNodeFieldsAfterMerge(new_root);

    return new_root;
}

template<class KeyType>
void AVLrankTree<KeyType>::mergeArrays(KeyType *arr1, KeyType *arr2,
        KeyType *merged_arr, int size1, int size2) {
    int i1 = 0;
    int i2 = 0;
    int i_merged = 0;

    while (i1 < size1 && i2 < size2) {
        if(arr1[i1] < arr2[i2]) {
            merged_arr[i_merged] = arr1[i1];
            i_merged++;
            i1++;
        } else {
            merged_arr[i_merged] = arr2[i2];
            i_merged++;
            i2++;
        }
    }

    while (i1 < size1) {
        merged_arr[i_merged] = arr1[i1];
        i_merged++;
        i1++;
    }

    while (i2 < size2) {
        merged_arr[i_merged] = arr2[i2];
        i_merged++;
        i2++;
    }
}

template<class KeyType>
void AVLrankTree<KeyType>::updateNodeFieldsAfterMerge(AVLrankTree::AVLvertex *curr_root) {
    /* base case */
    if(curr_root == nullptr){
        return;
    }

    updateNodeFieldsAfterMerge(curr_root->left);
    updateNodeFieldsAfterMerge(curr_root->right);

    updateHeight(curr_root);
    updateCount(curr_root);
    updateSum(curr_root);
}

template<class KeyType>
typename AVLrankTree<KeyType>::AVLvertex*
AVLrankTree<KeyType>::sortedArrayToAVLtree(KeyType *arr, int start, int end) {
    if (start > end) {
        return nullptr;
    }

    int mid = (start + end)/2;
    auto *new_root = new AVLvertex(arr[mid]);

    new_root->left = sortedArrayToAVLtree(arr, start, mid-1);
    new_root->right = sortedArrayToAVLtree(arr, mid+1, end);

    return new_root;
}

template<class KeyType>
void AVLrankTree<KeyType>::treeToSortedArray(AVLrankTree::AVLvertex *curr_root,
                                             KeyType *arr, int *curr_index) {
    if(curr_root == nullptr) {
        return;
    }

    treeToSortedArray(curr_root->left, arr, curr_index);

    arr[*curr_index] = curr_root->key;
    (*curr_index)++;

    treeToSortedArray(curr_root->right, arr, curr_index);
}

template<class KeyType>
void AVLrankTree<KeyType>::printTree() {
    printTreeRec(root);
}

template<class KeyType>
void AVLrankTree<KeyType>::printTreeRec(AVLrankTree::AVLvertex *curr_root) {
    if(curr_root == nullptr){
        return;
    }
    printTreeRec(curr_root->left);
    std::cout << "\nnode details: " << std::endl;
    std::cout << "node's key: " << (curr_root->key).getKey() << std::endl;
    std::cout << "node's count: " << curr_root->count << std::endl;
    std::cout << "node's sum: " << curr_root->sum << std::endl;
    printTreeRec(curr_root->right);
}

#endif //WET2CPP_AVLRANKTREE_H
