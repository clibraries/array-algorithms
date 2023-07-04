#include "defs.h"
#include <assert.h>
#include <time.h>

static
void print_array(int* x, int n) {
    for (int i = 0; i < n; ++i) printf("%d ", x[i]);
    printf("\n");
}

static
int pred_is_even(const int* x, void* ctx) {
    return (*x) % 2 == 0;
}

static
int pred_greater_100(const int* x, void* ctx) {
    return (*x) > 100;
}

static
int compare_char(const char* a, const char* b, void* ctx) {
    return (*a) - (*b);
}

static
int compare_int(const int* a, const int* b, void* ctx) {
    return *a - *b;
}

static
int compare_person_name(const Person* a, const Person* b, void* ctx) {
    return strncmp(a->name, b->name, 32);
}

char* string_end(const char* c) {
    while (*c) ++c;
    return (char*)c;
}

void test_predicates(void) {
    int numbers[] = { 1, 2, 3, 4, 5, 6 };
    {
        int all_even = intv_all_of(numbers, numbers + 6, pred_is_even, NULL);
        assert(!all_even);
    }
    {
        int any_even = intv_any_of(numbers, numbers + 6, pred_is_even, NULL);
        assert(any_even);
    }
    {
        int none_even = intv_none_of(numbers, numbers + 6, pred_is_even, NULL);
        assert(!none_even);
    }
    {
        int less_100 = intv_none_of(numbers, numbers + 6, pred_greater_100, NULL);
        assert(less_100);
    }
}

void test_count(void) {
    int numbers[] = { 1, 2, 3, 4, 5, 6 };
    {
        int count = intv_count_if(numbers, numbers + 6, pred_is_even, NULL);
        assert(count == 3);
    }
}

void test_mismatch(void) {
    const char* word1 = "abcd";
    const char* word2 = "abdc";

    string_mismatch(&word1, string_end(word1), &word2, compare_char, NULL);
    assert(*word1 == 'c' && *word2 == 'd');
}

void test_find(void) {
    int numbers[] = { 1, 2, 3, 4, 5, 6 };
    {
        int* x = intv_find_if(numbers, numbers + 6, pred_is_even, NULL);
        assert(*x == 2);
    }
    {
        int* x = intv_find_if_not(numbers, numbers + 6, pred_is_even, NULL);
        assert(*x == 1);
    }
    {
        int* x = intv_find_if(numbers, numbers + 6, pred_greater_100, NULL);
        assert(x == numbers + 6);
    }
    {
        int* x = intv_find_if_not(numbers, numbers + 6, pred_greater_100, NULL);
        assert(*x == 1);
    }
    {
        int* x = intv_find_last_if(numbers, numbers + 6, pred_is_even, NULL);
        assert(*x == 6);
    }
}

void test_adjacent_find(void) {
    {
        int numbers[] = { 1, 2, 3, 3, 3, 4 };
        int* num = intv_adjacent_find(numbers, numbers + 6, compare_int, NULL);
        assert(num[0] == num[1]);
    }
}

void test_copy(void) {
    int numbers[] = { 1, 2, 3, 4, 5, 6};
    {
        int evens[6];
        int* end = intv_copy_if(numbers, numbers + 6, evens, pred_is_even, NULL);

        int expected[] = {2, 4, 6};
        assert(memcmp(evens, expected, (end - evens) * sizeof(int)) == 0);
    }
    {
        int numbers2[6];
        int* end = intv_copy(numbers, numbers + 6, numbers2);
        assert(end - numbers2 == 6);
        assert(memcmp(numbers, numbers2, 6 * sizeof(int)) == 0);
    }
}

void test_copy_backward(void) {
    int numbers[] = { 1, 2, 3, 4, 5, 6 };
    {
        // shift up by one 
        intv_copy_backward(numbers, numbers + 5, numbers + 6);
        print_array(numbers, 6);
        int expected[] = { 1, 1, 2, 3, 4, 5 };
        assert(memcmp(numbers, expected, 6 * sizeof(int)) == 0);
    }
}

