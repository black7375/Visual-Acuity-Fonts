/*
 * snellen.h
 *
 *  Created on: Nov 2, 2008
 *      Author: Andrew Howlett, Daniel Eriksen
 *   Last Edit: May 15, 2009
 */

#ifndef SNELLEN_H_
#define SNELLEN_H_

#include <directfb.h>


/*
 *  The ten snellen octotype letters: C D E F L N O P T Z
 *  The ten sloan octotype glyphs: C D H K N O R S V Z
 *
 *  Note: we will start numbering the snellen letters at 1, and
 *  character 0 will be a blank or space.
 */

char glyphs[3][17] = {"           00000", " CDEFLNOPTZAVHBG"," CDHKNORSVZ00000"}; 
char glyphsEnumToChar[3][17]={"           00000", "0CDEFLNOPTZAVHBG","0CDHKNORSVZ00000"};

enum snellenGlyphsEnum { c=1,d,e,f,l,n,o,p,t,z,a,v,h,b,g } ;
enum sloanGlyphsEnum { C=1,D,H,K,N,O,R,S,V,Z };


/*
 * Screen height in mm as measured by a ruler.
 * The profile-3 measures 230mm in height
 *
 *  US Standard distance is 4m
 */
int screen_height_mm = 250;
int distance_to_patient = 4000;

/*
 *   The StateType stores the state of the finite state machine. The state
 *   can be fully described by the size of the text being displayed, the number
 *   of letters being displayed, the letters being displayed, and which
 *   letter (numbered starting at 0 from the left) is displayed first.
 */

typedef struct {
	int active;                   /*  1 means the mask is in use */
	int numLettersDisplayed;
	int firstLetterDisplayed;
	int topX;
	int topY;
	int height;
	int width;
} MaskType;

MaskType noMask = { 0, 0, 0, 0, 0, 0, 0};

typedef struct {
	int size;
	char name[4];
	float mar; /* Minimum Angle of Resolution - x5=Minutes Of Arc - x20=line*/
	char letters[11]; /* maximum of 11 characters in a line */
	int numLetters;
	int topX;
	int topY;
	float blockSize;
} LineType;

/* these are the default lines
 * first (0) block is for primitives (could have left it empty, but cut and paste
 *                                is easier)
 * second (1) block is for snellen
 * third  (2) block is for sloan
 */

