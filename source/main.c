#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <wiilight.h>
#include <time.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;


void adjustLEDLevel(bool shouldDecrease) {
  // 0 being lowest, 255 being brightest
  int lightLevel = WIILIGHT_GetLevel();
  
  shouldDecrease ? WIILIGHT_SetLevel(lightLevel - 1) : WIILIGHT_SetLevel(lightLevel + 1);
}

void printWithSpaces(char *message) {
  printf("\n      %s", message);
}

void printLEDLevel() {
  int lightLevel = WIILIGHT_GetLevel();
  printf("\n      LED level %d", lightLevel);
}

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

  // Initialise the video system
  VIDEO_Init();
  
  // This function initialises the attached gamecube controllers
  PAD_Init();
  
  // Obtain the preferred video mode from the system
  // This will correspond to the settings in the Wii menu
  rmode = VIDEO_GetPreferredMode(NULL);

  // Allocate memory for the display in the uncached region
  xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
  
  // Initialise the console, required for printf
  console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
  
  // Set up the video registers with the chosen mode
  VIDEO_Configure(rmode);
  
  // Tell the video hardware where our display memory is
  VIDEO_SetNextFramebuffer(xfb);
  
  // Make the display visible
  VIDEO_SetBlack(FALSE);

  // Flush the video register changes to the hardware
  VIDEO_Flush();

  // Wait for Video setup to complete
  VIDEO_WaitVSync();
  if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();


  // The console understands VT terminal escape codes
  // This positions the cursor on row 2, column 0
  // we can use variables for this with format codes too
  // e.g. printf ("\x1b[%d;%dH", row, column );
  printf("\x1b[2;0H\n\n");
  

  // Print options and stuff
  printWithSpaces("Made in DRNT's lab");
  printWithSpaces("The following instructions change the Wii disc drive LED\n");

  printWithSpaces("Pressing start returns you to the loader");
  printWithSpaces("Press A to view the LED level (ranges from 0-255)\n");

  printWithSpaces("D-PAD up turns on the LED");
  printWithSpaces("D-PAD down turns off the LED");
  printWithSpaces("D-PAD right turns up the LED level");
  printWithSpaces("D-PAD left turns down the LED level");

  // Initialize the LED
  WIILIGHT_Init();

  while(1) {

    // Call WPAD_ScanPads each loop, this reads the latest controller states
    PAD_ScanPads();

    // WPAD_ButtonsDown tells us which buttons were pressed in this loop
    // this is a "one shot" state which will not fire again until the button has been released
    u32 pressed = PAD_ButtonsDown(0);
    u32 held = PAD_ButtonsHeld(0);

    if (pressed & PAD_BUTTON_START) { // We return to the launcher application via exit
      printWithSpaces("Exiting...");
      exit(0);
    } else if (pressed & PAD_BUTTON_UP) { // We turn on the LED
      printWithSpaces("Turning on LED...\n");
      WIILIGHT_TurnOn();
    } else if (pressed & PAD_BUTTON_DOWN) { // We turn off the LED
      printWithSpaces("Turning off LED...");
      WIILIGHT_TurnOff();
    } else if (pressed & PAD_BUTTON_A) { // Print LED level
      printLEDLevel();
    }

    // Adjusting LED based on direction on the DPAD
    if(held & PAD_BUTTON_RIGHT) {
        usleep(500);
        adjustLEDLevel(false);
    } else if (held & PAD_BUTTON_LEFT) {
        usleep(500);
        adjustLEDLevel(true);
    }

    // Wait for the next frame
    VIDEO_WaitVSync();
  }
  return 0;
}
