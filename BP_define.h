
#ifndef BP_define
#define BP_Tree

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define M (4)
#define LIMIT_M (M % 2 ? (M + 1)/2 : M/2)


typedef struct BPNode *BPTree,*Position;
typedef int KeyType;
struct BPNode
{
    int KeyNum;
    KeyType Key[M + 1];
    BPTree Children[M + 1];
    BPTree Next;
};

/* init */
extern BPTree Initialize();
/* insert */
extern BPTree Insert(BPTree T,KeyType Key);
/* remove */
extern BPTree Remove(BPTree T,KeyType Key);
/* free */
extern BPTree Destroy(BPTree T);
/* ergodic */
extern void Travel_Node(BPTree T);
/* ergodic leaf */
extern void Travel_Leaf(BPTree T);

#endif

