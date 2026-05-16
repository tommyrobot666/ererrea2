#include <cstdlib>
#include <core/testAlertBeep.h>
// Source - https://stackoverflow.com/a/42963277
// Posted by ndrewxie
// Retrieved 2026-03-25, License - CC BY-SA 3.0

#ifdef WINDOWS
#include <Windows.h>
void beep() {
    Beep(440, 1000);
}
#elif LINUX
#include <cstdlib>
void beep() {
    std::system("echo -e "\007" >/dev/tty10");
}
#else
#include <iostream>
void beep() {
    std::cout << "\a" << std::flush;
}
#endif
