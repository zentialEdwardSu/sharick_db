#include"BP_define.h"
#include"const_define.h"

static KeyType UNAVALIABLE = INT_MIN;

/* malloc*/
static BPTree MallocNewNode()
{
    BPTree NewNode;
    int i;
    NewNode = malloc(sizeof(struct BPNode));
    if (NewNode == NULL)
        exit(EXIT_FAILURE);
    
    i = 0;
    while (i < M + 1){
        NewNode->Key[i] = UNAVALIABLE;
        NewNode->Children[i] = NULL;
        i++;
    }
    NewNode->Next = NULL;
    NewNode->KeyNum = 0;
    
    return NewNode;
}

/* init */
extern BPTree Initialize(){
    
    BPTree ROOT;    
    if (M < (3)){
        printf(M_VALUE_WARN);
        exit(EXIT_FAILURE);
        
    }
    /* Root of the BPTree */
    ROOT = MallocNewNode();
    
    return ROOT;
}


static Position FindMostLeft(Position P){
    Position Tmp;
    
    Tmp = P;
    
    while (Tmp != NULL && Tmp->Children[0] != NULL) {
        Tmp = Tmp->Children[0];
    }
    return Tmp;
}
 
static Position FindMostRight(Position P){
    Position Tmp;
    
    Tmp = P;
    
    while (Tmp != NULL && Tmp->Children[Tmp->KeyNum - 1] != NULL) {
        Tmp = Tmp->Children[Tmp->KeyNum - 1];
    }
    return Tmp;
}
/* find a sibling return NULL <-> it has full value */
static Position FindSibling(Position Parent,int i){
    Position Sibling;
    int Limit;
    
    Limit = M;
    
    Sibling = NULL;
    if (i == 0){
        if (Parent->Children[1]->KeyNum < Limit)
            Sibling = Parent->Children[1];
    }
    else if (Parent->Children[i - 1]->KeyNum < Limit)
        Sibling = Parent->Children[i - 1];
    else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum < Limit){
        Sibling = Parent->Children[i + 1];
    }
    
    return Sibling;
}

/* find a sibling return NULL <-> number of value > M/2*/
static Position FindSiblingKeyNum_M_2(Position Parent,int i,int *j){
    int Limit;
    Position Sibling;
    Sibling = NULL;
    
    Limit = LIMIT_M;
    
    if (i == 0){
        if (Parent->Children[1]->KeyNum > Limit){
            Sibling = Parent->Children[1];
            *j = 1;
        }
    }
    else{
        if (Parent->Children[i - 1]->KeyNum > Limit){
            Sibling = Parent->Children[i - 1];
            *j = i - 1;
        }
        else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum > Limit){
            Sibling = Parent->Children[i + 1];
            *j = i + 1;
        }
        
    }
    
    return Sibling;
}


