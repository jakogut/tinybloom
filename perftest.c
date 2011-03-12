#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include <tinybloom.h>

#define NUM_ITEMS 1000000
#define NUM_BUCKETS NUM_ITEMS * 8

int compare_unsigned(const void* a, const void* b)
{
	if(a == b) return 0;
	if(*(unsigned*)a > *(unsigned*)b) return 1;
	if(*(unsigned*)a < *(unsigned*)b) return -1;
}

unsigned hash_unsigned(const void* item, size_t size)
{
	return *(unsigned*)item;
}

int main()
{
	unsigned test_array[NUM_ITEMS];
	unsigned search_targets[NUM_ITEMS];

	srand(time(NULL));

	bloom_filter* bfilter = create_bfilter(NUM_BUCKETS, hash_unsigned);

	int i;
	for(i = 0; i < NUM_ITEMS; i++)
	{
		test_array[i] = rand();
		bfilter_add(bfilter, &test_array[i], sizeof(unsigned));

		search_targets[i] = rand();
	}

	qsort(test_array, NUM_ITEMS, sizeof(unsigned), compare_unsigned);

	for(i = 0; i < NUM_ITEMS; i++)
		if(bfilter_check(bfilter, &search_targets[i], sizeof(unsigned)))
			bsearch(&search_targets[i], test_array, NUM_ITEMS, sizeof(unsigned), compare_unsigned);
}
