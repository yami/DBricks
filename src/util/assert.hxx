#ifndef ASSERT_HXX
#define ASSERT_HXX

#include <cassert>


#define ASSERT assert
#define ASSERT_NOT_REACHED() ASSERT(0 && " should not be reached!")

#endif  // ASSERT_HXX
