#define WINDOWS 1
//#define LINUX 1
//#define ARDUINO

#include "emuasm.h"

int emuasm(int argc, char* argv) {
	int a = 1, b = 2;
	print("Line %d\nLine %d\n", a, b);
	char tmp[10];
	scan(tmp);
	print("You typed: %s\n", tmp);
	
	return 0;
}
