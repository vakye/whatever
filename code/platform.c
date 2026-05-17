
// --------------------------------------------------
// NOTE(vak): Forward declarations of function
// definitions and structures that are platform-related.
// The implementation is contained inside platform
// files such as linux.c, ...
// --------------------------------------------------

// --------------------------------------------------
// NOTE(vak): Dependencies:
//     + shared.c
// --------------------------------------------------

#pragma once

// --------------------------------------------------
// NOTE(vak): Forward declarations
// --------------------------------------------------

// NOTE(vak): Console

local usize OutputConsole(void* Data, usize Size);

// NOTE(vak): Memory

local void* ReserveMemory(usize Size);
local void CommitMemory(void* Memory, usize Size);
local void DecommitMemory(void* Memory, usize Size);
local void ReleaseMemory(void* Memory, usize Size);

// NOTE(vak): Process control

local void Exit(u8 ExitCode);
