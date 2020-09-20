/*
 ============================================================================
 Name        : snellen.c
 Author      : Andrew Howlett, Daniel Eriksen
 Version     : 1.0
 Copyright   : (c) Andrew Howlett  2010 & 2014
 License     : GPL 3
 Description : This is the second version of directfb.
               Added support for sloan optotype
 ============================================================================
 */

/*
	Dan's list of to-be-implemented

	- add support for metric and more lines
	- fix yellow
	- initial help screen graphic needs to be updated

 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <directfb.h>
#include <math.h>
#include <time.h>
#include <libconfig.h>

#include "snellen.h"

/*
 *    All state information is stored in currentState
 */

StateType *currentState = NULL;

/*
 *    Create and initilize input device pointers
 */

static IDirectFBInputDevice *keyboard = NULL;
static IDirectFBEventBuffer *eventBuffer = NULL;

//	We need font support to display information on-screen

static DFBFontDescription font_dsc;
static IDirectFBFont *font = NULL;

//	This is the super interface, it's the entry point to all functionality.

static IDirectFB *dfb = NULL;

/*
 *   	The surfaces, i.e. the "screen".
 *      In cooperative level DFSCL_FULLSCREEN it's the surface of the surface1 layer.
 */

static IDirectFBSurface *letterSurface = NULL;
static IDirectFBSurface *displaySurface = NULL;
static IDirectFBSurface *maskSurface = NULL;
static IDirectFBSurface *helpSurface = NULL;

//      Snellen font

static IDirectFBFont *fontTruetype = NULL;

//	Store the width and height of the surface1 surface here to support all resolutions.

static int screenWidth  = 0;
static int screenHeight = 0;

struct config_t configuration;

int main (int argc, char **argv)  {

	InitializeDirectFB(argc, argv);
	ReadConfiguration();

	printf("Starting event loop\n");
	EventLoop();

	WriteConfiguration();
	CloseDirectFB();

  return 0;
}

void InitializeDirectFB(int argc, char **argv) {

	/*
	 *   Initialize DirectFB, create the interface to the DirectFB library
	 *
	 *   We want to run snellen in fullscreen, with one primary surface
	 */

	printf("Initializing FB\n");
	DirectFBInit (&argc, &argv);
	DirectFBCreate (&dfb);

	dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN);

	/*
	 *   Create the Surfaces
	 *
	 *   This app uses four surfaces:
	 *
	 *   displaySurface  - corresponds to the computer screen
	 *                   - built by blitting the other surfaces to this surface
	 *                   - flippable
	 *                   - the primary surface.
	 *   letterSurface   - contains the letters of each line, black on white background
	 *                   - not flippable.
	 *   maskSurface     - contains the masks
	 *                   - not flippable
	 *   helpSurface     - contains help text
	 *                   - not flippable
	 */

	printf("Creating display surface\n");
	DFBSurfaceDescription surfaceDescription;
	surfaceDescription.flags = DSDESC_CAPS;
	surfaceDescription.caps  = DSCAPS_PRIMARY | DSCAPS_FLIPPING;
	dfb->CreateSurface( dfb, &surfaceDescription, &displaySurface );
	displaySurface->GetSize (displaySurface, &screenWidth, &screenHeight);

	printf("Creating letter surface\n");
	surfaceDescription.flags       = DSDESC_CAPS | DSDESC_PIXELFORMAT | DSDESC_WIDTH | DSDESC_HEIGHT;
	surfaceDescription.caps        = DSCAPS_PREMULTIPLIED;
	surfaceDescription.pixelformat = DSPF_ARGB;
	surfaceDescription.width       = screenWidth;
	surfaceDescription.height      = screenHeight;
	dfb->CreateSurface( dfb, &surfaceDescription, &letterSurface );

	printf("Creating mask surface\n");
	surfaceDescription.flags       = DSDESC_CAPS | DSDESC_PIXELFORMAT | DSDESC_WIDTH | DSDESC_HEIGHT;
	surfaceDescription.caps        = DSCAPS_PREMULTIPLIED;
	surfaceDescription.pixelformat = DSPF_ARGB;
	surfaceDescription.width       = screenWidth;
	surfaceDescription.height      = screenHeight;
  dfb->CreateSurface( dfb, &surfaceDescription, &maskSurface );

	printf("Creating help surface\n");
	surfaceDescription.flags       = DSDESC_CAPS | DSDESC_PIXELFORMAT | DSDESC_WIDTH | DSDESC_HEIGHT;
	surfaceDescription.caps        = DSCAPS_PREMULTIPLIED;
	surfaceDescription.pixelformat = DSPF_ARGB;
	surfaceDescription.width       = screenWidth;
	surfaceDescription.height      = screenHeight;
	dfb->CreateSurface( dfb, &surfaceDescription, &helpSurface );

	/*
	 *  Initialize the input devices and event buffer
	 */

	dfb->GetInputDevice (dfb, DIDID_KEYBOARD, &keyboard);
	dfb->CreateInputEventBuffer (dfb, DICAPS_ALL, DFB_TRUE, &eventBuffer);

	/*
	 * First we need to create a font interface by passing a filename
	 * and a font description to specify the desired font size. DirectFB will
	 * find (or not) a suitable font loader.
	 */
	font_dsc.flags = DFDESC_HEIGHT;
	font_dsc.height = 36;
	dfb->CreateFont (dfb, "../fonts/LiberationSans-Regular.ttf", &font_dsc, &font);

	/*
	 * Set the font to the surface we want to draw to.
	 */
	displaySurface->SetFont (displaySurface, font);

}

