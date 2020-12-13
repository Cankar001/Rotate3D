#include "Rotate3D.h"

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

Display       *MyDisplay;
Screen        *MyScreen;
Window         RootWin;
GC             MyGC, InvGC;           // Graphical Context
int            MyWindow;              // Window - ID
XTextProperty  WindowNameX;
XSizeHints     SizeHints;
XWMHints       WM_Hints;
XClassHint     C_Hints;
XFontStruct   *Font_Info, *FontSmall_Info, *FontBig_Info;
XGCValues      Values;
XEvent         Report;
XVisualInfo    MyVisualInfo;
ULONG          MyBlack, MyWhite, MyRed, MyBlue, MyGreen, MyPink;
ULONG          TempScala[100];
int            MyScreenNumber, Width, Height, err;
int            i, res;
char          *WindowName=(char*)"DemoX    <c> 2008 Dr. Gorontzi";
Pixmap         Bgnd;
WORD           BgndX=-1, BgndY=-1;
double			newXRight, newXLeft, newYUp, newYDown;

WORD				xTopLeftOld, yTopLeftOld, xTopRightOld, yTopRightOld;
WORD				xBottomLeftOld, yBottomLeftOld, xBottomRightOld, yBottomRightOld;

void OpenVideo()
   {
   XSetWindowAttributes Attr;
   unsigned int         AttrMask;
   XColor               Got, Should;
   XVisualInfo          *vi;
   GLXContext           glc;
   GLint                att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
   BOOL succ;
   
   MyDisplay             = XOpenDisplay(NULL);
   vi                    = glXChooseVisual(MyDisplay, 0, att);
   MyScreenNumber        = DefaultScreen(MyDisplay);
   RootWin               = RootWindow(MyDisplay, MyScreenNumber);
   MyScreen              = (Screen*)DefaultScreenOfDisplay(MyDisplay);
   Width                 = DisplayWidth(MyDisplay, MyScreenNumber);
   Height                = DisplayHeight(MyDisplay, MyScreenNumber);
   MyWindow              = XCreateWindow(MyDisplay, RootWin, 0, 0, XSIZE, YSIZE, 1, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &Attr);

   /*MyWindow              = XCreateSimpleWindow(MyDisplay, RootWin, 
                                               0, 0, XSIZE, YSIZE, 1,                   // 1 = Border Width
                                               WhitePixel(MyDisplay, MyScreenNumber),
                                               BlackPixel(MyDisplay, MyScreenNumber)); */
   err                   = XStringListToTextProperty(&WindowName, 1, &WindowNameX);
   

   C_Hints.res_class     = (char*)"PF Window";
   C_Hints.res_name      = (char*)"PF";
   WM_Hints.initial_state= NormalState;                                    // Nicht als Icon starten
   WM_Hints.input        = 1;                                              // Wir wollen Input!
   WM_Hints.flags        = InputHint;                                      // | StateHint

   SizeHints.flags       = USPosition | USSize | PPosition | PSize | PMinSize;
   SizeHints.x           = 1;
   SizeHints.y           = 1;                                                // Hier geben wir an, wo das
   SizeHints.width       = XSIZE;                                            // Window erscheinen soll, 
   SizeHints.height      = YSIZE;                                            // und dass man seine Groesse
   SizeHints.min_width   = 10;                                               // veraendern darf.
   SizeHints.max_width   = 10;

   XSetWMProperties(MyDisplay, MyWindow, &WindowNameX, &WindowNameX, 0, 0, &SizeHints, &WM_Hints, &C_Hints);
   Attr.save_under       = TRUE;                                            // Andere Windows nicht �berschreiben.
   Attr.backing_store    = Always; //WhenMapped;                            // Unser Window nicht ueberschreiben lassen.
   Attr.event_mask       = ExposureMask | KeyPressMask | ButtonMotionMask   // Das interessiert uns alles.
                         | ButtonPressMask | ButtonReleaseMask
                         | PointerMotionMask;
   AttrMask              = CWBackingStore | CWSaveUnder | CWEventMask;
   err                   = XChangeWindowAttributes(MyDisplay, MyWindow, AttrMask, &Attr);

   Font_Info             = XLoadQueryFont(MyDisplay, "-misc-fixed-medium-r-*-*-12-*-*-*-*-*-*-*");   // -bold- ist noch kleiner
   if (!Font_Info)  printf("Font -misc-fixed-medium-r-*-*-7-*-*-*-*-*-*-* wurde nicht gefunden\n");

   FontSmall_Info        = XLoadQueryFont(MyDisplay, "-misc-fixed-bold-r-*-*-7-*-*-*-*-*-*-*");   // -bold- ist noch kleiner
   if (!Font_Info)  printf("SmallFont -misc-fixed-bold-r-*-*-7-*-*-*-*-*-*-* wurde nicht gefunden\n");

   FontBig_Info          = XLoadQueryFont(MyDisplay, "-adobe-helvetica-bold-r-*-*-24-*-*-*-p-*-iso8859-*");
   if (!Font_Info)  printf("BigFont -adobe-helvetica-bold-r-*-*-24-*-*-*-p-*-iso8859-* wurde nicht gefunden\n");

   // Nun endlich sind wir in der Lage, das Fenster anzuzeigen
   XMapWindow(MyDisplay, MyWindow);

   // Zum Zeichen in das Fenster braucht man einen "Graphical Context" (GC)
   glc                   = glXCreateContext(MyDisplay, vi, NULL, 1);
   glXMakeCurrent(MyDisplay, MyWindow, glc);

/*
   // Zum Zeichen in das Fenster braucht man einen "Graphical Context" (GC)
   Values.cap_style = CapButt;
   MyGC = XCreateGC(MyDisplay, MyWindow, 0, &Values);
   InvGC= XCreateGC(MyDisplay, MyWindow, 0, &Values);
   //XSetFont(MyDisplay, MyGC, Font_Info->fid);
   XSetFont(MyDisplay, MyGC, FontBig_Info->fid);
   XSetLineAttributes(MyDisplay, MyGC, 1, LineSolid, CapRound, JoinRound);   // Width, Style, Cap, Join

   // Ein zweiter Context zum invertierten Zeichnen
   //InvGC = XCopyGC(MyDisplay, MyGC, I);
   XSetLineAttributes(MyDisplay, InvGC, 1, LineSolid, CapRound, JoinRound);   // Width, Style, Cap, Join
   Values.function = GXinvert;
   XChangeGC(MyDisplay, InvGC, GCFunction, &Values);
   XSetForeground(MyDisplay, InvGC, MyBlue);
*/

   // Jetzt warten wir auf ein "Expose", das ist ein Event,
   // der dann eintritt, wenn unser Fenster wirklich dargestellt wurde.
   XFlush(MyDisplay);
   do
      {
      if (!XCheckMaskEvent(MyDisplay, -1, &Report))  continue;
      } while (Report.type != Expose);
   
   //XSynchronize(MyDisplay, 0);      // Das macht unser Programm sehr langsam,
                                      // daf�r findet man die Fehler aber leichter.

   XAllocNamedColor(MyDisplay, DefaultColormapOfScreen(MyScreen), "#9900bb", &Got, &Should);  MyPink = Got.pixel;

   succ = XAllocNamedColor(MyDisplay, DefaultColormapOfScreen(MyScreen), "red",   &Got, &Should);  MyRed   = Got.pixel;  assert(succ);
   succ = XAllocNamedColor(MyDisplay, DefaultColormapOfScreen(MyScreen), "green", &Got, &Should);  MyGreen = Got.pixel;  assert(succ);
   succ = XAllocNamedColor(MyDisplay, DefaultColormapOfScreen(MyScreen), "blue",  &Got, &Should);  MyBlue  = Got.pixel;  assert(succ);
   succ = XAllocNamedColor(MyDisplay, DefaultColormapOfScreen(MyScreen), "black", &Got, &Should);  MyBlack = Got.pixel;  assert(succ);
   succ = XAllocNamedColor(MyDisplay, DefaultColormapOfScreen(MyScreen), "white", &Got, &Should);  MyWhite = Got.pixel;  assert(succ);

#ifdef BLAU_BIS_ROT
   for (ULONG c=0; c<100; ++c)        // Ein toller Farbkeil zur TEMPERATUR-Darstellung
      {
      ULONG F;
      UBYTE R, G, B;
      char  Farbe[128];
      
      F = (ULONG) ( 255.0 * c/100.0 );
      R = F;
      G = (F<128) ? 2*F : 512-2*F;
      B = 255 - F;
      sprintf(Farbe, "#%02x%02x%02x", R, G, B);
      succ = XAllocNamedColor(MyDisplay, DefaultColormapOfScreen(MyScreen), Farbe, &Got, &Should);
      if (succ)
         { TempScala[c] = Got.pixel; }                      // Farbe verf�gbar: alles prima
      else
         { TempScala[c] = (c % 2) ? MyWhite : MyBlack; }    // Farbe nicht verf�gbar: s/w Ersatz w�hlen
      }
#endif
   
   srand(2007);
   for (ULONG c=0; c<100; ++c)        // Ein toller HSV-Farbverlauf mit dem GANZEN Spektrum
      {
      double H = 360.0*c/100.0;
      double v = 255  *  (0.3 * rand()/RAND_MAX + 0.7);
      int    Hi= (int)(H/60);
      double f = H/60.0 - Hi;
      double s = 1; //0.1 * rand()/RAND_MAX + 0.9;
      double p = v * (1 - s);
      double q = v * (1 - s * f);
      double t = v * (1 - s * (1 - f));
      double R, G, B;      
      char   Farbe[128];
      
      switch (Hi)
         {
         case 0:  R=v;  G=t;  B=p;  break;
         case 1:  R=q;  G=v;  B=p;  break;
         case 2:  R=p;  G=v;  B=t;  break;
         case 3:  R=p;  G=q;  B=v;  break;
         case 4:  R=t;  G=p;  B=v;  break;
         case 5:  R=v;  G=p;  B=q;  break;
         }
  
      sprintf(Farbe, "#%02x%02x%02x", (UBYTE)R, (UBYTE)G, (UBYTE)B);
      succ = XAllocNamedColor(MyDisplay, DefaultColormapOfScreen(MyScreen), Farbe, &Got, &Should);
      if (succ)
         { TempScala[c] = Got.pixel; }                      // Farbe verf�gbar: alles prima
      else
         { TempScala[c] = (c % 2) ? MyWhite : MyBlack; }    // Farbe nicht verf�gbar: s/w Ersatz w�hlen
      }
   
   Bgnd = XCreatePixmap(MyDisplay, MyWindow, BGND_WIDTH, BGND_HEIGHT, 24);
   }

