
// --------------------------------------------------
// NOTE(vak): The main entry point on Linux that is
// responsible for calling Main().
// --------------------------------------------------

#include "shared.c"
#include "platform.c"
#include "main.c"

__attribute__((force_align_arg_pointer))
void LinuxEntry(void)
{
    Main();
    Exit(0);
}

// --------------------------------------------------
// NOTE(vak): Implementation of platform.c
// --------------------------------------------------

#define PROT_NONE  (0x00)
#define PROT_READ  (0x01)
#define PROT_WRITE (0x02)
#define PROT_EXEC  (0x04)

#define MAP_PRIVATE   (0x02)
#define MAP_ANONYMOUS (0x20)

#define STDIN_FILENO  (0x00)
#define STDOUT_FILENO (0x01)
#define STDERR_FILENO (0x02)

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
