//gcc -std=c99 -I/usr/include/libusb-1.0/ -Iinclude -Llib quiz_hid.c hid-libusb.c -lusb-1.0 lib/zentriert.a -o quiz_hid
//sudo ./quiz_hid

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include "hidapi.h"
#include "ansi_escape.h"
#include "zentriert.h"

#define BUFFER_SIZE 1000

static struct Daten
{
  char frage[200];
  char richtigeAntwort[40];
  char antwort2[40];
  char antwort3[40];
  char antwort4[40];
  char thema[40];
  char schwierigkeit[1];
}daten[1000];

//CSV einlesen
char buf[1000];
char tok[200];
int l = 0;

//Timer
time_t zeit_start, zeit_jetzt;
double zeit;

int anzahlFragen = 0;
int aktuelleFrage = 0;

int fragenummer = 1;
int y;
int aktuellerSpieler = 0;
int next = 0;

int punkte[4];

int x = 1;
int antworten[4];
int i;
int beenden = 0;

char temp_text[200];
char punkte_teil1[20];
char punkte_teil2[20];
char punkte_teil3[20];
char punkte_teil4[20];

//HID
int res;
unsigned char buf2[65];
unsigned char buf_temp[65];
hid_device *handle;

void csv_einlesen();
void gameinit();
int get_player();
int answer_correct();
void buzzer_led(int one, int two, int three, int four);
void punktestand();