void CloseDirectFB() {   	//	Cleanup in a stack like style.
	letterSurface->Release( letterSurface );
	maskSurface->Release(maskSurface);
	displaySurface->Release(displaySurface);
	helpSurface->Release(helpSurface);
	font->Release(font);
	dfb->Release( dfb );
}

void line(int line) {
	if (currentState->line.size==line) currentState->randomModeOn=1;
	else currentState->randomModeOn=0;
	currentState->line.size=line;
	RemoveMask();
	PaintRowOfLetters();
}


void EventLoop() {
	currentState = (StateType *) malloc(sizeof(StateType));
	currentState->truetypeMode = 2;
	currentState->line = defaultLine[2][1];   /* initialize to truetype snellen line 1 */
	currentState->mask = noMask;                    /* initialize to no mask   */
	currentState->randomModeOn = 0;
	currentState->mirrorModeOn = 1;

	/*
	 *   start with help screen
	 */

	Help();
	PaintRowOfLetters();

	DFBInputEvent event;
	int endLoop = 0;
	while(!endLoop) {
		WaitForEventWithScreensaver(eventBuffer);
		eventBuffer->GetEvent(eventBuffer, DFB_EVENT(&event));
		switch (event.key_symbol) {
		case DIKS_SMALL_C:
		case DIKS_GREEN:
			ChangeDistanceAndScreenHeight(); 
			PaintRowOfLetters();
			break;
		case DIKS_SMALL_H:
		case DIKS_BLUE:
			Help(); break;
		case DIKS_SMALL_I:
			HelpShowCurrentState(); 
			PaintRowOfLetters();
			break;
		case DIKS_SMALL_M:
		case DIKS_YELLOW:
		  MirrorModeToggle(); PaintRowOfLetters(); break;
		case DIKS_SMALL_R:
		case DIKS_RED:
			ToggleRandomMode(); PaintRowOfLetters(); break;
		case DIKS_SMALL_S:
			Screensaver(); break;
		case DIKS_SMALL_T:
			TruetypeToggleMode(); PaintRowOfLetters(); break;
		case DIKS_CURSOR_UP:
			if (currentState->line.size == 1) break;
			currentState->randomModeOn=0;
			currentState->line.size--;
			RemoveMask();
			PaintRowOfLetters();
			break;
		case DIKS_CURSOR_DOWN:
			if (currentState->line.size == 14) break;
			currentState->randomModeOn=0;
			currentState->line.size ++;
			RemoveMask();
			PaintRowOfLetters();
			break;
		case DIKS_CURSOR_LEFT:
			currentState->randomModeOn=1; PaintRowOfLetters();break;
		case DIKS_CURSOR_RIGHT:
			currentState->randomModeOn=1; PaintRowOfLetters();break;
		case DIKS_MUTE:
			line(1); break;
		case DIKS_0:
			line(8); break;
		case DIKS_1:
			line(9); break;
		case DIKS_2:
			line(5); break;
		case DIKS_3:
			line(2); break;
		case DIKS_4:
			line(10); break;
		case DIKS_5:
			line(6); break;
		case DIKS_6:
			line(3); break;
		case DIKS_7:
			line(11); break;
		case DIKS_8:
			line(7); break;
		case DIKS_9:
			line(4); break;
		case DIKS_ESCAPE:
			endLoop = 1;
			break;
		default:
		        break;
		}
		/*
		 *   have to implement some primitive keyboard debouncing
		 */
		SleepQuarterSecond();
		eventBuffer->Reset(eventBuffer);
	}
}


void PaintRowOfLetters () {

	/*
	 *   set the line, if we are in random mode, randomize the line
	 */

	currentState->line = defaultLine[currentState->truetypeMode][currentState->line.size];
	if (currentState->randomModeOn)	CreateRandomLine();

	/*
	 *   calculate important positioning variables
	 */

	CalculateTopXTopYBlockSize();
	int topX = currentState->line.topX;
	int topY = currentState->line.topY;
	float blockSize = currentState->line.blockSize;
	printf("PaintRow %d,%d %f\n",topX,topY,blockSize);

	/*
	 *   Clear the letter surface
	 */

	letterSurface->Clear (letterSurface, 0xff, 0xff, 0xff, 0xff);

	if (currentState->mirrorModeOn == 1) {
	  topX += blockSize * 8 * (currentState->line.numLetters - 1) ;
	  int i;
	  for (i=0; i<currentState->line.numLetters; i++) {
	    printf("Painting letter %d, %d with a blocksize of %f pixels\n",i, currentState->line.letters[i],blockSize);
	    PaintLetter(currentState->line.letters[i], topX, topY, blockSize);
	    topX -= blockSize*8;
	  }
	} else {
	  int i;
	  for (i=0; i<currentState->line.numLetters; i++) {
	    printf("Painting letter %d, %d with a blocksize of %f pixels\n",i, currentState->line.letters[i],blockSize);
	    PaintLetter(currentState->line.letters[i], topX, topY, blockSize);
	    topX += blockSize*8;
	  }
	}

	printf("blitting letterSurface\n");
	displaySurface->SetBlittingFlags(displaySurface, DSBLIT_NOFX);
	displaySurface->Blit (displaySurface, letterSurface, NULL, 0,0);
	printf("blitting maskSurface\n");
	ApplyMask();

	/*
	 * Determine the size of our string when drawn using the loaded font.
	 * Since we are interested in the full string, we pass -1 as string length.
	 */
	displaySurface->DrawString(displaySurface, currentState->line.name , -1, 20, screenHeight - 20, DSTF_LEFT);
	displaySurface->Flip(displaySurface, NULL, DSFLIP_WAITFORSYNC);
}

