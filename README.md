# interval-map

A simple container wrapper over std::map, allowing you to keep track of interval
values without extra memory allocation.
Allows you to extend, merge and overlap intervals.

```
git submodule update --init  # for googletest submodule in external folder,
                             # in case you would like to build tests.

mkdir build
cd build

cmake ..  # -DBUILD_TESTS=ON in case you would like to build tests.
make
```
