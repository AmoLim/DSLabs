//
// Created by Amo on 2022/10/21.
//

#ifndef LAB1_SLLIST_H
#define LAB1_SLLIST_H

#include <stdexcept>
#include <utility>

template<typename T>
class SLList {
private:
    /**
     * The basic Node structure of SLList. */
    struct Node {
        T data;
        Node* next;

        Node() : next(nullptr) {
            // empty
        }

        Node( T d, Node* n) : data(d), next(n) {
            // empty
        }
    };

public:
    /**
     * Const Iterator.\n
     * Nested class for iterating the list.\n
     * const. */
     class ConstIterator {
     public:
         ConstIterator() : current_(nullptr) {
             // empty
         }

         ConstIterator (const SLList<T> * lst, Node * c) : current_(c), lst_(lst) {
            // empty
         }

         /**
          * Indirect accessing operation
          */
         const T & operator *() const {
             return retrieve();
         }

         /**
          * Prefix self-increment operation.
          */
         ConstIterator & operator ++() {
             current_ = current_->next;
             return *this;
         }

         /**
          * Postfix self-increment operation.
          */
         ConstIterator operator ++(int) {
            ConstIterator old = *this;
            ++(*this);
             return old;
         }

         /**
          * Equality operation.
          * @return true if this.current_ == rhs.current_.
          */
          bool operator ==(const ConstIterator & rhs) {
            return rhs.current_ == current_;
          }

          /**
           * Inequality operation
           * @return !equality operation.
           */
          bool operator !=(const ConstIterator & rhs) {
              return !(*this == rhs);
          }


     protected:
         const SLList<T> * lst_;
         Node * current_;

         T & retrieve() const {
            return current_->data;
         }

         bool assertIsValid() const {
            if (current_ == nullptr || lst_ == nullptr ) {
                throw std::invalid_argument("current_ pointer miss-matched\n");
            }
            return true;
         }

         friend class SLList<T>;
     };

     /**
      * Iterator.\n
      * Nested class for iterating the list.\n
      * Inherit from ConstIterator.\n
      */
     class Iterator : public ConstIterator {
     public:
         Iterator() : ConstIterator() {
             // empty
         }

         /**
          * Indirect accessing operation
          */
         T & operator *() {
             return ConstIterator::retrieve();
         }

         /**
          * Prefix self-increment operation.
          */
         Iterator & operator ++() {
             this->current_ = this->current_->next;
             return *this;
         }

         /**
          * Postfix self-increment operation.
          */
         Iterator operator ++(int) {
             Iterator old = *this;
             ++(*this);
             return old;
         }

     protected:
         Iterator(const SLList<T> * lst, Node * c) : ConstIterator(lst, c) {
             // empty
         }

         bool assertIsValid() const {
             return ConstIterator::assertIsValid();
         }

         friend class SLList<T>;
     };

public:

    SLList() {
        init();
    }

    ~SLList() {
        clear();
        delete head_;
        tail_ = nullptr;
    }

    SLList(const SLList & rhs) {
        init();
        for (auto & x : rhs)
            pushBack(x);
    }

    SLList & operator = (const SLList & rhs) {
        if (&rhs == this) {
            return *this;
        }

        this->clear();

        for (auto & x : rhs)
            this->pushBack(x);

        return *this;
    }

    SLList(SLList && rhs) noexcept
    : size_(rhs.size_), head_(rhs.head_), tail_(rhs.tail_){
        // empty
    }

    SLList & operator = (SLList && rhs) noexcept {
        if (&rhs == this)
            return *this;

        clear();
        head_ = std::exchange(rhs.head_, nullptr);
        tail_ = std::exchange(rhs.tail_, nullptr);
        size_ = std::exchange(rhs.size_, 0);

        return *this;
    }

    Iterator begin() {
        Iterator itr(this, head_);
        return ++itr;
    }

    ConstIterator begin() const {
        ConstIterator itr(this, head_);
        return ++itr;
    }

    Iterator end() {
        Iterator itr(this, tail_);
        return ++itr;
    }

    ConstIterator end() const {
        ConstIterator itr(this, tail_);
        return ++itr;
    }

    Iterator getHead() {
        return Iterator(this, head_);
    }

    ConstIterator getHead() const {
        return ConstIterator(this,head_);
    }

    Iterator getTail() {
        return Iterator(this, tail_);
    }

    ConstIterator getTail() const {
        return ConstIterator(this, tail_);
    }


    /**
     * Get Attribution size_
     * @return size_
     */
    int size() const {
        return size_;
    }

    /**
     * Check if list is empty.
     * @return size_ == 0
     */
    bool empty() const {
        return size_ == 0;
    }

