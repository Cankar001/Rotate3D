#include <stdio.h>                 // stderr
#include <fcntl.h>                 // open
#include <unistd.h>                // seek
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#define MAX_SRC_SIZE  5            // Maximale Source-Größe in Megabytes
#define TRUE          1
#define FALSE         0

#define UBYTE unsigned char

char *Buffer;             // Gesamter Source
char  Line[1024];         // Eine einzelne Zeile
char  Format[1024];       // Eine fertig formatierte Zeile

int i;
int F;
unsigned long BufferSize, Size;
unsigned long P, Dest, Len, n;
unsigned long CurrentLine;

char Flag, SpaceFlag, Q, *Ptr;

char VerboseEnabled = FALSE, LineCounter = FALSE;
char ProcessedFiles = 0;

#define DEBUG  if (VerboseEnabled)  fprintf

int main(int argc, char *argv[])
   {
   if (argc < 2)
      {
      fprintf(stderr, "[?] USAGE:   %s -v[erbose] <Src01.c> -q[uiet] <Src02.C> ...\n", argv[0]);
      exit(-1);
      }

   DEBUG(stderr, "Creating prototypes:\n");

   for (i=1; i<argc; i++)            // Für alle angegebenen Source-Files...
      {
      if (0==strcmp("-v", argv[i]))  { VerboseEnabled = TRUE;  continue; }
      if (0==strcmp("-q", argv[i]))  { VerboseEnabled = FALSE; continue; }
      if (0==strcmp("-l", argv[i]))  { LineCounter    = TRUE; continue; }

      // ------------------------
      // Öffnen eines Sourcefiles
      // ------------------------
      DEBUG(stderr, "  File = \"%s\"\n", argv[i]);

      F = open(argv[i], O_RDONLY);
      if (-1 == F)
         {
         fprintf(stderr, "!   Open failed: can't access file \"%s\". File ignored.\n", argv[i]);
         continue;
         }

      // --------------------
      // Dateigröße bestimmen
      // --------------------

      BufferSize = 10 + lseek(F, 0, SEEK_END);
      if (0 == BufferSize)
         {
         fprintf(stderr, "0 bytes.\n!   Sourcefile \"%s\" is empty. File ignored.\n", argv[i]);
         continue;
         }

      lseek(F, 0, SEEK_SET);
      Buffer = (char*)malloc(BufferSize);

      DEBUG(stderr, "    Loading "); fflush(stderr);
      Size = read(F, Buffer, BufferSize);
      if (0 == Size)
         {
         fprintf(stderr, "\n!   Can't read \"%s\". File ignored.\n", argv[i]);
         continue;
         }
      DEBUG(stderr, "%d bytes.\n", Size);
      close(F);

      DEBUG(stderr, "    ");

      CurrentLine = 0;
      Dest = 0;
      for (P=0; P<Size; P++)
         {
         Line[Dest] = Buffer[P];
         if ('\r' == Line[Dest]) continue;										// ^M entfernen
         if ('\n' != Line[Dest++]) continue;
         // Nachdem eine Zeile komplett eingelesen wurde, wird am Ende ein Nullbyte gesetzt.
         Line[Dest-1] = 0;
         Len = Dest;
         Dest = 0;
         CurrentLine++;
         
         // Kommentare am Zeilenende wegschneiden
         Ptr = strstr(Line, "//");   if (Ptr)  *Ptr = 0;
         Ptr = strstr(Line, "/*");   if (Ptr)  *Ptr = 0;
         Len = strlen(Line);

         // Zeilen, die mit einem Space beginnen, können kein Funktionskopf sein.
         if (Line[0] == ' ')  continue;

         // Prototypen müssen mit einem Buchstaben beginnen
         if (!isalpha(Line[0]))  continue;

         // Anführungszeichen sind nicht erlaubt.
         for (n=0; n<Len; n++)  if ('\"' == Line[n])  continue;


         // -------------------------------------------
         // Kopfzeilen müssen ein Klammerpaar enthalten
         // -------------------------------------------

         // erstmal eine öffnende Klammer suchen
         Flag = FALSE;
         SpaceFlag = FALSE;
         for (n=0; n<Len; n++)
            {
            if (' ' == Line[n]) { SpaceFlag = TRUE; }
            if ('(' == Line[n]) { Flag = TRUE;  break; }
            }
         if (FALSE==Flag)  continue;                       // Eine Klammer muß vorhanden sein
         if (FALSE==SpaceFlag)  continue;                  // Vor der Klammer muß es ein Space gegeben haben

         // vor der öffnenden Klammer darf kein Space stehen
         if (' '==Line[n-1])  continue;

         // jetzt muß noch eine schließende Klammer folgen
         Flag = FALSE;
         for (   ; n<Len; n++)  if (')' == Line[n]) { Flag = TRUE;  break; }
         if (FALSE==Flag)  continue;

         // Die Zeile darf keine Anführungszeichen enthalten
         Flag = FALSE;
         for (n=0; n<Len; n++)  if ('\"' == Line[n]) Flag=TRUE;
         if (TRUE==Flag)  continue;

         // Die Funktion darf nicht "static" sein
         if (strstr(Line, "static")) continue;

         // Die Funktion sollte keine Klassenmethode sein
         if (strstr(Line, "::")) continue;

         // Ein Klammerpaar "()" wird durch "(void)" ersetzt
         if (Ptr=(char*)strstr(Line, "()"))
            {
            strcpy(Ptr, "(void)");
            }
       //DEBUG(stderr, ".");  fflush(stderr);

         // ------------------------------------------------------------
         // Ok. Das scheint was zu sein!
         // ------------------------------------------------------------

         // Spaces am Zeilenende entfernen:
         for (Ptr = Line + strlen(Line) - 1;  Ptr > Line; --Ptr)
         	{
         	if (' ' != *Ptr)  break;												// Stop bei ersten Nicht-Space
         	*Ptr = 0;																	// Sonst das Space entfernen
         	}
         
         // Haben wir schon ein Semikolon am Zeilenende?
         if (0 == index(Line, ';'))  strcat(Line, ";");        

         // --------------------
         // Ausgabe auf stdout !
         // --------------------
         Ptr = (char*)index(Line, ' ');                   // Das erste Space suchen.
         *Ptr = 0;                                        // Die Zeile hier unterbrechen.
         if (LineCounter)  printf("%d: ", CurrentLine);
         printf("%-8s ", Line);									 // Rückgabe-Type ausgeben
         ++Ptr;
         if ('*' == *Ptr)
            {
            printf("%-91s /* %s */\n", Ptr, argv[i]);
            }
         else                                             // Falls nun kein "*" folgt: Space einfügen. 
            {
            printf(" %-90s /* %s */\n",Ptr, argv[i]);
            }

         ++ProcessedFiles;
         }
      DEBUG(stderr, "\n");
      }

   return(ProcessedFiles ? 0 : -1);                       // Melden, ob min. eine Proto.h erzeugt wurde
   }
