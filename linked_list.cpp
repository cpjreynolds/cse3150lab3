#include <stdexcept>
#include <vector>

#include "linked_list.hpp"

node* make_list(size_t nelts)
{
    if (nelts < 1) {
        return nullptr;
    }
    node* root = new node(0);
    node* curr = root;
    for (size_t i = 1; i < nelts; ++i) {
        curr->next = new node(i);
        curr = curr->next;
    }
    return root;
}

node* make_list(std::initializer_list<int> lst)
{
    if (lst.size() == 0) {
        return nullptr;
    }
    const int* d = lst.begin();
    node* root = new node(*d++);
    node* curr = root;
    while (d != lst.end()) {
        curr->next = new node(*d++);
        curr = curr->next;
    }
    return root;
}

node* node::at(node* start, size_t idx)
{
    if (!start) {
        throw std::out_of_range("empty list");
    }
    node* tgt = start;
    for (size_t i = 0; i < idx; ++i) {
        // bounds check
        if (tgt == tgt->next) {
            throw std::out_of_range("index >= size");
        }
        tgt = tgt->next;
    }
    return tgt;
}

size_t node::size(node* start)
{
    if (!start) {
        return 0;
    }
    node* curr = start;
    size_t sz = 1;

    while (curr->next != curr) {
        ++sz;
        curr = curr->next;
    }
    return sz;
}

std::ostream& operator<<(std::ostream& os, const node* n)
{
    os << '{';
    while (n && n != n->next) {
        os << n->data << ", ";
        n = n->next;
    }
    if (n) {
        os << n->data;
    }
    return os << '}';
}

// performs the recursive step in `do_ptr_jump`
//
// stores a pointer to each visited node in `refs` so the nodes left dangling
// after jumping can be deleted.
static node* rec_jump(node* n, std::vector<node*>& refs)
{
    refs.push_back(n);
    if (n == n->next) {
        // we've hit the end
        return n;
    }
    else {
        // recurse
        n->next = rec_jump(n->next, refs);
        return n->next;
    }
}

// performs the pointer jumping algorithm recursively.
std::vector<node*> do_ptr_jump(node* start)
{
    // save references to the nodes that will dangle after jumping
    std::vector<node*> refs;
    if (!start) {
        return refs;
    }

    rec_jump(start, refs);

    return refs;
}

// given the list of nodes all pointing to the terminal node, correctly delete
// all the nodes.
//
// tested with valgrind
void do_jumped_delete(std::vector<node*>& lst)
{
    if (lst.size() == 0) {
        return;
    }
    node* terminal = lst[0]->next;
    for (node* n : lst) {
        if (n->next != terminal) {
            throw std::domain_error("deleting non-jumped list");
        }
        // ensure terminal is only deleted once.
        n->next = n;
        delete n;
    }
}

#ifdef TESTING
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>

// simply checks that all the nodes in a list point to a terminal node.
static bool verify_ptr_jump(std::vector<node*>& lst)
{
    if (lst.size() == 0) {
        return true;
    }
    node* terminal = lst[0]->next;
    for (node* n : lst) {
        if (n->next != terminal) {
            return false;
        }
    }
    return true;
}

TEST_CASE("verify_ptr_jump")
{
    // simple sanity check since I use this to verify other tests.
    node* terminal = new node(99);
    node* one = new node(1);
    node* two = new node(2);
    one->next = terminal;
    two->next = terminal;
    std::vector<node*> single{terminal};
    std::vector<node*> ptrs1{one, terminal};
    std::vector<node*> ptrs2{one, two, terminal};
    CHECK(verify_ptr_jump(single));
    CHECK(verify_ptr_jump(ptrs1));
    CHECK(verify_ptr_jump(ptrs2));
    do_jumped_delete(ptrs2);
}

TEST_CASE("do_ptr_jumping")
{
    SUBCASE("zero-element")
    {
        node* foo = make_list(0);
        auto refs = do_ptr_jump(foo);
        CHECK(verify_ptr_jump(refs));
        do_jumped_delete(refs);
    }
    SUBCASE("one-element")
    {
        node* foo = make_list(1);
        auto refs = do_ptr_jump(foo);
        CHECK(verify_ptr_jump(refs));
        do_jumped_delete(refs);
    }
    SUBCASE("two-element")
    {
        node* foo = make_list(2);
        auto refs = do_ptr_jump(foo);
        CHECK(verify_ptr_jump(refs));
        do_jumped_delete(refs);
    }
    SUBCASE("five-element")
    {
        node* foo = make_list(5);
        auto refs = do_ptr_jump(foo);
        CHECK(verify_ptr_jump(refs));
        do_jumped_delete(refs);
    }
}

