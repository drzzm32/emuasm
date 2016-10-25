#define WINDOWS
//#define LINUX
//#define ARDUINO

#include "emuasm.h"

int emuasm(int argc, char* argv) {
	int a = 1, b = 2;
	print("Line %d\nLine %d\n", a, b);
	char tmp[10], head[10], body1[10], body2[10], body3[10];
	print("Type: ");
	scan(tmp);
	print("You typed: %s\n", tmp);

	return 0;
}
