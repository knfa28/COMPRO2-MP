#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

typedef char String100[101];
typedef char String80[81];
typedef char String50[51];

/*display a series of lines*/
void displayLines(int nNum)
{
	int nCtr;	
	printf("\n");
	for(nCtr = 0; nCtr < nNum; nCtr++)
		printf("%c", 196);
}

/*displays a series of spaces*/
void displaySpace(int nNum)
{
	int nCtr;
	for(nCtr = 0; nCtr < nNum; nCtr++)
		printf("\t");
}

/*data structure of a mystery phrase*/
struct phraseTag
{
	String50 strPhrase[5],
	         strCat;
	int wordCnt;
};

/*data structure of a bonus question*/
struct bonusTag
{
	String80 strQuest;
	String50 strChoice[4],
	         strCat;
	char cAns;
};

/*node structure of a mystery phrase*/
typedef struct nodePhraseTag
{
	struct phraseTag mystPhrase;
	struct nodePhraseTag *pNext, *pPrev;
}nodePhraseType, *ptrPhrase ;


/*node structure of a bonus question*/
typedef struct nodeBonusTag
{
	struct bonusTag bonusQuest;
	struct nodeBonusTag *pNext, *pPrev;
}nodeBonusType, *ptrBonus;

/*converts a given string into lowercase*/
void getLowercase(char str[])
{
	int i;
	for(i = 0; i < strlen(str); i++)
	   if(str[i] >= 'A' && str[i] <= 'Z')
	      str[i] += 32;
}

/*compares two given strings*/
int Strcmp(char *str1, char *str2)
{
	String80 strTemp1, strTemp2;
    
	strcpy(strTemp1, str1);
	strcpy(strTemp2, str2);
	
	getLowercase(strTemp1);
	getLowercase(strTemp2);

	return (strcmp(strTemp1, strTemp2));
}

/*confirms whether the given file exists or not*/
int getFile(FILE *pFile, String50 strFile)
{
	char cRetry;
	
	do
	{
		printf("\nLoad filename: ");
		scanf("%s", strFile);
		
		if((pFile = fopen(strFile, "r")) != NULL)
			return 1;
			
		else
		{
			printf("ERROR: %s does not exist.\n", strFile);	
			printf("Would you like to try again [Y/N]? ");
			scanf("%c%*c", &cRetry);
		
			if(cRetry == 'Y' || cRetry == 'y')
				system("cls");
		}	
	}while(cRetry == 'Y' || cRetry == 'y');
	
	return 0;
}

/*creates a new mystery phrase node*/
ptrPhrase CreatePhraseNode(String50 phrase[], int wordCnt, String50 category)
{
	int i;
	ptrPhrase pTemp;

 	if ((pTemp = malloc(sizeof(nodePhraseType))) == NULL)
	{
 		printf ("\nERROR: not enough memory.\n");
 		exit (1);
 	}
 	
 	for(i = 0; i < wordCnt; i++)
 		strcpy(pTemp->mystPhrase.strPhrase[i], phrase[i]);
 	
	pTemp->mystPhrase.wordCnt = wordCnt;
	strcpy(pTemp->mystPhrase.strCat, category);
	
 	pTemp->pNext = NULL;
 	pTemp->pPrev = NULL;
	
	return pTemp ; 
}

/*adds a mystery phrase node to the end of a list*/
void addPhraseLast(ptrPhrase *pFirst, ptrPhrase pNew)
{
    ptrPhrase pCurr;
    
    if ( *pFirst == NULL ) 
	    *pFirst = pNew ;
	else
    { 
    	pCurr = *pFirst;
    
        while ( pCurr->pNext != NULL )
			pCurr = pCurr->pNext;

        pCurr->pNext = pNew;
        pNew->pPrev = pCurr;
	}
}

/*displays the data of a mystery phrase node*/
void printPhrase(ptrPhrase pFirst)
{
	ptrPhrase pTemp = pFirst;
	int i;
	
	for(i = 0; i < pTemp->mystPhrase.wordCnt; i++)
		printf("%s ", pTemp->mystPhrase.strPhrase[i]);
		
	printf("\nNumber of words: %d", pTemp->mystPhrase.wordCnt);
		
	printf("\nCategory: %s\n\n", pTemp->mystPhrase.strCat);
}

/*sorts the nodes of a mystery phrase linked list by category*/
void sortPhraseCat(ptrPhrase pFirst)
{
	struct phraseTag sTemp;
	ptrPhrase pTemp,
	          pCurr,
			  pSwap;
	
	if(pFirst != NULL && pFirst->pNext != NULL)
	   for(pTemp = pFirst; pTemp->pNext != NULL; pTemp = pTemp->pNext)
	   {
	  	   for(pSwap = pTemp, pCurr = pTemp->pNext; pCurr != NULL; 
		        pCurr = pCurr->pNext)
		      if(Strcmp(pCurr->mystPhrase.strCat, pSwap->mystPhrase.strCat) == -1)
		         pSwap = pCurr;
		         
		   if(pSwap != pTemp)
		   {
		  	   sTemp = pSwap->mystPhrase;
			   pSwap->mystPhrase = pTemp->mystPhrase;
			   pTemp->mystPhrase = sTemp;
		   }
	   }
}

/*sorts the nodes of a mystery phrase linked list alphabetcally*/
void sortPhrase(ptrPhrase pFirst)
{
	struct phraseTag sTemp;
	ptrPhrase pTemp,
	          pCurr,
			  pSwap;
	
	sortPhraseCat(pFirst);
	
	if(pFirst != NULL && pFirst->pNext != NULL)
	   for(pTemp = pFirst; pTemp->pNext != NULL; pTemp = pTemp->pNext)
	   {
	  	   for(pSwap = pTemp, pCurr = pTemp->pNext; pCurr != NULL &&
			   strcmp(pSwap->mystPhrase.strCat,pCurr->mystPhrase.strCat) == 0; 
		        pCurr = pCurr->pNext)
		      if(Strcmp(pCurr->mystPhrase.strPhrase[0], pSwap->mystPhrase.strPhrase[0]) == -1)
		         pSwap = pCurr;
		         
		   if(pSwap != pTemp)
		   {
		  	   sTemp = pSwap->mystPhrase;
			   pSwap->mystPhrase = pTemp->mystPhrase;
			   pTemp->mystPhrase = sTemp;
		   }
	   }
}

/*confirms whether a given category of a mystery phrase exists or not*/
int isPhraseCat(ptrPhrase *pFirst, String50 strCat)
{
	char cRetry = 'N';
	ptrPhrase pTrail;
	
	do
	{		
		printf("Category: ");
		fgets(strCat, 51, stdin);
		strCat[strlen(strCat) - 1 ]= '\0';
		
		while(*pFirst != NULL && strcmp((*pFirst)->mystPhrase.strCat, strCat) != 0)
		{
			pTrail = *pFirst;
			*pFirst = (*pFirst)->pNext;
		}
	
		if(*pFirst != NULL) 
			return 1;
		  
		else
		{
			system("cls");
			printf("ERROR: Category not found.\n\n");	
			printf("Would you like to try again [Y/N]? ");
			scanf("%c%*c", &cRetry);
			
			if(cRetry == 'Y' || cRetry == 'y')
			{
				*pFirst = pTrail;
				
				while((*pFirst)->pPrev != NULL)
					*pFirst = (*pFirst)->pPrev;
					
				system("cls");
			}	
		}
	}while(cRetry != 'N' && cRetry != 'n');
	
	return 0;
}

/*counts the number of nodes of a linked list of mystery phrases*/
int phraseNodeCount(ptrPhrase pFirst)
{
	int nCount = 0;
    ptrPhrase pCurr;
    pCurr = pFirst;
    
    if(pFirst == NULL)
    	return 0;
    
    while(pCurr != NULL)
    {
        nCount++;
        pCurr = pCurr->pNext;
    }
    
    return nCount;
}

