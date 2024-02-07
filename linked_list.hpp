#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <ostream>
#include <initializer_list>

struct node {
    int data;
    node* next;

    // deleting a node deletes all children
    ~node()
    {
        if (this != next) {
            delete next;
        }
    }

    // at and length are static members because they need to handle a nullptr

    // bounds-checked element access
    static node* at(node* start, size_t idx);

    // returns the number of nodes in the list beginning at `start`
    static size_t size(node* start);

    friend std::ostream& operator<<(std::ostream&, const node*);

    // create a list with data elements 0..nelts
    friend node* make_list(size_t nelts);
    // create a list with the given data elements
    friend node* make_list(std::initializer_list<int> lst);

private:
    // private as nodes should only be created through the factory function
    // this ensures nodes aren't created on the stack
    explicit node(int data) : data(data), next(this) {}
};

node* make_list(size_t nelts);
node* make_list(std::initializer_list<int> lst);

#endif
