#ifndef ALL_POSSIBLE_SUMS_H_   
#define ALL_POSSIBLE_SUMS_H_     
#include "templates.h"    
#ifdef T  
#undef T  
#endif  
#define T LeftistHeapPQ 
#include "BinarySearchTreeAsTemplate.h"    
#ifdef T  
#undef T  
#endif  
#define T BinaryHeapPriorityQueue
#include "BinarySearchTreeAsTemplate.h"    
#ifdef T  
#undef T  
#endif  
#define T SkewHeapPQ
#include "BinarySearchTreeAsTemplate.h"
#endif