/*visual menu for viewing the data of mystery phrases*/
void printPhraseSort(ptrPhrase pFirst)
{
	int c;
	ptrPhrase pTemp = pFirst;
	String50 strCat;
	
	sortPhrase(pFirst);
	
	if(isPhraseCat(&pTemp, strCat))
		do
		{
			system("cls");
			printPhrase(pTemp);
			
			if(pTemp->pNext != NULL &&
			   strcmp(pTemp->pNext->mystPhrase.strCat, strCat) == 0)
					printf("[d] Next page\n");
			
			if(pTemp->pPrev != NULL &&
			   strcmp(pTemp->pPrev->mystPhrase.strCat, strCat) == 0)
					printf("[a] Previous page\n");
			
			c = getch();			
					
			switch(c)
			{
				case 'd': if(pTemp->pNext != NULL &&
				            strcmp(pTemp->pNext->mystPhrase.strCat, strCat) == 0)
								pTemp = pTemp->pNext;	
						  else
						  {
							  printf("\nLast phrase.\n");
						 	  system("pause");
				 		  }
				    	  break;	
							
				case 'a': if(pTemp->pPrev != NULL &&
				            strcmp(pTemp->pPrev->mystPhrase.strCat, strCat) == 0)
								pTemp = pTemp->pPrev;
						  else
						  {
						  	  printf("\nFirst phrase.\n");
						      system("pause");	
						  }
						  break;
			}
		}while(c != 27);	
}

/*counts the number of words in a given sentence regardless of the spaces*/
int wordCnt(String50 arrWords[])
{
 	int i,       /*index of the word*/
	nCount = 0;  /*initial number of words*/
    
	/*checks every char in the string*/
    for(i = 0; arrWords[i][0] != '\0' && i < 5; i++)
        nCount++;  /*number of words is incremented*/
	
	/*returns the number of words*/
    return nCount;
}

/*stores the words of given string into an array of words*/
void storeWords(String100 sentence, String50 arrWords[])
{
	int i,          
	    nWord = 0, 
		nChar = 0, 
		len = strlen(sentence);
	
	for(i = 0; sentence[i] == ' '; i++);
	
	while(i < len)
	{	
		if(sentence[i] != ' ')
		{
			arrWords[nWord][nChar] = sentence[i];
			nChar++;  
			i++;     
		}
			
		else if(sentence[i] == ' ')
		{
			arrWords[nWord][nChar] = '\0';  
			while(sentence[i] == ' ') i++;	
			nWord++;   
			nChar = 0;  
		}				
	}

	arrWords[nWord][nChar] = '\0';
}

/*creates a phrase node then adds it to the end of a linked list*/
void addPhraseNode(ptrPhrase *pFirst)
{
	int i;
	ptrPhrase pTemp;
	String50 tempWords[5];
	String100 tempPhrase;
	
	if ((pTemp = malloc(sizeof(nodePhraseType))) == NULL)
	{
 		printf ("\nERROR: not enough memory.\n");
 		exit (1);
 	}
	
	printf("Mystery Phrase: ");
	fgets(tempPhrase, 101, stdin);
	tempPhrase[strlen(tempPhrase) - 1] = '\0';
	
	for(i = 0; i < 5; i++)
		strcpy(tempWords[i],"");
		
	storeWords(tempPhrase, tempWords);
	
	for(i = 0; i < wordCnt(tempWords); i++)
		strcpy(pTemp->mystPhrase.strPhrase[i], tempWords[i]);
	
	printf("Number of words: %d", wordCnt(tempWords));
	pTemp->mystPhrase.wordCnt = wordCnt(tempWords);
	
	printf("\nCategory: ");
	fgets(pTemp->mystPhrase.strCat, 51, stdin);
	pTemp->mystPhrase.strCat[strlen(pTemp->mystPhrase.strCat) - 1] = '\0';
	
	addPhraseLast(pFirst, pTemp);
}

/*deletes an existing mystery phrase node*/
deletePhrase(ptrPhrase *pFirst)
{
	ptrPhrase pTemp;
	
	pTemp = *pFirst;
	*pFirst = (*pFirst)->pNext;
	(*pFirst)->pPrev = (*pFirst)->pPrev->pPrev;
	(*pFirst)->pPrev->pNext = *pFirst;
	free(pTemp);
}

/*visual menu for deleting a mystery phrase node*/
void deletePhraseSort(ptrPhrase pFirst)
{
	int c;
	ptrPhrase pTemp = pFirst;
	String50 strCat;
	char cConfirm;
	
	sortPhrase(pFirst);
	
	if(isPhraseCat(&pTemp, strCat))
		do
		{
			system("cls");
			printPhrase(pTemp);
			
			printf("[x] Delete Question\n\n");
			
			if(pTemp->pNext != NULL &&
			   strcmp(pTemp->pNext->mystPhrase.strCat, strCat) == 0)
					printf("[d] Next page\n");
			
			if(pTemp->pPrev != NULL &&
			   strcmp(pTemp->pPrev->mystPhrase.strCat, strCat) == 0)
					printf("[a] Previous page\n");
			
			c = getch();			
					
			switch(c)
			{
				case 'd': if(pTemp->pNext != NULL &&
				            strcmp(pTemp->pNext->mystPhrase.strCat, strCat) == 0)
								pTemp = pTemp->pNext;	
						  else
						  {
					          printf("Last phrase.\n");
						 	  system("pause");
				 		  }
				    	  break;	
							
				case 'a': if(pTemp->pPrev != NULL &&
				            strcmp(pTemp->pPrev->mystPhrase.strCat, strCat) == 0)
								pTemp = pTemp->pPrev;
						  else
						  {
						  	  printf("First phrase.\n");
						      system("pause");	
						  }
						  break;
				case 'x': printf("\nDelete phrase [Y/N]? ");
				          scanf("%c%*c", &cConfirm);
				          if(cConfirm == 'Y' || cConfirm == 'y')
						  	deletePhrase(&pTemp);
				          break;
			}
		}while(c != 27);
		
	sortPhrase(pFirst);	
}

/*modifies an existing mystery phrase node*/
modifyPhrase(ptrPhrase *pFirst)
{
	int i;
	String50 tempWords[5];
	String100 tempPhrase;
	ptrPhrase pCurr = *pFirst;
	
	printf("Mystery Phrase: ");
	fgets(tempPhrase, 101, stdin);
	tempPhrase[strlen(tempPhrase) - 1] = '\0';
	
	storeWords(tempPhrase, tempWords);
	
	for(i = 0; i < wordCnt(tempWords); i++)
		strcpy(pCurr->mystPhrase.strPhrase[i], tempWords[i]);
	
	printf("Number of words: %d", wordCnt(tempWords));
	pCurr->mystPhrase.wordCnt = wordCnt(tempWords);
	
	printf("\nCategory: ");
	fgets(pCurr->mystPhrase.strCat, 51, stdin);
	pCurr->mystPhrase.strCat[strlen(pCurr->mystPhrase.strCat) - 1] = '\0';	
}

/*visual menu for modifying an existing phrase node*/
void modifyPhraseSort(ptrPhrase pFirst)
{
	int c;
	ptrPhrase pTemp = pFirst;
	String50 strCat;
	char cConfirm;
	
	sortPhrase(pFirst);
	
	if(isPhraseCat(&pTemp, strCat))
		do
		{
			system("cls");
			printPhrase(pTemp);
			
			printf("[m] Modify Question\n\n");
			
			if(pTemp->pNext != NULL &&
			   strcmp(pTemp->pNext->mystPhrase.strCat, strCat) == 0)
					printf("[d] Next page\n");
			
			if(pTemp->pPrev != NULL &&
			   strcmp(pTemp->pPrev->mystPhrase.strCat, strCat) == 0)
					printf("[a] Previous page\n");
			
			c = getch();			
					
			switch(c)
			{
				case 'd': if(pTemp->pNext != NULL &&
				            strcmp(pTemp->pNext->mystPhrase.strCat, strCat) == 0)
								pTemp = pTemp->pNext;	
						  else
						  {
					          printf("Last phrase.\n");
						 	  system("pause");
				 		  }
				    	  break;	
							
				case 'a': if(pTemp->pPrev != NULL &&
				            strcmp(pTemp->pPrev->mystPhrase.strCat, strCat) == 0)
								pTemp = pTemp->pPrev;
						  else
						  {
						  	  printf("First phrase.\n");
						      system("pause");	
						  }
						  break;
				case 'm': printf("\nModify question [Y/N]? ");
				          scanf("%c%*c", &cConfirm);
				          if(cConfirm == 'y' || cConfirm == 'Y')
						  	modifyPhrase(&pTemp);
				          break;
			}
		}while(c != 27);
		
	sortPhrase(pFirst);
}

