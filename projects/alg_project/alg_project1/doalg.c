#include <stdio.h>   
#include <stdlib.h> 

#define STORELIMIT 1000

typedef struct _Node
{
	int val;
	int deg;
	struct _Node* next;
	struct _Node* prev;
	struct _Node* firstchild;
	struct _Node* parent;
} Node;

typedef struct _CompRes
{
	int a;
	int b;
	int res;
} CompRes;

typedef struct _FibonacciHeap
{
	int count;
	struct _Node* max;
	int store_sz;
	CompRes stored[STORELIMIT];
} FibonacciHeap;


void InitRes(FibonacciHeap* h, int a, int b, int res)
{
	if (h->store_sz >= STORELIMIT)return;
	h->stored[h->store_sz].a = a;
	h->stored[h->store_sz].b = b;
	h->stored[h->store_sz++].res = res;
}

int MapResult(FibonacciHeap* h,int a, int b)
{
	for (int i = 0; i < h->store_sz; ++i)
	{
		if (h->stored[i].a == a && h->stored[i].b == b)
			return h->stored[i].res;
		if (h->stored[i].b == a && h->stored[i].a == b)
			return (h->stored[i].res == 2) ? 1 : 2;
	}
	return 0;
}



void _insert(Node* root, Node* node)
{
	if (root == NULL || node == NULL)
		return;

	root->prev->next = node;
	node->prev = root->prev;
	root->prev = node;
	node->next = root;
}

void Insert(FibonacciHeap* heap, Node* node)
{
	//assert the node is properly initialized
	if (heap->count == 0)
		heap->max = node;
	else
		_insert(heap->max, node);
	++(heap->count);
}

void _remove(Node* node)
{
	if (node == NULL)return;
	node->next->prev = node->prev;
	node->prev->next = node->next;

	node->next = node;
	node->prev = node;
}



int _merge(FibonacciHeap* heap, int record)
{
	Node* degs[20];
	for (int i = 0; i < 20; ++i)degs[i] = NULL;
	Node* iter = heap->max;
	int remained = 1;
	while (iter->next != heap->max)
	{
		iter = iter->next;
		remained++;
	}
	while (remained--)
	{
		Node* n = iter->next;
		if (degs[iter->deg] == NULL)
			degs[iter->deg] = iter;
		else
		{
			int d = iter->deg;
			while (degs[d] && degs[d]!=iter)
			{
				int res = MapResult(heap,iter->val, degs[d]->val);
				if (!res)
				{
					res = COMPARE(iter->val, degs[d]->val);
					if (record)InitRes(heap,iter->val, degs[d]->val, res);
				}
				if (res == 1)
				{
					degs[d]->parent = iter;
					_remove(degs[d]);
					if (iter->firstchild)
						_insert(iter->firstchild, degs[d]);
					else
						iter->firstchild = degs[d];
				}
				else if (res == 2)
				{
					iter->parent = degs[d];
					_remove(iter);
					if (degs[d]->firstchild)
						_insert(degs[d]->firstchild, iter);
					else
						degs[d]->firstchild = iter;
					iter = degs[d];
				}
				else
					return -1;
				iter->deg++;
				degs[d++] = NULL;
			}
			degs[d] = iter;
		}
		iter = n;
	}
	Node* newmax = NULL;
	for (int i = 0; i < 20; ++i)
	{
		if (degs[i])
		{
			if (!newmax)newmax = degs[i];
			else
			{
				int res = MapResult(heap,newmax->val, degs[i]->val);
				if (!res)
				{
					res = COMPARE(newmax->val, degs[i]->val);
					if (record)InitRes(heap,newmax->val, degs[i]->val, res);
				}
				if (res == 2)
					newmax = degs[i];
			}
		}
	}
	heap->max = newmax;
	return 0;
}

Node* GetTop(FibonacciHeap* heap)
{
	if (heap == NULL)return NULL;
	return heap->max;
}


int PopTop(FibonacciHeap* heap, int merge)
{
	if (heap == NULL || heap->max == NULL)return -2;
	Node* res = heap->max;
	if (res->firstchild)
	{
		Node* lc = res->firstchild;
		while (lc->next != res->firstchild)
		{
			lc->parent = NULL;
			lc = lc->next;
		}
		res->firstchild->parent = NULL;
		lc->parent = NULL;
		res->prev->next = res->firstchild;
		res->firstchild->prev = res->prev;
		lc->next = res;
		res->prev = lc;
	}
	Node* temp = res->next;
	_remove(res);
	if (temp == res)
		heap->max = NULL;
	else
	{
		heap->max = temp;
		if (merge && _merge(heap,1) == -1)return -1;
	}
	--(heap->count);
	return 0;
}

Node* _create(int val)
{
	Node* res = (Node*)malloc(sizeof(Node));
	res->val = val;
	res->deg = 0;
	res->next = res;
	res->prev = res;
	res->firstchild = NULL;
	res->parent = NULL;
	return res;
}

void _delete(Node* node)
{
	_remove(node);
	while (node->firstchild)
	{
		Node* old = node->firstchild;
		Node* temp = node->firstchild->next;
		_delete(old);
		if (old == temp)break;
		node->firstchild = temp;
	}
	free(node);
}
void Free(FibonacciHeap* heap)
{
	if (heap == NULL)return;
	while (heap->max)
	{
		Node* old = heap->max;
		Node* temp = heap->max->next;
		_delete(old);
		if (old == temp)break;
		heap->max = temp;
	}
}

int doalg(int n, int k, int* Best)
{
	FibonacciHeap fh;
                    fh.store_sz=0;
	fh.count = 0;
	fh.max = NULL;
	for (int i = 0; i < n; ++i)
		Insert(&fh, _create(i + 1));
	_merge(&fh, 0);
	Node* t = fh.max;
	int c=0;
	for (int i = 0; i < k; ++i)
	{
		Node* n = GetTop(&fh);
		int res = PopTop(&fh, (i == k - 1) ? 0 : 1);
		if (res == -1)
		{
			printf("x or y is out of range!\n");
			Free(&fh);
			return 0;
		}
		else if (res == -2)
		{
			printf("heap is NULL!\n");
			Free(&fh);
			return 0;
		}
		Best[c++] = n->val;
		free(n);
	}
	Free(&fh);
	return 1;
}