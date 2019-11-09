/* File: utils.hpp */

#ifndef __UTILS_HEADER__
#define __UTILS_HEADER__

#include <iostream>
#include "../templates/list.hpp"

uint32_t BitConversion (uint64_t, uint32_t);
void     SwitchElements(uint64_t **, uint32_t, uint32_t, uint32_t);
void     MergeTables   (List<uint64_t> &, uint64_t **, uint32_t *, uint32_t, uint32_t, uint64_t **, uint32_t *, uint32_t, uint32_t);

#endif // __UTILS_HEADER__