/*saves a linked list of mystery phrase nodes into a text file*/
void saveMystPhrase(ptrPhrase pFirst)
{
	ptrPhrase pTemp = pFirst;
	int i;
	String50 strFile;
	FILE *pFile; 
	
	sortPhrase(pFirst);
	
	printf("Save to filename: ");
	scanf("%s", strFile);
	
	pFile = fopen(strFile, "w");
	
	fprintf(pFile, "%d\n", phraseNodeCount(pFirst));
	
	while(pTemp != NULL)
	{
		fprintf(pFile, "%d ", pTemp->mystPhrase.wordCnt);
	   	
		for(i = 0; i < pTemp->mystPhrase.wordCnt; i++)
	   		fprintf(pFile, "%s ", pTemp->mystPhrase.strPhrase[i]);
	   	
		fprintf(pFile, "%s\n", pTemp->mystPhrase.strCat);
		
		pTemp = pTemp->pNext;
	}
	
	fclose(pFile);
	
	system("cls");
	printf("File saved successfully!\n\n");
	system("pause");
}

/*loads an existing text file containing the data of mystery phrases*/
void loadMystPhrase(ptrPhrase *pFirst)
{
	int wordCnt,
	    questCnt = 0,
		nCtr = 0,
		i;
	char cAns;
	ptrPhrase pTemp; 
	String50 strFile,
	         tempPhrase[5],
			 tempCat;    
	FILE *pFile; 
	
	do
	{
		system("cls");
		printf("Overwrite existing file [Y/N]? ");
		scanf("%c%*c", &cAns);
		
		if(cAns != 'y' && cAns != 'Y' && cAns != 'n' && cAns != 'N')
			printf("\nPlease input either Y or N only!\n");
	}while(cAns != 'y' && cAns != 'Y' && cAns != 'n' && cAns != 'N');
	
	if(cAns == 'y' || cAns == 'Y')
	{
		if(getFile(pFile, strFile))
		{
			pFile = fopen(strFile, "r");
			fscanf( pFile, "%d", &questCnt);
			
			while(nCtr < questCnt)
			{
				pTemp = malloc(sizeof(nodePhraseType));
			
				fscanf(pFile, "%d", &pTemp->mystPhrase.wordCnt);
       	         
       	    	for(i = 0; i < pTemp->mystPhrase.wordCnt; i++)
       	        	fscanf(pFile, "%s", &pTemp->mystPhrase.strPhrase[i]);
        	       
            	fgetc(pFile); 
              	 
            	fgets(pTemp->mystPhrase.strCat, 51, pFile);
        	    pTemp->mystPhrase.strCat[strlen(pTemp->mystPhrase.strCat) - 1] = '\0';
				
				pTemp->pNext = NULL;
				
				addPhraseLast(pFirst, pTemp);
				
				nCtr++;
			}
			fclose(pFile);
		}
	}
	
	system("cls");
	printf("File loaded successfully!\n\n");
	system("pause");
}
/*loads a default text file containing the data of mystery phrases*/
void loadDefaultPhrase(ptrPhrase *pFirst)
{
	int wordCnt,
	    questCnt = 0,
		nCtr = 0,
		i;
	char cRetry = 'N';
	ptrPhrase pTemp; 
	String50 tempPhrase[5],
			 tempCat;    
	FILE *pFile; 
	
	pFile = fopen("mystPhrase.txt", "r");
	fscanf( pFile, "%d", &questCnt);
		
	while(nCtr < questCnt)
	{
		pTemp = malloc(sizeof(nodePhraseType));
	
		fscanf(pFile, "%d", &pTemp->mystPhrase.wordCnt);
               
        for(i = 0; i < pTemp->mystPhrase.wordCnt; i++)
            fscanf(pFile, "%s", &pTemp->mystPhrase.strPhrase[i]);
               
        fgetc(pFile); 
               
        fgets(pTemp->mystPhrase.strCat, 51, pFile);
        pTemp->mystPhrase.strCat[strlen(pTemp->mystPhrase.strCat) - 1] = '\0';
			
		pTemp->pNext = NULL;
			
		addPhraseLast(pFirst, pTemp);
			
		nCtr++;
	}
	fclose(pFile);
}

/*creates a new bonus node*/
ptrBonus CreateBonusNode(String80 quest, String50 choice[], char  ans, String50 cat)
{
	int i;
	ptrBonus pTemp;

 	if ((pTemp = malloc(sizeof(nodeBonusType))) == NULL)
	{
 		printf ("\nERROR: not enough memory.\n");
 		exit (1);
 	}
	
	strcpy(pTemp->bonusQuest.strQuest, quest);
	
	for(i = 0; i < 4; i++)
		strcpy(pTemp->bonusQuest.strChoice[i], choice[i]);
	
	pTemp->bonusQuest.cAns = ans;
	strcpy(pTemp->bonusQuest.strCat,cat);
	
 	pTemp->pNext = NULL;
 	pTemp->pPrev = NULL;
	
	return pTemp ; 
}

/*adds a bonus question node to the end of a linked list*/
void addBonusLast(ptrBonus *pFirst, ptrBonus pNew)
{
    ptrBonus pCurr;
    
    if ( *pFirst == NULL ) 
	    *pFirst = pNew ;
	else
    { 
	    pCurr = *pFirst;

        while ( pCurr->pNext != NULL )
			pCurr = pCurr->pNext;

        pCurr->pNext = pNew;
        pNew->pPrev = pCurr;
	}
}

/*prints the details of a bonus question node*/
void printBonus(ptrBonus pFirst)
{
	ptrBonus pTemp = pFirst;
	int i;

	printf("%s\n", pTemp->bonusQuest.strQuest);
		
	for(i = 0; i < 4; i++)
		printf("%s\n", pTemp->bonusQuest.strChoice[i]);
		
	printf("Answer: %c\n", pTemp->bonusQuest.cAns);
		
	printf("Category: %s\n\n", pTemp->bonusQuest.strCat);
}

/*sorts the nodes of a linked list of bonus questions by category*/
void sortBQuestCat(ptrBonus pFirst)
{
	struct bonusTag sTemp;
	ptrBonus pTemp,
	         pCurr,
			 pSwap;
	
	if(pFirst != NULL && pFirst->pNext != NULL)
	   for(pTemp = pFirst; pTemp->pNext != NULL; pTemp = pTemp->pNext)
	   {
	  	   for(pSwap = pTemp, pCurr = pTemp->pNext; pCurr != NULL; 
		        pCurr = pCurr->pNext)

		      if(Strcmp(pCurr->bonusQuest.strCat, pSwap->bonusQuest.strCat) == -1)
		         pSwap = pCurr;
		         
		   if(pSwap != pTemp)
		   {
		  	   sTemp = pSwap->bonusQuest;
			   pSwap->bonusQuest = pTemp->bonusQuest;
			   pTemp->bonusQuest = sTemp;
		   }
	   }
}

/*sorts the nodes of a linked list of bonus questions alphabetically*/
void sortBQuest(ptrBonus pFirst)
{
	struct bonusTag sTemp;
	ptrBonus pTemp,
	          pCurr,
			  pSwap;
	
	sortBQuestCat(pFirst);
	
	if(pFirst != NULL && pFirst->pNext != NULL)
	   for(pTemp = pFirst; pTemp->pNext != NULL; pTemp = pTemp->pNext)
	   {
	  	   	for(pSwap = pTemp, pCurr = pTemp->pNext; pCurr != NULL &&
			   strcmp(pSwap->bonusQuest.strQuest,pCurr->bonusQuest.strQuest) == 0; 
			pCurr = pCurr->pNext)

		    	if(Strcmp(pCurr->bonusQuest.strQuest, pSwap->bonusQuest.strQuest) == -1)
					pSwap = pCurr;
		         
		   	if(pSwap != pTemp)
		   	{
		  	   sTemp = pSwap->bonusQuest;
			   pSwap->bonusQuest = pTemp->bonusQuest;
			   pTemp->bonusQuest = sTemp;
		   	}
	   }
}

