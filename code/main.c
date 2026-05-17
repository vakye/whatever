
// --------------------------------------------------
// NOTE(vak): The quintessential Main() function.
// --------------------------------------------------

// --------------------------------------------------
// NOTE(vak): Dependencies:
//     + shared.c
//     + platform.c
// --------------------------------------------------

#pragma once

#include "print.c"

void Main(void)
{
    Print(Str("XDG_RUNTIME_DIR=")); Println(GetEnvironment(Str("XDG_RUNTIME_DIR")));
    Print(Str("USER="));            Println(GetEnvironment(Str("USER")));
    Print(Str("WAYLAND_DISPLAY=")); Println(GetEnvironment(Str("WAYLAND_DISPLAY")));

    Print(Str("Hello, world!\n"));
    PrintSInt(-1249); PrintNewLine();
    PrintSInt(1337);  PrintNewLine();
    PrintUInt(35872); PrintNewLine();
    PrintUInt(-1);    PrintNewLine();

    MakeWindow(Str("My Window"), 1600, 900);

    DeleteWindow();
}
