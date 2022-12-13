#include "tests/test_allocator.h"
#include "tests/test_construct.h"
using namespace tinystl::test;

int main(){
    allocator_test::test_allocator();
    construct_test::test_construct();
    return 0;
}
