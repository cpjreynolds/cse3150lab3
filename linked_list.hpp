#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <ostream>
#include <vector>
#include <initializer_list>

struct node {
    int data;
    node* next;

    explicit node(int data) : data(data), next(this) {}

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
};

// redirects each node to point to the last node.
//
// returns a vector of pointers to every node so I don't lose references to them
// and leak the memory.
std::vector<node*> do_ptr_jumping(node* start);

// logic to delete the list after doing pointer jumping.
// ensures each node is deleted and the terminal node is only deleted once.
void do_jumped_delete(std::vector<node*>& lst);

node* make_list(size_t nelts);
node* make_list(std::initializer_list<int> lst);

#endif