/* This function returns pixel sizes for the requested line */
float CalculateLineHeight () {
	float x;
	float pixels_per_mm = (float)screenHeight / (float)screen_height_mm;

	// Tan(line factor) x (dist in mm) x (pixels per mm) = (pixels in height for this line)
	x = currentState->line.mar * 5.0 / 60.0;            // Line factor multiplied by 5 minutes of arc
	x = distance_to_patient * tan(x / 57.29578) ;       // Tan returns radians, converting to degrees
	//	x = x * distance_to_patient;         // How far away is the patient?
	printf("Attempting to draw letters %f mm in height\n",x);
	x = x * pixels_per_mm;               // convert mm in height to pixels for this particular display
	printf("We need to draw line %f pixels high, %d / %d = %f \n",x,screenHeight,screen_height_mm,pixels_per_mm);
	return x;
}

void CalculateTopXTopYBlockSize () {
	float LetterHeight = CalculateLineHeight();
	float blockSize = LetterHeight / 5.0;
	printf("LetterHeight is %f blockSize is %f\n", LetterHeight, blockSize);
	int topY = (screenHeight - LetterHeight)/2;
	int LettersWidthWithSpaces = currentState->line.numLetters*LetterHeight + (currentState->line.numLetters - 1)*3*blockSize;
	int topX = (screenWidth - LettersWidthWithSpaces ) / 2;

	currentState->line.topX = topX;
	currentState->line.topY = topY;
	currentState->line.blockSize = blockSize;
}

/*
 *   Originally there was a lot of code to draw snellen letters
 *   using directFB primitives (ie circles and rectangles.
 *   But the truetype rendering works much better
 *   so the original primitive drawing mode has been
 *   removed.
 *
 */

void PaintLetter (char letter, int topX, int topY, float blockSize) {

	printf("PaintLetter is %i,%i %f\n",topX,topY,blockSize);

	TruetypePrintLetter(letter, topX, topY, blockSize);
}


/*
 *   Masking functions
 */

void CreateMask(int maskSize) {

	printf("creating mask\n");

	/* set the maskSize and position */

	if (maskSize > currentState->line.numLetters) return; /* can't create a mask bigger than the line */
	int first = (currentState->line.numLetters - maskSize)/2 + 1;
	currentState->mask.firstLetterDisplayed = first;
	currentState->mask.numLettersDisplayed = maskSize;

	/* make the mask black */

	maskSurface->Clear(maskSurface, 0, 0, 0, 0xff);

	/* calculate the size of the hole */

	float blockSize = currentState->line.blockSize;
	int topX=currentState->line.topX - blockSize + blockSize*8 * (currentState->mask.firstLetterDisplayed - 1);
	if (topX<0) topX=0;
	int topY=currentState->line.topY - blockSize;
	if (topY<0) topY = 0;
	int width = blockSize*5*currentState->mask.numLettersDisplayed + blockSize*3*(currentState->mask.numLettersDisplayed-1) + blockSize *2;
	int height = blockSize*7;

	/* create a white hole in the center of the mask */

	maskSurface->SetColor (maskSurface, 0xff, 0xff, 0xff, 0x00);
	maskSurface->FillRectangle(maskSurface, topX, topY, width, height);

	/* save mask state */

	currentState->mask.active = 1;
	currentState->mask.topX = topX;
	currentState->mask.topY = topY;
	currentState->mask.width = width;
	currentState->mask.height = height;
}

void RemoveMask () {
	currentState->mask = noMask;
	/* copy the letterSurface */
	displaySurface->SetBlittingFlags(displaySurface, DSBLIT_NOFX);
	displaySurface->Blit (displaySurface, letterSurface, NULL, 0,0);
	/* flip the surface on the next sync */
	displaySurface->Flip(displaySurface, NULL, DSFLIP_ONSYNC);
}

void ApplyMask() {
	if (currentState->mask.active == 0) return;
	printf("applying mask\n");

	maskSurface->SetSrcColorKey(maskSurface, 0xff, 0xff, 0xff);
	displaySurface->SetBlittingFlags(displaySurface, DSBLIT_SRC_COLORKEY);
	displaySurface->Blit (displaySurface, maskSurface, NULL, 0,0);
	displaySurface->SetBlittingFlags(displaySurface, DSBLIT_NOFX);  /* turn source colorkey off! */
}

void MoveMaskLeft() {

	if (currentState->mask.active == 0) return;

	/* get mask state */

	int numLettersDisplayed = currentState->mask.numLettersDisplayed;
	int firstLetterDisplayed = currentState->mask.firstLetterDisplayed;
	int topX = currentState->mask.topX;
	int topY = currentState->mask.topY;
	int width = currentState->mask.width;
	int height = currentState->mask.height;
	float blockSize = currentState->line.blockSize;

	printf("moving mask left %d\n", firstLetterDisplayed);

	if ( (firstLetterDisplayed + numLettersDisplayed - 1) == 1) return;    /* sorry, it's already all the way to the left */


	int newTopX = topX - blockSize * 8;

	/*
	 *   The move is an animation. We will do the animation at a rate of
	 *   100 frames per second. We will assume a pixel density of 100 and we
	 *   want to move the frame one inch per second.
	 */

	int pixelsPerFrame = 10;
	for ( ; topX>=newTopX; topX -= pixelsPerFrame) {
		MoveMaskRender(topX, topY, width, height);
	}
	MoveMaskRender(newTopX, topY, width, height);

	currentState->mask.topX = newTopX;
	currentState->mask.firstLetterDisplayed = firstLetterDisplayed - 1;
}

