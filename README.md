# array_alg.h

Unintrusive algorithms for C arrays OR a C implementation of \<algorithm> from C++

## Pitch

The C++ STL is one of the most complete and reusable algorithm libraries available.
This single header file brings 80% of that functionality to C99 in a non-intrusive way.
There are no new data structures. Just include the library and call functions on C arrays.

Features:

- Sets (intersection, union, subset, etc)
- Heaps (priority queues)
- Binary search (lower bound, upper bound, etc)
- Sorts (insertion sort, quick sort, merge/stable sort, heap sort, partial sort, etc)
- Partitioning (partition, unique, etc)
- Predicates (all of, any of, none of, etc)
- Uniform random sampling and shuffling

## Usage

This library uses the preprocessor to implement generic functions.
Each time you include the library, you will need to define the array element type and a function prefix:

    #define ARRAY_ALG_TYPE int
    #define ARRAY_ALG_PREFIX intv_
    #include "array_alg.h"

The above will only generate the declarations.
In at least one C file, you will also need to generate implementations.
To generate implementations, define `ARRAY_ALG_IMPLEMENTATION` in a C file and include the library:

    #define ARRAY_ALG_TYPE int
    #define ARRAY_ALG_PREFIX intv_
    #define ARRAY_ALG_IMPLEMENTATION
    #include "array_alg.h"

Alternatively, you can include the algorithms as static functions to avoid the need for separate implementations:

    #define ARRAY_ALG_STATIC
    #define ARRAY_ALG_TYPE int
    #define ARRAY_ALG_PREFIX intv_
    #define ARRAY_ALG_IMPLEMENTATION
    #include "array_alg.h"

Repeat this process for each array type you want to use.

## Examples

Remove duplicate entries:

    #define ARRAY_ALG_TYPE int
    #define ARRAY_ALG_PREFIX intv_
    #include "array_alg.h"

    int compare_int(const int *a, const int *b, void *ctx) {
        return *a - *b;
    }

    ...

    int nums[100] = ...;
    intv_sort(nums, nums + 100, compare_int, NULL);
    int* end = intv_unique(nums, nums + 100, compare_int, NULL);


## Design

1. Iterators and Arrays

The C++ STL is designed around the concept of iterators.
With iterators, one algorithm can be reused not just for multiple types, but also for many data structures.
This is an ingenious design.
However, in practice, this capability is rarely needed.
The vast majority of real world \<algorithm> invocations are on contiguous arrays/vectors.

For those cases where you do have a fancy data structure (graphs, trees, etc),
copy its contents to an array, perform the algorithm, and then copy the contents back.
This will often help it perform better anyway!

2. Bounds vs counted ranges

STL algorithms typically operate on half-open ranges bounded by iterators [first, last).
This convention is not used as often in C, but we think it offers some benefits.
Internally, the functions can maintain less state by simply incrementing pointers
rather than keeping track of pointers, indices, and counts.

Operations also compose a little easier.
When a pointer is returned to an element of interest,
that same pointer can be used as an argument for another algorithm.

3. What's left out

Because it's a bit verbose to define a C closure (function pointer and context), some STL algorithms are less useful in C.
If an algorithm can be written as a simple for loop with no additional state or control flow, this library doesn't implement it.

    transform -> for (int i = 0; i < n; ++i) out[i] = f(in[i])
    fill -> for (int i = 0; i < n; ++i) out[i] = x;
    iota -> for (int i = 0; i < n; ++i) out[i] = i;
    generate -> for (int i = 0; i < n; ++i) out[i] = f();

The algorithms which rely on ordered types always require a comparison function.
We do not include any variants that operate on the `==` operator, as operators cannot be overloaded in C.

4. Generics vs `void*`

Including a header multiple times with various `#define`s is a little cumbersome.
However, we think it's a superior way to achieve C generics compared to the `void*` style used by `qsort` and `bsearch`.
The preprocessor approach provides:

- Better type safety and avoids verbose casting logic.

- Better peformance (as `void*` functions are difficult to optimize).

    Note: The C compiler can only create one non-inlined version of each function.
    For example, it could not choose to use `int` instructions, even if it knew the type at compile time.
    With the single header approach you get a new instance of each function optimized for each application.
