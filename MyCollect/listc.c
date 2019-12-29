#include <stdio.h>
#include <stdlib.h>

typedef int data_t;
 
typedef struct node_t {
    data_t data;            /* 节点数据域 */
    struct node_t * next;    /* 节点的后继指针域 */
}linknode_t, *linklist_t;

// 创建一个空链表
linklist_t CreateEmptyLinklist()
{
    linklist_t list;
    list = (linklist_t)malloc(sizeof(linknode_t));
    
    if(NULL != list) {
        list->next = NULL;
    }
    
    return list;
}

// 销毁链表
void DestroyLinklist(linklist_t list)
{
    if(NULL != list) {
        ClearLinklist(list);
        free(list);
    }   
}

// 清空链表
void ClearLinklist(linklist_t list)
{
    linknode_t * node; /* pointer to the node to be remove */
    if(NULL == list) return;
    
    while(NULL != list->next) {
        node = list->next;
        list->next = node->next; //此时node->next是第二node节点元素依次往后
        free(node);
    }
    return;
}

// 是否为空链表
int IsEmptyLinklist(linklist_t list)
{
    if(NULL != list) {
        if(NULL == list->next)  // 只有头节点
            return 1; // 返回1,是个空链表
        else 
            return 0; // 返回0,链表非空
        
    } else 
        return -1;    //  返回-1, 错误的类型
}

// 链表长度
int LengthLinklist(linklist_t list)
{
    int len = 0;
    linknode_t * node; // 遍历指针
    
    if(NULL == list) return -1;
    
    node = list->next; // node指针指向第一个节点
    while(NULL != node) {
        len++;
        node = node->next;
    }
    
    return len;
}

// 获去一个链表指定节点数据域的数据值
int GetLinklist(linklist_t list, int at, data_t * x)
{
    linknode_t *node;   // 遍历节点的指针
    int pos;            // 用于遍历比较
    
    if(NULL == list) return -1;
    /*at 必须要 >= 0*/
    if(at < 0) return -1;
    
    /* 从第一个元素开始 */
    node = list->next; // node指针指向一个元素
    pos = 0;
    while(NULL != node) {
        if(at == pos) {
            if(NULL != x) 
                *x = node->data;
            return 0;
        }
        // 下一个元素
        node = node->next;
        pos++;
    }
    return -1;
}

// 设置一个指定链表节点的数据域值
int SetLinklist(linklist_t list, int at, data_t x)
{
    linknode_t * node; // 遍历链表
    int pos;
    int found = 0;
    
    if(!list) return -1;
    /*at 必须 >= 0*/
    if(at < 0) return -1;
    
    /* node指针指向第一个元素 */
    node = list->next;
    pos = 0;
    while(NULL != node) {
        if(at == pos) {
            found = 1; // 找到了位置
            node->data = x;
            break;
        }
        /*往后移动元素*/
        node = node->next;
        pos++;
    }
    if(1 == found)
        return 0;
    else
        return -1;
}

// 插入节点
int InsertLinklist(linklist_t list, int at, data_t x)
{
    /* 
     * node_at and pos_at are used to locate the position of node_at.
     * node_prev follows the node_at and always points to previous node 
     *    of node_at.
     * node_new is used to point to the new node to be inserted.
     */
    linknode_t      * node_prev, * node_at, * node_new;
    int             pos_at;
    int             found = 0;
    
    if(NULL == list) return -1;
    
    /* at 必须 >= 0 */
    if(at < 0) return -1;
    
    node_new = malloc(sizeof(linknode_t));
    if(NULL == node_new)
        return -1;
    node_new->data = x; // assigned value
    node_new->next = NULL;
    
    node_prev = list; // head
    node_at = list->next; //node_at指针指向第一元素
    pos_at = 0;
    while(NULL != node_at) {
        if(pos_at == at) {
            found = 1; // found the node ‘at'
            break;
        }
        /* move to the next pos_at */
        node_prev = node_at;
        node_at = node_at->next;
        pos_at++;    
    }
    
    if(found) {
        /* insert */
        node_new->next = node_at;
        node_prev->next = node_new;
    } else{
        /*
         * If not found,means the provided 'at'
         * exceeds the upper limit of the list, just
         * append the new node to the end of the list
         */
        node_prev->next = node_new;   
    }
    
    return 0;
}

