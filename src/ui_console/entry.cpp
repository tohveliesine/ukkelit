#if _MSC_VER && _DEBUG
// Use Visual Leak Detector if compiling a debug build under Visual Studio.
// Requires it to be installed. Outputs any leak info to Output window.
// http://vld.codeplex.com/
#include <vld.h>
#endif

#include "Main.h"

int main() {
	Main main;

	main.run();

	return 0;
}