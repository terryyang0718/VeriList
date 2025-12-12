# VeriList ⚡

A C++ tool to parse, organize, and process Verilog file lists with include directories, environment variables, and package dependencies.  
VeriList generates a topologically sorted file list for compilation while providing warnings for missing imports.

---

## Features ✨

- Parse Verilog file lists with support for `+incdir+` and environment variables (`$VAR` / `${VAR}`).
- Handle Verilog `package` and `endpackage` declarations.
- Generate warnings for missing files or unresolved imports.
- Output a `.lst` file containing:
  - Unknown commands
  - Include directories
  - Verilog source files in topological order
- Automatically resolves dependencies between packages.

---

## Dependencies ⚙️

- C++11 or above
- Standard libraries: `<bits/stdc++.h>`, `<unistd.h>`, `<sys/types.h>`, `<sys/stat.h>`, `<dirent.h>`

---

## Installation 🚀

Compile the program:

```bash
g++ -o verilist main.cpp -std=c++11
