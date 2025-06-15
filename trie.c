#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_CHARS 256
#define MAX_CONTACTS 200
typedef struct{
   char name[100];
   char phno[100];
}contact; 
//contact contacts[MAX_CONTACTS];

int readfile();

typedef struct trienode{
	 struct trienode* child[MAX_CHARS];
	 bool terminal;
}trienode;
void printtrie_rec(trienode *node,unsigned char *prefix,int length);
trienode *createnode()
{
	trienode *newnode=malloc(sizeof *newnode);
	for(int i=0;i<MAX_CHARS;i++)
		{
			newnode->child[i]=NULL;
		}
	newnode->terminal = false;
	return newnode;
}

bool InsertTrie(trienode **root, contact *conts) {
	if (*root == NULL) {
		*root = createnode();
	}

	unsigned char *text = (unsigned char *)conts->name;
	unsigned char *phone = (unsigned char *)conts->phno;
	trienode *tmp = *root;
	int length = strlen(conts->name);
	int length_phone = strlen(conts->phno);

	for (int i = 0; i < length; i++) {
		if (tmp->child[text[i]] == NULL) {
			tmp->child[text[i]] = createnode();
		}
		tmp = tmp->child[text[i]];
	}

		if (tmp->child['\n'] == NULL) {
			tmp->child['\n'] = createnode();
		}
		tmp = tmp->child['\n'];

	for (int i = 0; i < length_phone; i++) {
		if (tmp->child[phone[i]] == NULL) {
			tmp->child[phone[i]] = createnode();
		}
		tmp = tmp->child[phone[i]];
	}

	if (tmp->terminal) {
		return false;
	} else {
		tmp->terminal = true;
		return true;
	}
}

bool prefix_search(trienode* root, char* signedtext) {
	unsigned char* text = (unsigned char*) signedtext;
	int length = strlen(signedtext);
	trienode* tmp = root;
	for (int i = 0; i < length; i++) {
		if (tmp->child[text[i]] == NULL) {
			printf("Prefix not found\n");
			return false;
		}
		tmp = tmp->child[text[i]];
	}
	printtrie_rec(tmp, text, length);
	return true;
}

void printtrie_rec(trienode *node,unsigned char *prefix,int length)
{
	unsigned char newprefix[length+2];
	memcpy(newprefix,prefix,length);
	newprefix[length+1]=0;
	if(node->terminal)
	{
		printf("NAME:%s\n",prefix);
	}
	for(int i=0;i<MAX_CHARS;i++)
		{
			if(node->child[i]!=NULL)
			{
				newprefix[length]=i;
				printtrie_rec(node->child[i],newprefix,length+1);
			}
		}
} 

void printtrie(trienode *root)
{
	if(root==NULL)
	{
		printf("Nothing found");
		return;
	}
	printtrie_rec(root,NULL,0);

}
void addrecord()
{
	FILE *f = fopen("file.txt", "a+");
	if (f == NULL) {
		printf("Failed to open file.\n");
		return;
	}

	contact c1;

	printf("Enter name:");
	//fgets(s, sizeof(s), stdin);
	scanf("%s",c1.name);
	printf("Enter a valid 10 digit phone number:");
	scanf("%s",c1.phno);
	if(strlen(c1.phno)!=10)
	{
	    printf("Invalid phone number entered.");
	    exit(0);
	}
	for(int i=0;i<10;i++)
	{
	    if(c1.phno[i]>=48&&c1.phno[i]<=57)
	    {
	        continue;
	    }
	    else
	    {
	        printf("Invalid phone number entered.");
	        exit(0);
	    }
	}
	c1.name[strcspn(c1.name, "\n")] = '\0';
	//fputs(s, f);
	//fprintf(f, "%lld\t", no);
	fprintf(f, "%s\t%s\n", c1.name, c1.phno);
	fclose(f);
}
void readrecord()
{
	trienode* root=NULL;
	FILE *fp;
	FILE *file = fopen("file.txt", "r");
	if (file == NULL) {
		perror("Error opening file");
		exit(1);
	}

	char name[100];
	char phone[100];
	contact conts;
	while (fscanf(file, "%s %s", name, phone) == 2) {
		strcpy(conts.name, name);
		strcpy(conts.phno, phone);
		InsertTrie(&root, &conts);

	}
	char prefix[100];
	printf("Enter prefix to search: ");
	scanf("%s", prefix);
	prefix_search(root, prefix);
	fclose(file);
}


int main()
{ 
	while(true){ 
		printf("1.Add details to the directory\n2.Search for a name\n3.Exit\n");
		printf("Enter your choice: \n");
		int choice;
		//choice = getch();
	  scanf("%d",&choice);
	system("clear");
		switch(choice)
			{
				case 1:addrecord();
				break;

				case 2:readrecord();
				break;

				case 3:exit(0);
				break;
		  default:
		  break;
			}
	 //system("clear");
	}
	return 0;
}