int main(int argc, char* argv[])
{
	NOCUR
	for(int a = 0; a<4;a++)
	{
		punkte[a] = 0;
	}
	CLSCR
	GOTO(0,0)
	ausgabe_zentriert("loading...\n");
	hid_init();
	while(handle == NULL)
	{
		handle = hid_open(0x054c, 0x1000, NULL);
		if(handle == NULL)
			ausgabe_zentriert("Bitte Buzz! Controller anschließen...\r");
	}
	printf("\n");
	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);
	// Set the hid_read() function to be blocking.
	//hid_set_nonblocking(handle, 0);
	buzzer_led(0,0,0,0);
	csv_einlesen();
	anzahlFragen = l;
	int fragenAbgearbeitet[l+1];
	int fragen[l+1];
	time_t t;
	time(&t);
	srand((unsigned int)t);
	gameinit();
	
	for( int n = 0; n<anzahlFragen;n++)
	{
		fragen[n] = n+1;
	}
	
	for (int j = 0; j<anzahlFragen; j++)
	{
		int r = j + (rand() % ((anzahlFragen) - j));
		int temp = fragen[j];
		fragen[j] = fragen [r];
		fragen[r] = temp;
	}
			
	while (get_player() == 0)
	{
		ausgabe_zentriert("Buzzern zum beginnen...\r");
	}
	while (get_player() != 0){}
	
	//Frageschleife
	while(beenden == 0 && fragenummer <= anzahlFragen+1)
	{	
		/*do
		{
			aktuelleFrage = rand() % anzahlFragen;
		}while(fragenAbgearbeitet[aktuelleFrage] == 1);
		fragenAbgearbeitet[aktuelleFrage] = 1;*/
		
		aktuelleFrage = fragen[fragenummer-1];
		
		//Bildschirminhalt leeren und Highlight auf linke obere Ecke	
		CLSCR
		GOTO(0,0)

		next = 0;
		aktuellerSpieler = 0;
		BRIGHT
		ausgabe_zentriert("Beat IT!\n\n");
		NORMAL
		ausgabe_zentriert("Punktestand: \n");
		
		sprintf(punkte_teil1, "Player 1: %d     ", punkte[0]);
		sprintf(punkte_teil2, "Player 2: %d     ", punkte[1]);
		sprintf(punkte_teil3, "Player 3: %d     ", punkte[2]);
		sprintf(punkte_teil4, "Player 4: %d", punkte[3]);
		
		sprintf(temp_text, "%s%s%s%s\n\n", punkte_teil1, punkte_teil2, punkte_teil3, punkte_teil4);
		ausgabe_zentriert(temp_text);
		sprintf(temp_text, "Frage Nummer: %d\n",fragenummer);
		ausgabe_zentriert(temp_text);
		sprintf(temp_text, "%s\n\n",daten[aktuelleFrage].frage);
		ausgabe_zentriert(temp_text);
		for (int j = 0; j<4; j++)
		{
			int r = j + (rand() % (4 - j));
			int temp = antworten[j];
			antworten[j] = antworten [r];
			antworten[r] = temp;
		}
		for (int c = 0; c<4;c++)
		{
			switch (antworten[c])
			{
				case 1: sprintf(temp_text, "%d: %s\n", c+1, daten[aktuelleFrage].richtigeAntwort); ausgabe_zentriert(temp_text); break;
				case 2: sprintf(temp_text, "%d: %s\n", c+1, daten[aktuelleFrage].antwort2); ausgabe_zentriert(temp_text); break;
				case 3: sprintf(temp_text, "%d: %s\n", c+1, daten[aktuelleFrage].antwort3); ausgabe_zentriert(temp_text); break;
				case 4: sprintf(temp_text, "%d: %s\n", c+1, daten[aktuelleFrage].antwort4); ausgabe_zentriert(temp_text); break;
			}
		}
		ausgabe_zentriert("\n");
				
		//Antwort Schleife
		while(aktuellerSpieler == 0)
		{
			int p = get_player();
			if (p != 0)
			{
				sprintf(temp_text, "Player %d - du bist dran - wähle eine Antwort!\n\n", p);
				ausgabe_zentriert(temp_text);
				switch(p)
				{
					case 1:
						buzzer_led(1,0,0,0);
						break;
					case 2:
						buzzer_led(0,1,0,0);
						break;
					case 3:
						buzzer_led(0,0,1,0);
						break;
					case 4:
						buzzer_led(0,0,0,1);
						break;
				}
			}
			aktuellerSpieler = p;
		}
		
		zeit_start = time(NULL);
		
		while(next == 0)	//auf Antwort warten
		{
			answer_correct();
			zeit_jetzt = time(NULL);
			zeit = difftime(zeit_jetzt, zeit_start);
			if(next == 0)
			{
				if(zeit < 5)
				{
					sprintf(temp_text, "Noch %.0f Sekunden zum wählen...\r", 5-zeit);
					ausgabe_zentriert(temp_text);
				}
				else
				{
					ausgabe_zentriert("Zu spät - das gibt einen Minuspunkt.\n\n");
					ausgabe_zentriert("Zum Fortsetzen buzzern.\n");
					punkte[aktuellerSpieler-1]--;
					next = 1;
				}
			}
		}
		while (get_player() != aktuellerSpieler){}
		buzzer_led(0,0,0,0);
		//punktestand();
		//while (get_player() == 0){}
		//while (get_player() != 0){}
		fragenummer++;
	}
	hid_close(handle);
	hid_exit();
	ausgabe_zentriert("Zum beenden beliebige Taste drücken");
	getchar();
	return 0;
}

void csv_einlesen()
{
	FILE    *fpData = NULL;        

	if ( ( fpData = fopen( "quiz-fragen.csv", "r" ) ) == NULL ) //Reading a file
	{
		  printf( "File could not be opened.\n" );
	}

	while(fgets(buf, BUFFER_SIZE, fpData) != NULL)
	{		
		strcpy(daten[l].frage, strtok(buf,";")); 
		//printf("%s, ",daten[l].frage );
		
		strcpy(daten[l].richtigeAntwort, strtok(NULL,";")); 
		//printf("%s, ",daten[l].richtigeAntwort );
		
		strcpy(daten[l].antwort2, strtok(NULL,";")); 
		//printf("%s, ",daten[l].antwort2 );
		
		strcpy(daten[l].antwort3, strtok(NULL,";")); 
		//printf("%s, ",daten[l].antwort3 );
		
		strcpy(daten[l].antwort4, strtok(NULL,";")); 
		//printf("%s, ",daten[l].antwort4 );
		
		strcpy(daten[l].thema, strtok(NULL,";")); 
		//printf("%s, ",daten[l].thema );
		
		strcpy(daten[l].schwierigkeit, strtok(NULL,"\n")); 
		//printf("%s\n",daten[l].schwierigkeit );
		
		l++;
	}
	fclose(fpData);
}

