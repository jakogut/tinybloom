#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
#include <time.h>

#include <tinybloom.h>

#define NUM_ITEMS 10000000
#define NUM_BUCKETS NUM_ITEMS * 20

int compare_unsigned(const void* a, const void* b)
{
	if(*(unsigned*)a > *(unsigned*)b) return 1;
	else if(*(unsigned*)a < *(unsigned*)b) return -1;
	else return 0;
}

int main()
{
	unsigned* test_array = malloc(NUM_ITEMS * sizeof(unsigned));
	unsigned* search_targets = malloc(NUM_ITEMS * sizeof(unsigned));

	srand(time(NULL));

	bloom_filter* bfilter = create_bfilter(NUM_BUCKETS);

	printf("Using %lu kilobytes for filter.\n\n", (bfilter->filter_size * sizeof(unsigned)) / 1024);

	printf("Generating randomized input.\n\n");

	int i;
	for(i = 0; i < NUM_ITEMS; i++)
	{
		test_array[i] = rand();
		bfilter_add(bfilter, &test_array[i]);

		search_targets[i] = rand();
	}


	printf ("Sorting array.\n\n");

	qsort(test_array, NUM_ITEMS, sizeof(unsigned), compare_unsigned);


	struct timespec begin, end;
	double time;

	printf("Running search without bloom filter.\n");

	clock_gettime(CLOCK_REALTIME, &begin);

	for(i = 0; i < NUM_ITEMS; i++)
		bsearch(&search_targets[i], test_array, NUM_ITEMS, sizeof(unsigned), compare_unsigned);

	clock_gettime(CLOCK_REALTIME, &end);
	time = (end.tv_sec - begin.tv_sec) + (double)(end.tv_nsec - begin.tv_nsec) / 1000000000L;

	printf("Completed in %f seconds.\n\n", time);

	clock_gettime(CLOCK_REALTIME, &begin);

	printf("Running search with bloom filter.\n");
	for(i = 0; i < NUM_ITEMS; i++)
		if(bfilter_check(bfilter, &search_targets[i]))
			bsearch(&search_targets[i], test_array, NUM_ITEMS, sizeof(unsigned), compare_unsigned);

	clock_gettime(CLOCK_REALTIME, &end);
	time = (end.tv_sec - begin.tv_sec) + (double)(end.tv_nsec - begin.tv_nsec) / 1000000000L;

	printf("Completed in %f seconds.\n\n", time);

	free(search_targets);
	free(test_array);

	return 0;
}
