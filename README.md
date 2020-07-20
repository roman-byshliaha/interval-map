# interval-map

A simple container wrapper over std::map, allowing you to keep track of interval
values without extra memory allocation.
Allows you to extend, merge and overlap intervals.

```
# For googletest submodule in external folder, in case you would like to build
# tests.
git submodule update --init

mkdir build
cd build

# Add '-DBUILD_TESTS=ON' in case you would like to build tests.
cmake ..

# Since library is header-only, make is needed to build test only.
make
```
