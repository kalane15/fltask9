#ifdef T    
#include "templates.h"    
typedef struct TEMPLATE(BSTNode, T) {
	char* id;
	T data;
} TEMPLATE(BSTNode, T);

typedef struct TEMPLATE(BST, T) {
	TEMPLATE(BSTNode, T)* root;
	bool (*cmp)(TEMPLATE(BSTNode, T)*, TEMPLATE(BSTNode, T)*);
} TEMPLATE(BST, T);

void TEMPLATE(Insert, T) (T* a, TEMPLATE(BST, T)* b)
{
	printf("s");
}
#endif

