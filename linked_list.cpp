#include <stdexcept>

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
            throw std::out_of_range("bad index");
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

#ifdef TESTING
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>

using std::ostringstream;

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
    SUBCASE("node::size")
    {
        node* foo = make_list(10);
        node* bar = make_list({10, 9, 8, 7, 6, 5, 4, 3, 2, 1});
        node* baz = make_list(0);
        node* buz = make_list({});
        CHECK(node::size(foo) == 10);
        CHECK(node::size(bar) == 10);
        CHECK(node::size(baz) == 0);
        CHECK(node::size(buz) == 0);
        delete foo;
        delete bar;
        delete baz;
        delete buz;
    }
    SUBCASE("node::operator<<")
    {
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