/*confirms whether a given category of a bonus question exists or not*/
int isBonusCat(ptrBonus *pFirst, String50 strCat)
{
	char cRetry = 'N';
	ptrBonus pTrail;
	
	do
	{		
		printf("Category: ");
		fgets(strCat, 51, stdin);
		strCat[strlen(strCat) - 1 ]= '\0';
		
		while(*pFirst != NULL && strcmp((*pFirst)->bonusQuest.strCat, strCat) != 0)
		{
			pTrail = *pFirst;
			*pFirst = (*pFirst)->pNext;
		}
			
	
		if(*pFirst != NULL) 
			return 1;
		
		else
		{
			system("cls");
			printf("ERROR: Category not found.\n\n");	
			printf("Would you like to try again [Y/N]? ");
			scanf("%c%*c", &cRetry);
			
			if(cRetry == 'Y' || cRetry == 'y')
			{
				*pFirst = pTrail;
				
				while((*pFirst)->pPrev != NULL)
					*pFirst = (*pFirst)->pPrev;
					
				system("cls");
			}	
		}
	}while(cRetry != 'N' && cRetry != 'n');
	
	return 0;
}

/*counts the number of bonus question nodes*/
int bonusNodeCount(ptrBonus pFirst)
{
	int nCount = 0;
    ptrBonus pCurr;
    pCurr = pFirst;
    
    if(pFirst == NULL)
    	return 0;
    
    while(pCurr != NULL)
    {
        nCount++;
        pCurr = pCurr->pNext;
    }
    
    return nCount;
}

/*visual menu for viewing the details of a bonus question*/
void printBonusSort(ptrBonus pFirst)
{
	int c;
	ptrBonus pTemp = pFirst;
	String50 strCat;
	
	sortBQuest(pFirst);
	
	if(isBonusCat(&pTemp, strCat))
		do
		{
			system("cls");
			printBonus(pTemp);
			
			if(pTemp->pNext != NULL &&
			   strcmp(pTemp->pNext->bonusQuest.strCat, strCat) == 0)
					printf("[d] Next page\n");
			
			if(pTemp->pPrev != NULL &&
			   strcmp(pTemp->pPrev->bonusQuest.strCat, strCat) == 0)
					printf("[a] Previous page\n");
			
			c = getch();
			
			switch(c)
			{
				case 'd': if(pTemp->pNext != NULL &&
				            strcmp(pTemp->pNext->bonusQuest.strCat, strCat) == 0)
								pTemp = pTemp->pNext;	
						  else
						  {
					 		  printf("Last question.\n");
							  system("pause");
				 		  }
				    	  break;	
							
				case 'a': if(pTemp->pPrev != NULL &&
				            strcmp(pTemp->pPrev->bonusQuest.strCat, strCat) == 0)
								pTemp = pTemp->pPrev;
						  else
						  {
						 	  printf("First question.\n");
						 	  system("pause");	
						  }
						  break;	
			}
		}while(c != 27);	
}

/*creates a bonus question node then adds it to the end of a linked list*/
void addBonusNode(ptrBonus *pFirst)
{
	int i, check;
	ptrBonus pTemp;
	String50 strChoice[4] = {"a. ","b. ","c. ","d. "},
	         tempChoice[4];
	
	displayLines(30);
    printf("\n\tAdd Bonus Question\n");
    displayLines(30);
	
	if ((pTemp = malloc(sizeof(nodeBonusType))) == NULL)
	{
 		printf ("\nERROR: not enough memory.\n");
 		exit (1);
 	}       
	         
	printf("Question: ");
	fgets(pTemp->bonusQuest.strQuest, 81, stdin);
	pTemp->bonusQuest.strQuest[strlen(pTemp->bonusQuest.strQuest) - 1] = '\0';
	
	printf("\nChoices:\n");
	for(i = 0; i < 4; i++)
	{
		printf("%s", strChoice[i]);
		fgets(tempChoice[i], 51, stdin);
		tempChoice[i][strlen(tempChoice[i]) - 1] = '\0';
		strcat(strChoice[i],tempChoice[i]);
		strcpy(pTemp->bonusQuest.strChoice[i], strChoice[i]);
	}
	
	do
	{
		printf("\nAnswer: ");
		scanf("%c%*c", &pTemp->bonusQuest.cAns);
		
		check = 0;
		
		for(i = 0; i < 4; i++)
			if(pTemp->bonusQuest.strChoice[i][0] == pTemp->bonusQuest.cAns)
				check = 1;	
				
		if(check  == 0)
			printf("\nPlease enter a letter within the choices.\n");
	}while(check == 0);
	
	printf("\nCategory: ");
	fgets(pTemp->bonusQuest.strCat, 51, stdin);
	pTemp->bonusQuest.strCat[strlen(pTemp->bonusQuest.strCat) - 1] = '\0';	
	
	addBonusLast(pFirst, pTemp);
}

/*deletes an existing bonus question node*/
deleteBonus(ptrBonus *pFirst)
{
	ptrBonus pTemp;
	
	pTemp = *pFirst;
	*pFirst = (*pFirst)->pNext;
	(*pFirst)->pPrev = (*pFirst)->pPrev->pPrev;
	(*pFirst)->pPrev->pNext = *pFirst;
	free(pTemp);
}

/*visual menu for deleting a bonus question node*/
void deleteBonusSort(ptrBonus pFirst)
{
	int c;
	ptrBonus pTemp = pFirst;
	String50 strCat;
	char cConfirm;
	
	sortBQuest(pFirst);
	
	if(isBonusCat(&pTemp, strCat))
		do
		{
			system("cls");
			printBonus(pTemp);
			
			printf("[x] Delete Question\n\n");
			
			if(pTemp->pNext != NULL &&
			   strcmp(pTemp->pNext->bonusQuest.strCat, strCat) == 0)
					printf("[d] Next page\n");
			
			if(pTemp->pPrev != NULL &&
			   strcmp(pTemp->pPrev->bonusQuest.strCat, strCat) == 0)
					printf("[a] Previous page\n");
			
			c = getch();
			
			switch(c)
			{
				case 'd': if(pTemp->pNext != NULL &&
				            strcmp(pTemp->pNext->bonusQuest.strCat, strCat) == 0)
								pTemp = pTemp->pNext;	
						  else
						  {
					 		  printf("Last question.\n");
							  system("pause");
				 		  }
				    	  break;	
							
				case 'a': if(pTemp->pPrev != NULL &&
				            strcmp(pTemp->pPrev->bonusQuest.strCat, strCat) == 0)
								pTemp = pTemp->pPrev;
						  else
						  {
						 	  printf("First question.\n");
						 	  system("pause");	
						  }
						  break;						  
				case 'x': printf("\nDelete Bonus Question [Y/N]? ");
				          scanf("%c%*c", &cConfirm);
				          if(cConfirm == 'y' || cConfirm == 'Y')
							deleteBonus(&pTemp);
				          break;	
			}
		}while(c != 27);
		
	sortBQuest(pFirst);	
}

/*modifies the data of an existing bonus question node*/
modifyBonus(ptrBonus *pFirst)
{
	int i, check;
	ptrBonus pCurr = *pFirst;
	String50 strChoice[4] = {"a. ","b. ","c. ","d. "},
	         tempChoice[4];     
	         
	printf("Question: ");
	fgets(pCurr->bonusQuest.strQuest, 81, stdin);
	pCurr->bonusQuest.strQuest[strlen(pCurr->bonusQuest.strQuest) - 1] = '\0';
	
	printf("\nChoices:\n");
	for(i = 0; i < 4; i++)
	{
		printf("%s", strChoice[i]);
		fgets(tempChoice[i], 51, stdin);
		tempChoice[i][strlen(tempChoice[i]) - 1] = '\0';
		strcat(strChoice[i],tempChoice[i]);
		strcpy(pCurr->bonusQuest.strChoice[i], strChoice[i]);
	}
	
	do
	{
		printf("\nAnswer: ");
		scanf("%c%*c", &pCurr->bonusQuest.cAns);
		
		check = 0;
		
		for(i = 0; i < 4; i++)
			if(pCurr->bonusQuest.strChoice[i][0] == pCurr->bonusQuest.cAns)
				check = 1;	
				
		if(check  == 0)
			printf("\nPlease enter a letter within the choices.\n");
	}while(check == 0);
	
	printf("\nCategory: ");
	fgets(pCurr->bonusQuest.strCat, 51, stdin);
	pCurr->bonusQuest.strCat[strlen(pCurr->bonusQuest.strCat) - 1] = '\0';
}

