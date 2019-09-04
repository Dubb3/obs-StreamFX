/*
* Modern effects for a modern Streamer
* Copyright (C) 2017 Michael Fabian Dirks
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#include "util-memory.hpp"
#include <cstdlib>
#include <stdexcept>
#include <stdlib.h>

#if defined(_MSC_VER) && (_MSC_VER <= 2100)
//#define USE_MSC_ALLOC
#elif defined(_cplusplus) && (__cplusplus >= 201103L)
#define USE_STD_ALLOC
#endif

using namespace std;

void* util::malloc_aligned(size_t align, size_t size)
{
#ifdef USE_MSC_ALLOC
	return _aligned_malloc(size, align);
#elif defined(USE_STD_ALLOC)
	return aligned_alloc(size, align);
#else
	// Ensure that we have space for the pointer and the data.
	size_t asize = aligned_offset(align, size + (sizeof(void*) * 2));

	// Allocate memory and store integer representation of pointer.
	void* ptr = malloc(asize);

	// Calculate actual aligned position
	intptr_t ptr_off = aligned_offset(align, reinterpret_cast<size_t>(ptr) + sizeof(void*));

	// Store actual pointer at ptr_off - sizeof(void*).
	*reinterpret_cast<intptr_t*>(ptr_off - sizeof(void*)) = reinterpret_cast<intptr_t>(ptr);

	// Return aligned pointer
	return reinterpret_cast<void*>(ptr_off);
#endif
}

void util::free_aligned(void* mem)
{
#ifdef USE_MSC_ALLOC
	_aligned_free(mem);
#elif defined(USE_STD_ALLOC_FREE)
	free(mem);
#else
	void* ptr = reinterpret_cast<void*>(*reinterpret_cast<intptr_t*>(static_cast<char*>(mem) - sizeof(void*)));
	free(ptr);
#endif
}
