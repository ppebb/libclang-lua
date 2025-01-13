# libclang-lua

Lua bindings for libclang. Docs soon.

Hard fork of [luaclang-parser](https://github.com/mkottman/luaclang-parser/)
with most functions renamed, many functions added. Fills in some gaps of the
libclang api by accessing the internal C++ types directly.

## Compiling

This project requires Lua 5.4 and Clang. Libclang is build and installed
alongside the Clang compiler. These can be obtained from your distribution's
package manager.

This project requires CMake for compilation. Ensure you have CMake.

Then run the following from the project root to compile:
```bash
$ mkdir build; cd build
$ cmake ..
$ make
```

## Examples

While no examples are contained within this repository,
[cosmo-stub-generator](https://github.com/ppebb/cosmo-stub-generator) provides
an example of nearly every function in the project and their use. See
`src/generate.lua`.
