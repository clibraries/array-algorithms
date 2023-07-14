/*
Unintrusive algorithms for C arrays
OR, a C implementation of <algorithm> from C++.

https://github.com/clibraries.
License: MIT (see bottom).

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

Alternatively, add `#define ARRAY_ALG_STATIC` before the original declaration
to avoid the need for separate implementations.

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

## Changelog

TODO:

- stable_partition
- set_symmetric_difference
- merge_inplace

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef ALGDEF
#ifdef ARRAY_ALG_STATIC
#define ALGDEF static
#else
#define ALGDEF extern
#endif
#endif

#ifndef ARRAY_ALG_TYPE
#error "Please define a type. For example int"
#endif

#ifndef ARRAY_ALG_PREFIX
#error "Please define a prefix. For example int_array_"
#endif

#ifndef ARRAY_ALG_RANDOM
#define ARRAY_ALG_RANDOM(_n) (rand() % (_n))
#endif

#define NAME2(prefix, fun) prefix ## fun
#define NAME1(prefix, fun) NAME2(prefix, fun)
#define NS(fun) NAME1(ARRAY_ALG_PREFIX, fun)

#define T ARRAY_ALG_TYPE


/// Find the first element satisfying a predicate.
ALGDEF T *NS(find_if)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        );

ALGDEF T *NS(find_if_not)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        );

/// Like find_if, but does not require an end pointer.
/// requires:
/// - the predicate must be true for some value [first, ...).

ALGDEF T *NS(find_if_unguarded)(
        const T *first,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        );

/// Like find_if_not, but does not require an end pointer.
/// requires:
/// - the predicate must be false for some value [first, ...)]

ALGDEF T* NS(find_if_not_unguarded)(
        const T *first,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        );

/// Find the last element satisfying a predicate.
ALGDEF T *NS(find_last_if)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        );

ALGDEF int NS(any_of)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        );

ALGDEF int NS(all_of)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        );

ALGDEF int NS(none_of)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        );

ALGDEF size_t NS(count_if)(
        const T* first,
        const T* last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        );

ALGDEF void NS(mismatch)(
        const T **first_1,
        const T *last_1,
        const T **first_2,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
        );

ALGDEF T *NS(adjacent_find)(
        const T *first,
        const T *last,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
        );

ALGDEF T *NS(copy)(
        const T *first,
        const T *last,
        T *out
        );

ALGDEF T *NS(copy_n)(
        const T *first,
        size_t n,
        T *out
        );

ALGDEF T *NS(copy_if)(
        const T *first,
        const T *last,
        T *out,
        int (*predicate)(const T*,  void*),
        void* predicate_ctx
        );

ALGDEF T *NS(copy_backward)(
        const T *first,
        const T *last,
        T *last_out
        );

ALGDEF int NS(lexicographical_compare)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        const T *last_2,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF int NS(equal)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF void NS(swap)(T *a, T *b);

ALGDEF T *NS(swap_ranges)(
        T *first_1,
        T *last_1,
        T *first_2
        );

ALGDEF void NS(reverse)(
        T *first,
        T *last
        );

ALGDEF T *NS(reverse_copy)(
        const T *restrict first,
        const T *restrict last,
        T *out
        );
/// requires:
/// - is_sorted(first_1, last_1)
/// - is_sorted(first_2, last_2)

ALGDEF T *NS(merge)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        const T *last_2,
        T *restrict out,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

/// requires:
/// - is_sorted(first, middle)
/// - is_sorted(middle, last)

ALGDEF void NS(merge_with_buffer)(
        T *first,
        T *middle,
        T *last,
        T *buffer,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

ALGDEF T *NS(remove_if)(
        T *first,
        T *last,
        int (*pred)(const T*, void*),
        void *pred_ctx
        );

ALGDEF T *NS(remove_if_not)(
        T *first,
        T *last,
        int (*pred)(const T*, void*),
        void *pred_ctx
        );

ALGDEF T *NS(replace_if)(
        T *first,
        T *last,
        const T *replacement,
        int (*pred)(const T*, void*),
        void *pred_ctx
        );

/// requires:
/// - is_sorted(first, last)

ALGDEF T *NS(unique)(
        T *first,
        T *last,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

/// requires:
/// - is sorted(first, last)
ALGDEF T *NS(unique_copy)(
        T *restrict first,
        T *last,
        T *restrict out,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF size_t NS(unique_count)(
        const T* first,
        const T* last,
        int (*cmp)(const T*, const T*, void*),
        void *cmp_ctx
        );

ALGDEF T *NS(fill)(
        T *first,
        T *last,
        const T *x
        );

ALGDEF T *NS(fill_n)(
        T *first,
        size_t count,
        const T *x
        );

ALGDEF int NS(set_includes)(
        const T *first_sub,
        const T *last_sub,
        const T *first_super,
        const T *last_super,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF T *NS(set_union)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        const T *last_2,
        T *restrict out,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF T *NS(set_difference)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        const T *last_2,
        T *out,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF T *NS(set_intersection)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        const T *last_2,
        T *out,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF T *NS(min)(
        const T* a,
        const T* b,
        int (*cmp)(const T*, const T*, void*),
        void* ctx
        );

ALGDEF T *NS(max)(
        const T *a,
        const T *b,
        int (*cmp)(const T*, const T*, void*),
        void* ctx
        );

ALGDEF T *NS(min_element)(
        const T *first,
        const T *last,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF T *NS(max_element)(
        const T *first,
        const T *last,
        int (*cmp)(const T*, const T*, void*),
        void* ctx
        );

/// Selecting the min and max element at the same time is more efficient
/// than selecting them separately.
ALGDEF void NS(minmax_element)(
        const T *first,
        const T *last,
        T** out_min,
        T** out_max,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

/// A range is partitioned if the predicate is true for the first n elements, and false for the rest:
/// For example: T T .. T F ... F
/// The predicate may be true or false on the entire range.
ALGDEF int NS(is_partitioned)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        );

ALGDEF T *NS(partition)(
        T *first,
        T *last,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        );

ALGDEF T *NS(partition_copy)(
        const T *first,
        const T *last,
        T *restrict out_false,
        T *restrict out_true,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        );

ALGDEF T *NS(partition_point_n)(
        const T *first,
        size_t n,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        );

ALGDEF T *NS(partition_point)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        );

ALGDEF T *NS(is_sorted_until)(
        const T* first,
        const T* last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

ALGDEF int NS(is_sorted)(
        const T *first,
        const T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

ALGDEF T *NS(lower_bound)(
        const T *first,
        const T *last,
        const T* value,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF T *NS(upper_bound)(
        const T *first,
        const T *last,
        const T *value,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF int NS(binary_search)(
        const T *first,
        const T *last,
        const T *value,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF void NS(equal_range)(
        const T *first,
        const T *last,
        const T *value,
        T **lower,
        T **upper,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

ALGDEF int NS(next_permutation)(
        T *first,
        T *last,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        );

/// See random_shuffle.
ALGDEF void NS(random_shuffle_n)(
        T *first,
        size_t n
        );

ALGDEF void NS(random_shuffle)(
        T *first,
        T *last
        );

/// Select count random elements from the range [first, last).
/// Each element has an equal probability of being selected.
/// Each element will be selected at most once.
ALGDEF T *NS(sample)(
        const T *first,
        const T *last,
        T* out,
        size_t count
        );

ALGDEF T *NS(is_heap_until)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void *),
        void* compare_ctx
        );

ALGDEF int NS(is_heap)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void *),
        void* compare_ctx
        );

/// See push_heap
ALGDEF void NS(push_heap_n)(
        T *first,
        size_t count,
        int (*compare)(const T*, const T*, void *),
        void* compare_ctx
        );

/// The item at the end of the range: (last - 1)
/// Will be inserted into the heap [first, last - 1)
/// [first, last) will then be a heap.
/// requires:
///   - is_heap(first, last)

ALGDEF void NS(push_heap)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void *),
        void *compare_ctx
        );


/// See pop_heap
ALGDEF void NS(pop_heap_n)(
        T *first,
        size_t count,
        int (*compare)(const T*, const T*, void *),
        void *compare_ctx
        );

/// The largest item in the heap will be moved to the end: (last - 1).
/// The remaining range [first, last - 1) will be a heap.
/// requires:
///    - is_heap(first, last - 1)
ALGDEF void NS(pop_heap)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void *),
        void *compare_ctx
        );

/// Create max heap from the range [first, first + count).
ALGDEF void NS(make_heap_n)(
        T *first,
        size_t count,
        int (*compare)(const T*, const T*, void *),
        void *compare_ctx
        );

/// Create max heap from the range [first, last).
/// requires:
/// - is_heap(first, last)
ALGDEF void NS(make_heap)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

/// Given a heap, reorder elements until they are sorted.
ALGDEF void NS(sort_heap)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

ALGDEF void NS(insertion_sort)(
        T *first,
        T *last,
        int (*compare)(const T* a, const T* b, void*),
        void* compare_ctx
        );

ALGDEF void NS(sort)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

ALGDEF void NS(insertion_sort_stable)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

/// Sort an array in a stable way with a merge sort variety.
/// By stable, we mean that the order of equivalent elements is preserved.
/// Note this function allocates memory with malloc.

ALGDEF void NS(stable_sort)(
        T* first,
        T* last,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
        );

/// Like above, but does not call `malloc`.
/// You provide a buffer.`
/// requires:
/// - sizeof(buffer) >= (last - first) / 2
ALGDEF void NS(stable_sort_with_buffer)(
        T* first,
        T* last,
        T* buffer,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
        );

ALGDEF void NS(partial_sort)(
        T *first,
        T *middle,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

ALGDEF T *NS(partial_sort_copy)(
        const T *first,
        const T *last,
        T *out_first,
        T *out_last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

ALGDEF void NS(nth_element)(
        T *first,
        T *nth,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
);

/// See is_strictly_increasing
ALGDEF T *NS(is_strictly_increasing_until)(
        const T *first,
        const T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

/// A sequence is strictly increasing when
/// compare(p, p+1) < 0 for all valid pointers in the range.
ALGDEF int NS(is_strictly_increasing)(
        const T *first,
        const T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

/// Insert n elements at first.
/// The elements at [first, last) will first be shifted up
/// to [first + n, last + n) to make room.
///
/// requires:
/// - [first, last + n) is a valid range.

ALGDEF T* NS(insert_n)(
        T *first,
        T *last,
        const T *to_insert_first,
        size_t n
        );

// == PRIVATE ==

ALGDEF void NS(_insertion_sort_unguarded)(
        T *restrict first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        );

ALGDEF void NS(_rotate_right_by_one)(
        T *first,
        T *last
        );

/// Hoare partitioning:
/// https://en.wikipedia.org/wiki/Quicksort#Hoare_partition_scheme
///
/// Partially sorts [first, last) into two parts, divided
/// by a pointer m such that:
/// for all a in [first, m) and b in [m, last) a <= b.
///
/// requires:
/// - first != last
ALGDEF T *NS(_sort_partition)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
        );

ALGDEF T *NS(_quick_sort_early_stop)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
        );

ALGDEF T *NS(_merge_sort_adaptive_with_buffer_n)(
        T* first,
        size_t count,
        T* buffer,
        int (*compare)(const T* a, const T* b, void*),
        void* compare_ctx
        );

#ifdef ARRAY_ALG_IMPLEMENTATION

ALGDEF T *NS(find_if)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        ) {

    while (first != last) {
        if (predicate(first, predicate_ctx)) break; 
        ++first;
    }
    return (T*)first;
}

ALGDEF T *NS(find_if_not)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        ) {

    while (first != last) {
        if (!predicate(first, predicate_ctx)) break; 
        ++first;
    }
    return (T*)first;
}

ALGDEF T *NS(find_if_unguarded)(
        const T *first,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        ) {

    while (!predicate(first, predicate_ctx)) ++first;
    return (T*)first;
}

ALGDEF T* NS(find_if_not_unguarded)(
        const T *first,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        ) {

    while (predicate(first, predicate_ctx)) ++first;
    return (T*)first;
}

ALGDEF T *NS(find_last_if)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        ) {

    const T *found = last;

    while (first != last) {
        if (predicate(first, predicate_ctx)) {
            found = first;
        }
        ++first;
    }
    return (T*)found;
}

ALGDEF int NS(any_of)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        ) {
    return NS(find_if)(first, last, predicate, predicate_ctx) != last;
}

ALGDEF int NS(none_of)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        ) {
    return NS(find_if)(first, last, predicate, predicate_ctx) == last;
}


ALGDEF int NS(all_of)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        ) {
    return NS(find_if_not)(first, last, predicate, predicate_ctx) == last;
}

ALGDEF size_t NS(count_if)(
        const T* first,
        const T* last,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        ) {

    size_t count = 0;
    while (first != last) {
        if (predicate(first, predicate_ctx)) ++count;
        ++first;
    }
    return count;
}

ALGDEF void NS(mismatch)(
        const T **first_1,
        const T *last_1,
        const T **first_2,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
        ) {

    const T* f1 = *first_1;
    const T* f2 = *first_2;
    while (f1 != last_1) {
        if (compare(f1, f2, compare_ctx) != 0) {
            break;
        } 
        ++f1;
        ++f2;
    }
    *first_1 = f1;
    *first_2 = f2;
}

ALGDEF T *NS(adjacent_find)(
        const T *first,
        const T *last,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
        ) {

    if (first != last)
    {
        const T* prev = first;
        const T* next = prev;
        ++next;

        while (next != last) {
            if (compare(prev, next, compare_ctx) == 0) {
                return (T*)prev;
            }
            prev = next;
            ++next;
        }
    }

    return (T*)last;
}

ALGDEF T *NS(copy)(
        const T *first,
        const T *last,
        T *out
        ) {
    size_t n = (last - first);
    memcpy(out, first, n * sizeof(T));
    return out + n; 
}

ALGDEF T *NS(copy_n)(
        const T *first,
        size_t n,
        T *out
        ) {
    memcpy(out, first, n * sizeof(T));
    return out + n;
}


ALGDEF T *NS(copy_if)(
        const T *first,
        const T *last,
        T *out,
        int (*predicate)(const T*,  void*),
        void* predicate_ctx
        ) {

    while (first != last) {
        if (predicate(first, predicate_ctx)) {
            *out = *first;
            ++out;
        }
        ++first;
    }
    return out;
}

ALGDEF T *NS(copy_backward)(
        const T *first,
        const T *last,
        T *last_out
        ) {

    while (first != last) {
        --last;
        --last_out;
        *last_out = *last;
    }
    return last_out;
}

ALGDEF int NS(lexicographical_compare)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        const T *last_2,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    while (1) {
        if (first_1 == last_1 && first_2 == last_2) {
            return 0;
        } else if (first_1 == last_1) {
            return -1;
        } else if (first_2 == last_2) {
            return 1;
        }

        int result = cmp(first_1, first_2, cmp_ctx);
        if (result != 0) return result;

        ++first_1;
        ++first_2;
    }

}

ALGDEF int NS(equal)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    while (first_1 != last_1) {
        int result = cmp(first_1, first_2, cmp_ctx);
        if (result != 0) return 0;

        ++first_1;
        ++first_2;
    }
    return 1;
}

ALGDEF void NS(swap)(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

ALGDEF T *NS(swap_ranges)(
        T *first_1,
        T *last_1,
        T *first_2
        ) {

    while (first_1 != last_1) {
        NS(swap)(first_1, first_2);
        ++first_1;
        ++first_2;
    }
    return first_2;
}

ALGDEF void NS(reverse)(
        T *first,
        T *last
        ) {
    if (first == last) return;

    --last;
    while (first < last) {
        NS(swap)(first, last);
        ++first;
        --last;
    }
}

ALGDEF T *NS(reverse_copy)(
        const T *restrict first,
        const T *restrict last,
        T *out
        ) {
    if (first == last) return out;

    do {
        --last;
        *out = *last;
        ++out;
    } while (first != last);
    return out;
}

ALGDEF T *NS(merge)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        const T *last_2,
        T *restrict out,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        ) {

    if (first_2 == last_2) {
        return NS(copy)(first_1, last_1, out);
    } else if (first_1 == last_1) {
        return NS(copy)(first_2, last_2, out);
    }

    while (1) {
        if (compare(first_1, first_2, compare_ctx) >= 0) {
            *out = *first_2;
            ++out;
            ++first_2;

            if (first_2 == last_2) {
                return NS(copy)(first_1, last_1, out);
            }
        } else {
            *out = *first_1;
            ++out;
            ++first_1;

            if (first_1 == last_1) {
                return NS(copy)(first_2, last_2, out);
            }
        }
    }
}

ALGDEF void NS(merge_with_buffer)(
        T *first,
        T *middle,
        T *last,
        T *buffer,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        )  {
    T* buffer_last = NS(copy)(first, middle, buffer);
    NS(merge)(buffer, buffer_last, middle, last, first, compare, compare_ctx);
}

ALGDEF T *NS(remove_if)(
        T *first,
        T *last,
        int (*pred)(const T*, void*),
        void *pred_ctx
        ) {
    if (first == last) return first;

    T *out = first;

    while (first != last) {
        if (!pred(first, pred_ctx)) {
            *out = *first;
            ++out;
        }
        ++first;
    }
    return out;
}

ALGDEF T *NS(remove_if_not)(
        T *first,
        T *last,
        int (*pred)(const T*, void*),
        void *pred_ctx
        ) {
    if (first == last) return first;

    T *out = first;

    while (first != last) {
        if (pred(first, pred_ctx)) {
            *out = *first;
            ++out;
        }
        ++first;
    }
    return out;
}


ALGDEF T *NS(replace_if)(
        T *first,
        T *last,
        const T *replacement,
        int (*pred)(const T*, void*),
        void *pred_ctx
        ) {
    while (first != last) {
        if (pred(first, pred_ctx)) {
            *first = *replacement;
        }
        ++first;
    }
    return first;
}

ALGDEF T *NS(unique)(
        T *first,
        T *last,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    if (first == last) return first;

    T *out = first;
    ++first;

    while (first != last) {
        if (cmp(out, first, cmp_ctx) != 0) {
            ++out;
            *out = *first;
        }
        ++first;
    }
    ++out;
    return out;
}

ALGDEF T *NS(unique_copy)(
        T *restrict first,
        T *last,
        T *restrict out,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    if (first == last) return out;

    *out = *first;

    while (first != last) {
        if (cmp(out, first, cmp_ctx) != 0) {
            ++out;
            *out = *first;
        }
        ++first;
    }
    ++out;
    return out;
}

ALGDEF T *NS(fill)(
        T *first,
        T *last,
        const T *x
        ) {
    while (first != last) {
        *first = *x;
        ++first;
    }
    return first;
}

ALGDEF T *NS(fill_n)(
        T *first,
        size_t count,
        const T *x
        ) {
    for (size_t i = 0; i < count; ++i) {
        first[i] = *x;
    }
    return first + count;
}

ALGDEF int NS(set_includes)(
        const T *first_sub,
        const T *last_sub,
        const T *first_super,
        const T *last_super,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {

    if (first_sub == last_sub) return 1;

    while (first_super != last_super) {
        int result = cmp(first_sub, first_super, cmp_ctx);
        if (result < 0) {
            return 0;
        } else if (result == 0) {
            ++first_sub;
            if (first_sub == last_sub) return 1;
        }
        ++first_super;
    }
    return 0;
}


ALGDEF T *NS(set_union)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        const T *last_2,
        T *restrict out,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    while (1) {
        if (first_1 == last_1) {
            return NS(copy)(first_2, last_2, out);
        } else if (first_2 == last_2) {
            return NS(copy)(first_1, last_1, out);
        }

        int result = cmp(first_1, first_2, cmp_ctx);
        if (result == 0) {
            *out = *first_1;
            ++first_1;
            ++first_2;
        } else if (result >= 0) {
            *out = *first_2;
            ++first_2;
        } else {
            *out = *first_1;
            ++first_1;
        }
        ++out;
    }
}

ALGDEF T *NS(set_difference)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        const T *last_2,
        T *out,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    if (first_1 == last_1 || first_2 == last_2) return out;

    while (1) {
        int result = cmp(first_1, first_2, cmp_ctx);
        if (result == 0) {
            ++first_1;
            ++first_2;
            if (first_1 == last_1) return out;
            if (first_2 == last_2) {
                return NS(copy)(first_1, last_1, out);
            }
        } else if (result < 0) {
            if (out != first_1) *out = *first_1;
            ++first_1;
            ++out;
            if (first_1 == last_1) return out;
        } else if (result > 0) {
            ++first_2;
            if (first_2 == last_2) {
                return NS(copy)(first_1, last_1, out);
            }
        }
    }
}

ALGDEF T *NS(set_intersection)(
        const T *first_1,
        const T *last_1,
        const T *first_2,
        const T *last_2,
        T *out,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    if (first_1 == last_1 || first_2 == last_2) return out;

    while (1) {
        int result = cmp(first_1, first_2, cmp_ctx);
        if (result == 0) {
            *out = *first_1;
            ++out;
            ++first_1;
            ++first_2;
            if (first_1 == last_1 || first_2 == last_2) return out;
        } else if (result < 0) {
            ++first_1;
            if (first_1 == last_1) return out;
        } else if (result > 0) {
            ++first_2;
            if (first_2 == last_2) return out;
        }
    }
}

ALGDEF T *NS(min)(
        const T* a,
        const T* b,
        int (*cmp)(const T*, const T*, void*),
        void* ctx
        ) {
    if (cmp(b, a, ctx) < 0) {
        return (T*)b;
    } else {
        return (T*)a;
    }
}

ALGDEF T *NS(max)(
        const T *a,
        const T *b,
        int (*cmp)(const T*, const T*, void*),
        void* ctx
        ) {
    if (cmp(a, b, ctx) > 0) {
        return (T*)a;
    } else {
        return (T*)b;
    }
}

ALGDEF T *NS(min_element)(
        const T *first,
        const T *last,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    if (first == last) return (T*)last;

    const T* best = first;
    ++first;

    while (first != last) {
        if (cmp(first, best, cmp_ctx) < 0) {
            best = first;
        }
        ++first;
    }
    return (T*)best;
}

ALGDEF T *NS(max_element)(
        const T *first,
        const T *last,
        int (*cmp)(const T*, const T*, void*),
        void* ctx
        ) {
    if (first == last) return (T*)first;

    const T* best = first;
    ++first;

    while (first != last) {
        if (cmp(first, best, ctx) > 0) best = first;
        ++first;
    }
    return (T*)best;
}

ALGDEF void NS(minmax_element)(
        const T *first,
        const T *last,
        T** out_min,
        T** out_max,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    if (first == last) return;
    const T *min_p = first;
    ++first;

    if (first == last) {
        *out_min = (T*)min_p;
        *out_max = (T*)min_p;
        return;
    }

    const T *max_p = first;
    if (cmp(max_p, min_p, cmp_ctx) < 0) {
        const T* temp = min_p;
        min_p = max_p;
        max_p = temp;
    }

    ++first;

    while (first != last && first + 1 != last) {
        const T *potential_min = first;
        const T *potential_max = first + 1;

        if (cmp(potential_max, potential_min, cmp_ctx) < 0) {
            const T* temp = potential_min;
            potential_min = potential_max;
            potential_max = temp;
        }

        if (cmp(potential_min, min_p, cmp_ctx) < 0) {
            min_p = potential_min;
        }

        if (cmp(potential_max, max_p, cmp_ctx) >= 0) {
            max_p = potential_max;
        }
        ++first;
        ++first;
    }

    if (first != last) {
        if (cmp(first, min_p, cmp_ctx) < 0) {
            min_p = first;
        } else if (cmp(first, max_p, cmp_ctx) >= 0) {
            max_p = first;
        }
    }
    *out_min = (T*)min_p;
    *out_max = (T*)max_p;
}

ALGDEF int NS(is_partitioned)(
        const T *first, 
        const T *last,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        ) {
    first = NS(find_if_not)(first, last, predicate, predicate_ctx);
    first = NS(find_if)(first, last, predicate, predicate_ctx);
    return first == last;
}

ALGDEF T *NS(partition)(
        T *first,
        T *last,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        )
{
    T* out = first;

    while (first != last)
    {
        if (predicate(first, predicate_ctx))
        {
            NS(swap)(out, first);
            ++out;
        }
        ++first;
    }
    return out;
}

ALGDEF T *NS(partition_copy)(
        const T *first,
        const T *last,
        T *restrict out_false,
        T *restrict out_true,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        ) {

    while (first != last)
    {
        if (predicate(first, predicate_ctx))
        {
            *out_true = *first;
            ++out_true;
        }
        else
        {
            *out_false = *first;
            ++out_false;
        }
        ++first;
    }
    return out_false;
}

ALGDEF T *NS(partition_point_n)(
        const T *first,
        size_t n,
        int (*predicate)(const T*, void*),
        void* predicate_ctx
        ) {
    while (n) {
        size_t half = n >> 1;
        const T* middle = first + half;

        if (predicate(middle, predicate_ctx)) {
            ++middle;
            first = middle;
            n -= (half + 1);
        } else {
            n = half;
        }
    }
    return (T*)first;
}

ALGDEF T *NS(partition_point)(
        const T *first,
        const T *last,
        int (*predicate)(const T*, void*),
        void *predicate_ctx
        ) {
    return NS(partition_point_n)(first, last - first, predicate, predicate_ctx);
}

ALGDEF T *NS(is_sorted_until)(
        const T* first,
        const T* last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        ) {
    if (first != last)
    {
        const T* prev = first;
        const T* next = prev;
        ++next;

        while (next != last)
        {
            if (compare(prev, next, compare_ctx) > 0) return (T*)next;
            prev = next;
            ++next;
        }
    }
    return (T*)last;
}

ALGDEF int NS(is_sorted)(
        const T *first,
        const T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        ) {
    return NS(is_sorted_until)(first, last, compare, compare_ctx) == last;
}

typedef struct {
    const T* value;
    int (*compare)(const T*, const T*, void*);
    void* compare_ctx;
} NS(_lower_upper_bound_closure);

ALGDEF int NS(_lower_bound_predicate)(const T* x, void* ctx) {
    const NS(_lower_upper_bound_closure)* c = ctx;
    return c->compare(x, c->value, c->compare_ctx) < 0;
}

ALGDEF T *NS(lower_bound)(
        const T *first,
        const T *last,
        const T* value,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {

    NS(_lower_upper_bound_closure) ctx = {
        .value = value, 
        .compare = cmp,
        .compare_ctx = cmp_ctx
    };
    return NS(partition_point)(first, last, NS(_lower_bound_predicate), &ctx);
}


ALGDEF int NS(_upper_bound_predicate)(const T* x, void* ctx) {
    const NS(_lower_upper_bound_closure)* c = ctx;
    return c->compare(c->value, x, c->compare_ctx) >= 0;
}

ALGDEF T *NS(upper_bound)(
        const T *first,
        const T *last,
        const T *value,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {

    NS(_lower_upper_bound_closure) ctx = {
        .value = value, 
        .compare = cmp,
        .compare_ctx = cmp_ctx
    };
    return NS(partition_point)(first, last, NS(_upper_bound_predicate), &ctx);
}

ALGDEF int NS(binary_search)(
        const T *first,
        const T *last,
        const T *value,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    first = NS(lower_bound)(first, last, value, cmp, cmp_ctx);
    return (first != last) && cmp(first, value, cmp_ctx) == 0;
}

ALGDEF void NS(equal_range)(
        const T *first,
        const T *last,
        const T *value,
        T **lower,
        T **upper,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    *lower = NS(lower_bound)(first, last, value, cmp, cmp_ctx);
    *upper = NS(upper_bound)(*lower, last, value, cmp, cmp_ctx);
}

ALGDEF int NS(next_permutation)(
        T *first,
        T *last,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {

    if (first == last) return 0;
    T *l = last;
    --l;


    T *next = l - 1;
    while (l != first && cmp(next, l, cmp_ctx) >= 0)
    {
        l = next;
        --next;
    }

    if (l != first)
    {
        T *f = last - 1;
        while (f != next && cmp(next, f, cmp_ctx) >= 0)
        {
            --f;
        }

        NS(swap)(next, f);
        NS(reverse)(l, last);
        return 1;
    }
    else
    {
        NS(reverse(first, last));
        return 0;
    }
}


ALGDEF void NS(random_shuffle_n)(
        T *first,
        size_t n
        ) {
    while (n > 1)
    {
        size_t j = ARRAY_ALG_RANDOM(n);
        --n;
        NS(swap)(first + n, first + j);
    }
}


ALGDEF void NS(random_shuffle)(
        T *first,
        T *last
        ) {
    NS(random_shuffle_n)(first, last - first);
}

ALGDEF T* NS(sample)(
        const T *first,
        const T *last,
        T* out,
        size_t count
        ) {
    for (size_t i = 0; i < count; ++i) {
        if (first == last) {
            return out + i;
        }
        out[i] = *first;
        ++first;
    }

    size_t range = count + 1;
    while (first != last) {
        size_t j = ARRAY_ALG_RANDOM(range);

        if (j < count) {
            out[j] = *first;
        }
        ++first;
        ++range;
    }
    return out + count;
}

ALGDEF T *NS(is_heap_until)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void *),
        void* compare_ctx
        ) {
    T *half = first;
    ++first;

    while (1) {
        if (first == last || compare(half, first, compare_ctx) < 0) {
            return first;
        }
        ++first;
        if (first == last || compare(half, first, compare_ctx) < 0) {
            return first;
        }
        ++first;
        ++half;
    }
    return first;
}


ALGDEF int NS(is_heap)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void *),
        void* compare_ctx
        ) {
    return NS(is_heap_until)(first, last, compare, compare_ctx) == last;
}

ALGDEF void NS(push_heap_n)(
        T *first,
        size_t count,
        int (*compare)(const T*, const T*, void *),
        void* compare_ctx
        ) {
    if (count <= 1) return;

    size_t index = count - 1;

    while (index != 0) {
        size_t parent_index = (index - 1) >> 1;

        T* elem = first + index;
        T* parent = first + parent_index;

        if (compare(elem, parent, compare_ctx) <= 0) {
            break;
        }
        NS(swap)(elem, parent);
        index = parent_index;
    }
}

ALGDEF void NS(push_heap)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void *),
        void *compare_ctx
        ) {
    NS(push_heap_n)(first, last - first, compare, compare_ctx);
}

ALGDEF void NS(pop_heap_n)(
        T *first,
        size_t count,
        int (*compare)(const T*, const T*, void *),
        void *compare_ctx
        ) {
    if (count <= 1) return;

    --count;
    T *end = first + count;
    NS(swap)(first, end);

    size_t index = 0;
    while (1) {
        T *elem = first + index;
        index = index * 2 + 1;
        if (index >= count) break;

        T *child = first + index;
        if (compare(child, elem, compare_ctx) > 0) {
            // left child is larger

            // if right is larger use that 
            if (index + 1 < count
                    && compare(child + 1, child, compare_ctx) > 0) {
                ++index;
                ++child;
            }
        } else {
            ++index;
            ++child;
            if (index >= count || compare(child, elem, compare_ctx) <= 0) {
                break;
            }
            // right child is larger
        }
        NS(swap)(elem, child);
    }
}

ALGDEF void NS(pop_heap)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void *),
        void *compare_ctx
        ) {
    return NS(pop_heap_n)(first, last - first, compare, compare_ctx);
}

ALGDEF void NS(make_heap_n)(
        T *first,
        size_t count,
        int (*compare)(const T*, const T*, void *),
        void* compare_ctx
        ) {
    for (size_t n = 2; n <= count; ++n) {
        NS(push_heap_n)(first, n, compare, compare_ctx);
    }
}

ALGDEF void NS(make_heap)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        ) {
    NS(make_heap_n)(first, last - first, compare, compare_ctx);
}

ALGDEF void NS(sort_heap)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        ) {
    while (last != first) {
        NS(pop_heap)(first, last, compare, compare_ctx);
        --last;
    }
}

ALGDEF void NS(_insertion_sort_unguarded)(
        T *restrict first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        ) {
    if (first == last) return;
    T x;

    // first element has no place to go, move on.
    ++first;
    while (first != last)
    {
        x = *first;
        T *prev = first - 1;

        // we don't check that we run into first
        while (compare(&x, prev, compare_ctx) < 0)
        {
            *(prev + 1) = *prev;
            --prev;
        }
        *(prev + 1) = x;
        ++first;
    }
}

ALGDEF void NS(insertion_sort)(
        T *first,
        T *last,
        int (*compare)(const T* a, const T* b, void*),
        void* compare_ctx
        ) {

    if (first == last) return;
    T *suffix = first + 1;
    if (suffix == last) return;

    T* min = NS(min_element)(first, last, compare, compare_ctx);
    NS(swap)(first, min);
    NS(_insertion_sort_unguarded)(suffix, last, compare, compare_ctx);
}

ALGDEF T* NS(_sort_partition)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
) {

    size_t n = (size_t)(last - first);
    assert(n != 0);

    // cannot pick the last element as the pivot
    T pivot = first[(n - 1) / 2];
    --last;

    while (1) {
        while (compare(first, &pivot, compare_ctx) < 0) {
            ++first;
        }
        while (compare(&pivot, last, compare_ctx) < 0) {
            --last;
        }
        if (first >= last) {
            return last + 1;
        }
        NS(swap(first, last));
        ++first;
        --last;
    }
}

ALGDEF T *NS(_quick_sort_early_stop)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
        ) {
    enum { SIZE_WHEN_INSERTION_IS_FASTER = 32 };

    while (last - first > SIZE_WHEN_INSERTION_IS_FASTER) {
        T *p = NS(_sort_partition)(first, last, compare, compare_ctx);
        NS(_quick_sort_early_stop)(p, last,  compare, compare_ctx);
        // tail call
        last = p;
    }
    return last;
}

ALGDEF void NS(sort)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        ) {

    T *min_partition = NS(_quick_sort_early_stop)(first, last, compare, compare_ctx);
    // Find sentinel in first partition
    T *min = NS(min_element)(first, min_partition, compare, compare_ctx);
    NS(swap)(first, min);
    NS(_insertion_sort_unguarded)(first, last, compare, compare_ctx);
}

ALGDEF void NS(_rotate_right_by_one)(
        T *first,
        T *last
        ) {
    if (first == last) return;

    T *butlast = last;
    --butlast;

    T temp = *butlast;
    memmove(first + 1, first, (butlast - first) * sizeof(T));
    *first = temp;
}

ALGDEF void NS(insertion_sort_stable)(
        T *first,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        ) {
    if (first == last) return;
    T *suffix = first + 1;
    if (suffix == last) return;

    T* min = NS(min_element)(first, last, compare, compare_ctx);
    ++min;
    NS(_rotate_right_by_one)(first, min);
    NS(_insertion_sort_unguarded)(suffix, last, compare, compare_ctx);
}

ALGDEF T *NS(_merge_sort_adaptive_with_buffer_n)(
        T* first,
        size_t count,
        T* buffer,
        int (*compare)(const T* a, const T* b, void*),
        void* compare_ctx
        ) {
    if (count < 1) return first;

    enum { SIZE_WHEN_INSERTION_IS_FASTER = 24 };
    if (count <= SIZE_WHEN_INSERTION_IS_FASTER ) {
        NS(insertion_sort_stable)(first, first + count, compare, compare_ctx);
        return first + count;
    }

    size_t half = count / 2;
    if (half < 1) return first + 1;

    T* middle = NS(_merge_sort_adaptive_with_buffer_n)(first, half, buffer, compare, compare_ctx);
    T* last = NS(_merge_sort_adaptive_with_buffer_n)(middle, count - half, buffer, compare, compare_ctx);
    NS(merge_with_buffer)(first, middle, last, buffer, compare, compare_ctx);
    return last;
}

ALGDEF void NS(stable_sort)(
        T* first,
        T* last,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
        ) {
    size_t count = last - first;
    T* buffer = malloc((count >> 1) * sizeof(T));
    NS(_merge_sort_adaptive_with_buffer_n)(first, count, buffer, compare, compare_ctx);
    free(buffer);
}

ALGDEF void NS(stable_sort_with_buffer)(
        T* first,
        T* last,
        T* buffer,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
        )
{
    size_t count = last - first;
    NS(_merge_sort_adaptive_with_buffer_n)(first, count, buffer, compare, compare_ctx);
}

ALGDEF void NS(partial_sort)(
        T *first,
        T *middle,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        )
{

    size_t n = middle - first;
    if (n == 0) { return; }

    NS(make_heap_n)(first, n, compare, compare_ctx);

    T *p = middle;
    while (p != last)
    {
        if (compare(p, first, compare_ctx) < 0)
        {
            NS(pop_heap_n)(first, n, compare, compare_ctx);
            NS(swap)(middle - 1, p);
            NS(push_heap_n)(first, n, compare, compare_ctx);
        }
        ++p;
    }
    NS(sort_heap)(first, middle, compare, compare_ctx);
}

ALGDEF T *NS(partial_sort_copy)(
        const T *first,
        const T *last,
        T *out_first,
        T *out_last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        )
{
    if (first == last || out_first == out_last) return out_first;
    T *out = out_first;
    *out = *first;
    ++out;
    ++first;

    while (out != out_last)
    {
        if (first == last) {
             NS(sort_heap)(out_first, out, compare, compare_ctx);
             return out;
        }
        *out = *first;
        ++first;
        ++out;
        NS(push_heap)(out_first, out, compare, compare_ctx);
    }

    while (first != last)
    {
        if (compare(first, out_first, compare_ctx) < 0)
        {
            NS(pop_heap)(out_first, out, compare, compare_ctx);
            *(out - 1) = *first;
            NS(push_heap)(out_first, out, compare, compare_ctx);
        }
        ++first;
    }
    NS(sort_heap)(out_first, out, compare, compare_ctx);
    return out;
}

ALGDEF void NS(nth_element)(
        T *first,
        T *nth,
        T *last,
        int (*compare)(const T*, const T*, void*),
        void* compare_ctx
) {

    while (last - first > 1) {
        T *m = NS(_sort_partition)(first, last, compare, compare_ctx);

        if (m <= nth) {
            first = m;
        } else {
            last = m;
        }
    }
    // Tried insertion sort when < 16 elements and it didn't seem to help.
    assert(first == nth);
}

// EXTENSIONS
// These are not in the C++ STL, but in the same spirit.

ALGDEF size_t NS(unique_count)(
        const T* first,
        const T* last,
        int (*cmp)(const T*, const T*, void*),
        void* cmp_ctx
        ) {
    if (first == last) return 0;

    const T* group = first;
    ++first;

    size_t count = 1;
    while (first != last) {
        if (cmp(group, first, cmp_ctx) != 0) {
            ++count;
            group = first;
        }
        ++first;
    }
    return count;
}


ALGDEF T *NS(is_strictly_increasing_until)(
        const T *first,
        const T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        ) {
    if (first != last)
    {
        const T *prev = first;
        const T *next = prev;
        ++next;

        while (next != last)
        {
            if (compare(prev, next, compare_ctx) >= 0) return (T*)next;
            prev = next;
            ++next;
        }
    }
    return (T*)last;
}

ALGDEF int NS(is_strictly_increasing)(
        const T *first,
        const T *last,
        int (*compare)(const T*, const T*, void*),
        void *compare_ctx
        ) {
    return NS(is_strictly_increasing_until)(first, last, compare, compare_ctx) == last;
}

ALGDEF T* NS(insert_n)(
        T *first,
        T *last,
        const T *to_insert_first,
        size_t n
        ) {
    memmove(first + n, first, sizeof(T) * (last - first));
    NS(copy_n)(to_insert_first, n, first);
    return last + n;
}

#endif

#undef T
#undef NS
#undef NAME1
#undef NAME2
#undef ALGDEF

// Convenience for including multiple times.
#ifdef ARRAY_ALG_TYPE
#undef ARRAY_ALG_TYPE
#endif

#ifdef ARRAY_ALG_PREFIX
#undef ARRAY_ALG_PREFIX
#endif

#ifdef __cplusplus
}
#endif

/*
Copyright 2022 clibraries
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

