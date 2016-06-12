## Synopsis

A faster C++ routine to convert 64 bit integers to ASCII form or std::string.

## Example Usage

Just include the source file `fast_int_to_string.hpp` into the translation unit.

In big data processing (e.g. columnar storage engines) even 10% improvement due to algorithmic/macro optimization results in cumulative effect (assuming other parts of the code is competitive enough). In my testing the conversion routine is 30-90% faster than standard counterparts.

## Testing

Added few unit and performance tests.

## License

[Apache 2](http://www.apache.org/licenses/LICENSE-2.0)

## Bugs?
Please report.