// 删除节点
int DeleteLinklist(linklist_t list, int at)
{
   /* 
    * node_at and pos_at are used to locate the position of node_at.
    * node_prev follows the node_at and always points to previous node 
    *    of node_at.
    */ 
    linknode_t      * node_prev, * node_at;
    int             pos_at;
    int             found = 0;
    
    if(!list) return -1;
    if(at < 0) return -1;
    
    node_prev = list; // node_prev指针指向链表头
    node_at = list->next; // node_at指针指向第一元素
    pos_at = 0;
    
    while(NULL != node_at) {
        if(pos_at == at) {
            // found the node 'at'
            found = 1;
            break;
        }
        // move to the next pos_at
        node_prev = node_at;
        node_at = node_at->next;
        pos_at++;    
    }
    if(found) {
        // remove 
        node_prev->next = node_at->next;
        free(node_at); 
        return 0;
    }else
        return -1;   
}

// 链表转置
linklist_t ReverseLinklist(linklist_t list)
{
    linknode_t * node;       // iterator
    linknode_t * node_prev;  // previous node of iterator
    linknode_t * node_next;  /* next node of iterator
                             * used to backup next of iterator
                             */
    if(NULL == list) return NULL;
    node_prev = NULL;
    node = list->next; // node指针指向第一个元素
    while(NULL != node) {
        /*
         * step1: backup node->next
         * due to the next of iterator will be
         * modified in step2
         */
        node_next = node->next;			//
        /* 
         * when iterator reaches the last node 
         * of original list, make the list head
         * point to the last node, so the original
         * last one becomes the first one.
         */
        if(NULL == node_next)
            list->next = node;
        /* 
         * step2: reverse the linkage between nodes
         * make the node pointer to the previous node,
         * not the next node
         */        
        node->next = node_prev;		//node的next指向前一个元素。node
        /* 
         * step3: move forward 
         */
        node_prev = node;
        node = node_next;
    }
    
    return list;
}

// 打印链表
int Display(linklist_t list)
{
    linknode_t * node;
    
    if(NULL == list) return -1;
    
    node = list->next;
    while(node != NULL) {
        printf(" %d ", node->data);
        node = node->next;
    } 
    printf("\n");
    
    return 0;
}


int main(int argc, char * argv[])
{
    int i;
    data_t x;
    linklist_t p;
    
    /*创建链表*/
    p = CreateEmptyLinklist();
    Display(p);
    //data_t a[10] = {1,3,5,7,9,11,13,15,17,19};
	data_t a[2] = { 1, 3};

    for(i = 0; i < sizeof(a)/sizeof(data_t); i++) {
        /*插入链表*/
        InsertLinklist(p, i, a[i]);
    }
    Display(p);
    
    /*链表转置*/
    ReverseLinklist(p);
    /*链表长度*/
    printf("The length of the list is [%d]\n", LengthLinklist(p));
    Display(p);
    
    /*获取特定节点值*/
    GetLinklist(p, 4, &x);
    printf("The No.4 of this list is [%d]\n", x);
    
    /*设置特定节点的值*/
    SetLinklist(p, 4, 100);
    GetLinklist(p, 4, &x);
    printf("After updating!The No.4 of this list is [%d]\n", x);
    Display(p);
    
    /*删除节点*/
    DeleteLinklist(p,5);
    printf("After delete!The length of list is [%d]\n", LengthLinklist(p));
    Display(p);
    
    /*清空链表*/
    ClearLinklist(p);
    if(IsEmptyLinklist(p)) 
        printf("This list is empty!\n");
    /*销毁链表*/
    DestroyLinklist(p);
    printf("This list is destroyed!\n");
    
   
    return 0;
    
}

