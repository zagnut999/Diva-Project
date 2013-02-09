//
//  main.c
//  ReadingAFile
//
//  Created by Nate Pink on 10/20/12.
//  Copyright (c) 2012 Nate Pink. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


const int buffer = sizeof(char)*80;

struct menuItemNode
{
	char* text;
	struct menuItemNode *previousItem;
    struct menuItemNode *nextItem;
};

struct menuItemNode *menu1 = NULL, *menu2 = NULL;
struct menuItemNode *currentMenu = NULL, *previousMenuItem = NULL;
int nodeCount = 0;

char* substring(const char* str, size_t begin, size_t len)
{
    if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
        return 0;
    
    char *result = NULL;
    result = malloc(len);
    memcpy(result, str + begin, len);
    result[len] = 0;
    return result;
    //return strndup(str + begin, len);  //Not in arduino
}

int main2(int argc, const char * argv[])
{
    FILE* inputFile = 0; //fopen("SampleMenus.txt", "r");
    
    if (inputFile == 0)
    {
        fprintf(stderr, "Error: Cannot open file '%s'\n", "SampleMenus.txt");
        return 1;
    }
    
    char* line;
	if ((line = malloc(buffer))== NULL)
	{
		fprintf(stderr, "Memory not allocated\n");
		return 1;
	}
    
	char* tempLine;
	int tempLineSize = buffer;
	if ((tempLine = malloc(tempLineSize))== NULL)
	{
		fprintf(stderr, "Memory not allocated\n");
		return 1;
	}
	strcpy(tempLine , "");
    
    while(fgets(line, buffer, inputFile))
	{
		// if the tempLine buffer is less than or equal to its current string + new line + the '\0' character
		if (tempLineSize <= (strlen(tempLine) + strlen(line) +1))
		{
			tempLineSize += sizeof(char)*buffer*2;
			tempLine = realloc(tempLine, tempLineSize);
		}
		strcat(tempLine, line);
        
		if (tempLine[strlen(tempLine) - 1] == '\n')
		{
            //Only act on Menu or Item
            if (tempLine[0] == '@')
            {
                fprintf(stdout, "%s", tempLine);
                if (strcmp(tempLine, "@Menu1\n") == 0)
                {
                    //TODO clean up Menu1 if it already exists
                    menu1 = malloc(sizeof(struct menuItemNode));
                    currentMenu = menu1;
                }
                else if (strcmp(tempLine, "@Menu2\n") == 0)
                {
                    //TODO clean up Menu2 if it already exists
                    menu2 = malloc(sizeof(struct menuItemNode));
                    currentMenu = menu2;
                }
                else
                {
                    fprintf(stdout, "%s", "menu not found\n");
                    return 1;
                }
                
                currentMenu->previousItem = NULL;
                currentMenu->nextItem = NULL;
                currentMenu->text = NULL;
                previousMenuItem = currentMenu;
            }
            else if (tempLine[0] == '-')
            {
                fprintf(stdout, "%s", tempLine);
                struct menuItemNode *newItem = NULL;
                newItem = malloc(sizeof(struct menuItemNode));
                newItem->text = substring(tempLine, 1, strlen(tempLine)-2);
                //newItem->text = malloc(sizeof(tempLine));
                //strncpy(newItem->text, tempLine+1, strlen(tempLine-2));
                newItem->previousItem = previousMenuItem;
                newItem->nextItem = NULL;
                previousMenuItem->nextItem = newItem;
                previousMenuItem = newItem;
            }
            strcpy(tempLine , "");
		}
	}
	free(tempLine);
	free(line);
    
    fclose(inputFile);
    
    fprintf(stdout, "\n\n");
    struct menuItemNode *current = NULL;
    current = menu1;
    while (current != NULL)
    {
        if (current->text != NULL)
            fprintf(stdout, "%s\n", current->text);
        current = current->nextItem;
    }
    
    fprintf(stdout, "\n\n");
    current = menu2;
    while (current != NULL)
    {
        if (current->text != NULL)
            fprintf(stdout, "%s\n", current->text);
        current = current->nextItem;
    }
    
    return 0;
}