void test_compare(void) {
    const char* word1 = "dog1";
    const char* word2 = "dog2";

    {
        assert(string_equal(word1, word1 + 3,  word2, compare_char, NULL));
        assert(!string_equal(word1, word1 + 4, word2, compare_char, NULL));
    }

    {
        int result = string_lexicographical_compare(
                word1, string_end(word1),
                word2, string_end(word2),
                compare_char, NULL
                );
        assert(result == strcmp(word1, word2));
    }
}

void test_swap(void) {
    {
        int a = 3;
        int b = 4;
        intv_swap(&a, &b);
        assert(b == 3);
        assert(a == 4);
    }
    {
        // swap with self.
        int a = 4;
        intv_swap(&a, &a);
        assert(a == 4);
    }
    {
        char dog[] = "dog";
        char cat[] = "cat";
        string_swap_ranges(dog, dog + 3, cat);

        assert(strcmp(dog, "cat") == 0);
        assert(strcmp(cat, "dog") == 0);
    }
}

void test_reverse(void) {
    {
        char word[] = "dog";
        string_reverse(word, word + 3);
        assert(strcmp(word, "god") == 0);
    }
    {
        char word[] = "people";
        char out[16];
        char* end = string_reverse_copy(word, word + 6, out);
        *end = '\0';
        printf("%s\n", out);
        assert(strcmp(out, "elpoep") == 0);
    }

}

void test_merge(void) {
    {
        int a[] = { 1, 1, 3, 4 };
        int b[] = { -1, 1, 2, 3, 4, 5 };

        int out[4 + 6];
        int* start = out;
        int* end = intv_merge(a, a + 4, b, b + 6, start, compare_int, NULL);

        int expected[] = { -1, 1, 1, 1, 2, 3, 3, 4, 4, 5 };
        assert(memcmp(out, expected, sizeof(expected)) == 0);
    }
    {
        int buffer[8];
        int a[] = { 1, 1, 3, 4, -1, 1, 2, 3, 4, 5 };
        intv_merge_with_buffer(a, a + 4, a + 10, buffer, compare_int, NULL);

        int expected[] = { -1, 1, 1, 1, 2, 3, 3, 4, 4, 5 };
        assert(memcmp(a, expected, sizeof(expected)) == 0);
    }
}

void test_remove(void) {
    int numbers[] = { 1, 2, 3, 4, 5, 6};

    {
        int* f = numbers;
        int* l = intv_remove_if(numbers, numbers + 6, pred_is_even, NULL);

        int expected[] = {1, 3, 5};
        assert(memcmp(f, expected, (l - f) * sizeof(int)) == 0);
    }
}

void test_replace(void) {
    int numbers[] = { 1, 2, 3, 4, 5, 6};
    {
        int zero = 0;
        intv_replace_if(numbers, numbers + 6,  &zero, pred_is_even, NULL);

        int expected[] = { 1, 0, 3, 0, 5, 0 };
        assert(memcmp(numbers, expected, 6 * sizeof(int)) == 0);
    }
}

void test_fill(void) {
    int nums[] = { 1, 1, 1, 1, 1 };
    int x = 2;
    intv_fill(nums, nums + 5, &x);
    for (int i = 0; i < 5; ++i) {
        assert(nums[i] == 2);
    }

    x = -5;
    int* ptr = intv_fill_n(nums, 3, &x);
    x = -10;
    intv_fill_n(ptr, 2, &x);

    for (int i = 0; i < 3; ++i) {
        assert(nums[i] == -5);
    }
    assert(nums[3] == -10);
    assert(nums[4] == -10);
}



void test_unique(void) {
    int expected[] = { 1, 3, 4, 7, 8 };
    {
        int a[] = { 1, 3, 3, 3, 4, 4, 7, 8, 8, 8 };

        int* start = a;
        int* end = intv_unique(a, a + 10, compare_int, NULL);

        assert(memcmp(start, expected, sizeof(expected)) == 0);
    }
    {
        int a[] = { 1, 3, 3, 3, 4, 4, 7, 8, 8, 8 };
        int b[5];

        int* out = intv_unique_copy(a, a + 10, b, compare_int, NULL);

        assert(out - b == 5);
        assert(memcmp(b, expected, sizeof(expected)) == 0);
    }
}