/*
* key insert to X
*@ i: position of X in parent
*@ j: position to insert
* X insert to parent
*@ i: position to insert
*@ key&j: useless
*/
static Position InsertElement(int isKey, Position Parent,Position X,KeyType Key,int i,int j){
    
    int k;
    if (isKey){
        /* insert key */
        k = X->KeyNum - 1;
        while (k >= j){
            X->Key[k + 1] = X->Key[k];k--;
        }
        
        X->Key[j] = Key;
        
        if (Parent != NULL)
            Parent->Key[i] = X->Key[0];
        
        X->KeyNum++;
        
    }else{
        /* insert Node */
        
        /* connect the leaf */
        if (X->Children[0] == NULL){
            if (i > 0)
                Parent->Children[i - 1]->Next = X;
            X->Next = Parent->Children[i];
        }
        
        k = Parent->KeyNum - 1;
        while (k >= i){
            Parent->Children[k + 1] = Parent->Children[k];
            Parent->Key[k + 1] = Parent->Key[k];
            k--;
        }
        Parent->Key[i] = X->Key[0];
        Parent->Children[i] = X;
        
        Parent->KeyNum++;
        
    }
    return X;
}
static Position RemoveElement(int isKey, Position Parent,Position X,int i,int j){
    
    int k,Limit;
    
    if (isKey){
        Limit = X->KeyNum;
        /* del key */
        k = j + 1;
        while (k < Limit){
            X->Key[k - 1] = X->Key[k];k++;
        }
        
        X->Key[X->KeyNum - 1] = UNAVALIABLE;
        
        Parent->Key[i] = X->Key[0];
        
        X->KeyNum--;
    }else{
        /* del node */
        
        /* change link between leaf*/
        if (X->Children[0] == NULL && i > 0){
            Parent->Children[i - 1]->Next = Parent->Children[i + 1];
        }
        Limit = Parent->KeyNum;
        k = i + 1;
        while (k < Limit){
            Parent->Children[k - 1] = Parent->Children[k];
            Parent->Key[k - 1] = Parent->Key[k];
            k++;
        }
        
        Parent->Children[Parent->KeyNum - 1] = NULL;
        Parent->Key[Parent->KeyNum - 1] = UNAVALIABLE;
        
        Parent->KeyNum--;
        
    }
    return X;
}
/*
*move element in Src to Dst
*@ Src and DSt is neighbour Node
*@ i position of Src in Parent
*@ n number of moving element
*/
static Position MoveElement(Position Src,Position Dst,Position Parent,int i,int n){
    KeyType TmpKey;
    Position Child;
    int j,SrcInFront;
    
    SrcInFront = 0;
    
    if (Src->Key[0] < Dst->Key[0])
        SrcInFront = 1;
    
    j = 0;
    /* Src infront of Dst */
    if (SrcInFront){
        if (Src->Children[0] != NULL){
            while (j < n) {
                Child = Src->Children[Src->KeyNum - 1];
                RemoveElement(0, Src, Child, Src->KeyNum - 1, UNAVALIABLE);
                InsertElement(0, Dst, Child, UNAVALIABLE, 0, UNAVALIABLE);
                j++;
            }
        }else{
            while (j < n) {
                TmpKey = Src->Key[Src->KeyNum -1];
                RemoveElement(1, Parent, Src, i, Src->KeyNum - 1);
                InsertElement(1, Parent, Dst, TmpKey, i + 1, 0);
                j++;
            }
            
        }
        
        Parent->Key[i + 1] = Dst->Key[0];
        /* reconnect leaf node */
        if (Src->KeyNum > 0)
            FindMostRight(Src)->Next = FindMostLeft(Dst);
        
    }else{
        if (Src->Children[0] != NULL){
            while (j < n) {
                Child = Src->Children[0];
                RemoveElement(0, Src, Child, 0, UNAVALIABLE);
                InsertElement(0, Dst, Child, UNAVALIABLE, Dst->KeyNum, UNAVALIABLE);
                j++;
            }
            
        }else{
            while (j < n) {
                TmpKey = Src->Key[0];
                RemoveElement(1, Parent, Src, i, 0);
                InsertElement(1, Parent, Dst, TmpKey, i - 1, Dst->KeyNum);
                j++;
            }
            
        }
        
        Parent->Key[i] = Src->Key[0];
        if (Src->KeyNum > 0)
            FindMostRight(Dst)->Next = FindMostLeft(Src);
        
    }
    
    return Parent;
}
/*spilt node*/
static BPTree SplitNode(Position Parent,Position X,int i){
    int j,k,Limit;
    Position NewNode;
    
    NewNode = MallocNewNode();
    
    k = 0;
    j = X->KeyNum / 2;
    Limit = X->KeyNum;
    while (j < Limit){
        if (X->Children[0] != NULL){
            NewNode->Children[k] = X->Children[j];
            X->Children[j] = NULL;
        }
        NewNode->Key[k] = X->Key[j];
        X->Key[j] = UNAVALIABLE;
        NewNode->KeyNum++;X->KeyNum--;
        j++;k++;
    }
    
    if (Parent != NULL)
        InsertElement(0, Parent, NewNode, UNAVALIABLE, i + 1, UNAVALIABLE);
    else{
        /* create new RooT and return it */
        Parent = MallocNewNode();
        InsertElement(0, Parent, X, UNAVALIABLE, 0, UNAVALIABLE);
        InsertElement(0, Parent, NewNode, UNAVALIABLE, 1, UNAVALIABLE);
        
        return Parent;
    }
    
    return X;
}

/*
*compile Node
*@ x<M/2
*@ S>=M/2
*/
static Position MergeNode(Position Parent, Position X,Position S,int i){
    int Limit;
    
    /* keywords in S >= m/2 */
    if (S->KeyNum > LIMIT_M){
        /* move a element from S to X */
        MoveElement(S, X, Parent, i,1);
    }else{
        /* Move all X to S & del X */
        Limit = X->KeyNum;
        MoveElement(X,S, Parent, i,Limit);
        RemoveElement(0, Parent, X, i, UNAVALIABLE);
        
        free(X);
        X = NULL;
    }
    
    return Parent;
}
 
static BPTree RecursiveInsert(BPTree T,KeyType Key,int i,BPTree Parent){
    int j,Limit;
    Position Sibling;
    
    /* find branch */
    j = 0;
    while (j < T->KeyNum && Key >= T->Key[j]){
        /* check repeat value */
        if (Key == T->Key[j])
            return T;
        j++;
    }
    if (j != 0 && T->Children[0] != NULL) j--;
    
    /* leaf */
    if (T->Children[0] == NULL)
        T = InsertElement(1, Parent, T, Key, i, j);
    /* inner node */
    else
        T->Children[j] = RecursiveInsert(T->Children[j], Key, j, T);
    
    /* adjust node */
    
    Limit = M;
    
    if (T->KeyNum > Limit){
        /* root */
        if (Parent == NULL){
            T = SplitNode(Parent, T, i);
        }
        else{
            Sibling = FindSibling(Parent, i);
            if (Sibling != NULL){
                /*move a element in T(key|Child) to sibling*/
                MoveElement(T, Sibling, Parent, i, 1);
            }else{
                /* spilt Node */
                T = SplitNode(Parent, T, i);
            }
        }
        
    }
    
    if (Parent != NULL)
        Parent->Key[i] = T->Key[0];
    
    
    return T;
}

