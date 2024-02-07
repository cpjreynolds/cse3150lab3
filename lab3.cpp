#ifndef TESTING

#include "linked_list.hpp"
#include <iostream>

// prints a node and its pointed-to value
static void print_node(node* n)
{
    std::cout << "(" << n->data << ")"
              << "\t=>\t"
              << "(" << n->next->data << ")" << std::endl;
}

// prints the nodes and pointed-to values in a list
static void print_trace(node* n)
{
    while (n != n->next) {
        print_node(n);
        n = n->next;
    }
    print_node(n);
}

// prints the list trace after doing pointer jumping
static void print_post_trace(const std::vector<node*>& lst)
{
    for (node* n : lst) {
        print_node(n);
    }
}

int main(int argc, char** argv)
{
    node* lst = make_list(
        {1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7});
    print_trace(lst);
    auto refs = do_ptr_jumping(lst);
    std::cout << std::endl << "[do pointer jumping]" << std::endl << std::endl;
    print_post_trace(refs);
    do_jumped_delete(refs);
}

#endif