void Backup(UWORD x, UWORD y)
   {
   BgndX = x;
   BgndY = y;
   XCopyArea(MyDisplay, MyWindow, Bgnd, MyGC, x, y, BGND_WIDTH, BGND_HEIGHT, 0, 0);
   }

void Restore()
   {
   if (BgndX < 0)  return;
   XCopyArea(MyDisplay, Bgnd, MyWindow, MyGC, 0, 0, BGND_WIDTH, BGND_HEIGHT, BgndX, BgndY);
   BgndX = -1;    // Mehrfach-Restore verhindern
   }

// Inverted painting
void PointInv(UWORD x, UWORD y)
	{
	XDrawPoint(MyDisplay, MyWindow, InvGC, x, y);
	}

void CPointInv(UWORD x, UWORD y, ULONG fg)
	{
	XSetForeground(MyDisplay, InvGC, fg);
	XDrawPoint(MyDisplay, MyWindow, InvGC, x, y);
	}

void LineInv(UWORD xa, UWORD ya, UWORD xe, UWORD ye)
	{
	XDrawLine(MyDisplay, MyWindow, InvGC, xa, ya, xe, ye);
	}

void CLineInv(UWORD xa, UWORD ya, UWORD xe, UWORD ye, UWORD fg)
	{
	XSetForeground(MyDisplay, InvGC, fg);
	XDrawLine(MyDisplay, MyWindow, InvGC, xa, ya, xe, ye);
	}

