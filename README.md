# libclang-lua

Lua bindings for libclang. Docs soon.

This project is a hard fork of
[luaclang-parser](https://github.com/mkottman/luaclang-parser/) with most
functions renamed, and many functions added. It fills in some gaps of the
libclang api by accessing the internal C++ types directly.

## Compiling

This project requires Lua 5.4 and Clang. Libclang is usually built and
installed alongside the Clang compiler, but some distributions may have a
separate libclang-XX-dev package (see #1). These can be obtained from your
distribution's package manager.

Note that this project has only been tested with Clang-18, earlier versions may
not compile (versions 15 and below are missing required headers), and later
versions may have affected functionality. Your mileage may vary.

This project requires CMake for compilation. Ensure you have CMake >= 3.18.

Then run the following from the project root to compile:
```bash
$ mkdir build; cd build
$ cmake ..
$ make
```

## Type Definitions
There are included type definitions in `libclang-lua-types.lua` for use with
LuaLS. They will not be automatically picked up by LuaLS, so you will need to
point to them using `workspace.library` (see [LuaLS
docs](https://luals.github.io/wiki/definition-files/)).

Because libclang-lua is directly required as a shared object, LuaLS does not
know what type it returns, so annotate the require with `--- @type Clang`

## Examples

While no examples are contained within this repository,
[cosmo-stub-generator](https://github.com/ppebb/cosmo-stub-generator) provides
an example of nearly every function in the project and their use. See
`src/generate.lua`.
