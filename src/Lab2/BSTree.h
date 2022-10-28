//
// Created by Amo on 2022/10/23.
//

#ifndef MYDSLEARNING_BSTREE_H
#define MYDSLEARNING_BSTREE_H

#include <utility>
#include <iostream>
// comparable type
template<typename CT>
class BinarySearchTree {

public:
    // interface

    BinarySearchTree()
    : root_(nullptr) {
        // empty
    }

    ~BinarySearchTree() {
        makeEmpty();
        root_ = nullptr;
    }

    BinarySearchTree( const BinarySearchTree & rhs)
    : root_(nullptr) {
        root_ = clone(rhs.root_);
    }

    BinarySearchTree( BinarySearchTree && rhs) noexcept
    : root_(rhs.root_) {
        rhs.root_ = nullptr;
    }

    // Does it actually work?
    BinarySearchTree & operator =(const BinarySearchTree & rhs) {
        if (&rhs == this)
            return *this;

        BinarySearchTree copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    BinarySearchTree & operator =(BinarySearchTree && rhs) noexcept {
        if (&rhs == this)
            return *this;

        makeEmpty();
        root_ = std::exchange(rhs.root_, nullptr);

        return *this;
    }

    /**
     * Check if data is in BST
     * @param data object search for
     * @return true if data is found
     */
    bool isContain(const CT & data) const {
        return isContain(data, root_);
    }

    /**
     * Find the minimum of the BST
     * @return a constant reference of minimum
     */
    const CT & findMin() const {
        return findMin(root_)->data;
    }

    /**
     * Find the maximum of BST
     * @return a constant reference of maximum
     */
    const CT & findMax() const {
        return findMax(root_)->data;
    }

    /**
     * Insert data into BST
     * @param data the object to insert
     */
    void insert(const CT & data) {
        insert(data, root_);
    }

    /**
     * Insert data into BST
     * @param data the object to insert with move
     */
    void insert(CT && data) {
        insert(std::move(data), root_);
    }

    /**
     * remove data from BST
     * @param data the object desired to remove
     */
    void remove(const CT & data) {
        remove(data, root_);
    }

    /**
     * print BST in order
     * @param out an ostream
     */
    void print(std::ostream & out) {
        print(out, root_);
    }

    /**
     * Check if this BST is empty
     * @return true if root_ points to null
     */
    bool isEmpty() {
        return root_ == nullptr;
    }

    /**
     *  Make this BST empty
     */
    void makeEmpty() {
        makeEmpty(root_);
    }

protected:
    /**
     * Basic node structure of BST
     */
    struct BinaryNode {
        CT data;
        BinaryNode * left;
        BinaryNode * right;

        BinaryNode( CT & d, BinaryNode * l, BinaryNode * r)
                : data(d), left(l), right(r) {
            // empty
        }

        BinaryNode( CT && d, BinaryNode * l, BinaryNode * r) noexcept
                : data(std::move(d)), left(l), right(r) {
            // empty
        }
    };

protected:
    // Internal functions
    /**
     * Internal method to Judge whether x is in subtree t
     * @param x the item to search for
     * @param t node roots the subtree
     * @return true if x is in subtree
     */
    bool isContain(const CT & x, BinaryNode * t) const {
        if (t == nullptr)
            return false;
        else if (x < t->data)
            isContain(x, t->left);
        else if (x > t->data)
            isContain(x, t->right);
        else // found
            return true;
    }

    /**
     * Internal method to Find the minimum of subtree
     * @param t node roots subtree
     * @return Minimum BinaryNode
     */
    BinaryNode * findMin(BinaryNode * t) const {
        if (t == nullptr)
            return nullptr;
        if (t->left == nullptr)
            return t;
        else
            return findMin(t->left);
    }

    /**
     * Internal method to Find the maximum of subtree
     * @param t node roots subtree
     * @return Maximum BinaryNode
     */
    BinaryNode * findMax(BinaryNode * t) const {
        if (t == nullptr)
            return nullptr;
        if (t->right == nullptr)
            return t;
        else
            return findMax(t->right);
    }

    /**
     * Internal method to insert data into subtree
     * @param data the object to insert
     * @param t node roots subtree
     */
    void insert(const CT & data, BinaryNode * & t) {
        if (t == nullptr) {
            t = new BinaryNode(data, nullptr, nullptr);
            return;
        }
        if (data < t->data)
            insert(data, t->left);
        else if (data > t->data)
            insert(data, t->right);
    }

    /**
     * Internal method to insert data into subtree
     * @param data the object to insert with move
     * @param t node roots subtree
     */
    void insert(CT && data, BinaryNode * & t) {
        if (t == nullptr) {
            t = new BinaryNode(std::move(data), nullptr, nullptr);
            return;
        }
        if (data < t->data)
            insert(std::move(data), t->left);
        else if (data > t->data)
            insert(std::move(data), t->right);
    }

    /**
     * Internal method to remove data from subtree
     * @param data the object to remove
     * @param t node roots subtree
     */
    void remove(const CT & data, BinaryNode * & t) {
        if (t == nullptr)   // data not found
            return;
        if (data < t->data)
            remove(data, t->left);
        else if (data > t->data)
            remove(data, t->right);
        else // data found, prepare to remove
        if (t->left != nullptr && t->right != nullptr) {
            t->data = findMin(t->right)->data;
            remove(t->data, t->right);
        }
        else {
            BinaryNode * old_node = t;
            t = t->left == nullptr? t->right: t->left;
            delete old_node;
        }
    }

    /**
     * Internal method to make a subtree empty
     * @param t node roots subtree
     */
    void makeEmpty(BinaryNode * & t) {
        if (t == nullptr)
            return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }

    /**
     * Internal method to print subtree in order
     * @param out out stream
     * @param t node roots subtree
     */
    void print( std::ostream & out, BinaryNode * t) {
        if (t == nullptr)
            return;
        print(out, t->left);
        out << t->data << std::endl;
        print(out, t->right);
    }

    /**
     * Internal method to clone subtree
     * @param t node roots subtree
     * @return new node roots cloned subtree
     */
    BinaryNode * clone(BinaryNode * t) {
        if (t == nullptr)
            return nullptr;
        else
            return new BinaryNode(t->data, clone(t->left), clone(t->right));
    }


protected:
    // Data members
    BinaryNode * root_;
};

#endif //MYDSLEARNING_BSTREE_H