/*visual menu for modifying the details of an existing bonus question node*/
void modifyBonusSort(ptrBonus pFirst)
{
	int c;
	ptrBonus pTemp = pFirst;
	String50 strCat;
	char cConfirm;
	sortBQuest(pFirst);
	
	if(isBonusCat(&pTemp, strCat))
		do
		{
			system("cls");
			printBonus(pTemp);
			
			printf("[m] Delete Question\n\n");
			
			if(pTemp->pNext != NULL &&
			   strcmp(pTemp->pNext->bonusQuest.strCat, strCat) == 0)
					printf("[d] Next page\n");
			
			if(pTemp->pPrev != NULL &&
			   strcmp(pTemp->pPrev->bonusQuest.strCat, strCat) == 0)
					printf("[a] Previous page\n");
			
			c = getch();
			
			switch(c)
			{
				case 'd': if(pTemp->pNext != NULL &&
				            strcmp(pTemp->pNext->bonusQuest.strCat, strCat) == 0)
								pTemp = pTemp->pNext;	
						  else
						  {
					 		  printf("Last question.\n");
							  system("pause");
				 		  }
				    	  break;	
							
				case 'a': if(pTemp->pPrev != NULL &&
				            strcmp(pTemp->pPrev->bonusQuest.strCat, strCat) == 0)
								pTemp = pTemp->pPrev;
						  else
						  {
						 	  printf("First question.\n");
						 	  system("pause");	
						  }
						  break;
				case 'm': printf("\nModify Bonus Question [Y/N]? ");
						  scanf("%c%*c", &cConfirm);
						  if(cConfirm == 'Y' || cConfirm == 'y')
						  	modifyBonus(&pTemp);
				          break;	
			}
		}while(c != 27);
		
	sortBQuest(pFirst);	
}

/*saves a linked list of bonus questions into a text file*/
void saveBonusQuest(ptrBonus pFirst)
{
	ptrBonus pTemp = pFirst;
	int i;
	String50 strFile;
	FILE *pFile; 
	
	sortBQuest(pFirst);
	
	printf("Save to filename: ");
	scanf("%s", strFile);
	
	pFile = fopen(strFile, "w");

	while(pTemp != NULL) 
	{
		fprintf(pFile, "%s\n", pTemp->bonusQuest.strQuest);
		
		for(i = 0; i < 4; i++)
			fprintf(pFile, "%s\n", pTemp->bonusQuest.strChoice[i]);
		
		fprintf(pFile, "%c\n", pTemp->bonusQuest.cAns);
		
		fprintf(pFile, "%s\n", pTemp->bonusQuest.strCat);
		
		pTemp = pTemp->pNext;
	}		
	
	fprintf(pFile, "$");
	
	fclose(pFile);
	
	system("cls");
	printf("File saved successfully!\n\n");
	system("pause");
}

/*loads an existing text file into a linked list of bonus questions*/
void loadBonusQuest(ptrBonus *pFirst)
{
	ptrBonus pTemp; 
	char    cTemp, check, cAns; 
	int     i;
	String50 strFile; 
	FILE   *pFile; 
	
	do
	{
		system("cls");
		printf("Overwrite existing file [Y/N]? ");
		scanf("%c%*c", &cAns);
		
		if(cAns != 'y' && cAns != 'Y' && cAns != 'n' && cAns != 'N')
			printf("\nPlease input either Y or N only!\n");
	}while(cAns != 'y' && cAns != 'Y' && cAns != 'n' && cAns != 'N');
	
	
	if(cAns == 'y' || cAns == 'Y')
	{
		if(getFile(pFile, strFile))
		{
			pFile = fopen(strFile, "r");
		
			while(check != '$')
			{	
				pTemp = malloc(sizeof(nodeBonusType));
				
				check = fgetc(pFile);
				
				if(check != '$')
				{
					pTemp->bonusQuest.strQuest[0] = check;
					
					fgets(pTemp->bonusQuest.strQuest + 1 , 81, pFile);
					pTemp->bonusQuest.strQuest[strlen(pTemp->bonusQuest.strQuest) - 1] = '\0';
			
					for(i = 0; i < 4; i++)
					{
						fgets(pTemp->bonusQuest.strChoice[i], 51, pFile);
						pTemp->bonusQuest.strChoice[i][strlen(pTemp->bonusQuest.strChoice[i]) - 1] = '\0';
					}
				
					fscanf(pFile, "%c\n", &pTemp->bonusQuest.cAns);
				
					fgets(pTemp->bonusQuest.strCat,51,pFile);
					pTemp->bonusQuest.strCat[strlen(pTemp->bonusQuest.strCat) - 1] = '\0';
				
					pTemp->pNext = NULL;
				
					addBonusLast(pFirst, pTemp);
				}	
			}
			fclose(pFile);
		}
	}
	
	system("cls");
	printf("File loaded successfully!\n\n");
	system("pause");
}
/*loads a default text file containing the details of a linked ist of bonus questions*/
void loadDefaultBonus(ptrBonus *pFirst)
{
	ptrBonus pTemp; 
	char    cTemp, check; 
	int     i;
	FILE   *pFile; 
	
	pFile = fopen("bonusQuest.txt", "r");
	
	while(check != '$')
	{	
		pTemp = malloc(sizeof(nodeBonusType));
						
		check = fgetc(pFile);
			
		if(check != '$')
		{
			pTemp->bonusQuest.strQuest[0] = check;
				
			fgets(pTemp->bonusQuest.strQuest + 1 , 81, pFile);
			pTemp->bonusQuest.strQuest[strlen(pTemp->bonusQuest.strQuest) - 1] = '\0';
			
			for(i = 0; i < 4; i++)
			{
				fgets(pTemp->bonusQuest.strChoice[i], 51, pFile);
				pTemp->bonusQuest.strChoice[i][strlen(pTemp->bonusQuest.strChoice[i]) - 1] = '\0';
			}
		
			fscanf(pFile, "%c\n", &pTemp->bonusQuest.cAns);
		
			fgets(pTemp->bonusQuest.strCat,51,pFile);
			pTemp->bonusQuest.strCat[strlen(pTemp->bonusQuest.strCat) - 1] = '\0';
		
			pTemp->pNext = NULL;
		
			addBonusLast(pFirst, pTemp);
		}	
	}
	fclose(pFile);
}

/*confirms whether the user is the admin or not*/
int isAdmin(String50 strName, String50 strPass)
{
	int i, c;
	char cRetry = 'N', inptPass[20];
	String50 inptName;
	
	do
	{
		displayLines(30);
		printf("\n\t    Login");
		displayLines(30);
	
		printf("\nUsername: ");
    	fgets(inptName,20,stdin);
    	inptName[strlen(inptName) - 1] = '\0';
        
    	printf("\nPassword: ");
    	
    	for ( i = 0; i < 20 && (c = getch()) != 13; ++i )
		{
    		inptPass[i] = c;
    		putch('*');
  		}
  			
  		inptPass[i] = '\0';
		
		if(strcmp(inptName,strName) == 0 && strcmp(inptPass,strPass) == 0)
		{
			system("cls");
			return 1;
		}
		
		else
		{
			system("cls");
			displayLines(30);
			printf("\n Invalid Username or Password");
			displayLines(30);
			
			printf("\n\n Retry Login [Y/N]? ");
			scanf("%c%*c", &cRetry);
			
			if(cRetry == 'Y' || cRetry == 'y')
				system("cls");	
		}
	}while(cRetry != 'N' && cRetry != 'n');
	
	return 0;
}