void test_union(void) {
    {
        int a[] = { 1, 3, 4 };
        int b[] = { -1, 1, 2, 3, 4, 5 };

        int out[3 + 6];
        int* start = out;
        int* end = intv_set_union(a, a + 3, b, b + 6, start, compare_int, NULL);

        int expected[] = { -1, 1, 2, 3, 4, 5 };
        assert(memcmp(out, expected, sizeof(expected)) == 0);
    }
    {
        int a[] = { 1, 3, 4 };

        int out[3];
        int* start = out;
        int* end = intv_set_union(a, a + 3, a, a, start, compare_int, NULL);

        assert(memcmp(out, a, sizeof(a)) == 0);
    }
}

void test_intersect(void) {
    int a[] = { 1, 3, 4 };
    int b[] = { 1, 2, 3, 5 };

    int* start = a;
    int* end = intv_set_intersection(a, a + 3, b, b + 4, a, compare_int, NULL);

    int expected[] = { 1, 3 };
    assert(end - start == 2);;
    assert(memcmp(start, expected, sizeof(expected)) == 0);
}

void test_difference(void) {
    int a[] = { 1, 3, 4, 7 };
    int b[] = { 1, 2, 3, 5 };

    int* start = a;
    int* end = intv_set_difference(a, a + 4, b, b + 4, a, compare_int, NULL);

    int expected[] = { 4, 7 };
    assert(end - start == 2);
    assert(memcmp(start, expected, sizeof(expected)) == 0);
}

void test_minmax(void) {
    {
        int nums[] = { 1, 2 };
        assert( *intv_min(nums + 0, nums + 1, compare_int, NULL) == 1);
        assert( *intv_max(nums + 0, nums + 1, compare_int, NULL) == 2);
    }
    {
        int nums[] = { 4, 4 };
        assert( intv_min(nums + 0, nums + 1, compare_int, NULL) == nums + 0);
        assert( intv_max(nums + 0, nums + 1, compare_int, NULL) == nums + 1);
    }
}

void test_minmax_element(void) {
    {
        int nums[] = { 5, -2, 3, 10 };
        assert(*intv_min_element(nums, nums + 4, compare_int, NULL) == -2);
        assert(*intv_max_element(nums, nums + 4, compare_int, NULL) == 10);
    }
    {
        Person people[] = {
            { 4, "Xavier" },
            { 0, "June" },
            { 2, "Ren" },
            { 3, "Baby" },
            { 1, "Haley" },
        };
        Person* min_person = person_array_min_element(people, people + 5, compare_person_name, NULL);
        assert(strcmp(min_person->name, "Baby") == 0);
        Person* max_person = person_array_max_element(people, people + 5, compare_person_name, NULL);
        assert(strcmp(max_person->name, "Xavier") == 0);

        Person *min, *max;
        person_array_minmax_element(people, people + 5, &min, &max, compare_person_name, NULL);

        assert(min == min_person);
        assert(max == max_person);
    }
}

void test_partition(void) {
    {
        int numbers[] = { 1, 2, 3, 4, 5, 6};
        intv_partition(numbers, numbers + 6, pred_is_even, NULL);

        for (int i = 0; i < 3; ++i) {
            assert(numbers[i] % 2 == 0);
            assert(numbers[i + 3] % 2 == 1);
        }

        assert(intv_is_partitioned(numbers, numbers + 6, pred_is_even, NULL));

        int* point = intv_partition_point(numbers, numbers + 6, pred_is_even, NULL);
        printf("%ld\n", point - numbers);

        assert(point - numbers == 3);
    }
    {
        int numbers[] = { 1, 2, 3, 4, 5, 6};
        int odd[3];
        int even[3];
        int* false_end = intv_partition_copy(numbers, numbers + 6, odd, even, pred_is_even, NULL);

        assert(false_end - odd == 3);

        for (int i = 0; i < 3; ++i) {
            assert(odd[i] % 2 == 1); 
            assert(even[i] % 2 == 0); 
        }
    }
}

void test_is_sorted(void) {
    {
        int numbers[] = { 1, 2, 3, 4, 5, 6};
        assert(intv_is_sorted(numbers, numbers + 6,compare_int, NULL));
    }
    {
        int numbers[] = { 6, 5, 4, 3, 2, 1 };
        assert(!intv_is_sorted(numbers, numbers + 6, compare_int, NULL));
    }
    {
        int numbers[] = { 1, 2, 3, 6, 5, 4};
        assert(!intv_is_sorted(numbers, numbers + 6, compare_int, NULL));

        int* x = intv_is_sorted_until(numbers, numbers + 6, compare_int, NULL);
        assert(*x == 5);
    }
}

