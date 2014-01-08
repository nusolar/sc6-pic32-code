#include "nupp/allocator.hpp"

using namespace nu;

size_t Arena::N = 0;
char *Arena::stack_base = NULL;
char *Arena::ptr = NULL;
