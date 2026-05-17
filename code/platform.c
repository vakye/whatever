
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
// NOTE(vak): Arguments and environment variables
// --------------------------------------------------

typedef struct
{
    string Arg;
    usize Count;
} arg_iterator;

typedef struct
{
    string Env;
    usize Count;
} env_iterator;

local arg_iterator IterateAllArg(void);
local void NextArg(arg_iterator* Iter);

local env_iterator IterateAllEnv(void);
local void NextEnv(env_iterator* Iter);

local string GetEnvironment(string TargetKey);

// --------------------------------------------------
// NOTE(vak): Window, input, desktop, ...
// --------------------------------------------------

local void MakeWindow(string Title, u32 Width, u32 Height);
local void DeleteWindow(void);

// --------------------------------------------------
// NOTE(vak): Console
// --------------------------------------------------

local usize OutputConsole(void* Data, usize Size);

// --------------------------------------------------
// NOTE(vak): Memory
// --------------------------------------------------

local void* ReserveMemory(usize Size);
local void CommitMemory(void* Memory, usize Size);
local void DecommitMemory(void* Memory, usize Size);
local void ReleaseMemory(void* Memory, usize Size);

// --------------------------------------------------
// NOTE(vak): Process control
// --------------------------------------------------

local void Exit(u8 ExitCode);
