
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

local void Main(void)
{
    Print(Str("Hello, world!\n"));
    PrintSInt(-1249); PrintNewLine();
    PrintSInt(1337); PrintNewLine();
    PrintUInt(35872); PrintNewLine();
    PrintUInt(-1); PrintNewLine();
}