void MoveMaskRight() {

	if (currentState->mask.active == 0) return;

	/* get mask state */

	//int numLettersDisplayed = currentState->mask.numLettersDisplayed;
	int firstLetterDisplayed = currentState->mask.firstLetterDisplayed;
	int topX = currentState->mask.topX;
	int topY = currentState->mask.topY;
	int width = currentState->mask.width;
	int height = currentState->mask.height;
	float blockSize = currentState->line.blockSize;

	printf("moving mask left %d\n", firstLetterDisplayed);

	if (firstLetterDisplayed == currentState->line.numLetters) return;    /* sorry, it's already all the way to the right */

	int newTopX = topX + blockSize * 8;

	int pixelsPerFrame = 10;
	for ( ; topX<=newTopX; topX += pixelsPerFrame) {
		MoveMaskRender(topX, topY, width, height);
	}
	MoveMaskRender(newTopX, topY, width, height);

	currentState->mask.topX = newTopX;
	currentState->mask.firstLetterDisplayed ++;

}

void MoveMaskRender (int topX, int topY, int width, int height) {
	/* build the mask   */
	maskSurface->Clear(maskSurface, 0, 0, 0, 0xff);
	maskSurface->SetColor (maskSurface, 0xff, 0xff, 0xff, 0x00);
	maskSurface->FillRectangle(maskSurface, topX, topY, width, height);
	/* copy the letterSurface */
	displaySurface->SetBlittingFlags(displaySurface, DSBLIT_NOFX);
	displaySurface->Blit (displaySurface, letterSurface, NULL, 0,0);
	/* apply the mask */
	maskSurface->SetSrcColorKey(maskSurface, 0xff, 0xff, 0xff);
	displaySurface->SetBlittingFlags(displaySurface, DSBLIT_SRC_COLORKEY);
	displaySurface->Blit (displaySurface, maskSurface, NULL, 0,0);
	displaySurface->SetBlittingFlags(displaySurface, DSBLIT_NOFX);  /* turn source colorkey off! */
	/* flip the surface on the next sync */
	displaySurface->Flip(displaySurface, NULL, DSFLIP_ONSYNC);
}

void ToggleRandomMode () {
	if (currentState->randomModeOn == 1) currentState->randomModeOn = 0 ; else currentState->randomModeOn=1;
}

void CreateRandomLine () {
	int i,r;
	for (i=0; i<currentState->line.numLetters; i++) {
		currentState->line.letters[i]=MyRandom(10) + 1;
		for (r=0; r<i; r++) {
			if (currentState->line.letters[i]==currentState->line.letters[r]) {
				// letter in use, let's try again for something else
				r=i; i--;
			}
		}
	}
}

/*
 * MyRandom(int) returns an integer x , 0 <= x < N
 */

int MyRandom (int N) {
	int returnValue;
	while (N <= (returnValue = rand() / (  ((double) RAND_MAX) /N ) ) );
	return returnValue;
}

