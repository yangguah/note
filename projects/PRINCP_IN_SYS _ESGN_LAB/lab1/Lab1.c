//  Name: Yangguang He     ID: 50947171
//  Lab1
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCOURSE 20
#define MAXNUMCOURSE 100
#define MAXLINE 100


void offering(const char* filename, char* offeringcourse[3][MAXNUMCOURSE])
{
    FILE* fpRead = fopen(filename, "r");
    int idx;
    char course[MAXCOURSE]={0};
    int a = 0; int b = 0; int c = 0;
    while (fscanf(fpRead, "%s %d\n", course, &idx) == 2)
    {
        if (idx == 1)
        {
            offeringcourse[0][a] = (char*)malloc(MAXCOURSE * sizeof(char));
            strcpy(offeringcourse[0][a], course);
            a += 1;
        }
        else if (idx == 2)
        {
            offeringcourse[1][b] = (char*)malloc(MAXCOURSE * sizeof(char));
            strcpy(offeringcourse[1][b], course);
            b += 1;
            
        }
        else if (idx == 3)
        {
            offeringcourse[2][c] = (char*)malloc(MAXCOURSE * sizeof(char));
            strcpy(offeringcourse[2][c], course);
            c += 1;
        }
    }
    
}
void Prerequisite(const char* filename, char* prerequisitecourse[MAXNUMCOURSE][MAXNUMCOURSE])
{
    
    FILE* fpRead = fopen(filename, "r");
    char* buffer = (char*)malloc(MAXLINE * sizeof(char));
    memset(buffer,0,MAXLINE * sizeof(char));
    size_t bufsz = MAXLINE;
    int a = 0;
    size_t num;
    while ((num = getline(&buffer, &bufsz, fpRead)) != -1)
    {
        if (buffer[num - 1] != '\n')
        {
            buffer[num] = ' ';
            buffer[num + 1] = '\0';
        }
        int i = 0; int b = 0; char course[MAXCOURSE]={0}; int q = 0;
        while (buffer[i] != '\0')
        {
            
            if (buffer[i] == ' ' || buffer[i] == '\n')
            {
                if(strlen(course)>0)
                {
                    prerequisitecourse[a][b] = (char*)malloc(MAXCOURSE * sizeof(char));
                    strcpy(prerequisitecourse[a][b], course);
                }
                memset(course,0,MAXCOURSE*sizeof(char));
                q = 0; i++; b++;
                continue;
                
            }
            course[q] = buffer[i];
            q++; i++;
        }
        
        a += 1;
    }
    
    
    
}

void Requirement(const char* filename, char* requirementcourse[MAXNUMCOURSE])
{
    FILE* fpRead = fopen(filename, "r");
    char* buffer = (char*)malloc(MAXLINE * sizeof(char));
    memset(buffer,0,MAXLINE * sizeof(char));
    size_t bufsz = MAXLINE;
    int a = 0;
    size_t num = getline(&buffer, &bufsz, fpRead);
    if (buffer[num - 1] != '\n')
    {
        buffer[num] = ' ';
        buffer[num + 1] = '\0';
    }
    int i = 0;  char course[MAXCOURSE]={0}; int q = 0;
    while (buffer[i] != '\0')
    {
        
        if (buffer[i] == ' ' || buffer[i] == '\n')
        {
            requirementcourse[a] = (char*)malloc(MAXCOURSE * sizeof(char));
            strcpy(requirementcourse[a], course);
            memset(course,0,MAXCOURSE*sizeof(char));
            q = 0; i++; a++;
            continue;
            
        }
        course[q] = buffer[i];
        q++; i++;
    }
}

void Delete2(int idx,int idx2, char* arr[MAXNUMCOURSE][MAXNUMCOURSE])
{
    free(arr[idx][idx2]);
    int i;
    for (i = idx2; arr[idx][i] != NULL; ++i)
        arr[idx][i] = arr[idx][i + 1];
    arr[idx][i] = NULL;
}
void Delete1(int idx, char* arr[MAXNUMCOURSE])
{
    free(arr[idx]);
    int i;
    for (i = idx; arr[i] != NULL; ++i)
        arr[i] = arr[i + 1];
    arr[i] = NULL;
}

int SearchInReq(char* course, char* req[MAXNUMCOURSE])
{
    int i;
    for(i=0;req[i]!=NULL;++i)
        if(!strcmp(req[i],course))
            return 1;
    return 0;
}

