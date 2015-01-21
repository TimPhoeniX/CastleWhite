#include<string.h>
#include<stdio.h>
#include<ctype.h>
#include<sys/ioctl.h>
#include "mybanner.h"
#include "charset.h"

int simple_banner(char *source)
{
	int i=0,j=0;
	char *(*letter);
	for(i=0;i<7;i++)
	{
		for(j=0;source[j]!='\0';j++)
		{
			letter=get_letter(source[j]);
			printf("%s ",letter[i]);
		}
	puts("");	
	}
	return 0;
}

int advanced_banner(char *source)
{
	int i=0,j=0,current=0,endofstring=strlen(source);
	char **letter, **tmp, line[256];
	struct winsize SIZE;
	ioctl(0,TIOCGWINSZ,&SIZE);
	puts("");
	while(current<endofstring)
	{
		for(i=0;i<7;i++)
		{
			memset(line,0,256);
			for(j=current;source[j]!='\0';j++)
			{
				letter=get_letter(source[j]);
				strcat(line,letter[i]);	
				tmp=get_letter(source[j+1]);
				if((strlen(line)+strlen(tmp[i])+2)>SIZE.ws_col) break;
				strcat(line," ");			
			}
			puts(line);
		}
		puts("");
		current=j+1;
	}
	return 0;
}