void HelpShowCurrentState() {
	static IDirectFBFont *font = NULL;
	DFBFontDescription font_dsc;
	int width, height, topX, topY;
	char sizeString[32];

	font_dsc.flags = DFDESC_HEIGHT;
	font_dsc.height = 32;
	dfb->CreateFont (dfb, "../fonts/LiberationSans-Regular.ttf", &font_dsc, &font);

	helpSurface->SetFont (helpSurface, font);
	helpSurface->Clear (helpSurface, 0, 0, 0, 0);
	helpSurface->SetColor (helpSurface, 0xa0, 0xa0, 0xff, 0xFF);

	topX = 100; topY = 25;
	font->GetHeight(font, &height); height*=1.5;
	font->GetStringWidth(font, "Current Settings", -1, &width);
	helpSurface->DrawString (helpSurface, "Current Settings", -1, 	(screenWidth-width)/2, topY+=height, DSTF_LEFT);
	helpSurface->FillRectangle(helpSurface, (screenWidth-width)/2,topY,width,2);
	topY+=height;

	if (currentState->truetypeMode == 1) {
		font->GetHeight(font, &height);
		helpSurface->DrawString (helpSurface, "Optotype: Snellen", -1, topX, topY+=height, DSTF_LEFT);
	}
	if (currentState->truetypeMode == 2) {
		font->GetHeight(font, &height);
		helpSurface->DrawString (helpSurface, "Optotype: Sloan", -1, topX, topY+=height, DSTF_LEFT);
	}

	helpSurface->DrawString (helpSurface, "Size: ", -1, topX, topY+=height, DSTF_LEFT);
	snprintf(sizeString, 32, "%s", currentState->line.name);
	font->GetStringWidth(font, "Size: ", -1, &width);
	helpSurface->DrawString (helpSurface, sizeString, -1, topX+width, topY, DSTF_LEFT);

	if (currentState->randomModeOn) {
		helpSurface->DrawString (helpSurface, "Random: On", -1, topX, topY+=height, DSTF_LEFT);
	} else {
		helpSurface->DrawString (helpSurface, "Random: Off", -1, topX, topY+=height, DSTF_LEFT);
	}
	if (currentState->mask.active) {
		helpSurface->DrawString (helpSurface, "Mask: On", -1, topX, topY+=height, DSTF_LEFT);
	} else {
		helpSurface->DrawString (helpSurface, "Mask: Off", -1, topX, topY+=height, DSTF_LEFT);
	}

	helpSurface->DrawString (helpSurface, "Distance to patient: ", -1, topX, topY+=height, DSTF_LEFT);
	snprintf(sizeString, 32, "%d mm", distance_to_patient);
	font->GetStringWidth(font, "Distance to patient: ", -1, &width);
	helpSurface->DrawString (helpSurface, sizeString, -1, topX+width, topY, DSTF_LEFT);
	
	helpSurface->DrawString (helpSurface, "Screen height: ", -1, topX, topY+=height, DSTF_LEFT);
	snprintf(sizeString, 32, "%d mm", screen_height_mm);
	font->GetStringWidth(font, "Screen height: ", -1, &width);
	helpSurface->DrawString (helpSurface, sizeString, -1, topX+width, topY, DSTF_LEFT);
	
  TruetypeCreateFont(currentState->line.blockSize);
	helpSurface->DrawString (helpSurface, "Glyph O height: ", -1, topX, topY+=height, DSTF_LEFT);
	int tempInt;
	DFBRectangle rectangle;
  fontTruetype->GetGlyphExtents(fontTruetype, 'O', &rectangle, &tempInt);
  fontTruetype->Release (fontTruetype);
	snprintf(sizeString, 32, "%d pixels", rectangle.h);
	font->GetStringWidth(font, "Glyph O height: ", -1, &width);
	helpSurface->DrawString (helpSurface, sizeString, -1, topX+width, topY, DSTF_LEFT);
	printf("Blocksizex5: %f   actual height: %d \n", currentState->line.blockSize*5, rectangle.h);  

	helpSurface->DrawString (helpSurface, "Blocksize x 5: ", -1, topX, topY+=height, DSTF_LEFT);
	snprintf(sizeString, 32, "%f pixels", currentState->line.blockSize*5);
	font->GetStringWidth(font, "Bolocksize x 5:", -1, &width);
	helpSurface->DrawString (helpSurface, sizeString, -1, topX+width, topY, DSTF_LEFT);

  
	topY+=height/2;
	helpSurface->DrawString (helpSurface, "Press any key to continue ...", -1, topX, topY+=height, DSTF_LEFT);

	displaySurface->SetBlittingFlags(displaySurface, DSBLIT_NOFX);
	displaySurface->Blit (displaySurface, helpSurface, NULL, 0,0);
	displaySurface->Flip (displaySurface, NULL, DSFLIP_WAITFORSYNC);

	/*
	 *   wait for event then reset event buffer
	 */

	SleepQuarterSecond();
	eventBuffer->Reset(eventBuffer);
	eventBuffer->WaitForEvent(eventBuffer);
	SleepQuarterSecond();
	eventBuffer->Reset(eventBuffer);

	font->Release (font);

	/*
	 *  flip back to original surface
	 */

	displaySurface->Flip(displaySurface, NULL, DSFLIP_ONSYNC);
}

void SleepQuarterSecond() {
	struct timespec reqTime, remTime;
	reqTime.tv_sec = 0 ; reqTime.tv_nsec=100000000;
	nanosleep(&reqTime, &remTime);
}

void Screensaver() {

	DFBSurfaceDescription   dsc;
	IDirectFBImageProvider *imageProvider;
	IDirectFBSurface *logoSurface = NULL;

	/* save the displaySurface using the helpSurface */
//	displaySurface->Flip(displaySurface, NULL, DSFLIP_ONSYNC);
	helpSurface->SetBlittingFlags(helpSurface, DSBLIT_NOFX);
	helpSurface->Blit (helpSurface, displaySurface, NULL, 0,0);

	/*
	 * (Load the logo)
	 */
	dfb->CreateImageProvider (dfb, "logo.png", &imageProvider);
	imageProvider->GetSurfaceDescription (imageProvider, &dsc);
	dfb->CreateSurface (dfb, &dsc, &logoSurface);
	imageProvider->RenderTo (imageProvider, logoSurface, NULL);
	imageProvider->Release (imageProvider);

	/*
	 * Set blitting flags to DSBLIT_BLEND_ALPHACHANNEL that enables alpha
	 * blending using the alpha channel of the source.
	 */

	displaySurface->SetBlittingFlags (displaySurface, DSBLIT_BLEND_ALPHACHANNEL);

	/*
	 *   draw the logo on the screen until someone hits a key or button
	 */

	SleepQuarterSecond();
	eventBuffer->Reset(eventBuffer);
	int count = 0;
	while(eventBuffer->HasEvent (eventBuffer) != DFB_OK) {
		if ( count >= 13 ) {
			displaySurface->Clear(displaySurface, 0, 0, 0, 0xff);
			displaySurface->Blit (displaySurface, logoSurface, NULL, MyRandom(screenWidth - dsc.width), MyRandom(screenHeight - dsc.height));
			displaySurface->Flip(displaySurface, NULL, DSFLIP_ONSYNC);
			count = 0;
		} else count++;
		SleepQuarterSecond();
	}
	SleepQuarterSecond();
	eventBuffer->Reset(eventBuffer);

	/*
	 *   copy original image back to display
	 */
	displaySurface->SetBlittingFlags(displaySurface, DSBLIT_NOFX);
	displaySurface->Blit (displaySurface, helpSurface, NULL, 0,0);
	displaySurface->Flip(displaySurface, NULL, DSFLIP_ONSYNC);
	logoSurface->Release(logoSurface);
}

