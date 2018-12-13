#include <stdio.h>   
#include <stdlib.h> 

#define MAXSIZE 3
#ifndef MAXN
#define MAXN 10000
#endif
#ifndef NULL
#define NULL 0
#endif
#define ABS(x) ((x)>0?(x):-(x))
#define MIN(x,y) ((x)>(y)?(y):(x))
#define DEBUG 0
#if DEBUG>=2
#define MYQCOUNT COUNT
#else
#define MYQCOUNT QCOUNT
#endif


typedef struct _ListNode
{
	int size;
	int idx[4];
	char merged;
	struct _ListNode* next;
} ListNode;




ListNode* CreateLN(int size, int* arr)
{
	ListNode* res = (ListNode*)malloc(sizeof(ListNode));
	res->size = size;
	for (int i = 0; i < 4; i++)
		res->idx[i] = arr[i];
	res->next = NULL;
	res->merged = 0;
	return res;
}

void Fill2x2(int* arr, int* arr1, int* arr2)
{
	for (int i = 0; i < 2; ++i)
		arr[i] = arr1[i];
	for (int i = 2; i < 4; ++i)
		arr[i] = arr2[i-2];
}

void FillWithArr(int* arr, int* fill_arr)
{
	for (int i = 0; i < 4; ++i)
		arr[i] = fill_arr[i];
}



ListNode* MergePair(ListNode* l1, ListNode* l2, ListNode** p_zero)
{
	int arr[4];
	if (l2->size > l1->size)
	{
		ListNode* temp = l1;
		l1 = l2;
		l2 = temp;
	}
	Fill2x2(arr, l1->idx, l2->idx);
	int ans = MYQCOUNT(1, arr);
	if (l1->size >= 4 && l2->size >= 4)
	{
		if (ans == 0)
		{
			if (!*p_zero)*p_zero = CreateLN(0, arr);
			l1->size -= l2->size;
			if (l1->size == 0)
			{
				free(l1);
				free(l2);
				return NULL;
			}
		}
		else
			l1->size += l2->size;
	}
	else if(l1->size>=4 && l2->size==2)
	{
		if (ans == 4)
			l1->size += 2;
		else if (ans == 0)
			l1->size -= 2;
		else
		{
			Fill2x2(arr, l1->idx, l2->idx + 2);
			ans = MYQCOUNT(1, arr);
			if (ans == 4)
				l1->size += 2;
			else
				l1->size -= 2;
		}
	}
	else if(!l1->merged && !l2->merged)
	{
		if (ans == 2)
		{
			Fill2x2(arr, l1->idx, l2->idx + 2);
			ans = MYQCOUNT(1, arr);
			if (ans == 0)
			{
				if (!*p_zero)*p_zero = CreateLN(0, arr);
				Fill2x2(arr, l1->idx + 2, l2->idx);
				ans = MYQCOUNT(1, arr);
				if (ans == 0)
				{
					free(l1);
					free(l2);
					return NULL;
				}
				else
				{
					l1->size = 4;
					l1->idx[0] = l2->idx[0];
					l1->idx[1] = l2->idx[1];
				}
			}
			else
			{
				l1->size = 4;
				l1->idx[2] = l2->idx[2];
				l1->idx[3] = l2->idx[3];
			}
		}
		else if (ans == 4)
		{
			l1->size = 4;
			l1->idx[2] = l2->idx[0];
			l1->idx[3] = l2->idx[1];
		}
		else
		{
			Fill2x2(arr, l1->idx+2, l2->idx + 2);
			ans = MYQCOUNT(1, arr);
			if (ans == 0)
			{
				if (!*p_zero)*p_zero = CreateLN(0, arr);
				free(l1);
				free(l2);
				return NULL;
			}
			else
			{
				l1->size = 4;
				l1->idx[0] = l2->idx[2];
				l1->idx[1] = l2->idx[3];
			}
		}
	}
	else if (l1->merged && l2->merged)
	{
		if (ans == 0)
		{
			if (!*p_zero)*p_zero = CreateLN(0, arr);
			free(l1);
			free(l2);
			return NULL;
		}
		else
		{
			l1->size = 4;
			l1->idx[2] = l2->idx[0];
			l1->idx[3] = l2->idx[1];
		}
	}
	else
	{
		if (l2->merged)
		{
			ListNode* temp = l1;
			l1 = l2;
			l2 = temp;
		}
		if (ans == 0)
		{
			if (!*p_zero)*p_zero = CreateLN(0, arr);
			free(l1);
			free(l2);
			return NULL;
		}
		else if (ans == 4)
		{
			l1->size = 4;
			l1->idx[2] = l2->idx[0];
			l1->idx[3] = l2->idx[1];
		}
		else
		{
			Fill2x2(arr, l1->idx, l2->idx + 2);
			ans = MYQCOUNT(1, arr);
			if (ans == 0)
			{
				if (!*p_zero)*p_zero = CreateLN(0, arr);
				free(l1);
				free(l2);
				return NULL;
			}
			else
			{
				l1->size = 4;
				l1->idx[2] = l2->idx[0];
				l1->idx[3] = l2->idx[1];
			}
		}
	}
	free(l2);
	l1->next = NULL;
	l1->merged = 1;
	return l1;
}


