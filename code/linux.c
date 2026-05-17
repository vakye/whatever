
// --------------------------------------------------
// NOTE(vak): The main entry point on Linux that is
// responsible for calling Main().
// --------------------------------------------------

#include "shared.c"
#include "platform.c"
#include "main.c"

typedef struct
{
    usize  ArgCount;
    char** Args;

    usize  EnvCount;
    char** Envp;
} linux_context;

local linux_context Linux = {0};

__attribute__((naked))
void LinuxEntry(void)
{
    // NOTE(vak): x86-64 Linux Entry Point

    __asm volatile (
        // NOTE(vak): When execve is called, the initial
        // stack layout of the LinuxEntry() looks like this:

        // [rsp]:                      ArgCount
        // [rsp + 8]:                  Args[0]
        // ...
        // ...
        // [rsp + 8 + 8*ArgCount]:     Args[n] Null terminator
        // [rsp + 8 + 8*ArgCount + 8]: Envp[0]
        // ...
        // ...
        // [rsp + ???]:                Envp[n] null terminator

        // NOTE(vak): We need to retrieve those parameters
        // and put them into variables that we can easily
        // access for future use.

        // ArgCount = [rsp]
        // Args     = rsp + 8
        // Envp     = rsp + 8 + 8*ArgCount + 8

        // NOTE(vak): ArgCount
        "movq (%%rsp), %%rax\n"
        "movq %%rax, %0\n"

        // NOTE(vak): Args
        "leaq 8(%%rsp), %%rdi\n"
        "movq %%rdi, %1\n"

        // NOTE(vak): Envp
        "leaq 16(%%rsp, %%rax, 8), %%rdi\n"
        "movq %%rdi, %2\n"

        // NOTE(vak): Find the Envp null terminator and
        // calculate EnvCount

        "xorq %%rax, %%rax\n"

        ".Next:\n"
        "    cmpq $0, (%%rdi, %%rax, 8)\n"
        "    jz .Finished\n"
        "    inc %%rax\n"
        "    jmp .Next\n"

        ".Finished:\n"

        // NOTE(vak): EnvCount
        "movq %%rax, %3\n"

        // NOTE(vak): Before calling Main(), we need to
        // create atleast 128 bytes of stack space and
        // align the stack pointer to a 16-byte boundary.

        "sub  $128, %%rsp\n"
        "andq $-16, %%rsp\n"

        // NOTE(vak): Finally call Main()

        "call Main\n"

        // NOTE(vak): Exit(0)

        "movq $60, %%rax\n"
        "xorq %%rdi, %%rdi\n"
        "syscall\n"

        :: "m"(Linux.ArgCount), "m"(Linux.Args), "m"(Linux.Envp), "m"(Linux.EnvCount)
    );
}

// --------------------------------------------------
// NOTE(vak): Implementation of platform.c
// --------------------------------------------------

// NOTE(vak): Memory protection flags

#define PROT_NONE  (0x00)
#define PROT_READ  (0x01)
#define PROT_WRITE (0x02)
#define PROT_EXEC  (0x04)

// NOTE(vak): Memory mapping property flags

#define MAP_PRIVATE   (0x02)
#define MAP_ANONYMOUS (0x20)

// NOTE(vak): Standard console file descriptor values

#define STDIN_FILENO  (0x00)
#define STDOUT_FILENO (0x01)
#define STDERR_FILENO (0x02)

// NOTE(vak): Syscall

enum
{
    Syscall_Write    = 1,
    Syscall_MMap     = 9,
    Syscall_MProtect = 10,
    Syscall_MUnmap   = 11,
    Syscall_Exit     = 60,
};

local usize LinuxSyscall(
    usize Number,
    usize A,
    usize B,
    usize C,
    usize D,
    usize E,
    usize F)
{
    usize Result = 0;

    // NOTE(vak): x86-64 Linux Syscall

    register usize R10 __asm ("r10") = D;
    register usize R9  __asm ("r9")  = E;
    register usize R8  __asm ("r8")  = F;

    __asm volatile (
        // NOTE(vak): Perform a syscall instruction with the constraints
        // listed below.
        "syscall" :

        // NOTE(vak): Retrieve the result from rax.
        "=a"(Result) :

        // NOTE(vak): Move the corresponding arguments into their
        // registers before performing the syscall.
        "a"(Number),
        "D"(A),
        "S"(B),
        "d"(C),
        "r"(R10),
        "r"(R8),
        "r"(R9) :

        // NOTE(vak): The kernel may clobber register rcx, r11,
        // and may also perform memory accesses.
        "rcx", "r11", "memory"
    );

    return (Result);
}

