# Assassins Creed Startup Crash Fix (WIP)

This repository contains different methods to prevent Assassins Creed 1
from crashing at startup by "disabling" telemetry crap.

Currently there's only one method.

### Method 1: pass /notracking option
The game engine has a function that reads command-line options.
There's a "/notracking" option that seems to disable telemetry.
Problem is that this function is not being called from anywhere,
so this method works by injecting a call to this function into Engine::Init,
which makes it possible to pass "/notracking" option as a command-line argument
and "disable" telemetry.

Work-in-progress, so only supports DX10 1.02 version from GOG.

## License
This is free and unencumbered software released into the **public domain**.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