void Fill(int* arr, int start)
{
	for (int i = 0; i < 4; ++i)
		arr[i] = start + i;
}



ListNode* Rearrange(ListNode* root)
{
	ListNode *iter1 = NULL, *iter2 = NULL;
	ListNode* first = NULL;
	ListNode* second = NULL;
	while (root)
	{
		if (root->size >= 4)
		{
			if (!iter1)
				first = root;
			else
				iter1->next = root;
			iter1 = root;
		}
		else
		{
			if (!iter2)
				second = root;
			else
				iter2->next = root;
			iter2 = root;
		}
		root = root->next;
	}
	if(iter1)
		iter1->next = second;
	if(iter2)
		iter2->next = NULL;
	return first ? first : second;
}


void FreeChain(ListNode* root)
{
	if (!root)return;
	if (root->next)
		FreeChain(root->next);
	free(root);
}


void PrintChain(ListNode* l)
{
	ListNode* temp = l;
	while (temp)
	{
		printf("(size:%d next:%d idx:[%d %d %d %d])->", temp->size, temp->next,temp->idx[0],temp->idx[1],temp->idx[2],temp->idx[3]);
		temp = temp->next;
	}
	printf("\n");

}


int mysub(int n)
{
	int arr[4];
	ListNode* root = NULL;
	ListNode* now = NULL;
	ListNode* zero = NULL;
	int cn = 0;
	//get around 312 nodes
	for (int i = 1; i+3 <= n; i += 4)
	{
		Fill(arr, i);
		int ans = MYQCOUNT(1, arr);
		if (ans != 0)
		{
			cn++;
			if (!root)
			{
				root = CreateLN(ans, arr);
				now = root;
			}
			else
			{
				now->next = CreateLN(ans, arr);
				now = now->next;
			}
		}
		else if (!zero)
			zero = CreateLN(0, arr);
	}
	//process winner
	root = Rearrange(root);

	int leftnum = n % 4;
	while (root && root->next)
	{
		int sum_size = 0;
		now = root;
		ListNode* now_2 = root->next;
		ListNode* chain = NULL;
		root = NULL;
		while (1)
		{
			ListNode* next = now_2->next;
			ListNode* newnode = MergePair(now, now_2, &zero);
			if (newnode)
			{
				sum_size += newnode->size;
				if (chain == NULL)
				{
					root = newnode;
					chain = newnode;
				}
				else
				{
					chain->next = newnode;
					chain = chain->next;
				}
			}
			now = next;
			if (!now)break;
			now_2 = now->next;
			if (!now_2) 
			{
				sum_size += now->size;
				if (chain == NULL)
					root = now;
				else
					chain->next = now;
				break;
			}
		}
		
		root = Rearrange(root);
		//early stop
		ListNode* temp = root;
		while (temp)
		{
			if (temp->size > 1 + (leftnum + sum_size) / 2)
			{
				int res = temp->idx[0];
				FreeChain(root);
				FreeChain(zero);
				return res;
			}
			temp = temp->next;
		}
	}

	
	if (root && root->size >= 4)
	{
		int res = root->idx[0];
		FreeChain(root);
		FreeChain(zero);
		return res;
	}
	if (!leftnum && !root)
	{
		FreeChain(root);
		FreeChain(zero);
		return 0;
	}
	if (root && root->merged)
	{
		int res = root->idx[0];
		if (leftnum < 2)
		{
			FreeChain(root);
			FreeChain(zero);
			return res;
		}
		if (leftnum == 2)
		{
			Fill(arr, n - leftnum + 1);
			arr[2] = root->idx[0];
			arr[3] = root->idx[1];
			int ans = MYQCOUNT(1, arr);
			FreeChain(root);
			FreeChain(zero);
			if (ans == 0) return 0;
			else return res;
		}
		if (leftnum == 3)
		{
			Fill(arr, n - leftnum + 1);
			arr[2] = root->idx[0];
			arr[3] = root->idx[1];
			int ans = MYQCOUNT(1, arr);
			FreeChain(root);
			FreeChain(zero);
			if (ans == 0)return n;
			else return res;
		}
	}
	//process leftover
	int group1[4] = { 0 };
	int group2[4] = { 0 };
	int sz1 = 0, sz2 = 0;
	for (int i = n - leftnum + 1; i <= n; ++i)
	{
		FillWithArr(arr, zero->idx);
		arr[0] = i;
		int ans = MYQCOUNT(1, arr);
		if (ans == 0)
			group1[sz1++] = i;
		else
			group2[sz2++] = i;
	}
	if (root)
	{
		for (int i = 0; i < 4; ++i)
		{
			FillWithArr(arr, zero->idx);
			arr[0] = root->idx[i];
			int ans = MYQCOUNT(1, arr);
			if (ans == 0)
				group1[sz1++] = arr[0];
			else
				group2[sz2++] = arr[0];
			if (sz1 > (leftnum + 4) / 2)
			{
				FreeChain(zero);
				FreeChain(root);
				return group1[0];
			}
			if (sz2 > (leftnum + 4) / 2)
			{
				FreeChain(zero);
				FreeChain(root);
				return group2[0];
			}
		}
	}
	FreeChain(zero);
	FreeChain(root);
	if (sz1 == sz2)
		return 0;
	if (sz1 > sz2)
		return group1[0];
	else
		return group2[0];

}
