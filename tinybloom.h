/* Copyright (C) 2010 by Joseph Kogut

This software is licensed under the MIT license.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.	*/

#ifndef TINYBLOOM_H_
#define TINYBLOOM_H_

#ifdef __cplusplus
extern "C" {
#endif

// Take a void argument, and hash it into a positive integer
typedef unsigned (*bfilter_hash)(const void*, size_t);

typedef struct
{
	bfilter_hash hash;

	size_t filter_size;
	unsigned int* filter;

	unsigned int num_buckets;

} bloom_filter;

bloom_filter* create_bfilter(size_t size, bfilter_hash hash);
void destroy_bfilter(bloom_filter* bFilter);

inline void bfilter_add(const bloom_filter* bFilter, const void* input, size_t input_size);

// Checks the bloom filter for potential matches; may return a false positive.
inline int bfilter_check(const bloom_filter* bFilter, const void* input, size_t input_size);

#ifdef __cplusplus
}
#endif

#endif
