# ckdl forked for V

This is a fork of Tjol's [ckdl](https://github.com/tjol/ckdl) modified to work a
little bit better with V (i.e, renaming some struct fields) for
[vkdl](https://github.com/emmathemartian/vkdl).

This is temporary until there's a better solution that allows me to not have to
modify the C code of the library when binding from V.

## Differences

- Renamed `kdl_number`'s `string` to `str`
- Renamed `kdl_value`'s `string` to `str`

---

**Original readme:**

# ckdl - KDL reading and writing for C, Python, C++

**ckdl** is a C (C11) library that implements reading and writing the
[KDL Document Language](https://github.com/kdl-org/kdl).

ckdl provides a simple “streaming“ API, inspired by SAX, instead of reading the
entire document into a data structure. While this may be inconvenient to work
with directly for most applications, the idea is to enable **bindings for
different languages** which can then read the document into a suitable data
structure relatively directly.

This repository currently contains language bindings for:

- Python
- C++20

The C and C++ parts are built with CMake.

To install the Python bindings, run

    pip install ckdl

For more details about how to build and use ckdl, check out the
[documentation on RTD](https://ckdl.readthedocs.io/en/latest/index.html) or
under `doc/` in this repository.

### Status

ckdl has full support for **KDL 2.0.0** and **KDL 1.0.0**. The parser passes
the upstream test suite.

By default, the parser will allow documents of either KDL version. You can
restrict the parser to one version or the other using parse options.