void LinesInv(UWORD *coords, UWORD n)
	{
	XDrawLines(MyDisplay, MyWindow, InvGC, (XPoint*)coords, n, CoordModePrevious);
	}

void RectInv(UWORD x, UWORD y, UWORD b, UWORD h)
   {
   XDrawRectangle(MyDisplay, MyWindow, InvGC, x, y, b, h);
   }

void BlockInv(UWORD x, UWORD y, UWORD b, UWORD h)
	{
	XFillRectangle(MyDisplay, MyWindow, InvGC, x, y, b, h);
	}

void CBlockInv(UWORD x, UWORD y, UWORD b, UWORD h, ULONG fg)
	{
	XSetForeground(MyDisplay, InvGC, fg);
	XFillRectangle(MyDisplay, MyWindow, InvGC, x, y, b, h);
	}

void ArcInv(UWORD x, UWORD y, UWORD b, UWORD h, WORD from, WORD to)
	{
	XDrawArc(MyDisplay, MyWindow, InvGC, x, y, b, h, from, to);
	}

void CircleInv(UWORD x, UWORD y, UWORD r)
	{
	ArcInv(x, y, r, r, 0, 360*64);
	}

void TextInv(UWORD x, UWORD y, char *txt)
	{
	XDrawImageString(MyDisplay, MyWindow, InvGC, x, y, txt, strlen(txt));
	}