    /**
     *  Clear the list.
     */
    void clear() {
        for (;!empty();) {
            popFront();
        }
    }

    /**
     * Insert a new node after itr.
     * @param itr a iterator points to current node
     * @param x l-value data
     * @return new iterator points to new node.
     */
    Iterator insert(Iterator itr, const T & x) {
        itr.assertIsValid();

        if (itr.lst_ != this)
            throw std::invalid_argument("List pointer miss-matched\n");

        Node *new_node = new Node(x, itr.current_->next);
        itr.current_->next = new_node;

        // update the invariants.
        if (itr.current_ == tail_)
            tail_ = itr.current_->next;
        size_++;

        Iterator new_itr(this, new_node);
        return new_itr;
    }

    /**
     * Insert a new node after itr
     * @param itr a iterator points to current node
     * @param x a r-value data
     * @return new iterator points to new node
     */
    Iterator insert(Iterator itr, T && x) {
        itr.assertIsValid();

        if (itr.lst_ != this)
            throw std::invalid_argument("List pointer miss-matched\n");

        Node *new_node = new Node(std::move(x), itr.current_->next);
        itr.current_->next = new_node;

        // update the invariants.
        if (itr.current_ == tail_)
            tail_ = itr.current_->next;
        size_++;

        Iterator new_itr(this, new_node);
        return new_itr;
    }

    /**
     * Erase the node that iterator points to.
     * @param itr a iterator points to current node
     * @return new iterator points to the previous node
     */
    Iterator erase(Iterator itr) {
        itr.assertIsValid();

        if (itr.lst_ != this)
            throw std::invalid_argument("List pointer miss-matched\n");

        Iterator prev_itr = findPrev(itr);
        prev_itr.current_->next = itr.current_->next;

        // update the invariants.
        if (itr.current_ == tail_)
            tail_ = prev_itr.current_;
        size_--;

        delete itr.current_;

        return prev_itr;
    }

    /**
     * Get reference to the last item.
     * @return tail_->data
     */
    T & back() {
        Iterator itr(this, tail_);
        itr.assertIsValid();
        return itr.current_->data;
    }

    /**
     * Get constant reference to the last item.
     * @return tail_->data
     */
    const T & back() const {
        ConstIterator itr(this, tail_);
        itr.assertIsValid();
        return itr.current_->data;
    }

    /**
     * Get reference to the first item
     * @return begin().current_->data
     */
    T & front() {
        Iterator itr = begin();
        itr.assertIsValid();
        return itr.current_->data;
    }

    /**
     * Get constant reference to the first item
     * @return begin().current_->data
     */
    const T & front() const {
        ConstIterator itr = begin();
        itr.assertIsValid();
        return itr.current_->data;
    }

    /**
     * Push x to the front of list
     * @param x the data, l-value
     */
    void pushFront(const T & x) {
        insert(getHead(), x);
    }

    /**
     * Push x to the front of list
     * @param x the data, r-value
     */
    void pushFront(T && x) {
        insert(getHead(), std::move(x));
    }

    /**
    * Push x to the back of list
    * @param x the data, l-value
    */
    void pushBack(const T & x) {
        insert(getTail(), x);
    }

    /**
    * Push x to the back of list
    * @param x the data, r-value
    */
    void pushBack(T && x) {
        insert(getTail(), std::move(x));
    }

    /**
     * Erase front of the list
     */
    void popFront() {
        Iterator itr = begin();
        itr.assertIsValid();
        erase(itr);
    }

    /**
     * Erase back of the list.
     */
    void popBack() {
        Iterator itr = getTail();
        itr.assertIsValid();
        erase(itr);
    }


private:
    /*
     * Some internal functions.*/

    /**
     * Find the previous node of itr.current_
     * @param itr iterator points to current node.
     * @return iterator points to the previous node
     */
    Iterator findPrev(Iterator & itr) {
        Iterator p(this, head_);

        for (;p != end(); p++) {
            if (p.current_->next == itr.current_) {
                return p;
            }
        }

        throw std::invalid_argument("Can not find the previous node\n");
    }

    void init() {
        size_ = 0;
        head_ = new Node();
        tail_ = head_;
    }

private:
    // data members
    /**
     * head_ node
     * @Invariant
     *      head_->next (if exists) is the first item of the list.
     */
    Node *head_;

    /**
     * tail_ node
     * @Invariant
     *      tail_ always points to the last items of the list.
     *      if the last item does not exists, tail_ points to head_.
     */
    Node *tail_;

    /**
     * size
     * @Invariant
     *      size_ always equals to the number of nodes.
     */
    int size_;
};

#endif //LAB1_SLLIST_H
