#include "tests/test_allocator.h"
#include "tests/test_construct.h"
#include "tests/test_vector.h"
#include "tests/test_list.h"
#include "tests/test_deque.h"
using namespace tinystl::test;

int main(){
    //allocator_test::test_allocator();
    //construct_test::test_construct();
    //vector_test::test_vector();
    //list_test::test_list();
    deque_test::test_deque();
    return 0;
}
