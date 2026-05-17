
// --------------------------------------------------
// NOTE(vak): Contains printing functions for strings,
// integers, floats, ...
// --------------------------------------------------

// --------------------------------------------------
// NOTE(vak): Dependencies:
//     + shared.c
//     + platform.c
// --------------------------------------------------

#pragma once

// --------------------------------------------------
// NOTE(vak): Definitions
// --------------------------------------------------

local usize PrintCharacter(char Character);

local usize PrintNewLine(void);

local usize Print(string Message);

local usize Println(string Message);

local usize PrintSInt(ssize Integer);

local usize PrintUInt(usize Integer);

// --------------------------------------------------
// NOTE(vak): Implementation
// --------------------------------------------------

local usize PrintCharacter(char Character)
{
    usize Result = OutputConsole(&Character, 1);
    return (Result);
}

local usize PrintNewLine(void)
{
    usize Result = PrintCharacter('\n');
    return (Result);
}

local usize Print(string Message)
{
    usize Result = OutputConsole(Message.Data, Message.Size);
    return (Result);
}

local usize Println(string Message)
{
    usize Result = 0;

    Result += Print(Message);
    Result += PrintNewLine();

    return (Result);
}

local usize PrintSInt(ssize Integer)
{
    usize Result = 0;

    if (Integer < 0)
    {
        Result += PrintCharacter('-');
        Integer = -Integer;
    }

    Result += PrintUInt(Integer);

    return (Result);
}

local usize PrintUInt(usize Integer)
{
    char Buffer[32];
    usize DigitCount = 0;
    usize DigitIndex = ArrayCount(Buffer);

    do
    {
        usize Digit = Integer % 10;
        Integer /= 10;

        DigitCount++;
        DigitIndex--;

        Buffer[DigitIndex] = (char)(Digit + '0');
    } while (Integer);

    usize Result = Print(StrData(Buffer + DigitIndex, DigitCount));
    return (Result);
}
