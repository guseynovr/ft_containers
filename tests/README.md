## Tests
In order to run tests:
1. Download googletest and prepare the build system: `cmake -S . -B build`
2. Run the build system and wait for it to complete: `cmake --build build`
3. Run tests: `./build/stl_test`

To run selected tests:
1. List all tests: `./build/stl_test --gtest_list_tests`
2. Run and select tests:  `./build/stl_test --gtest_filter=...[*]`
