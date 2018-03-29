#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/queue.h>
#include <ctype.h>
#include <unistd.h>

#define hashsize 997
#define getsize 512

struct hash {
	char word[30];
	int count;
	TAILQ_ENTRY(hash) entry;
};

int main(int argc, char **argv)
{
	int q;

	TAILQ_HEAD(tailhead,entry) hashhead[hashsize];

	for (q = 0; q < hashsize; q++)
	{
		TAILQ_INIT(&hashhead[q]);
	}

	FILE *fp;
	char *path1;
	char *path2;
	path1 = argv[1];
	path2 = argv[2];

	if((fp=fopen(path2,"r"))==NULL) {
		printf("get file error");
	}

	int i;
	int a = 0;
	int key=0;
	char get[getsize];
	char aword[30];
	while (fgets(get, getsize, fp) != NULL)
	{
		for (i = 0; i < strlen(get);i++) {
			if((get[i]>='a'&&get[i]<='z')||(get[i]>='A'&&get[i]<='Z')||(get[i]>='0'&&get[i]<='9')||(get[i]=='_')){
				if(get[i]>='A'&&get[i]<='Z')
					get[i] += ('a'-'A');
				aword[a] = get[i];
				a++;
				key += get[i];
			}
			else {
				if (strlen(aword) > 0)
				{
					key = key % hashsize;
					if (TAILQ_EMPTY(&hashhead[key]))
					{
						struct hash *create = malloc(sizeof(struct hash));
						create->count = 1;
						strcpy(create->word, aword);
						TAILQ_INSERT_TAIL(&hashhead[key], create, entry);
					}
					else
					{
						int i = 0;
						struct hash *search = malloc(sizeof(struct hash));
						TAILQ_FOREACH(search, &hashhead[key], entry)
						{
							if (strcmp(search->word, aword) == 0)
							{
								search->count++;
								i++;
							}
						}
						if (i == 0)
						{
							struct hash *add = malloc(sizeof(struct hash));
							add->count = 1;
							strcpy(add->word, aword);
							TAILQ_INSERT_TAIL(&hashhead[key], add, entry);
						}
					}
				}
				key = 0;
				a = 0;
				memset(aword,'\0',30);
			}
		}
	}

	fclose(fp);

	FILE *output;
	output = fopen("output.txt", "w");

	if ((fp = fopen(path1, "r")) == NULL)
	{
		printf("get file error");
	}
	while (fgets(get,getsize,fp)!=NULL)
	{
		int y;
		for (y = 0; y < strlen(get); y++){
			for (i = 0; i < strlen(get);i++) {
				if(get[i]>='A'&&get[i]<='Z')
					get[i] += ('a'-'A');

				if(!((get[i]>='a'&&get[i]<='z')||(get[i]>='A'&&get[i]<='Z')||(get[i]>='0'&&get[i]<='9')||(get[i]=='_'))){
				get[i]='\0';
				}
			}
		}
		int searchkey=0;
		int a;
		for (a = 0; a < strlen(get);a++) {
			searchkey += get[a];
		}
		searchkey =searchkey % hashsize;
		if (TAILQ_EMPTY(&hashhead[searchkey]))
		{
			fprintf(output,"0\n");
		}
		else {
			int j=0;
			struct hash *search = malloc(sizeof(struct hash));
			TAILQ_FOREACH(search,&hashhead[searchkey],entry) {
				if (strcmp(search->word,get)==0) {
					fprintf(output,"%d\n",search->count);
					j++;
				}
			}
			if(j==0) {
				fprintf(output,"0\n");
			}
		}
	}
	fclose(output);
	fclose(fp);
	return 0;
}
