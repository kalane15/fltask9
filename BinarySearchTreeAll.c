#include "SkewHeap.h"
#include "BinaryHeap.h"
#include "LeftistHeap.h"


#include "templates.h"  
#include "BinarySearchTreeAll.h"
#ifdef T 
#undef T  
#endif  
#define T LeftistHeapPriorityQueue 
#include "BinarySearchTreeAsTemplate.c"    
#ifdef T  
#undef T  
#endif  
#define T BinaryHeapPriorityQueue
#include "BinarySearchTreeAsTemplate.c"    
#ifdef T  
#undef T  
#endif 
#define T SkewHeapPriorityQueue
#include "BinarySearchTreeAsTemplate.c"