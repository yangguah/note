//Yangguang He  ID:50947171
//Rocky Zhang   ID: 54487165



#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>
#include<sys/wait.h>

#define MAXLEN 10
#define MAXSTR 10
#define MAXBUF 256

int* result;
FILE* fp;
int arr[MAXLEN] = { 0 };
int count = 0;
int target;
char numstrs[MAXLEN][MAXSTR] = { 0 };
int shm_id;
int pid;


void Boom(const char* str)
{
	printf(str);
	exit(-1);
}



void Search(int lo, int hi)
{
	if (lo == hi)
	{
		if (arr[lo] == target)
			*result = lo;
		printf("Process %d Found %d at index %d\n", getpid(), arr[lo], lo);
	}
	else
	{
		int m = (lo + hi) >> 1;
		int id1 = fork();
		if (id1 == -1)
			Boom("Error occurs when executing fork\n");
		if (id1==0)
			Search(lo,m);
		else
		{
			int id2 = fork();
			if (id1 == -1)
				Boom("Error occurs when executing fork\n");
			if(id2==0)
				Search(m+1,hi);
		}
      while (wait(NULL) != -1);
	}
}



int Str2Int(const char* str, int* res)
{
	*res = 0;
	int minus = 0;
	int i = 0;
	int size = strlen(str);
	if (str[0] == '-')minus = ++i;
	else if (str[0] == '+')++i;
	for (; i < size; ++i)
		if (str[i] >= '0' && str[i] <= '9')
		{
			*res *= 10;
			*res += str[i] - '0';
		}
		else
			return -1;
	if (minus)*res *= -1;
	return 0;
}



void ProcessFile(const char* filename)
{
	if (!(fp = fopen(filename,"r")))
		Boom("The file doesn't exist\n");
	char* buf=(char*)malloc(MAXBUF*sizeof(char));
	ssize_t buflen = MAXBUF;
	ssize_t chars=getline(&buf, &buflen, fp);
	int i=0;
	while (buf[i] == ' ')++i;
	while (buf[chars - 1] == '\r' || buf[chars - 1] == '\n' || buf[chars - 1] == ' ')--chars;
	int flag = 0;
	int j = 0;
	for (; i < chars; ++i)
	{
		if (buf[i] == ' ')
		{
			if (flag) 
			{
				numstrs[count++][j] = '\0'; 
				j = 0;
			}
			flag = 0;
		}
		else
		{
			if (count == 10)Boom("Too many numbers\n");
			flag = 1;
			numstrs[count][j++] = buf[i];
		}
	}
	++count;
	for (i = 0; i < count; ++i)
		if (Str2Int(numstrs[i], &arr[i]))
			Boom("Error occurs when parsing strings\n");
	free(buf);
}



int main(int argc, const char * argv[]) {
	if (argc != 3)
		Boom("# of Arguments != 3\n");
	ProcessFile(argv[1]);
	if(Str2Int(argv[2], &target))
		Boom("Error occurs when parsing target number\n");
	shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
	if (shm_id < 0)Boom("Error occurs when creating shared mem\n");
	void* addr = shmat(shm_id, NULL, 0);
	if (addr == (void*)-1)Boom("Error occurs when executing shmat\n");
	result = (int*)addr;
	*result = -1;
	pid = getpid();
	Search(0,count-1);
	if(getpid()==pid)
		printf("Search Result: %d\n", *result);
}