// NOTE(vak): Arguments

local arg_iterator IterateAllArg(void)
{
    arg_iterator Result =
    {
        .Arg = CString(Linux.Args[0]),
        .Count = Linux.ArgCount,
    };

    return (Result);
}

local void NextArg(arg_iterator* Iter)
{
    if (Iter->Count)
        Iter->Count--;

    if (Iter->Count)
        Iter->Arg = CString(Linux.Args[Linux.ArgCount - Iter->Count]);
}

// NOTE(vak): Environment variables

local env_iterator IterateAllEnv(void)
{
    env_iterator Result =
    {
        .Env = CString(Linux.Envp[0]),
        .Count = Linux.EnvCount,
    };

    return (Result);
}

local void NextEnv(env_iterator* Iter)
{
    if (Iter->Count)
        Iter->Count--;

    if (Iter->Count)
        Iter->Env = CString(Linux.Envp[Linux.EnvCount - Iter->Count]);
}

local string GetEnvironment(string TargetKey)
{
    string Result = {0};

    for (env_iterator Iter = IterateAllEnv(); Iter.Count; NextEnv(&Iter))
    {
        // NOTE(vak): Retrieve a key-value pair in the form
        // of "Key" '=' "Value" from the environment variable
        // list.

        string KeyValue = Iter.Env;

        // NOTE(vak): Determine the Key and Name by searching
        // for the equal symbol '='.

        usize KeySize = 0;

        for (usize Scan = 0; Scan < KeyValue.Size; Scan++)
        {
            if (KeyValue.Data[Scan] == '=')
            {
                KeySize = Scan;
                break;
            }
        }

        // NOTE(vak): Match

        string Key   = ViewString(KeyValue, 0,           KeySize);
        string Value = ViewString(KeyValue, KeySize + 1, KeyValue.Size);

        if (StringEqual(Key, TargetKey))
        {
            Result = Value;
            break;
        }
    }

    return (Result);
}

// NOTE(vak): Window

#include "wayland.c"

// NOTE(vak): Console

local usize OutputConsole(void* Data, usize Size)
{
    ssize Error = LinuxSyscall(
        Syscall_Write,
        STDOUT_FILENO,
        (usize)Data,
        Size,
        0, 0, 0
    );

    usize Result = Maximum(0, Error);

    return (Result);
}

// NOTE(vak): Memory

local void* ReserveMemory(usize Size)
{
    ssize Error = LinuxSyscall(
        Syscall_MMap,
        0, Size,
        PROT_NONE,
        MAP_ANONYMOUS | MAP_PRIVATE,
        -1,
        0
    );

    Assert(Error > 0);

    void* Result = (void*)Error;

    return (Result);
}

local void CommitMemory(void* Memory, usize Size)
{
    usize Error = LinuxSyscall(
        Syscall_MProtect,
        (usize)Memory,
        Size,
        PROT_READ | PROT_WRITE,
        0, 0, 0
    );

    Assert(!Error);
}

local void DecommitMemory(void* Memory, usize Size)
{
    usize Error = LinuxSyscall(
        Syscall_MProtect,
        (usize)Memory,
        Size,
        PROT_NONE,
        0, 0, 0
    );

    Assert(!Error);
}

local void ReleaseMemory(void* Memory, usize Size)
{
    usize Error = LinuxSyscall(
        Syscall_MUnmap,
        (usize)Memory,
        Size,
        0, 0, 0, 0
    );

    Assert(!Error);
}

// NOTE(vak): Process control

local void Exit(u8 ExitCode)
{
    LinuxSyscall(Syscall_Exit, ExitCode, 0, 0, 0, 0, 0);
}

// --------------------------------------------------
// NOTE(vak): CRT stuff
// --------------------------------------------------

void* memset(void* DestInit, s32 Byte, usize Size)
{
    u8* Dest = (u8*)DestInit;
    while (Size--) *Dest++ = (u8)Byte;

    return (DestInit);
}