void ColorInv(ULONG fg, ULONG bg)
	{
	XSetForeground(MyDisplay, InvGC, fg);
	XSetBackground(MyDisplay, InvGC, bg);
	}

// Normal painting
void CPoint(UWORD x, UWORD y, ULONG fg)
   {
   XSetForeground(MyDisplay, MyGC, fg);
   XDrawPoint(MyDisplay, MyWindow, MyGC, x, y);
   }

void Line(UWORD xa, UWORD ya, UWORD xe, UWORD ye)
   {
   XDrawLine(MyDisplay, MyWindow, MyGC, xa, ya, xe, ye);
   }

void CLine(UWORD xa, UWORD ya, UWORD xe, UWORD ye, UWORD fg)
	{
	XSetForeground(MyDisplay, MyGC, fg);
	XDrawLine(MyDisplay, MyWindow, MyGC, xa, ya, xe, ye);
	}

void Lines(UWORD *coords, UWORD n)
   {
   XDrawLines(MyDisplay, MyWindow, MyGC, (XPoint*)coords, n, CoordModePrevious);
   }

void Rect(UWORD x, UWORD y, UWORD b, UWORD h)
   {
   XDrawRectangle(MyDisplay, MyWindow, MyGC, x, y, b, h);
   }

void Block(UWORD x, UWORD y, UWORD b, UWORD h)
   {
   XFillRectangle(MyDisplay, MyWindow, MyGC, x, y, b, h);
   }

void CBlock(UWORD x, UWORD y, UWORD b, UWORD h, ULONG fg)
   {
   XSetForeground(MyDisplay, MyGC, fg);
   XFillRectangle(MyDisplay, MyWindow, MyGC, x, y, b, h);
   }

void Arc(UWORD x, UWORD y, UWORD b, UWORD h, WORD from, WORD to)
   {
   XDrawArc(MyDisplay, MyWindow, MyGC, x, y, b, h, from, to);
   }

void Circle(UWORD x, UWORD y, UWORD r)
   {
   Arc(x, y, r, r, 0, 360*64);
   }

void Text(UWORD x, UWORD y, char *txt)
   {
   XDrawImageString(MyDisplay, MyWindow, MyGC, x, y, txt, strlen(txt));
   }

