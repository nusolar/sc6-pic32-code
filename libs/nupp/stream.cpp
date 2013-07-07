#include "nupp/stream.hpp"

nu::OStream& nu::end(nu::OStream& os) {
	os.puts(os._str.c_str());
	os._str.clear();
	return os;
}

void nu::OStream::printf(const char *fmt, ...) {
	char buffer[72];
	va_list fmtargs;
	va_start(fmtargs, fmt);
	if (likely(vsnprintf(NULL, 0, fmt, fmtargs) >= 0)) {
		vsprintf(buffer, fmt, fmtargs);
		puts((char *)buffer);
	}
	va_end(fmtargs);
}
