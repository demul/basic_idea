#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* strstrip(char* str)
{
	// Strip the leading path.
	char *p = str + strlen(str);
	while (str < p && p[-1] != '/' && p[-1] != '\\')
		--p;

	str = p;

	// Strip the .exe suffix.
	p = strrchr(p, '.');
	if (p != NULL)
		*p = '\0';

	return str;
}

int main(int argc, char* argv[])
{
	if(argc != 1)
	{
		printf("Do not need additional argument\n");
		exit(1);
	}

	char* str = strstrip(argv[0]);

	if(strcmp(str, "hello")!=0)
	{
		printf("I am Hello%d\n", atoi(str+5));
		//hello2 will print "I am Hello2"
	}
	else
	{
		printf("Hello world\n");
	}



	return 0;
}