TEST_CASE("node")
{
    SUBCASE("make_list(size_t)")
    {
        SUBCASE("zero-elts")
        {
            node* foo = make_list(0);
            CHECK(node::size(foo) == 0);
            CHECK_THROWS(node::at(foo, 0));
            CHECK_THROWS(node::at(foo, 1));
            delete foo; // superfluous but not harmful
        }
        SUBCASE("one-elt")
        {
            node* foo = make_list(1);
            CHECK(node::size(foo) == 1);
            CHECK(node::at(foo, 0)->data == 0);
            CHECK_THROWS(node::at(foo, 1));
            delete foo;
        }
        SUBCASE("two-elt")
        {
            node* foo = make_list(2);
            CHECK(node::size(foo) == 2);
            CHECK(node::at(foo, 0)->data == 0);
            CHECK(node::at(foo, 1)->data == 1);
            CHECK_THROWS(node::at(foo, 2));
            delete foo;
        }
        SUBCASE("five-elts")
        {
            node* foo = make_list(5);
            CHECK(node::size(foo) == 5);
            CHECK(node::at(foo, 0)->data == 0);
            CHECK(node::at(foo, 1)->data == 1);
            CHECK(node::at(foo, 2)->data == 2);
            CHECK(node::at(foo, 3)->data == 3);
            CHECK(node::at(foo, 4)->data == 4);
            CHECK_THROWS(node::at(foo, 5));
            delete foo;
        }
    }
    SUBCASE("make_list(initializer_list<int>)")
    {
        SUBCASE("zero-element")
        {
            node* foo = make_list({});
            CHECK(node::size(foo) == 0);
            CHECK_THROWS(node::at(foo, 0));
            CHECK_THROWS(node::at(foo, 1));
            delete foo;
        }
        SUBCASE("one-element")
        {
            node* foo = make_list({99});
            CHECK(node::size(foo) == 1);
            CHECK(node::at(foo, 0)->data == 99);
            CHECK_THROWS(node::at(foo, 1));
            delete foo;
        }
        SUBCASE("two-element")
        {
            node* foo = make_list({0xa, 0xb});
            CHECK(node::size(foo) == 2);
            CHECK(node::at(foo, 0)->data == 0xa);
            CHECK(node::at(foo, 1)->data == 0xb);
            CHECK_THROWS(node::at(foo, 2));
            delete foo;
        }
        SUBCASE("five-element")
        {
            node* foo = make_list({4, 3, 2, 1, 0});
            CHECK(node::size(foo) == 5);
            CHECK(node::at(foo, 0)->data == 4);
            CHECK(node::at(foo, 1)->data == 3);
            CHECK(node::at(foo, 2)->data == 2);
            CHECK(node::at(foo, 3)->data == 1);
            CHECK(node::at(foo, 4)->data == 0);
            CHECK_THROWS(node::at(foo, 5));
            delete foo;
        }
    }
    SUBCASE("node::at")
    {
        SUBCASE("zero-element")
        {
            node* foo = make_list(0);
            CHECK_THROWS(node::at(foo, 0));
            delete foo;
        }
        SUBCASE("one-element")
        {
            node* foo = make_list({21});
            CHECK(node::at(foo, 0)->data == 21);
            CHECK_THROWS(node::at(foo, 1));
            delete foo;
        }
        SUBCASE("two-element")
        {
            node* foo = make_list(2);
            CHECK(node::at(foo, 0)->data == 0);
            CHECK(node::at(foo, 1)->data == 1);
            CHECK_THROWS(node::at(foo, 2));
            delete foo;
        }
        SUBCASE("five-element")
        {
            node* foo = make_list({-100, 14, 2, 0, 0xbeef});
            CHECK(node::at(foo, 0)->data == -100);
            CHECK(node::at(foo, 1)->data == 14);
            CHECK(node::at(foo, 2)->data == 2);
            CHECK(node::at(foo, 3)->data == 0);
            CHECK(node::at(foo, 4)->data == 0xbeef);
            CHECK_THROWS(node::at(foo, 5));
            CHECK_THROWS(node::at(foo, 6));
            delete foo;
        }
    }
    SUBCASE("node::size")
    {
        SUBCASE("zero-element")
        {
            node* foo = make_list(0);
            node* bar = make_list({});
            CHECK(node::size(foo) == 0);
            CHECK(node::size(bar) == 0);
            delete foo;
            delete bar;
        }
        SUBCASE("one-element")
        {
            node* foo = make_list(1);
            CHECK(node::size(foo) == 1);
            delete foo;
        }
        SUBCASE("two-element")
        {
            node* foo = make_list(2);
            CHECK(node::size(foo) == 2);
            delete foo;
        }
        SUBCASE("five-element")
        {
            node* foo = make_list(5);
            CHECK(node::size(foo) == 5);
            delete foo;
        }
    }
    SUBCASE("node::operator<<")
    {
        using std::ostringstream;
        SUBCASE("zero-element")
        {
            node* foo = make_list(0);
            ostringstream out;
            out << foo;
            CHECK(out.str() == "{}");
            delete foo;
        }
        SUBCASE("one-element")
        {
            node* foo = make_list(1);
            ostringstream out;
            out << foo;
            CHECK(out.str() == "{0}");
            delete foo;
        }
        SUBCASE("two-element")
        {
            node* foo = make_list(2);
            ostringstream out;
            out << foo;
            CHECK(out.str() == "{0, 1}");
            delete foo;
        }
        SUBCASE("five-element")
        {
            node* foo = make_list(5);
            ostringstream out;
            out << foo;
            CHECK(out.str() == "{0, 1, 2, 3, 4}");
            delete foo;
        }
    }
}

#endif