void Color(ULONG fg, ULONG bg)
   {
   XSetForeground(MyDisplay, MyGC, fg);
   XSetBackground(MyDisplay, MyGC, bg);
   }

void Clear()
   {
   CBlock(0, 0, XSIZE, YSIZE, MyBlack);
   }

void TestEvents()
   {
   while (TRUE==XCheckMaskEvent(MyDisplay, -1, &Report))
      {
      switch(Report.type)
         {
         case KeyPress:
            {
            KeySym TastenSym;
            char   B[256];
            int    N;
            
            N = XLookupString( (XKeyEvent*)&Report, B, 256, &TastenSym, NULL);
            B[N] = 0;
            
            switch(TastenSym)
               {
            	case XK_space:
            		extern bool shouldManualRotate;
            		if (shouldManualRotate)
            			shouldManualRotate = false;
            		else
            			shouldManualRotate = true;
            		break;

            	case XK_R:
            	case XK_r:
            		break;

               case XK_KP_Subtract:
               case XK_minus:
               	break;

               case XK_KP_Add:
               case XK_plus:
               	break;

               case XK_Escape:
               case XK_KP_Enter:
               case XK_Pause:
               case XK_Return:
               	CBlock(0,0,XSIZE,YSIZE,MyPink);
               	XFlush(MyDisplay);
               	exit(0);
               	break;

               case XK_Up:
               	break;
               case XK_Down:
               	break;

               case XK_Right:
               case XK_j:
               	break;

               case XK_Left:
               case XK_J:
               	break;

               case XK_KP_Divide:
               case XK_l:
               	break;

               case XK_m:
               	break;

               case XK_End:
               	break;

               case 0x73:
               	extern bool shouldManualRotate;
               	extern double Alpha;
               	extern double angleOffset;

               	if (shouldManualRotate)
               		Alpha -= angleOffset;
               	break;

               case 0x77:
               	extern bool shouldManualRotate;
               	extern double Alpha;
               	extern double angleOffset;

               	if (shouldManualRotate)
               		Alpha += angleOffset;
               	break;

               case 0x61:
               	extern bool shouldManualRotate;
               	extern double Beta;
               	extern double angleOffset;

               	if (shouldManualRotate)
               		Beta += angleOffset;
               	break;

               case 0x64:
               	extern bool shouldManualRotate;
               	extern double Beta;
               	extern double angleOffset;

               	if (shouldManualRotate)
               		Beta -= angleOffset;
               	break;

               case 0x71:
               	extern bool shouldManualRotate;
               	extern double Gamma;
               	extern double angleOffset;

               	if (shouldManualRotate)
               		Gamma -= angleOffset;
               	break;

               case 0x65:
               	extern bool shouldManualRotate;
               	extern double Gamma;
               	extern double angleOffset;

               	if (shouldManualRotate)
               		Gamma += angleOffset;
               	break;

               case XK_Page_Down:
               case XK_3:
               	break;

               case XK_KP_Multiply:
               case XK_BackSpace:
               case XK_Delete:
               default:
               	printf("Unknown keysym press $%x (%s)\n", TastenSym, B);
               	break;
               }
            break;
            }
         case KeyRelease:
            {
            break;
            }
         case ButtonPress:
            {
            XButtonEvent *e = (XButtonEvent*) &Report;
            Restore();
            break;
            }
         case ButtonRelease:
         	break;

         case Expose:
            {
            while (XCheckMaskEvent(MyDisplay, -1, &Report));   // Weitere Events ueberspringen
            RedrawCallback();
            break;
            }
         case NoExpose:      break;                            // Das sind irgendwelche Focus-Events, die uns zur Zeit nicht interessieren
         case MotionNotify:
            {
            XButtonEvent *e = (XButtonEvent*) &Report;
            break;
            }
         default:            printf("Unknown type of report %d\n", Report.type); break;
         }
      }   // if event
   }