/*changes the password*/
void changePass(FILE *fPass, String50 strName, String50 strPass)
{
	int i, c;
	char cRetry = 'N', newPass1[20], newPass2[20];
	
	do
	{
		displayLines(30);
		printf("\n\tChange Password");
		displayLines(30);
		
		printf("\n Enter new password: ");			
		for ( i = 0; i < 20 && (c = getch()) != 13; ++i )
		{
    		newPass1[i] = c;
    		putch('*');
  		}
		newPass1[i] = '\0';

		printf("\n\n Confirm new password: ");
		for ( i = 0; i < 20 && (c = getch()) != 13; ++i )
		{
    		newPass2[i] = c;
    		putch('*');
  		}
		newPass2[i] = '\0';
		
		if(strcmp(newPass1,newPass2) == 0)
		{
			system("cls");		
			strcpy(strPass,newPass1);
			
			displayLines(31);
			printf("\n Password successfully changed");
			displayLines(31);
		}
							
		else
		{
			system("cls");
			displayLines(30);
			printf("\n The passwords do not match");
			displayLines(30);
		
			printf("\n\nRetry changing password [Y/N]? ");
			scanf("%c%*c", &cRetry);
		
			if(cRetry == 'Y' || cRetry == 'y')
				system("cls");
		}
		printf("\n");
		system("pause");
	}while(cRetry != 'N' && cRetry != 'n');
	
	fPass = fopen("password.txt","w");
	fprintf(fPass,"%s", strPass);
	fclose(fPass);
}

/*visual menu for the maitenance of the game*/
void Maintenance(FILE *fPassword, String50 strName, String50 strPass,
                 ptrPhrase *pPhrase, ptrBonus *pBonus)
{
	int nChoice;
		
	if(isAdmin(strName, strPass))
		do
    	{
    		displayLines(30);
    	    printf("\n\t  Maintenance\n");
    	    displayLines(30);
    		printf("\n1. Change Password\n"
   		           "\n2. View Questions\n"
   		           "\n3. View Bonus Round Questions\n"
   		           "\n4. Add Questions\n"
   		           "\n5. Add Bonus Round Questions\n"
   		           "\n6. Delete Questions\n"
    	           "\n7. Delete Bonus Questions\n"
    	           "\n8. Modify Question\n"
    	           "\n9. Modify Bonus Question\n"
    	           "\n10. Save Questions\n"
    	           "\n11. Save Bonus Questions\n"
    	           "\n12. Load Questions\n"
    	           "\n13. Load Bonus Questions\n"
   	               "\n14. Return");
    		displayLines(30);
        
			do
			{
				printf("\n\nInput choice: ");
	        	scanf("%d%*c",&nChoice);
	        	if(nChoice < 1 || nChoice > 14)
	        		printf("\nPlease input a valid option"); 
			}while(nChoice < 1 || nChoice > 14);            
                              
			switch(nChoice)
			{
    	        case 1: system("cls");
						changePass(fPassword, strName, strPass);
   	                 	break;
                                
				case 2: system("cls");
						printPhraseSort(*pPhrase);
   		                break;
    
				case 3: system("cls");
    					printBonusSort(*pBonus);
				        break;
				        
				case 4: system("cls");
    					addPhraseNode(pPhrase);
				        break;
			
				case 5: system("cls");
    					addBonusNode(pBonus);
				        break;
			
				case 6: system("cls");
    					deletePhraseSort(*pPhrase);
				        break;
			
				case 7: system("cls");
    					deleteBonusSort(*pBonus);
				        break;
			
				case 8: system("cls");
    					modifyPhraseSort(*pPhrase);
				        break;
			
				case 9: system("cls");
	    				modifyBonusSort(*pBonus);
				        break;
				
				case 10: system("cls");
  		  				 saveMystPhrase(*pPhrase);
				         break;
				        
				case 11: system("cls");
	    				 saveBonusQuest(*pBonus);
				         break;
				        
				case 12: system("cls");
	    				 loadMystPhrase(pPhrase);
				         break;
			        
				case 13: system("cls");
	    				 loadBonusQuest(pBonus);
				         break;
	        }      
	        printf("\n");
	        system("cls");
	    } while (nChoice != 14);
}

/*creates a dynamic list of mystery phrases with a single word*/
ptrPhrase *getRound1(ptrPhrase pFirst, int *nCount)
{
	int i = 0; 
	ptrPhrase pCurr = pFirst, *pList;
	
	pList = malloc(sizeof(nodePhraseType) * phraseNodeCount(pFirst));
	
	while(pCurr->pNext != NULL)
	{	
		if(pCurr->mystPhrase.wordCnt == 1)
		{
			*(pList + i) = pCurr;
			*nCount += 1;
			i++;
		}
	
		pCurr = pCurr->pNext;
	}
	
	return pList;
}

/*creates a dynamic list of mystery phrases with two words*/
ptrPhrase *getRound2(ptrPhrase pFirst, int *nCount)
{
	int i = 0; 
	ptrPhrase pCurr = pFirst, *pList;
	
	pList = malloc(sizeof(nodePhraseType) * phraseNodeCount(pFirst));
	
	while(pCurr->pNext != NULL)
	{	
		if(pCurr->mystPhrase.wordCnt == 2)
		{
			*(pList + i) = pCurr;
			*nCount += 1;
			i++;
		}
	
		pCurr = pCurr->pNext;
	}
	
	return pList;
}

/*creates a dynamic list of mystery phrases with more than two words*/
ptrPhrase *getRound3(ptrPhrase pFirst, int *nCount)
{
	int i = 0; 
	ptrPhrase pCurr = pFirst, *pList;
	
	pList = malloc(sizeof(nodePhraseType) * phraseNodeCount(pFirst));
	
	while(pCurr->pNext != NULL)
	{	
		if(pCurr->mystPhrase.wordCnt > 2)
		{
			*(pList + i) = pCurr;
			*nCount += 1;
			i++;
		}
	
		pCurr = pCurr->pNext;
	}
	
	return pList;
}

/*creates a dynamic list of bonus questions*/
ptrBonus *getBonusRound(ptrBonus pFirst, int *nCount)
{
	int i = 0; 
	ptrBonus pCurr = pFirst, *pList;
	
	pList = malloc(sizeof(nodeBonusType) * bonusNodeCount(pCurr));
	
	while(pCurr->pNext != NULL)
	{	
		*(pList + i) = pCurr;
		*nCount += 1;
		i++;

		pCurr = pCurr->pNext;
	}
	
	return pList;
}

/*reveals the given charcter present within an array of words*/
void revealChar(String50 arrWords[],String50 arrBlanks[], int n, char c)
{
	int	i, j;
	String50 arrTemp[5];
	
	for(i = 0; i < n; i++)
	{
		strcpy(arrTemp[i], arrWords[i]);
		getLowercase(arrTemp[i]);
	}	
	
	for(i = 0; i < n; i++)
		for(j = 0; j <= strlen(arrWords[i]); j++)
		{
			if(arrTemp[i][j] == c )
				arrBlanks[i][j] = c;
			else if(arrTemp[i][j] == arrBlanks[i][j])
				arrBlanks[i][j] = arrTemp[i][j];
			else if(arrTemp[i][j] >= 'a' && arrTemp[i][j] <= 'z' )
				arrBlanks[i][j] = '-';
			else if(arrTemp[i][j]!='\0')
				arrBlanks[i][j] = arrTemp[i][j];
			else
				arrBlanks[i][j] = '\0';
		}
}

/*displays a given array of words as blanks*/
void getBlanks(String50 arrWords[],String50 arrBlanks[], int n)
{
	int	i, j;
	String50 arrTemp[5];
	
	for(i = 0; i < n; i++)
	{
		strcpy(arrTemp[i], arrWords[i]);
		getLowercase(arrTemp[i]);
	}	
	
	for(i = 0; i < n; i++)
		for(j = 0; j <= strlen(arrWords[i]); j++)
		{
			if(arrTemp[i][j] >= 'a' && arrTemp[i][j] <= 'z' )
				arrBlanks[i][j] = '-';
			else if(arrTemp[i][j]!='\0')
				arrBlanks[i][j] = arrTemp[i][j];
			else
				arrBlanks[i][j] = '\0';
		}
}

/*count the number of times a character is present in a given array of words*/
int countChar(String50 arrWords[], char c)
{
	String50 arrTemp[wordCnt(arrWords)];
	int i, j, nCount = 0;
	
	for(i = 0; i < wordCnt(arrWords); i++)
	{
		strcpy(arrTemp[i], arrWords[i]);
		getLowercase(arrTemp[i]);
	}
	
	for(i = 0; i < wordCnt(arrTemp); i++)
		for(j = 0; j < strlen(arrTemp[i]); j++)
			if(arrTemp[i][j] == c)
				nCount++;
				
	return nCount;		
}

