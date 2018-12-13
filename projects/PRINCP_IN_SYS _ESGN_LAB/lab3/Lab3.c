
//Yangguang He  50947171
//Aoran Chen 54044152

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int vpage;
    int valid;
    int dirty;
    int page;
} PageTableEntry;

PageTableEntry* LRU[4]; //做一个简单的前pop堆来处理最晚刷新的地址。
int LRUsz=0;
#define LRUMAX 4
int mem[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };   //The "physical address that is a location in your "main memory" array.
int disk[16] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
PageTableEntry page_table[8] = {
        { 0, 0, 0, 0 },
        { 1, 0, 0, 1 },
        { 2, 0, 0, 2 },
        { 3, 0, 0, 3 },
        { 4, 0, 0, 4 },
        { 5, 0, 0, 5 },
        { 6, 0, 0, 6 },
        { 7, 0, 0, 7 },
};


// tools for LRU

void update(PageTableEntry* to_update)
{
	int i;
	int f=0;
	for(i=LRUsz-1;i>0;--i)
	{
		if(LRU[i]==to_update || f)
		{
			f=1;
			LRU[i]=LRU[i-1];
		}
	}
	LRU[0]=to_update;
}

void insert(PageTableEntry* to_insert)
{
	int i;
	if(++LRUsz>LRUMAX)
		LRUsz=LRUMAX;
	for(i=LRUsz-1;i>=0;--i)
		LRU[i]=LRU[i-1];
	LRU[0]=to_insert;
}

int find(PageTableEntry* idx)
{
	int i;
	for(i=0;i<LRUsz;++i)
		if(LRU[i]==idx)return i;
	return -1;
}

void Run_LRU(int idx)
{
	PageTableEntry* i=page_table+idx;
	int f=find(i);
	if(f!=-1)
		update(i);
	else
		insert(i);
}


// tools for handler

void diskwrite(int address)
{
    int disk_addresss = page_table[address].vpage;
    int maddress = page_table[address].page;
    disk[disk_addresss * 2] = mem[maddress * 2];
    disk[disk_addresss * 2 + 1] = mem[maddress * 2 + 1];
}

void handle_function(int address) //
{
	if(LRUsz==LRUMAX)
	{
		PageTableEntry* pte=LRU[LRUsz-1];
		int pn=pte->page;
		if(pte->dirty)
		{
			int vpn=pte->vpage;
			disk[vpn*2]=mem[pn*2];
			disk[vpn*2+1]=mem[pn*2+1];
			pte->dirty=0;
		}
		pte->valid=0;
		pte->page=pte->vpage;
		page_table[address].page=pn;
		mem[pn*2]=disk[address*2];
		mem[pn*2+1]=disk[address*2+1];
	}
	else
		page_table[address].page=LRUsz;
	
	page_table[address].valid=1;
}


// command functions

void read(int address)
{
    int number,page;
    if(address %2 == 0)
    {
        page = address / 2;
        if(page_table[page].valid == 1)           //
        {
            number =  mem[page_table[page].page*2];
        }
        else                                     //
        {
            handle_function(page);               //
            number = mem[page_table[page].page*2];
        }
    }
    else
    {
        page = (address-1) / 2;
        if(page_table[page].valid == 1)
        {
            number = mem[page_table[page].page*2+1];
        }
        else
        {
            handle_function(page);
            number = mem[page_table[page].page*2 + 1];
        }
    }
    Run_LRU(page);
    printf("read: %i     %i\n",address,number);

}

void  write(int address, int number)
{
	int page;
    if(address %2 == 0)
    {
        page = address / 2;
        if(page_table[page].valid == 1)           //直接找到内存
        {
            mem[page_table[page].page*2] = number;
            page_table[page].dirty = 1;
        }
        else                                     //没找到要写入
        {
            handle_function(page);               //处理页缺失
            mem[page_table[page].page*2] = number;
            page_table[page].dirty = 1;
        }
    }
    else
    {
        address = address -1;
        page = address / 2;
        if(page_table[page].valid == 1)
        {
            mem[page_table[page].page*2+1] = number;
            page_table[page].dirty = 1;
        }
        else
        {
            handle_function(page);
            mem[page_table[page].page*2+1] = number;
            page_table[page].dirty = 1;
        }
    }
    Run_LRU(page);
}

void showmain(int ppn)   //只print有值的，没有值的跳过
{
	int i;
    printf("%-10s%-10s\n","address","contents");
    for(i = ppn*2; i < ppn*2+2; i++)
    {
        printf("%-10i%-10i\n",i,mem[i]);
    }
}

void showdisk(int page)                       //无论有没有值都print
{
        printf("%-10s%-10s\n","address","contents");
        printf("%-10i%-10i\n",page*2,disk[page * 2]);
        printf("%-10i%-10i\n",page*2+1,disk[page * 2 + 1]);
}

void showptable()
{
	int i;
    printf("%-10s%-10s%-10s%-10s\n","VPageNum","Valid","Dirty","PN");
    for(i = 0; i < 8; i ++)
    {
        printf("%-10i%-10i%-10i%-10i\n",page_table[i].vpage,page_table[i].valid,page_table[i].dirty,page_table[i].page);
    }
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
		else if (line[i] == ' '||line[i]=='\r' ||line[i]=='\n')
		{
			flag = 0;
			line[i]='\0';
		}
		++i;
	}
}





int main() {
    char line[256];
    while (1)
    {
	printf("$ ");
        fgets(line,256,stdin);
		if (strlen(line) == 0)
			printf("Wrong command!\n");
		else
		{
			char* cmd[5];
			int i;
			for (i = 0; i < 5; ++i)
				cmd[i] = line;
			process_cmd(line, cmd);
			if (!strcmp(cmd[0], "read"))
				read(atoi(cmd[1]));
			else if (!strcmp(cmd[0], "write"))
				write(atoi(cmd[1]), atoi(cmd[2]));
			else if (!strcmp(cmd[0], "showmain"))
				showmain(atoi(cmd[1]));
			else if (!strcmp(cmd[0], "showdisk"))
				showdisk(atoi(cmd[1]));
			else if (!strcmp(cmd[0], "showptable"))
				showptable();
			else if (!strcmp(cmd[0], "quit"))
				break;
			else
				printf("Wrong command\n");
		}
			
    }
    return 0;
}