void test_binary_search(void) {
    int numbers[] = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };

    {
        int search = 3;
        int* lower;
        int* upper;
        intv_equal_range(
                numbers, numbers + 10,
                &search, &lower, &upper,
                compare_int, NULL
                );

        assert(upper - lower == 2);
        assert(*lower == 3);
    }
    {
        int x = 6;
        assert(intv_binary_search(numbers, numbers + 10, &x, compare_int, NULL) == 0);
        x = -1;
        assert(intv_binary_search(numbers, numbers + 10, &x, compare_int, NULL) == 0);
        x = 3;
        assert(intv_binary_search(numbers, numbers + 10, &x, compare_int, NULL) == 1);
    }
}

void test_permutation(void) {
    int nums[] = { 1, 2, 3, 4 };

    int expected_total = 4 * 3 * 2 * 1;

    int result;
    do {
        result = intv_next_permutation(nums, nums + 4, compare_int, NULL);

        for (int i = 0; i < 4; ++i) {
            printf("%d ", nums[i]);
        }
        printf("\n");

        --expected_total;
    } while (result);
    assert(expected_total == 0);
}

void test_random_shuffle(void) {
    char word[] = "abc";
    const char* words[] = { "acb", "abc",  "bac", "bca", "cba", "cab" };

    // should be able to get each word if we try enough.
    for (int i = 0; i < 6; ++i) {
        printf("%s\n", words[i]);
        while (strcmp(word, words[i]) != 0) {
            string_random_shuffle_n(word, 3);
        }
    }
}

void test_sample(void) {
    const char* letters = "abcd";
    {
        char letter = 'z';

        // should be able to sample each letter if we try enough.
        for (int i = 0; i < 4; ++i) {
            printf("%c\n", letters[i]);
            while (letter != letters[i]) {
                string_sample(letters, string_end(letters), &letter, 1);
            }
        }
    }
    {
        char word[5];
        char* end = string_sample(letters, string_end(letters), word, 5);
        char* start = word;
        assert(end - start == 4);
        word[4] = '\0';

        printf("%s\n", word);

        int total = word[0] + word[1] + word[2] + word[3];
        assert(total == ('a' + 'b' + 'c' + 'd'));
    }
}

void test_heap() {
    int nums[] = { 19, 7, 2, 36, 3, 25, 100, 1, 17, 25 };
    int count = sizeof(nums) / sizeof(int);
    assert(
            !intv_is_heap(nums, nums + count,  compare_int, NULL)
          );

    for (int i = 2; i <= count; ++i) {
        intv_push_heap_n(nums, i, compare_int, NULL);
        print_array(nums, i);
        assert(intv_is_heap(nums, nums + i, compare_int, NULL));
    }

    int i = count;
    while (i > 0) {
        print_array(nums, i);
        assert(intv_is_heap(nums, nums + i, compare_int, NULL));
        intv_pop_heap(nums, nums + i, compare_int, NULL);
        --i;
    }

    intv_make_heap(nums, nums + count,  compare_int, NULL);
    print_array(nums, count);
    assert(intv_is_heap(nums, nums + count, compare_int, NULL));
}

static
int _is_less_equal_than(const int* a, void* ctx)
{
    int* mark = ctx;
    return *a <= *mark;
}

void test_sort_partition() {

    enum { N = 20 };
    int nums[N];

    for (int i = 0; i < 100; ++i) {
        for (int i = 0; i < N; ++i) {
            nums[i] = ARRAY_ALG_RANDOM(100);
        }

        int* part = intv__sort_partition(nums, nums + N, compare_int, NULL);
        if (!intv_all_of(nums, part, _is_less_equal_than, part)) {
            print_array(nums, N);
            printf("partition: %d\n", *part);
            assert(0);
        }
    }
}

void test_nth_element() {
    enum { N = 32 };
    int nums[N];
    for (int i = 0; i < N; ++i) {
        nums[i] = i;
    }

    for (int i = 0; i < 10; ++i) {
        intv_random_shuffle(nums, nums + N);

        intv_nth_element(nums, nums + N / 2, nums + N, compare_int, NULL);
        print_array(nums, N);
        assert(nums[N / 2] == N / 2);
    }
}