/*checks if a character is present in a given array of words*/
int checkChar(String50 arrWords[], char c)
{
	String50 arrTemp[wordCnt(arrWords)];
	int i, j;
	
	for(i = 0; i < wordCnt(arrWords); i++)
	{
		strcpy(arrTemp[i], arrWords[i]);
		getLowercase(arrTemp[i]);
	}
	
	for(i = 0; i < wordCnt(arrTemp); i++)
		for(j = 0; j < strlen(arrTemp[i]); j++)
			if(arrTemp[i][j] == c)
				return 1;
				
	return 0;		
}

/*randomizes a specific set of numbers and options*/
void spinWheel(int *totalMoney, int *roundMoney, int *currMoney, int *loseTurn)
{
	srand(time(NULL));
	
	int random;
	
	random = rand() % 100 + 1;
	
	if(random > 95)
	{
		printf("\nLose a turn!\n");
	    *loseTurn = 1;	
	}
		         		
	else if(random > 91)
	{
		printf("\nYou got bankrupt!\n");
		*totalMoney = 0;
		*roundMoney = 0;
		*loseTurn = 1;		
	}
				 			
	else if(random > 87) *currMoney = 5000;		
	else if(random > 79) *currMoney = 900;
	else if(random > 71) *currMoney = 800;		
	else if(random > 67) *currMoney = 700;		
	else if(random > 54) *currMoney = 600;			
	else if(random > 50) *currMoney = 550;		
	else if(random > 37) *currMoney = 500;			
	else if(random > 33) *currMoney = 450;		
	else if(random > 25) *currMoney = 400;
	else if(random > 21) *currMoney = 350;
	else if(random > 0) *currMoney = 300;
}

/*compares two given array of words*/
 int phraseComp(String50 arrWords[], String50 arrComp[])
{
	String50 arrTemp[wordCnt(arrWords)], arrTemp2[wordCnt(arrComp)];
	int i, j;
	
	for(i = 0; i < wordCnt(arrWords); i++)
	{
		strcpy(arrTemp[i], arrWords[i]);
		getLowercase(arrTemp[i]);
	}
	
	for(i = 0; i < wordCnt(arrComp); i++)
	{
		strcpy(arrTemp2[i], arrComp[i]);
		getLowercase(arrTemp2[i]);
	}
	
	for(i = 0; i < wordCnt(arrTemp); i++)
		if(strcmp(arrTemp[i], arrTemp2[i]) != 0)
			return 0;
	
	return 1;
}

/*checks if a given array of blanks is already filled*/
int checkComplete(String50 arrWords[], String50 arrComp[])
{
	String50 arrTemp[wordCnt(arrWords)],
	         arrTemp2[wordCnt(arrComp)];
	int i, j;
	
	for(i = 0; i < wordCnt(arrWords); i++)
	{
		strcpy(arrTemp[i], arrWords[i]);
		getLowercase(arrTemp[i]);
	}
	
	for(i = 0; i < wordCnt(arrComp); i++)
	{
		strcpy(arrTemp2[i], arrComp[i]);
		getLowercase(arrTemp2[i]);
	}
	
	for(i = 0; i < wordCnt(arrTemp); i++)
		if(strcmp(arrTemp[i], arrTemp2[i]) != 0)
			return 0;
	
	return 1;
}

/*checks whether an input charcter is a consonant or not*/
int checkConsonant(char cChar)
{
	int check;
	
	switch(cChar)
	{
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u': check = 0;
		          break;
		default: check= 1;
	}
	
	return check;	
}

/*checks whether an input character is a vowel or not*/ 
int checkVowel(char cChar)
{
	int check;
	
	switch(cChar)
	{
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u': check = 1;
		          break;
		default: check= 0;
	}
	
	return check;	
}

/*counts the number of blanksnin a given array of words*/
int getBlankCnt(String50 arrBlanks[])
{
	int i, j, nCount = 0;
	
	for(i = 0; i < wordCnt(arrBlanks); i++)
		for(j = 0; j <= strlen(arrBlanks[i]); j++)
			if(arrBlanks[i][j] == '-')
				nCount++;
				
	return nCount;
}

/*visual menu of the game per round*/
int Round(int nRound, String50 playerName[], ptrPhrase pFirst, int totalMoney[])
{
	srand(time(NULL));
	
	int i, nTurn = 0, nQuit = 0,
	    roundComplete = 0,  
		nCurrMoney[2] = {0,0},
		nRoundMoney[2] = {0,0},
		charCnt, loseTurn = 0, nAns = 0,
		rndCat,rndRound;
	char cChar, cChoice, cPass, cQuit;
	String50 arrGuess[5] = {"","","","",""},
			 arrBlanks[5] = {"","","","",""};
	String100 guessPhrase;
	
	getBlanks(pFirst->mystPhrase.strPhrase, arrBlanks, pFirst->mystPhrase.wordCnt);
	
	do
	{
		system("cls");
		
		printf("Round %d\n\n", nRound);
		printf("Player %d: %s\n", nTurn + 1, playerName[nTurn]);
		printf("Total winnings: %d\n\n", totalMoney[nTurn]);
		printf("Category: %s\n\n", pFirst->mystPhrase.strCat);
		
		nRoundMoney[nTurn] = 0;
		loseTurn = 0;
		
		for(i = 0; i < pFirst->mystPhrase.wordCnt; i++)
				printf("%s ", arrBlanks[i]);
	
		printf("\nChoose action:\n"
	       	   "[a] Spin\n"
	           "[b] Buy a vowel\n"
	    	   "[c] Guess the word\n"
	    	   "[d] Pass\n"
			   "[e] Quit\n");
		
		do
		{
			printf("\nInput choice: ");
			scanf("%c%*c", &cChoice);
			
			if(cChoice >= 'A' && cChoice <= 'Z')
	      		cChar += 32;
	      			
	      	if(cChoice < 'a' || cChoice > 'z')
	      		printf("\nPlease enter a letter within the choices!\n");
		}while(cChoice < 'a' || cChoice > 'z');
		
	
		if(cChoice == 'a' || cChoice == 'A')
		{		
			spinWheel(&totalMoney[nTurn], &nRoundMoney[nTurn], &nCurrMoney[nTurn], &loseTurn);
			printf("\nRandomized amount: %d\n", nCurrMoney[nTurn]);
			
			if(loseTurn == 0)
			{
				printf("\nChoose a consonant: ");
				scanf("%c%*c", &cChar);
	
				if(cChar >= 'A' && cChar <= 'Z')
	      			cChar += 32;
				
				if(checkConsonant(cChar))
				{
					if(checkChar(pFirst->mystPhrase.strPhrase, cChar))
					{
						nCurrMoney[nTurn] *= countChar(pFirst->mystPhrase.strPhrase, cChar);
						printf("\nTurn Winnings: %d\n", nCurrMoney[nTurn]);
						nRoundMoney[nTurn] += nCurrMoney[nTurn];
						totalMoney[nTurn] += nRoundMoney[nTurn];
						nAns = 1;
					}
					else
					{
						printf("\nSorry!\n");
						if(nTurn == 0) nTurn = 1;
						else if(nTurn == 1) nTurn = 0;
					}
				}
				else printf("\nPlease input a consonant!\n");
			}
			else
			{
				printf("\nSorry!\n");
				if(nTurn == 0) nTurn = 1;
				else if(nTurn == 1) nTurn = 0;
			}
		}
		else if(cChoice == 'b' || cChoice == 'B')
		{
			if(totalMoney[nTurn] > 0)
			{
				nRoundMoney[nTurn] -= 250;
				totalMoney[nTurn] -= 250;
				printf("\nChoose a vowel: ");
				scanf("%c%*c", &cChar);
							
				if(cChar >= 'A' && cChar <= 'Z')
	      			cChar += 32;
	      			
	      		if(checkVowel(cChar))
	      		{
	      			if(checkChar(pFirst->mystPhrase.strPhrase, cChar))
						nAns = 1;	
					else
					{	
						printf("\nSorry!\n");
						if(nTurn == 0) nTurn = 1;
						else if(nTurn == 1) nTurn = 0;			
					}
				}
				else printf("\nPlease input a vowel!\n");
			}
			else printf("\nNot enough money!\n");	
		}
		else if(cChoice == 'c' || cChoice == 'C')
		{
			printf("\nGuess: ");
			fgets(guessPhrase, 101, stdin);
			guessPhrase[strlen(guessPhrase) - 1] = '\0';
			
			storeWords(guessPhrase, arrGuess);
			
			if(phraseComp(pFirst->mystPhrase.strPhrase, arrGuess))
			{
				printf("\nCongratulations!\n");
				nCurrMoney[nTurn] = 500 * getBlankCnt(arrBlanks);
				nRoundMoney[nTurn] += nCurrMoney[nTurn];
				totalMoney[nTurn] += nRoundMoney[nTurn];
				printf("\nTurn Winnings: %d\n", nCurrMoney[nTurn]);			
				roundComplete = 1;
			}
			else
			{
				printf("\nSorry!\n");
				if(nTurn == 0) nTurn = 1;
				else if(nTurn == 1) nTurn = 0;
			}
		}
		else if(cChoice == 'd' || cChoice == 'D')
		{
			printf("\nPass turn [Y/N]? ");
			scanf("%c%*c", &cPass);
			
			if(cPass == 'y' || cPass == 'Y')
			{
				if(nTurn == 0) nTurn = 1;
				else if(nTurn == 1) nTurn = 0;
			}
		}
		else if(cChoice == 'e' || cChoice == 'E')
		{
			printf("\nQuit game [Y/N]? ");
			scanf("%c%*c", &cQuit);
			
			if(cQuit == 'y' || cQuit == 'Y')
			{
				system("cls");
				nQuit = 1;
				roundComplete = 0;
				if(nTurn == 0)
					printf("Player 2: %s wins!\n", playerName[nTurn + 1]);
				else if(nTurn == 1) 
					printf("Player 1: %s wins!\n", playerName[nTurn - 1]);
			}	
		}

		if(nAns == 1)
		{
			revealChar(pFirst->mystPhrase.strPhrase, arrBlanks, pFirst->mystPhrase.wordCnt, cChar);
			for(i = 0; i < pFirst->mystPhrase.wordCnt; i++)
				printf("%s ", arrBlanks[i]);
		}
		
		printf("\n");
		if(checkComplete(pFirst->mystPhrase.strPhrase, arrBlanks))
			roundComplete = 1;
			
		nAns = 0;
		system("pause");
	}while(nQuit == 0 && roundComplete == 0);

	return roundComplete;
}

