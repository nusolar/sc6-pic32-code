#include "nupp/buffer.hpp"

using namespace nu;

int StringStream::sync() {
	parent->puts(str().c_str()); // WARNING: Deep copy str
	this->str("");
	return 0;
}