void Help() {
	DFBSurfaceDescription   dsc;
	IDirectFBImageProvider *imageProvider;
		
	/* save the displaySurface using the helpSurface */
	helpSurface->SetBlittingFlags(helpSurface, DSBLIT_NOFX);
	helpSurface->Blit (helpSurface, displaySurface, NULL, 0,0);

	/*
	 *    Load keyboard help image
	 */

	//dfb->CreateImageProvider (dfb, "onscreen_help_keyboard.png", &imageProvider);
	//imageProvider->GetSurfaceDescription (imageProvider, &dsc);
	//imageProvider->RenderTo (imageProvider, displaySurface, NULL);
	//displaySurface->Flip(displaySurface, NULL, DSFLIP_ONSYNC);

	// wait for a key

	//WaitForEventWithScreensaver(eventBuffer);

	/*
	 *    Load remote help image
	 */

	dfb->CreateImageProvider (dfb, "onscreen_help_remote.png", &imageProvider);
	imageProvider->GetSurfaceDescription (imageProvider, &dsc);
	imageProvider->RenderTo (imageProvider, displaySurface, NULL);
	displaySurface->Flip(displaySurface, NULL, DSFLIP_ONSYNC);

	// wait for a key

	WaitForEventWithScreensaver(eventBuffer);

	imageProvider->Release (imageProvider);

	/*
	 *   copy original image back to display
	 */
	displaySurface->SetBlittingFlags(displaySurface, DSBLIT_NOFX);
	displaySurface->Blit (displaySurface, helpSurface, NULL, 0,0);
	displaySurface->Flip(displaySurface, NULL, DSFLIP_ONSYNC);

	/*  debounce keyboard */

	SleepQuarterSecond();
	eventBuffer->Reset(eventBuffer);
}

void WaitForEventWithScreensaver(IDirectFBEventBuffer* eventBuffer) {
	DFBResult result = DFB_TIMEOUT;

 	SleepQuarterSecond();
	eventBuffer->Reset(eventBuffer);

	while ( result == DFB_TIMEOUT) {
		result=eventBuffer->WaitForEventWithTimeout(eventBuffer, 300, 0);
		if (result == DFB_TIMEOUT) Screensaver();
	}
}

void WaitForEventWithScreensaverNoDebounce(IDirectFBEventBuffer* eventBuffer) {
	DFBResult result = DFB_TIMEOUT;

	eventBuffer->Reset(eventBuffer);

	while ( result == DFB_TIMEOUT) {
		result=eventBuffer->WaitForEventWithTimeout(eventBuffer, 300, 0);
		if (result == DFB_TIMEOUT) Screensaver();
	}
}

void MirrorModeToggle() {
	if (currentState->mirrorModeOn == 1) currentState->mirrorModeOn = 0 ; else currentState->mirrorModeOn=1;
}

/*
 *  these functions are related to the display of true type glyphs
 */

void TruetypeCreateFont (float blockSize) {

	DFBFontDescription font_dsc;

	//	DFBFontAttributes font_attributes = DFFA_MONOCHROME || DFFA_NOKERNING ;
	//	font_dsc.attributes = font_attributes;
	font_dsc.flags = DFDESC_HEIGHT;
  font_dsc.height = blockSize*5;
	if (currentState->truetypeMode==1) dfb->CreateFont (dfb, "../fonts/Snellen.ttf", &font_dsc, &fontTruetype);
  if (currentState->truetypeMode==2) dfb->CreateFont (dfb, "../fonts/Sloan.ttf", &font_dsc, &fontTruetype);

	letterSurface->SetFont (letterSurface, fontTruetype);
	// the global pointer fontSnellen points to the created font so no return value required
}

void TruetypeToggleMode() {
	currentState->truetypeMode++;
  if ( currentState->truetypeMode == 3 ) currentState->truetypeMode=1;
}

void TruetypePrintLetter (int letter, int topX, int topY, float blockSize) {

  char printMe;

  printf("Truetype printing letter %d at %d, %d with block size %f\n", letter, topX, topY, blockSize);
  TruetypeCreateFont(blockSize);
  letterSurface->SetColor (letterSurface, 0x00, 0x00, 0x00, 0xFF);
	printMe=glyphsEnumToChar[currentState->truetypeMode][letter];
	if (currentState->mirrorModeOn == 1 ) printMe+=32;
  letterSurface->DrawString (letterSurface, &printMe, 1, topX, topY + blockSize*5, DSTF_LEFT);
  fontTruetype->Release (fontTruetype);
}
/*
 *    ReadConfiguration
 *
 *    uses libconfig to read the configuration file
 *    valid Names:
 *
 *    ScreenHeightInMM      ->  an integer value, the screen height in mm, default 250
 *    DistanceToScreenInMM  ->  integer, the viewing distance between patient and screen, default 4000
 *
 */
void ReadConfiguration() {
	FILE *configFile;
	int error;
	int tempInt;
	char pathToScreenHeight[]="ScreenHeightInMM";
	char pathToDistance[]="DistanceToScreenInMM";
	char read='r';
	

	/*
	 * read config file. First check in /etc/snellen.rc
	 * then look in ~/.snellen.rc
	 * then look in present working directory for snellen.rc
	 * if no config file is found, just use the defaults
	 */

	config_init(&configuration);
	configFile = fopen ("/etc/snellen.rc", &read);
	if (configFile == NULL) configFile = fopen ("~/.snellen.rc", &read);
	if (configFile == NULL) configFile = fopen ("snellen.rc", &read);

	if (configFile == NULL) {
		printf("Couldn't open config file for reading.\n");
		return;
	}

	/* read the configuration file and set the values */

	error = config_read(&configuration, configFile);
	if (error == CONFIG_TRUE) {
		printf ("Reading config file ");
    // read ScreenHeightInMM
		error = config_lookup_int(&configuration, pathToScreenHeight, &tempInt);
		if (error == CONFIG_TRUE) {
			screen_height_mm = tempInt;
			printf ("screen_height=%d", screen_height_mm);
		}
		// read DistanceToScreenInMM
		error = config_lookup_int(&configuration, pathToDistance, &tempInt);
		if (error == CONFIG_TRUE) {
			distance_to_patient = tempInt;
			printf (" distance_to_patient=%d", distance_to_patient);
		}
		printf("\n");
	} else {
		printf ("problem parsing config file.\n");
	}
	fclose(configFile);
}

