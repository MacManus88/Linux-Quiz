#ifndef smdANSI_H 
#define smdANSI_H 

/*-------------Farbe Vordergrund-------------------------------*/ 


#define BLACK       printf("\033[30m"); 
#define RED     printf("\033[31m"); 
#define GREEN       printf("\033[32m"); 
#define YELLOW          printf("\033[33m"); 
#define BLUE        printf("\033[34m"); 
#define MAGENTA         printf("\033[35m"); 
#define CYAN        printf("\033[36m"); 
#define WHITE       printf("\033[37m"); 


/*------------------------Farbe Hintergrund----------------------*/ 

#define HBLACK           printf("\033[40m"); 
#define HRED         printf("\033[41m"); 
#define HGREEN           printf("\033[42m"); 
#define HYELLOW          printf("\033[43m"); 
#define HBLUE        printf("\033[44m"); 
#define HMAGENTA     printf("\033[45m"); 
#define HCYAN        printf("\033[46m"); 
#define HWHITE       printf("\033[47m"); 


/*----------------- EFFECTE setzen-----------------------------*/ 


#define SETEFFECT(a,b,c) printf("\033[%d;%d;%dm",(a),(b),(c)); 


/* Bsp:  SETEFFECT(5,32,45)    Kombination aus BLINK GREEN und HMAGENTA */ 


/*----------------Sonstiges Schrift--------------------------------*/ 


#define BRIGHT      printf("\033[1m"); 
#define DARK        printf("\033[2m");  /* abgedunkelte Schrift */ 
#define ITALIC      printf("\033[3m"); 
#define UNDERLINE   printf("\033[4m"); /* Unterstrichen */ 
#define BLINK       printf("\033[5m"); /*Blinken ein */ 
#define QBLINK      printf("\033[6m"); /* schnelles Blinken ein */ 
#define REVERSE         printf("\033[7m");  /* reverse video */ 
#define NOECHO          printf("\033[8m"); 
#define NORMAL          printf("\033[0m"); /* Alles wieder normal */ 



/* ----------------Curser und CO--------------------------------*/ 

#define CLSCR       printf("\033[2J"); 
#define CLREOL      printf("\033[K"); /* Loescht bis Zeilenende */ 
#define SAVECURPOS  printf("\033[s"); /*speicher actuelle Position */ 

#define SETSAVEDPOS printf("\033[u"); /* geht zur gespeicherten Position */ 

#define SCURUP      printf("\033[A"); /* Eine Zeile hoch */ 
#define SCURDOWN    printf("\033[B"); /* eine Zeile runter */ 

#define SCURRIGHT   printf("\033[C"); /* eine Zeile nach rechts */ 

#define SCURLEFT    printf("\033[D"); /* eine Spalte nach links */ 

#define CURUP(x)    printf("\033[%dA",(x)); /* Gehe x Zeilen nach oben */ 
#define CURDOWN(x)  printf("\033[%dB",(x)); /*gehe x Zeile nach unten */ 
#define CURRIGHT(x) printf("\033[%dC",(x)); /* gehe x Spalten nach rechts */ 
#define CURLEFT(x)  printf("\033[%dD",(x)); /* gehe x Spalten nach links */
#define GOTO(x,y)   printf("\033[%d;%dH",(y),(x)); /*  Gehe zur Position x,y */ 

#define CURGOHOME(x)    printf("\033[%df",(x)); /* Geh nach Zeile x und setze Curser auf Anfang */ 

#define CURDOWNHOME(x)  printf("\033[%dE",(x)); /*Gehe x Zeilen nach  unten und setzte Curser an den Anfang */ 

#define CURUPHOME(x)    printf("\033[%dF",(x)); /* Gehe x Zeilen nach oben und setze Curser an den Anfang */ 

#define NOCUR			printf("\033[?25l"); /* Cursor deaktivieren */


/*------------ Ende Ansi.h-------------------------------------*/ 



#endif