/*control function of the mechanics of the game*/
void PlayGame(ptrPhrase pPhrase, ptrBonus pBonus)
{
	srand(time(NULL));
	
	int i,
		rndPhrase1, rndPhrase2, rndPhrase3, rndBonus,
		phrase1Cnt = 0, phrase2Cnt = 0, phrase3Cnt = 0, bonusCnt = 0,
		nTotalMoney[2] = {0,0};
	char cBonusAns, cChar;
	String50 strPlayerName[2], strWinner;
	ptrPhrase *pRound1, *pRound2, *pRound3,
	          pQuest1, pQuest2, pQuest3;
	ptrBonus *pBonusRound, pBonusQuest;
	
	for(i = 0; i < 2; i++)
	{
		printf("Player %d: ", i+1);
		fgets(strPlayerName[i], 51, stdin);
		strPlayerName[i][strlen(strPlayerName[i]) - 1] = '\0';
		printf("\n");
	}
		
	pRound1 = getRound1(pPhrase, &phrase1Cnt);
	rndPhrase1 = rand() % phrase1Cnt;
	pQuest1 = *(pRound1 + rndPhrase1);
	
	if(Round(1, strPlayerName, pQuest1, nTotalMoney))
	{	
		pRound2 = getRound2(pPhrase, &phrase2Cnt);
		rndPhrase2 = rand() % phrase2Cnt;
		pQuest2 = *(pRound2 + rndPhrase2);
		
		if(Round(2, strPlayerName, pQuest2, nTotalMoney))
		{	
			pRound3 = getRound3(pPhrase, &phrase3Cnt);
			rndPhrase3 = rand() % phrase3Cnt;
			pQuest3 = *(pRound3 + rndPhrase3);
			
			if(Round(3, strPlayerName, pQuest3, nTotalMoney))
			{
				system("cls");
				
				printf("Winnings:\n\n"
				       "Player 1: %d\n"
					   "Player 2: %d\n", nTotalMoney[0], nTotalMoney[1]);
				
				if(nTotalMoney[0] > nTotalMoney[1])
				{
					printf("\nPlayer 1: %s wins!\n\n", strPlayerName[0]);
					strcpy(strWinner, strPlayerName[0]);
				}	
				else if(nTotalMoney[1] > nTotalMoney[2])
				{
					printf("\nPlayer 2: %s wins!\n\n", strPlayerName[1]);
					strcpy(strWinner, strPlayerName[1]);
				}
				
				system("pause");
				
				pBonusRound = getBonusRound(pBonus, &bonusCnt);
				rndBonus = rand() % bonusCnt;
				pBonusQuest = *(pBonusRound + rndBonus); 
				
				system("cls");
				printf("Bonus Round\n");
				printf("\n%s\n", strWinner);
				
				printf("\n%s\n", pBonusQuest->bonusQuest.strQuest);
				
				for(i = 0; i < 4; i++)
					printf("%s\n", pBonusQuest->bonusQuest.strChoice[i]);
				
				do
				{
					printf("\nAnswer: ");
					scanf("%c%*c", &cBonusAns);
				
					if(cBonusAns >= 'A' && cBonusAns  <= 'Z')
	      				cChar += 32;
	      			
					if(cBonusAns < 'a' || cBonusAns > 'd')
						printf("\nPlease enter a letter within the choices!\\n");	
				}while(cBonusAns < 'a' || cBonusAns > 'd');	
				
				if(cBonusAns == pBonusQuest->bonusQuest.cAns)
				{
					system("cls");
					printf("\nCongratulations %s!\n", strWinner);
					if(strcmp(strWinner, strPlayerName[0]) == 0)
					{
						nTotalMoney[0] += 5000;
						printf("\nTotal Winnings: %d\n\n", nTotalMoney[0]);
					}
					else
					{
						nTotalMoney[1] += 5000;
						printf("\nTotal Winnings: %d\n\n", nTotalMoney[1]);
					}	
				}
				
				else
				{
					system("cls");
					printf("\nToo bad!\n");
					if(strcmp(strWinner, strPlayerName[0]) == 0)
						printf("\nTotal Winnings: %d\n\n", nTotalMoney[0]);
				
					else	
						printf("\nTotal Winnings: %d\n\n", nTotalMoney[1]);
				}
			}
		}
	}
	system("pause");
	system("cls");
	displayLines(30);
	printf("\n\t  GAME OVER\n");
	displayLines(30);
	printf("\n");

}

/*main function*/
int main()
{
	int nChoice;
	String50 strName, strPass;
	ptrPhrase pPhrase = NULL;
	ptrBonus pBonus = NULL;
	FILE *fPassword;
	
	strcpy(strName,"Kurt Aquino");
	fPassword = fopen("password.txt","r");
	fscanf(fPassword,"%s",&strPass);
	fclose(fPassword);
    
	loadDefaultPhrase(&pPhrase);
	loadDefaultBonus(&pBonus);
	                 
	sortPhrase(pPhrase);
	sortBQuest(pBonus);
	                                   
    do
    {
    	displayLines(30);
        printf("\n\t  MAIN MENU\n");
        displayLines(30);
    	printf("\n1. Maintenance\n"
               "\n2. Play Game\n"
               "\n3. Exit");
        displayLines(30);
        
		do
		{
			printf("\n\nInput choice: ");
        	scanf("%d%*c",&nChoice);
        	if(nChoice < 1 || nChoice > 3)
        		printf("\nPlease input a valid option"); 
		}while(nChoice < 1 || nChoice > 3);            
                              
        switch(nChoice)
        {
            case 1: system("cls");
            		Maintenance(fPassword, strName, strPass, &pPhrase, &pBonus);
                    break;
                                
            case 2: system("cls");
					PlayGame(pPhrase, pBonus);
                    break;

            case 3: system("cls");
    				displayLines(30);
        			printf("\n\t!!!THANK YOU!!!");
        			displayLines(30);
			        break;
        }  
		    
        printf("\n");
        system("pause");
        system("cls");
    }while(nChoice != 3); 
    
	return 0;                                       
}