void WriteConfiguration() {
	FILE *configFile;
	char pathToScreenHeight[]="ScreenHeightInMM";
	char pathToDistance[]="DistanceToPatientInMM";
	char write='w';
	config_setting_t *configSetting;

	/*
	 * read config file. First check in /etc/snellen.rc
	 * then look in ~/.snellen.rc
	 * then look in present working directory for snellen.rc
	 * if no config file is found, just use the defaults
	 */

	configFile = fopen ("/etc/snellen.rc", &write);
	if (configFile == NULL) configFile = fopen ("~/.snellen.rc", &write);
	if (configFile == NULL) configFile = fopen ("snellen.rc", &write);

	if (configFile == NULL) {
		printf("Couldn't open config file for writing.\n");
		return;
	}

	/* read the configuration file and set the values */

	printf ("Writing config file ");
	configSetting = config_lookup(&configuration, pathToScreenHeight);
	if (configSetting != NULL) {
		config_setting_set_int(configSetting, (long) screen_height_mm);
		printf ("screen_height=%d",screen_height_mm);
	}
	configSetting = config_lookup(&configuration, pathToDistance);
	if (configSetting != NULL) {
		config_setting_set_int(configSetting, (long) distance_to_patient);
		printf (" distance_to_patient=%d \n", distance_to_patient);
	}
	config_write(&configuration, configFile);
	config_destroy(&configuration);
	fclose(configFile);
}


/*  
 *  These routines change the distance to patient
 *  and screen height values
 */

void ChangeDistanceAndScreenHeight() {
	int choice=0, endLoop=0;
	MenuSpecsType menuSpecs;
	DFBInputEvent event;

	while (!endLoop) {
		/*
		 * show screen
		 */
		DrawDistanceOrHeightSurface(&menuSpecs);
		DrawBoxAroundChoice(menuSpecs, choice);
		displaySurface->SetBlittingFlags(displaySurface, DSBLIT_NOFX);
		displaySurface->Blit (displaySurface, helpSurface, NULL, 0,0);
		displaySurface->Flip (displaySurface, NULL, DSFLIP_WAITFORSYNC);

		/*
		 * get keyboard input
		 */

		WaitForEventWithScreensaver(eventBuffer);
		eventBuffer->GetEvent(eventBuffer, DFB_EVENT(&event));
		if (event.key_symbol == DIKS_MENU) printf ("yes\n");
		switch (event.key_symbol) {
		case DIKS_CURSOR_DOWN:
			choice++; if (choice == 3) choice = 0;
			break;
		case DIKS_CURSOR_UP:
			choice--; if (choice == -1) choice = 2;
			break;
		case DIKS_ENTER:
		case DIKS_OK:
			if (choice == 0) SetDistance();
			if (choice == 1) SetHeight();
	    if (choice == 2) endLoop = 1;
			break;
		case DIKS_ESCAPE:
			endLoop = 1;
			break;
		default:
			break;
		}
		/*
		 * keyboard debouncing
		 */
		SleepQuarterSecond();
		eventBuffer->Reset(eventBuffer);
	}
}

void DrawDistanceOrHeightSurface(MenuSpecsType *menuSpecs) {
	static IDirectFBFont *font = NULL;
	DFBFontDescription font_dsc;
	int topX, topY, height, width;

	font_dsc.flags = DFDESC_HEIGHT;
	font_dsc.height = 32;
	dfb->CreateFont (dfb, "../fonts/LiberationSans-Regular.ttf", &font_dsc, &font);

	helpSurface->SetFont (helpSurface, font);
	helpSurface->Clear (helpSurface, 0, 0, 0, 0);
	helpSurface->SetColor (helpSurface, 0xff, 0xff, 0xff, 0xFF);

	font->GetStringWidth(font, "Change Distance to Patient", -1, &width);
	topX = (screenWidth - width)/2;
	font->GetHeight(font, &height); 
	topY = (screenHeight - height*4.5)/2;
	menuSpecs->topX = topX; menuSpecs->topY = topY; menuSpecs->width=width; menuSpecs->height=height;

	helpSurface->DrawString (helpSurface, "Change Distance to Patient", -1, 	topX, topY, DSTF_LEFT);
	helpSurface->DrawString (helpSurface, "Change Screen Height", -1, 	topX, topY+height*1.5, DSTF_LEFT);
	helpSurface->DrawString (helpSurface, "Return to snellen", -1, topX, topY+height*3.0, DSTF_LEFT);

	font->Release(font);
}

void DrawBoxAroundChoice(MenuSpecsType menuSpecs, int choice) {
	
	int width = menuSpecs.width;
	int height = menuSpecs.height;
	int leftX = menuSpecs.topX - 20;
	int rightX = menuSpecs.topX + width + 20;
	int topY = menuSpecs.topY + height*1.5*(choice-1) + height/2;
	int bottomY = menuSpecs.topY + height*1.5*choice + height/2;
	
	printf("creating box %d, (%d, %d) - (%d %d)\n", choice, leftX, topY, rightX, bottomY);

	helpSurface->SetColor (helpSurface, 0xff, 0x00, 0x00, 0xFF);
	helpSurface->FillRectangle(helpSurface, leftX, topY, width+44, 4);
	helpSurface->FillRectangle(helpSurface, leftX, topY, 4, height*1.5+4);
	helpSurface->FillRectangle(helpSurface, leftX, bottomY, width+44, 4);
	helpSurface->FillRectangle(helpSurface, rightX, topY, 4, height*1.5+4);

}


