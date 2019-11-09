/* File: utils.hpp */

#ifndef __UTILS_HEADER__
#define __UTILS_HEADER__

#include <iostream>
#include "../templates/list.hpp"

inline uint32_t BitConversion (uint64_t, int);
void            SwitchElements(uint64_t **, int, int, int);
void            MergeTables   (List<int> &, uint64_t **, uint32_t *, int, int, uint64_t **, uint32_t *, int, int);

#endif // __UTILS_HEADER__
