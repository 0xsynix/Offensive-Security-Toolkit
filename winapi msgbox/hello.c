#include <Windows.h>

int main(void)
{
    MessageBoxW(
        NULL,
        L"My First Message Box!",
        L"Hello Wordl!",
        MB_YESNOCANCEL | MB_ICONEXCLAMATION
    );
    return EXIT_SUCCESS;
}