LineType defaultLine[3][15] = {

	{
		{ 0, "   ", 0   ,         { },  0, 0, 0, 0 }, /*   null line for size zero */
		{ 1, "200", 10  ,         {e},  1, 0, 0, 0 }, // 200
		{ 2, "160", 8   ,       {a,p},  2, 0, 0, 0 }, // 160
		{ 3, "125", 6.25,       {d,o},  2, 0, 0, 0 }, // 125
		{ 4, "100", 5   ,     {p,h,t},  3, 0, 0, 0 }, // 100
		{ 5, "80" , 4   ,   {v,l,n,e},  4, 0, 0, 0 }, //  80
		{ 6, "60" , 3   ,   {d,a,o,f},  4, 0, 0, 0 }, //  60
		{ 7, "50" , 2.5 , {e,g,n,d,h},  5, 0, 0, 0 }, //  50
		{ 8, "40" , 2   , {f,z,b,d,e},  5, 0, 0, 0 }, //  40
		{ 9, "30" , 1.5 , {o,f,l,c,t},  5, 0, 0, 0 }, //  30
		{10, "25" , 1.25, {a,p,e,o,t},  5, 0, 0, 0 }, //  25
		{11, "20" , 1   , {t,z,v,o,a},  5, 0, 0, 0 }, //  20
		{12, "15" , 0.75, {o,h,p,n,t},  5, 0, 0, 0 }, //  15
		{13, "12" , 0.6,  {t,z,v,o,a},  5, 0, 0, 0 }, //  12  // not on typical chart
		{14, "10" , 0.5,  {o,h,p,n,t},  5, 0, 0, 0 }  //  10  // not on typical chart
	},

	{
		{ 0, "   ", 0   ,         { },  0, 0, 0, 0 }, /*   null line for size zero */
		{ 1, "200", 10  ,         {e},  1, 0, 0, 0 }, // 200
		{ 2, "160", 8   ,       {a,p},  2, 0, 0, 0 }, // 160
		{ 3, "125", 6.25,       {d,o},  2, 0, 0, 0 }, // 125
		{ 4, "100", 5   ,     {p,h,t},  3, 0, 0, 0 }, // 100
		{ 5, "80" , 4   ,   {v,l,n,e},  4, 0, 0, 0 }, //  80
		{ 6, "60" , 3   ,   {d,a,o,f},  4, 0, 0, 0 }, //  60
		{ 7, "50" , 2.5 , {e,g,n,d,h},  5, 0, 0, 0 }, //  50
		{ 8, "40" , 2   , {f,z,b,d,e},  5, 0, 0, 0 }, //  40
		{ 9, "30" , 1.5 , {o,f,l,c,t},  5, 0, 0, 0 }, //  30
		{10, "25" , 1.25, {a,p,e,o,t},  5, 0, 0, 0 }, //  25
		{11, "20" , 1   , {t,z,v,o,a},  5, 0, 0, 0 }, //  20
		{12, "15" , 0.75, {o,h,p,n,t},  5, 0, 0, 0 }, //  15
		{13, "12" , 0.6,  {t,z,v,o,a},  5, 0, 0, 0 }, //  12  // not on typical chart
		{14, "10" , 0.5,  {o,h,p,n,t},  5, 0, 0, 0 }  //  10  // not on typical chart
	},

  {
		{ 0, "   ", 0   ,         { },  0, 0, 0, 0 }, /*   null line for size zero */
		{ 1, "1.0", 10  ,   {O,H,R,Z,K},  5, 0, 0, 0 }, // 1.0
		{ 2, "0.9", 8   ,   {Z,V,O,H,R},  5, 0, 0, 0 }, //  .9
		{ 3, "0.8", 6.25,   {C,O,H,S,K},  5, 0, 0, 0 }, //  .8
		{ 4, "0.7", 5   ,   {O,V,N,C,Z},  5, 0, 0, 0 }, //  .7
		{ 5, "0.6" , 4   ,  {V,H,N,D,O},  5, 0, 0, 0 }, //  .6
		{ 6, "0.5" , 3.15,  {S,R,O,K,D},  5, 0, 0, 0 }, //  .5
		{ 7, "0.4" , 2.5 ,  {N,K,C,D,V},  5, 0, 0, 0 }, //  .4
		{ 8, "0.3" , 2   ,  {S,N,C,H,D},  5, 0, 0, 0 }, //  .3
		{ 9, "0.2" , 1.6 ,  {V,O,K,R,N},  5, 0, 0, 0 }, //  .2
		{10, "0.1" , 1.25,  {K,O,N,C,H},  5, 0, 0, 0 }, //  .1
		{11, "0.0" , 1   ,  {C,V,H,D,S},  5, 0, 0, 0 }, // 0.0
		{12, "-.1" , 0.8,   {O,Z,S,K,V},  5, 0, 0, 0 }, // -.1
		{13, "-.2" , 0.625, {D,H,K,Z,R},  5, 0, 0, 0 }, // -.2
		{14, "-.3" , 0.5,   {R,N,Z,O,S},  5, 0, 0, 0 }  // -.3
  }
  };

typedef struct {
	LineType line;
	MaskType mask;
	int randomModeOn;
	int truetypeMode;  //  0 for primitives, 1 for snellen, 2 for sloan
  int mirrorModeOn;
} StateType;


/*
 * this type helps 
 * with the set height and distance
 * menu
 */

typedef struct {
	int topX;
	int topY;
	int width;
	int height;
} MenuSpecsType;



/*
 *   function prototypes
 */

void InitializeDirectFB(int argc, char **argv);
void CloseDirectFB();
void EventLoop();
DFBInputDeviceKeyIdentifier GetKeyPress ();
void PaintRowOfLetters ();

float CalculateLineHeight ();
void CalculateTopXTopYBlockSize ();
void PaintLetter (char letter, int topX, int topY, float blockSize);

void TruetypeCreateFont (float blockSize);
void TruetypeToggleMode();
void TruetypePrintLetter(int letter, int topX, int topY, float blockSize);

void CreateMask (int maskSize);
void RemoveMask ();
void ApplyMask();
void MoveMaskLeft();
void MoveMaskRight();
void MoveMaskRender(int topX, int topY, int width, int height);

void MirrorModeToggle();
void ToggleRandomMode ();
void CreateRandomLine();
int  MyRandom(int N);

void HelpShowCurrentState();
void SleepQuarterSecond();
void Screensaver();
void Help();
void WaitForEventWithScreensaver(IDirectFBEventBuffer* eventBuffer);
void WaitForEventWithScreensaverNoDebounce(IDirectFBEventBuffer* eventBuffer);

void ReadConfiguration();
void WriteConfiguration();

void ChangeDistanceAndScreenHeight();
void DrawDistanceOrHeightSurface(MenuSpecsType *menuSpecs);
void DrawBoxAroundChoice(MenuSpecsType menuSpecs, int choice);
void SetDistance();
void SetHeight();



#endif /* SNELLEN_H_ */
