#include "nu/platform/common_pragmas.h"
#include "nupp/bms.hpp"

/** Call BatteryMs::main(), NEVER RETURN */
int main(int argc, const char * argv[]){
	nu::BatteryMs::main();
    return 0;
}