int SearchInPre(char* course, char* pre[MAXNUMCOURSE][MAXNUMCOURSE])
{
    int i;
    for (i = 0; pre[i][0] != NULL; ++i)
    {
        if (!strcmp(course, pre[i][0]))
            return 1;
    }
    return 0;
}

int SearchInOffers(int term,char* course, char* offer[3][MAXNUMCOURSE])
{
    int i;
    for (i = 0; offer[term-1][i] != NULL; ++i)
    {
        if (!strcmp(course, offer[term-1][i]))
            return 1;
    }
    return 0;
}

void PreProcess(char* pre[MAXNUMCOURSE][MAXNUMCOURSE],char* req[MAXNUMCOURSE])
{
    int reqcount = 0;
    while (req[reqcount])++reqcount;
    int i;
    for(i=0;req[i]!=NULL;++i)
    {
    	int j;
        for(j=0;pre[j][0]!=NULL;++j)
        {
            if(!strcmp(pre[j][0],req[i]))
	    {int k;
            for(k=1;pre[j][k]!=NULL;++k)
            {
                if(!SearchInReq(pre[j][k], req))
                {
                    req[reqcount]=(char*)malloc(MAXCOURSE * sizeof(char));
                    strcpy(req[reqcount++],pre[j][k]);
                }
            }}
        }
    }
}


void Process(char* offer[3][MAXNUMCOURSE], char* pre[MAXNUMCOURSE][MAXNUMCOURSE], char* req[MAXNUMCOURSE])
{
    int year = 1;
    int reqcount = 0;
    while (req[reqcount])++reqcount;
    while (year<=10)
    {
	int i;
        for (i = 1; i < 4; ++i)
        {
            int takes = 0;
            int indexs[MAXNUMCOURSE]={0};
	    int j;
            for (j = 0; j < reqcount; ++j)
                if (SearchInOffers(i,req[j],offer) && !SearchInPre(req[j], pre))
                    indexs[takes++]=j;
	    if(takes>0)
		printf("%d %d ",year,i);
            for(j = takes-1;j>=0;--j)
            {
		int k;
                for (k = 0; pre[k][0] != NULL;)
                {
                    int x;
                    for (x = 1; pre[k][x] != NULL; ++x)
                    {
                        if (!strcmp(pre[k][x], req[indexs[j]]))
                            Delete2(k, x, pre);
                    }
                    for (x = 1; pre[k][x] != NULL; ++x);
                    if (x == 1)
                    {
                        free(pre[k][0]);
                        int p,o;
                        for(p=k;pre[p][0]!=NULL;++p)
                        {
                            memset(pre[p],0,MAXNUMCOURSE * sizeof(char));
                            for(o=0;pre[p+1][o]!=NULL;++o)
                                pre[p][o]=pre[p+1][o];
                        }
                        for(o=0;pre[p][o]!=NULL;++o)
                            pre[p][o]=NULL;
                    }
                    else ++k;
                    
                }
		printf("%s ",req[indexs[j]]);
                Delete1(indexs[j],req);
            }
            reqcount -= takes;
            if(takes>0)printf("\n");
            if(reqcount==0)return;
            
        }
        ++year;
    }
    
}

int main(int argc, const char * argv[]){
    if(argc!=4)
    {
        printf("invaild number of arguments\n");
        return -1;
    }
    char* requirementcourse[MAXNUMCOURSE] = { 0 };
    char* prerequisitecourse[MAXNUMCOURSE][MAXNUMCOURSE];
    int i,j;
    for (i = 0; i < MAXNUMCOURSE; ++i)
        for (j = 0; j < MAXNUMCOURSE; ++j)
            prerequisitecourse[i][j] = NULL;
    char* offeringcourse[3][MAXNUMCOURSE];
    for (i = 0; i < 3; ++i)
        for (j = 0; j < MAXNUMCOURSE; ++j)
            offeringcourse[i][j] = NULL;

    offering(argv[1], offeringcourse);
    Prerequisite(argv[2], prerequisitecourse);
    Requirement(argv[3], requirementcourse);
    PreProcess(prerequisitecourse, requirementcourse);
    Process(offeringcourse, prerequisitecourse, requirementcourse);
    return 0;

}