/* insert */
extern BPTree Insert(BPTree T,KeyType Key){
    return RecursiveInsert(T, Key, 0, NULL);
}
 
static BPTree RecursiveRemove(BPTree T,KeyType Key,int i,BPTree Parent){
    
    int j,NeedAdjust;
    Position Sibling,Tmp;
    
    Sibling = NULL;
    
    /* find branch */
    j = 0;
    while (j < T->KeyNum && Key >= T->Key[j]){
        if (Key == T->Key[j])
            break;
        j++;
    }
    
    if (T->Children[0] == NULL){
        /* NOTFIND */
        if (Key != T->Key[j] || j == T->KeyNum)
            return T;
    }else
        if (j == T->KeyNum || Key < T->Key[j]) j--;
    
    
    
    /* leaf */
    if (T->Children[0] == NULL){
        T = RemoveElement(1, Parent, T, i, j);
    }else{
        T->Children[j] = RecursiveRemove(T->Children[j], Key, j, T);
    }
    
    NeedAdjust = 0;
    /* Root | a leaf which child number 2< <M*/
    if (Parent == NULL && T->Children[0] != NULL && T->KeyNum < 2)
        NeedAdjust = 1;
    /*except root, child ofall non leaf node M< <[M/2](Rounding up)*/
    else if (Parent != NULL && T->Children[0] != NULL && T->KeyNum < LIMIT_M)
        NeedAdjust = 1;
    /*NoRoot leaf has keyword M< <[M/2](Rounding up)*/
    else if (Parent != NULL && T->Children[0] == NULL && T->KeyNum < LIMIT_M)
        NeedAdjust = 1;
    
    /* Adjust */
    if (NeedAdjust){
        /* Root */
        if (Parent == NULL){
            if(T->Children[0] != NULL && T->KeyNum < 2){
                Tmp = T;
                T = T->Children[0];
                free(Tmp);
                return T;
            }
            
        }else{
            /* find sibling which has keyword more than M/2 */
            Sibling = FindSiblingKeyNum_M_2(Parent, i,&j);
            if (Sibling != NULL){
                MoveElement(Sibling, T, Parent, j, 1);
            }else{
                if (i == 0)
                    Sibling = Parent->Children[1];
                else
                    Sibling = Parent->Children[i - 1];
                
                Parent = MergeNode(Parent, T, Sibling, i);
                T = Parent->Children[i];
            }
        }
        
    }
 
    
    return T;
}
 
/* DEL a node*/
extern BPTree Remove(BPTree T,KeyType Key){
    return RecursiveRemove(T, Key, 0, NULL);
}
 
/* free a node */
extern BPTree Destroy(BPTree T){
    int i,j;
    if (T != NULL){
        i = 0;
        while (i < T->KeyNum + 1){
            Destroy(T->Children[i]);i++;
        }
        
        printf("Destroy:(");
        j = 0;
        while (j < T->KeyNum)/*  T->Key[i] != Unavailable*/
            printf("%d:",T->Key[j++]);
        printf(") ");
        free(T);
        T = NULL;
    }
    
    return T;
}
 
static void RecursiveTravel(BPTree T,int Level){
    int i;
    if (T != NULL){
        printf("  ");
        printf("[Level:%d]-->",Level);
        printf("(");
        i = 0;
        while (i < T->KeyNum)/*  T->Key[i] != Unavailable*/
            printf("%d:",T->Key[i++]);
        printf(")");
        
        Level++;
        
        i = 0;
        while (i <= T->KeyNum) {
            RecursiveTravel(T->Children[i], Level);
            i++;
        }
        
        
    }
}
 
/* ergodic */
extern void Travel_Node(BPTree T){
    RecursiveTravel(T, 0);
    printf("\n");
}
 
/* ergodic leaf */
extern void Travel_Leaf(BPTree T){
    Position Tmp;
    int i;
    if (T == NULL)
        return ;
    printf("All Data:");
    Tmp = T;
    while (Tmp->Children[0] != NULL)
        Tmp = Tmp->Children[0];
    /* first leaf */
    while (Tmp != NULL){
        i = 0;
        while (i < Tmp->KeyNum)
            printf(" %d",Tmp->Key[i++]);
        Tmp = Tmp->Next;
    }
}