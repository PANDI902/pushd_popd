#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

typedef struct Node
{
char *dir;
struct Node *next;
}Node;

Node *stack = NULL;

void pushd(const char *dir)
{
Node *newNode = (Node *)malloc(sizeof(Node));
if(newNode == NULL)
{
perror("malloc error");
return;
}
newNode->dir = strdup(dir);
if(!newNode->dir)
{
perror("strdup error ");
free(newNode);
return;
}
newNode->next = stack;
stack = newNode;
if(chdir(dir)!=0)
{
perror("chdir error ");
stack = stack->next;
free(newNode->dir);
free(newNode);
}
else
{
printf("directory pushed: %s\n",dir);
}
}
void popd()
{
if(!stack)
{
fprintf(stderr,"stack is empty, cannot pop\n");
return;
}
Node *nodeToRemove = stack;
stack = stack->next;

if(stack && chdir(stack->dir)!=0)
{
perror("chdir error");
}
else
{
printf("Directory popped: %s\n",nodeToRemove->dir);
}
free(nodeToRemove->dir);
free(nodeToRemove);
}

void printstack()
{
Node *current =stack;
printf("Current Directory stack : \n");
while(current)
{
printf("%s\n",current->dir);
current = current->next;
}
}
int main()
{
char *cwd;
cwd = getcwd(NULL,0);

if(cwd == 0)
{
perror("cwd error");
return 1;
}
printf("Current Working Directory : %s\n",cwd);
char input[256];
while(1)
{
printf("enter command (pushd <dir>, popd,exit) : ");
if(fgets(input,sizeof(input),stdin)==NULL)
{
perror("directory get :");
continue;
}
input[strcspn(input,"\n")]='\0';

if(strncmp(input,"pushd",5)==0)
{
char *dir = input +6;
pushd(dir);
}
else if(strcmp(input,"popd")==0)
{
popd();
}
else if(strcmp(input,"exit")==0)
{
break;
}
else{
printf("unknown command %s\n",input);
}
}
return 0;
}
