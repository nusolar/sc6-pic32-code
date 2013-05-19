#include "nupp/buffer.hpp"

nu::OStream& nu::end(nu::OStream& os) {
	os.puts(os.out.str().c_str()); // WARNING: Deep copy str
	os.out.str("");
	return os;
}