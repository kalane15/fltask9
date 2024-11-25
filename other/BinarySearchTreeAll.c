#include "SkewHeap.h"
#include "BinaryHeap.h"
#include "LeftistHeap.h"


#include "templates.h"  
#include "BinarySearchTreeAll.h"
#ifdef T 
#undef T  
#endif  
#define T LeftistHeapPQ 
#include "BinarySearchTreeAsTemplate.c"    
#ifdef T  
#undef T  
#endif  
#define T BinaryHeapPriorityQueue
#include "BinarySearchTreeAsTemplate.c"    
#ifdef T  
#undef T  
#endif 
#define T SkewHeapPQ
#include "BinarySearchTreeAsTemplate.c"