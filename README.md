**Description:**

`blob2c` is a small utility for embedding arbitrary binary files directly into C programs.

The primary motivation for this project is portability. While Unix-like systems often provide the excellent `xxd` utility and its `-i` option, `xxd` is not guaranteed to be available on Windows systems and is commonly distributed as part of Vim or other third-party packages.

`blob2c` is a small, standalone, dependency-free replacement for `xxd -i` that can be built from a single portable C99 source file on Linux, macOS, and Windows.

The tool converts any binary file into a valid C99 byte array and accompanying length variable, making it easy to embed:

* images and icons;
* fonts;
* shaders;
* configuration files;
* arbitrary binary resources.

**Build:**

Compile with a standard C compiler:

```bash
cc -std=c99 -Wall -Wextra -pedantic -o blob2c blob2c.c
```


**Usage:**

```bash
blob2c logo.png > logo_png.h
```

Output:

```c
unsigned char logo_png[] = {
    0x89, 0x50, 0x4e, 0x47
};
unsigned int logo_png_len = 4;
```


The generated output is conceptually equivalent to:

```bash
xxd -i logo.png
```

but without requiring `xxd` to be installed.

**Embedding Fonts**

A common use case is embedding `TrueType` fonts directly into an executable.

Convert a font file into a `C` header:

`blob2c Roboto-Regular.ttf > roboto_regular_ttf.h`

This generates:

```c
unsigned char roboto_regular_ttf[] = {
    /* font data */
};
unsigned int roboto_regular_ttf_len = ...;
```

The generated header can then be included in a C program:

```c
#include "roboto_regular_ttf.h"
```

For example, many graphics libraries can load fonts directly from memory instead of reading them from disk:

```c
Font font = LoadFontFromMemory(
    ".ttf",
    Roboto_Regular_ttf,
    Roboto_Regular_ttf_len,
    32,
    NULL,
    0
);
```

Embedding fonts can simplify deployment by eliminating external resource files and producing a single self-contained executable.