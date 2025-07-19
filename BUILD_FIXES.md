# Melted Build Fixes

This document describes fixes applied to resolve build issues with modern MLT library versions.

## Issue: Missing MLT Header Files

### Problem
When building Melted, you might encounter an error stating that MLT header files are not present in `/usr/local/include`. This typically manifests as:

```
fatal error: mlt/framework/mlt.h: No such file or directory
```

Or linking errors such as:
```
/usr/bin/ld: cannot find -lmlt: No such file or directory
```

### Root Cause
Modern Linux distributions (Ubuntu 22.04+) install MLT version 7 with versioned package names:
- Package config names are `mlt++-7` and `mlt-framework-7` instead of `mlt++` and `mlt-framework`
- Library names are `libmlt-7` and `libmlt++-7` instead of `libmlt` and `libmlt++`
- Development packages are required but not automatically installed

### Solution

#### 1. Install MLT Development Packages
```bash
sudo apt update
sudo apt install libmlt-dev libmlt++-dev
```

#### 2. Updated Package Config References
The following files have been updated to use versioned MLT package names:

**Core Configuration:**
- `src/melted++/configure`: Updated to use `mlt++-7`
- `src/examples/Makefile`: Updated to use `mlt++-7`
- `src/swig/configure`: Updated to use `mlt-framework-7`

**SWIG Language Bindings:**
- `src/swig/perl/Makefile.PL`: Updated to use `mlt++-7`
- `src/swig/ruby/build`: Updated to use `mlt++-7`
- `src/swig/python/build`: Updated to use `mlt++-7`
- `src/swig/tcl/build`: Updated to use `mlt++-7`
- `src/swig/java/build`: Updated to use `mlt++-7`
- `src/swig/php/build`: Updated to use `mlt++-7`

#### 3. Removed Hardcoded Library References
The following Makefiles had hardcoded `-lmlt` references that conflicted with the versioned libraries:

- `src/mvcp/Makefile`: Removed hardcoded `-lmlt` reference
- `src/modules/mvcp/Makefile`: Removed hardcoded `-lmlt` reference

The main configure script properly adds the correct versioned library flags to `config.mak`.

## Build Process

After applying these fixes, the standard build process works:

```bash
./configure
make
sudo make install
sudo ldconfig
```

## Verification

Test that the build was successful:

```bash
# Test the installed melted server
melted --help

# Should output:
# Usage: melted [-prio NNNN|max] [-test] [-port NNNN] [-c config-file]
```

The LADSPA plugin warning is normal if LADSPA audio plugins are not installed.

## Package Dependencies

**Required packages for building:**
- `build-essential`
- `libmlt-dev`
- `libmlt++-dev`

**Runtime dependencies:**
- `libmlt7`
- `libmlt++7`
- `libmlt-data`

## Compatibility

These fixes ensure compatibility with:
- Ubuntu 22.04 LTS and later
- MLT Framework version 7.x
- Modern pkg-config systems

## Legacy Compatibility

For systems with older MLT versions (6.x and earlier), the original unversioned package names may still work. However, these fixes are backward compatible and will work with both old and new MLT installations.

## Files Modified

### Configuration Scripts
- `src/melted++/configure`
- `src/swig/configure`

### Makefiles
- `src/examples/Makefile`
- `src/mvcp/Makefile`
- `src/modules/mvcp/Makefile`

### SWIG Build Scripts
- `src/swig/perl/Makefile.PL`
- `src/swig/ruby/build`
- `src/swig/python/build`
- `src/swig/tcl/build`
- `src/swig/java/build`
- `src/swig/php/build`

## Change Summary

| Component | Original Reference | Updated Reference |
|-----------|-------------------|-------------------|
| MLT++ pkg-config | `mlt++` | `mlt++-7` |
| MLT Framework pkg-config | `mlt-framework` | `mlt-framework-7` |
| Library linking | `-lmlt` (hardcoded) | Uses config.mak flags |

---

*Last updated: July 19, 2025*
*Applied to branch: chore/fix-build-issues*
