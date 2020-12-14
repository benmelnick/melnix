#include "framebuf.h"
#include "asm/x86.h"

const char* banner =
#include <banner.h>
;

static ushort *buff = (ushort *)FB_ADDRESS;
static ushort s_fbx = 0;
static ushort s_fby = 0;

// Byte for creating black background w/ white foreground
// Becomes the lower 8 bits in data written to buffer
#define ATTR_BYTE (FB_BLACK << 4) | FB_WHITE
// Constant for writing a blank to screen
ushort blank = (ATTR_BYTE << 8) | 0x20;

/*
 * Calculates the offset from the start of the framebuffer
 */
static inline ushort
fb_position()
{
  return (s_fby * FB_COLUMNS) + s_fbx;
}

/*
 * Moves the cursor on the screen to the most recently updated spot
 * in the frame buffer
 * Calculates position of most recently written data in framebuf
 */
static inline void
fb_movecursor()
{
  ushort pos = fb_position();
  // Tell device we are sending highest 8 bits of data
  outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
  outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
  // Tell device we are sending lowest 8 bits of data
  outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  outb(FB_DATA_PORT, pos & 0x00FF);
}

/*
 * Scrolls the text on the screen up by one line
 */
static inline void
fb_scroll()
{
  if (s_fby >= FB_ROWS) {
    // Move the current text chunk that makes up the screen back
    // in the buffer by a line
    int i;
    for (i = 0; i < FB_COLUMNS*(FB_ROWS - 1); i++) {
      buff[i] = buff[i + 80];
    }

    // Make the last line blank
    for (i = 24*80; i < FB_COLUMNS*(FB_ROWS - 1); i++) {
      buff[i] = blank;
    }
  }
}

/*
 * Writes a single character to the screen by writing it to the frame buffer
 * Updates x and y pointers in the framebuf based on the character
 * Scrolls the screen (if needed) and updates the cursor
 */
void
fb_put(char c)
{
  if (c == 0x08 && s_fbx) {
    // Backspace
    s_fbx--;
  } else if (c == 0x09) {
    // Tab
    
  } else if (c == '\r') {
    // Carriage return
    s_fbx = 0;
  } else if (c == '\n') {
    // New line
    s_fbx = 0;
    s_fby++;
  } else if (c >= ' ') {
    // Any other printable character
    ushort buf_idx = fb_position();
    buff[buf_idx] = (ATTR_BYTE << 8) | c;
    s_fbx++;
  }

  // Check if printing the character forces a new line
  if (s_fbx >= FB_COLUMNS) {
    s_fbx = 0;
    s_fby++;
  }

  fb_scroll();
  fb_movecursor();
}

/*
 * Writes a null-terminated string to the screen
 */
void
fb_write(char *str)
{
  int i = 0;
  while (str[i]) {
    fb_put(str[i++]);
  }
}

/*
 * Clears the screen
 */
 void
 fb_clear()
 {
   for (int i = 0; i < FB_COLUMNS*FB_ROWS; i++) {
     buff[i] = blank;
   }
   s_fbx = 0;
   s_fby = 0;
   fb_movecursor();
 }

 void
 init_fb()
 {
   fb_clear();
   fb_write(banner);
 }