void test_partial_sort() {
    enum { N = 100 };
    int nums[N];
    for (int i = 0; i < N; ++i) {
        nums[i] = i;
    }

    for (int iteration = 0; iteration < 1000; ++iteration) {
        intv_random_shuffle(nums, nums + N);
        intv_partial_sort(nums, nums + 10, nums + N, compare_int, NULL);

        for (int i = 0; i < 10; ++i) {
            assert(nums[i] == i);
        }
    }
}

typedef void (*SortFunc)(int*, int*, int (*cmp)(const int*, const int*, void*), void*);

static inline
void do_sort_checks(SortFunc sort)
{
    int nums[500];

    for (int M = 0; M < ARRAY_LEN(nums); ++M) {
        for (int j = 0; j < M; ++j) {
            nums[j] = ARRAY_ALG_RANDOM(10000);
        }
        sort(nums, nums + M, compare_int, NULL);

        if (!intv_is_sorted(nums, nums + M, compare_int, NULL)) {
            printf("sorted: ");
            print_array(nums, M);
            printf("\n");
            assert(0);
        }
    }
}

void intv_heap_sort(
    int *first,
    int *last,
    int (*compare)(const int*, const int*, void*),
    void *compare_ctx) {
    intv_make_heap(first, last, compare, compare_ctx);
    intv_sort_heap(first, last, compare, compare_ctx);
}

void test_heap_sort(void) {
    do_sort_checks(intv_heap_sort);
}

void test_insertion_sort(void) {
    do_sort_checks(intv_insertion_sort);
}

void test_stable_sort(void) {
    do_sort_checks(intv_stable_sort);
}

void test_sort(void) {
    do_sort_checks(intv_sort);
}

    static inline
