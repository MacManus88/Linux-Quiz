/*#include <stdio.h> 
#include <string.h> 

#define GESAMTBREITE 80

char frageTeil1[70];
char frageTeil2[70];
char frageTeil3[70];
char frageTeil4[70];

int main(void) 
{ 
    char ueberschrift[200];

    strcpy(ueberschrift, "Dieser Text ist zentriertDieser Text ist zentriertDieser Text ist zentriertDieser Text ist zentriertDieser Text ist zentriertDieser Text ist zentriert"); 

    ausgabe_zentriert(ueberschrift);
    
    return 0; 
}
*/

#include "zentriert.h"

char frageTeil1[70];
char frageTeil2[70];
char frageTeil3[70];
char frageTeil4[70];

int ausgabe_zentriert(char ausgabe_string[200])
{		
	if (strlen(ausgabe_string) <= 70)
		ausgabe_zentrieren(ausgabe_string);
	else
	{
		frage_splitten(ausgabe_string);
		ausgabe_zentrieren(frageTeil1);
		printf("\n");
		ausgabe_zentrieren(frageTeil2);
		printf("\n");
		ausgabe_zentrieren(frageTeil3);
		printf("\n");
		ausgabe_zentrieren(frageTeil4);
	}
	
	return 0;
}

int ausgabe_zentrieren(char ausgabe_string[200])
{	
	int i;
	for(i=0;i<=(int)((GESAMTBREITE - strlen(ausgabe_string))/2);i++)
	{ 
		printf(" "); 
	} 
	
	printf("%s", ausgabe_string);
	
  return 0;
}

int frage_splitten(char gesamte_frage[200])
{
		char frageTemp[200];
		int zuschneidVar = 50;
		int teilVar = 1;
		strcpy(frageTemp, gesamte_frage); 
		while(frageTemp[zuschneidVar] != 0)
		{
			if (frageTemp[zuschneidVar] == 32)
			{
				frageTemp[zuschneidVar] = 10;
				zuschneidVar += 50;
				teilVar++;
			}
			else
				zuschneidVar++;
		}
		switch(teilVar)
		{
			case 1:
				strcpy(frageTeil1, frageTemp);
				strcpy(frageTeil2, "\r");
				strcpy(frageTeil3, "\r");
				strcpy(frageTeil4, "\r");
				break;
			case 2:
				strcpy(frageTeil1, strtok(frageTemp,"\n"));
				strcpy(frageTeil2, strtok(NULL,"\0"));
				strcpy(frageTeil3, "\r");
				strcpy(frageTeil4, "\r");
				break;
			case 3:
				strcpy(frageTeil1, strtok(frageTemp,"\n"));
				strcpy(frageTeil2, strtok(NULL,"\n"));
				strcpy(frageTeil3, strtok(NULL,"\0"));
				strcpy(frageTeil4, "\r");
				break;
			case 4:
				strcpy(frageTeil1, strtok(frageTemp,"\n"));
				strcpy(frageTeil2, strtok(NULL,"\n"));
				strcpy(frageTeil3, strtok(NULL,"\n"));
				strcpy(frageTeil4, strtok(NULL,"\0"));
				break;
		}
		
		return 0;
}
