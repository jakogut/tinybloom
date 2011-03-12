#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <tinybloom.h>

#define NUM_ITEMS 16384
#define NUM_BUCKETS NUM_ITEMS * 2

int shitty_search(const unsigned int* array, unsigned int target, size_t size)
{
	int i;
	for(i = 0; i < size; i++)
		if(array[i] == target) return 1;

	return 0;
}

unsigned pass(const void* item, size_t size)
{
	return *(unsigned*)item;
}

int main()
{
	unsigned false_positives = 0, true_positives = 0;
	unsigned false_negatives = 0, true_negatives = 0;

	unsigned* filter_contents = malloc(sizeof(unsigned) * NUM_ITEMS);
	unsigned* test_array = malloc(sizeof(unsigned) * NUM_ITEMS);

	// Create a bloom filter with (2 << 13) buckets
	bloom_filter* bFilter = create_bfilter(NUM_BUCKETS, pass);

	printf("Using %lu kilobytes for filter.\n\n", (bFilter->filter_size * sizeof(unsigned)) / 1024);

	srand(time(NULL));

	int i;
	for(i = 0; i < NUM_ITEMS; i++)
	{
		filter_contents[i] = rand();
		bfilter_add(bFilter, &filter_contents[i], sizeof(unsigned));

		// Fill our test array with a 50/50 mix of numbers that have been entered into the filter, and random numbers
		if(rand() & 1)
			test_array[i] = filter_contents[i];
		else
			test_array[i] = rand();
	}

	for(i = 0; i < NUM_ITEMS; i++)
	{
		int filter_present = 0;
		int array_present = 0;

		filter_present = bfilter_check(bFilter, &test_array[i], sizeof(unsigned));

		array_present = shitty_search(filter_contents, test_array[i], NUM_ITEMS);

		if(array_present && filter_present) 	   true_positives++;
		else if(!array_present && !filter_present) true_negatives++;
		else if(!array_present && filter_present)  false_positives++;
		else if(array_present && !filter_present)  false_negatives++;
	}

	printf("True positives: %i\n"
		"True negatives: %i\n"
		"False positives: %i\n"
		"False negatives: %i\n",
		true_positives, true_negatives, false_positives, false_negatives);

	float success_rate = ((100.0f / NUM_ITEMS) * (true_positives + true_negatives));
	printf("\n%3.2f%% success rate\n\n", success_rate);

	if(success_rate < 80.0f) printf("WARNING: Success rate less than 80%%.\n\n");

	if(false_negatives) printf("**ERROR:** Test FAILED.\n");
	else printf("Test PASSED.\n");

	free(test_array);
	free(filter_contents);
	destroy_bfilter(bFilter);

	return 0;
}
