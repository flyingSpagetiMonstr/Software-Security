#pragma code_seg("XXX XXX")

#include <windows.h>
#include <stdio.h>

void fun()
// int main() {
//     HMODULE hDLL = LoadLibrary("user32.dll"); // Load the user32.dll library as an example

//     if (hDLL != NULL) {
//         // Function pointer to the MessageBoxA function in user32.dll
//         int (*MessageBoxA)(HWND, LPCSTR, LPCSTR, UINT);
//         MessageBoxA = (int (*)(HWND, LPCSTR, LPCSTR, UINT))GetProcAddress(hDLL, "MessageBoxA");

//         if (MessageBoxA != NULL) {
//             // Use the function pointer to call MessageBoxA
//             MessageBoxA(NULL, "Hallo!", "Message", MB_OK);
//         }

//         FreeLibrary(hDLL); // Unload the library
//     }

//     return 0;
// }