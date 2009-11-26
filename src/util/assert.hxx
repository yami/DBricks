#ifndef ASSERT_HXX
#define ASSERT_HXX

#include <cassert>


#define ASSERT assert
#define ASSERT_NOT_REACHED() ASSERT(0 && " should not be reached!")
#define ASSERT_NOT_IMPLEMENTED() ASSERT(0 && " not implemented yet!");

#endif  // ASSERT_HXX