void SetDistance() {
	char string[256];
	int x, y, height, width;
	int endLoop = 0;
	IDirectFBFont *font = NULL;
	DFBFontDescription font_dsc;
	DFBInputEvent event;

	font_dsc.flags = DFDESC_HEIGHT;
	font_dsc.height = 32;
	dfb->CreateFont (dfb, "../fonts/LiberationSans-Regular.ttf", &font_dsc, &font);

	helpSurface->SetFont (helpSurface, font);

	font->GetStringWidth(font, "Distance to Patient: 12345 mm", -1, &width);
	x = (screenWidth - width)/2;
	font->GetHeight(font, &height); 
	y = (screenHeight - height)/2;

	/* 
	 * clear event buffer
   */ 

	SleepQuarterSecond();
	eventBuffer->Reset(eventBuffer);

	/*
   * change distance value
   */

	while ( !endLoop ) {

		/* 
		 * draw surface
	   */

		helpSurface->Clear (helpSurface, 0, 0, 0, 0);
		helpSurface->SetColor (helpSurface, 0xff, 0xff, 0xff, 0xFF);
		snprintf(string, (size_t) 256, "Distance to Patient: %5d mm", distance_to_patient); 
		helpSurface->DrawString (helpSurface, string, -1, x, y, DSTF_LEFT);

		displaySurface->SetBlittingFlags(displaySurface, DSBLIT_NOFX);
		displaySurface->Blit (displaySurface, helpSurface, NULL, 0,0);
		displaySurface->Flip (displaySurface, NULL, DSFLIP_WAITFORSYNC);

		/*
		 * get keyboard input
		 */

		WaitForEventWithScreensaverNoDebounce(eventBuffer);
		eventBuffer->GetEvent(eventBuffer, DFB_EVENT(&event));
		if (event.key_symbol == DIKS_MENU) printf ("yes\n");
		switch (event.key_symbol) {
		case DIKS_CURSOR_DOWN:
			distance_to_patient-=1; break;
		case DIKS_CURSOR_UP:
			distance_to_patient+=1; break;
		case DIKS_FASTFORWARD:
		case DIKS_CURSOR_RIGHT:
			distance_to_patient+=50; break;
		case DIKS_REWIND:
		case DIKS_CURSOR_LEFT:
			distance_to_patient-=50; break;
		case DIKS_ENTER:
		case DIKS_OK:
		case DIKS_ESCAPE:
			endLoop=1; break;
		default:
			break;
		}
	}

	/*
	 * keyboard debouncing
	 */
	SleepQuarterSecond();
	eventBuffer->Reset(eventBuffer);
	font->Release(font);
}

void SetHeight () {
	char string[256];
	int x, y, height, width;
	int endLoop = 0;
	IDirectFBFont *font = NULL;
	DFBFontDescription font_dsc;
	DFBInputEvent event;

	font_dsc.flags = DFDESC_HEIGHT;
	font_dsc.height = 32;
	dfb->CreateFont (dfb, "../fonts/LiberationSans-Regular.ttf", &font_dsc, &font);

	helpSurface->SetFont (helpSurface, font);

	font->GetStringWidth(font, "Screen Height: 1234 mm", -1, &width);
	x = (screenWidth - width)/2;
	font->GetHeight(font, &height); 
	y = (screenHeight - height)/2;

	/* 
	 * clear event buffer
   */

	SleepQuarterSecond();
	eventBuffer->Reset(eventBuffer);

	/*
   * change distance value
   */

	while ( !endLoop ) {

		/* 
		 * draw surface
	   */

		helpSurface->Clear (helpSurface, 0, 0, 0, 0);
		helpSurface->SetColor (helpSurface, 0xff, 0xff, 0xff, 0xFF);
		snprintf(string, (size_t) 256, "Screen Height: %4d mm", screen_height_mm); 
		helpSurface->DrawString (helpSurface, string, -1, x, y, DSTF_LEFT);

		displaySurface->SetBlittingFlags(displaySurface, DSBLIT_NOFX);
		displaySurface->Blit (displaySurface, helpSurface, NULL, 0,0);
		displaySurface->Flip (displaySurface, NULL, DSFLIP_WAITFORSYNC);

		/*
		 * get keyboard input
		 */

		WaitForEventWithScreensaverNoDebounce(eventBuffer);
		eventBuffer->GetEvent(eventBuffer, DFB_EVENT(&event));
		switch (event.key_symbol) {
		case DIKS_CURSOR_DOWN:
			screen_height_mm-=1; break;
		case DIKS_CURSOR_UP:
			screen_height_mm+=1; break;
		case DIKS_FASTFORWARD:
		case DIKS_CURSOR_RIGHT:
			screen_height_mm+=50; break;
		case DIKS_REWIND:
		case DIKS_CURSOR_LEFT:
			screen_height_mm-=50; break;
		case DIKS_ENTER:
		case DIKS_OK:
		case DIKS_ESCAPE:
			endLoop=1; break;
		default:
			break;
		}
	}

	/*
	 * keyboard debouncing
	 */
	SleepQuarterSecond();
	eventBuffer->Reset(eventBuffer);
	font->Release(font);
}
