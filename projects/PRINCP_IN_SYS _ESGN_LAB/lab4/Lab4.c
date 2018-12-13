//Yangguang He 50947171 Aoran Chen 54044152
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned char* heap_space;
int number; //记录一共allocate的次数

void init_heap()
{
    number = 1;
    heap_space = malloc(127);
    int i;
    for(i = 0; i < 127; i ++ )
    {
        heap_space[i] = 0;     //全部统一为一个统一字符 PS：没卵用，因为用了pointer和number
    }
	heap_space[0] = 0;
	heap_space[1] = 250;
}

void exit_heap()
{
    free(heap_space);
}

//tools
int isvalid(int idx)
{
	return heap_space[idx + 1] & 0x01;
}

int getsize(int idx)
{
	return heap_space[idx + 1]>>1;
}

int findblock(int i,int number)
{
	return isvalid(i) && heap_space[i] == number;
}

void find_and_alloc(int size) //用来找一个合适的allocate的空间
{
    int i;
    for(i=0; i < 127;)
    {
		int bsz = getsize(i);
		if (!isvalid(i) && bsz >= size)
		{
			if (bsz - size <= 2)
				size = bsz;
			heap_space[i] = number;
			heap_space[i + 1] = (size << 1) + 1;
			if (bsz>size)
				heap_space[i + size + 3] = (bsz - size-2) << 1;
			printf("%d\n", number++);
			return;
		}
		i += bsz+2;
    }
}

int check_finder(int number) //找到具体位置的block
{
	int i;
	for (i = 0; i < 127;)
	{
		if (findblock(i, number))
		{
			return i;
		}
		i += getsize(i) + 2;
	}
	return -1;
}

//function
void allocate(int size)
{
    if(size < 0)
    {
        printf("invaild size\n");
        return;
    }
    else
    {
		find_and_alloc(size);
		return;
    }
}

void freeheap(int number)
{
	int i;
	for (i = 0; i < 127; )
	{
		if (findblock(i,number))
		{
			heap_space[i + 1] -= 1;
			return;
		}

		i += getsize(i)+2;
	}
    
    return;
}

void blocklist()
{
    int int_space = 0;
    printf("Size Allocated Start           End           \n");
    int i;
    for(i = 0; i < 127 ; )
    {
		if (isvalid(i))
			printf("%-5d%-10s%-16i%-16i\n", 2 + getsize(i) , "yes", i, i+1 + getsize(i));
		else
			printf("%-5d%-10s%-16i%-16i\n", 2 + getsize(i), "no", i, i+1 + getsize(i));
		i += getsize(i) + 2;
    }
    return;
}

void writeheap(int number, char element, int times)
{
    int int_space;
    if(number < 0)
    {
        printf("number error\n");
        return;
    }
    if(times < 0)
    {
        printf("times error\n");
        return;
    }
	int_space = check_finder(number);
	if (int_space == -1)
	{
		printf("block had been freed\n");
		return;
	}
    if(times > getsize(int_space))
    {
        printf("block size too small\n");
        return;
    }
    int_space = int_space + 2;
    int i;
    for(i = 0; i < times; i ++)
    {
        heap_space[int_space + i] = element;
    }
    return;
}

void printheap(int number, int times)
{
    int int_space;
    if(number < 0)
    {
        printf("number error\n");
        return;
    }
    if(times < 0)
    {
        printf("times error\n");
        return;
    }
    int_space = check_finder(number);
    if(int_space == -1)
    {
        printf("block had been freed\n");
        return;
    }
    if(times > getsize(int_space))
		times = getsize(int_space);
    int_space = int_space + 2;
    int i;
    for(i = 0; i < times; i++)
		printf("%c", heap_space[int_space + i]);
	printf("\n");
}

void printheader(int number)
{
	int i=check_finder(number);
	if (i!=-1)
	{
		printf("%02x%02x\n", heap_space[i], 4+heap_space[i + 1]);
		return;
	}
    printf("block not found\n");
}

void process_cmd(char line[256], char* cmd[5])
{
    int i = 0;
    int c = 0;
    int stop = strlen(line);
    int flag = 0;
    while (i < stop)
    {
        if (!flag && line[i] != ' ')
        {
            flag = 1;
            cmd[c++] = line + i;
        }
        else if (line[i] == ' ' || line[i] == '\r' || line[i] == '\n')
        {
            flag = 0;
            line[i] = '\0';
        }
        ++i;
    }
}

int main() {
    char line[256];
    init_heap();
    while (1)
    {
        printf("> ");
        fgets(line, 256, stdin);
        if (strlen(line) == 0)
            printf("Wrong command!\n");
        else
        {
            char* cmd[5];
            int i;
            for (i = 0; i < 5; ++i)
                cmd[i] = line;
            process_cmd(line, cmd);
            if (!strcmp(cmd[0], "allocate"))
                allocate(atoi(cmd[1]));
            else if (!strcmp(cmd[0], "free"))
                freeheap(atoi(cmd[1]));
            else if (!strcmp(cmd[0], "blocklist"))
                blocklist();
            else if (!strcmp(cmd[0], "writeheap"))
                writeheap(atoi(cmd[1]), *cmd[2], atoi(cmd[3]));
            else if (!strcmp(cmd[0], "printheap"))
                printheap(atoi(cmd[1]), atoi(cmd[2]));
            else if (!strcmp(cmd[0], "printheader"))
                printheader(atoi(cmd[1]));
            else if (!strcmp(cmd[0], "quit"))
                break;
            else
                printf("Wrong command\n");
        }

    }
	exit_heap();
    return 0;

}