#include "nu/platform/common_pragmas.h"
#include "nupp/bms.hpp"

using namespace std;
using namespace nu;

int main(int argc, const char * argv[]){
	WDT::enable_clear();
	BatteryMs bms{};
	while (true) {
		bms.run();
	}
    return 0;
}