int qsort_int_compare_(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

static inline
void intv_c_qsort(int* first, int* last, int (*unused)(const int* a, const int* b, void*), void* unused_ctx ) {
    qsort(first, last - first, sizeof(int), qsort_int_compare_);
}

void test_c_qsort()
{
    do_sort_checks(intv_c_qsort);
}

void test_find_unguarded(void) {
    {
        int nums[] = { 1, 2, 3, 101 };
        int* num = intv_find_if_unguarded(nums, pred_greater_100, NULL);
        assert(*num == 101);
    }
    {

        int nums[] = { 200, 300, 3 };
        int* num = intv_find_if_not_unguarded(nums,  pred_greater_100, NULL);
        assert(*num == 3);
    }
}

void test_unique_count(void) {
    int a[] = { 1, 3, 3, 3, 4, 4, 7, 8, 8, 8 };
    assert(intv_unique_count(a, a + 10, compare_int, NULL) == 5);
    assert(intv_unique_count(a, a + 0, compare_int, NULL) == 0);
}

void test_is_strictly_increasing(void) {
    int a[] = { 1, 5, 7, 8, 20, 50001 };

    assert(intv_is_strictly_increasing(a, a + ARRAY_LEN(a), compare_int, NULL));
}

void test_insert(void) {
    Person people[128];

    Person* first = people;
    Person* last = first;

    for (int i = 0; i < 26; ++i) {
        Person person;
        person.id = i;
        person.name[0] = 'a' + i;
        person.name[1] = '\0';
        last = person_array_insert_n(first, last, &person, 1);
    }

    while (first != last) {
        printf("%s\n", first->name);
        ++first;
    }
}

static inline
clock_t _sort_benchmark_iteration(SortFunc sort, int N) {

    int* nums = malloc(N * sizeof(int));

    for (int i = 0; i < N; ++i) {
        nums[i] = i;
    }

    clock_t total = 0;
    for (int i = 0; i < 10; ++i)
    {
        intv_random_shuffle_n(nums, N);
        clock_t start = clock();
        sort(nums, nums + N, compare_int, NULL);
        total += clock() - start;
    }


    for (int i = 0; i < N; ++i) {
        if (i != nums[i]) {
            print_array(nums, N);
            assert(i == nums[i]);
        }
    }
    free(nums);
    return total;
}

static inline
void benchmark_sort(SortFunc sort, int iterations) {
    int count = 16;
    while (count < iterations) {
        clock_t time = _sort_benchmark_iteration(sort, count);
        printf("%d %lu\n", count, time);
        count *= 2;
    }
}

static inline
clock_t _nth_element_benchmark_iteration(int N) {

    int* nums = malloc(N * sizeof(int));

    for (int i = 0; i < N; ++i) {
        nums[i] = i;
    }

    clock_t total = 0;
    for (int i = 0; i < 10; ++i)
    {
        intv_random_shuffle_n(nums, N);
        clock_t start = clock();
        intv_nth_element(nums, nums + N / 2, nums + N, compare_int, NULL);
        total += clock() - start;
    }

    if (nums[N / 2] != N / 2) {
        print_array(nums, N);
        assert(nums[N / 2] == N / 2);
    }

    free(nums);
    return total;
}

static inline
void benchmark_nth_element(int iterations) {
    int count = 16;
    while (count < iterations) {
        clock_t time = _nth_element_benchmark_iteration(count);
        printf("%d %lu\n", count, time);
        count *= 2;
    }
}

int main() {
    srand(time(NULL));
    printf("-- test_predicates --\n"); test_predicates();
    printf("-- test_count --\n"); test_count();
    printf("-- test_mismatch --\n"); test_mismatch();
    printf("-- test_find --\n"); test_find();
    printf("-- test_find_unguarded --\n"); test_find_unguarded();
    printf("-- test_adjacent_find --\n"); test_adjacent_find();
    printf("-- test_find_unguarded --\n"); test_find_unguarded();
    printf("-- test_copy --\n"); test_copy();
    printf("-- test_copy_backward --\n"); test_copy_backward();
    printf("-- test_compare --\n"); test_compare();
    printf("-- test_swap --\n"); test_swap();
    printf("-- test_reverse --\n"); test_reverse();
    printf("-- test_merge --\n"); test_merge();
    printf("-- test_remove --\n"); test_remove();
    printf("-- test_replace --\n"); test_replace();
    printf("-- test_fill --\n"); test_fill();
    printf("-- test_unique --\n"); test_unique();
    printf("-- test_unique_count --\n"); test_unique_count();
    printf("-- test_union --\n"); test_union();
    printf("-- test_intersect --\n"); test_intersect();
    printf("-- test_difference --\n"); test_difference();
    printf("-- test_minmax --\n"); test_minmax();
    printf("-- test_minmax_element --\n"); test_minmax_element();
    printf("-- test_partition --\n"); test_partition();
    printf("-- test_is_sorted --\n"); test_is_sorted();
    printf("-- test_binary_search --\n"); test_binary_search();
    printf("-- test_permutation --\n"); test_permutation();
    printf("-- test_random_shuffle --\n"); test_random_shuffle();
    printf("-- test_sample --\n"); test_sample();
    printf("-- test_heap --\n"); test_heap();

    // SORTS
    printf("-- test_sort_partition -- \n"); test_sort_partition();
    printf("-- test_nth_element --\n"); test_nth_element();
    printf("-- test_partial_sort --\n"); test_partial_sort();
    printf("-- test_heap_sort --\n"); test_heap_sort();
    printf("-- test_insertion_sort --\n"); test_insertion_sort();
    printf("-- test_stable_sort --\n"); test_stable_sort();
    printf("-- test_sort --\n"); test_sort();
    printf("-- test_c_qsort --\n"); test_c_qsort();


    // EXTENSIONS
    printf("-- test_is_strictly_increasing --\n"); test_is_strictly_increasing();
    printf("-- test_insert--\n"); test_insert();


    // BENCHMARKS
    printf("BENCHMARKS\n");
    printf("-- heap_sort --\n"); benchmark_sort(intv_heap_sort, 1000000);
    printf("-- insertion_sort --\n"); benchmark_sort(intv_insertion_sort, 20000);
    printf("-- stable_sort --\n"); benchmark_sort(intv_stable_sort, 1000000);
    printf("-- sort --\n"); benchmark_sort(intv_sort, 1000000);
    printf("-- qsort --\n"); benchmark_sort(intv_c_qsort, 1000000);

    printf("-- nth_element --\n"); benchmark_nth_element(1000000);
    return 0;
}