void gameinit()
{
	//Game
	struct Daten *daten = NULL;
	
	for (int j = 0; j<4; j++)
	{
		antworten[j] = j+1;
	}
	daten = realloc(daten, x * sizeof(struct Daten));
	if (daten == NULL)
	{
		printf("Es konnte kein Speicher angelegt werden.\n");
		//return 1;
	}
}

int get_player()
{
	res = hid_read(handle, buf2, 6);
	if (res < 0)
		printf("Unable to read()\n");
				
	//Buzzer Abfrage
	if(buf2[2] == 1)
		return 1;
	else if(buf2[2] == 32)
		return 2;
	else if(buf2[3] == 4)
		return 3;
	else if(buf2[3] == 128)
		return 4;
	else
		return 0;
}

int answer_correct()
{
	int ret = -1;
	int nummer = 0;
	
	res = hid_read(handle, buf2, 6);
	if (res < 0)
		printf("Unable to read()\n");
		
	switch(aktuellerSpieler)
	{
		case 1:
			if(buf2[2] == 16)
				nummer = 1;
			else if(buf2[2] == 8)
				nummer = 2;
			else if(buf2[2] == 4)
				nummer = 3;
			else if(buf2[2] == 2)
				nummer = 4;
			break;
		case 2:
			if(buf2[3] == 2)
				nummer = 1;
			else if(buf2[3] == 1)
				nummer = 2;
			else if(buf2[2] == 128)
				nummer = 3;
			else if(buf2[2] == 64)
				nummer = 4;
			break;
		case 3:
			if(buf2[3] == 64)
				nummer = 1;
			else if(buf2[3] == 32)
				nummer = 2;
			else if(buf2[3] == 16)
				nummer = 3;
			else if(buf2[3] == 8)
				nummer = 4;
			break;
		case 4:
			if(buf2[4] == 248)
				nummer = 1;
			else if(buf2[4] == 244)
				nummer = 2;
			else if(buf2[4] == 242)
				nummer = 3;
			else if(buf2[4] == 241)
				nummer = 4;
			break;
	}
	if (nummer > 0 && nummer < 5)
	{
		if (antworten[nummer-1] == 1)
		{
			sprintf(temp_text, "Du hast Antwort Nummer %d gewählt: \"%s\"\n", nummer, daten[aktuelleFrage].richtigeAntwort);
			ausgabe_zentriert(temp_text);
			ausgabe_zentriert("Richtige Antwort!\n\n");
			ausgabe_zentriert("Buzzern zum fortsetzen.\n");
			punkte[aktuellerSpieler-1]++;
			ret = 1;
		}
		else
		{
			sprintf(temp_text, "Du hast Antwort Nummer %d gewählt.\n", nummer);
			ausgabe_zentriert(temp_text);
			ausgabe_zentriert("Falsche Antwort!\n");
			sprintf(temp_text, "Richtige Antwort: \"%s\"\n\n", daten[aktuelleFrage].richtigeAntwort);
			ausgabe_zentriert(temp_text);
			ausgabe_zentriert("Buzzern zum fortsetzen.\n");
			punkte[aktuellerSpieler-1]--;
			ret = 0;
		}
		next = 1;
	}
	return ret;
}

void buzzer_led(int one, int two, int three, int four)
{
	buf2[0] = 0;
	
	buf2[1] = 0;
	
	if(one == 1)
		buf2[2] = 0xff;
	else
		buf2[2] = 0;
		
	if(two == 1)
		buf2[3] = 0xff;
	else
		buf2[3] = 0;
		
	if(three == 1)
		buf2[4] = 0xff;
	else
		buf2[4] = 0;
		
	if(four == 1)
		buf2[5] = 0xff;
	else
		buf2[5] = 0;
		
	buf2[6] = 0;
	
	buf2[7] = 0;
	
	res = hid_write(handle, buf2, 8);	
}
/*
void punktestand()
{
	printf("\033[2J");
	printf("\033[;H");
	printf("Aktueller Punktestand:\n\n");
	for(int b = 1; b < 5; b++)
	{
		printf("Player %d: %d\n", b, punkte[b-1]);
	}
	//sleep(5);
	printf("\nZum Fortsetzen buzzern!\n");
}*/
