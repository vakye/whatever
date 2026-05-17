
// --------------------------------------------------
// NOTE(vak): Included in linux.c. Implements window,
// desktop, and input for the Wayland window manager.
// --------------------------------------------------

#pragma once

// NOTE(vak): !! WORK IN PROGRESS !!

typedef struct
{
    s32 Display;
} wayland_context;

local wayland_context Wayland = {0};

local string WaylandSearchEnvironment(string Name)
{
    string Result = {0};

    return (Result);
}

local void WaylandConnectDisplay(void)
{

}

local void MakeWindow(string Title, u32 Width, u32 Height)
{
    WaylandConnectDisplay();
}

local void DeleteWindow(void)
{

}
