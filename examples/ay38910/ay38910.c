#include <ay-3-8910.h>
#include <ay-notes.h>
#include <tms_patterns.h>
#include <cpm.h>
#include <tms.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
 * Tempo is 104 bbm using 1/60 FPS we can roughly time the song like this.
 * (♩) quarter beat is 35 frames
 * (♪) eighth beat is 17 frames
 * (♩.) dotted quarter is 52 frames
 * 𝄾 eighth rest is 17 frames
 *
 * This music was taken from Goolge Gemini AI.  I know it's wrong.  I just
 * don't know how much it's wrong by.
 *
 * Selecting Octave 3, the piano keys used are:
 *
 * G  = 35
 * Eb = 31 (same as D#)
 * Bb = 38 (same as A#)
 * D  = 30
 *
 * G (♩) G (♩) G (♩) E♭ (♪) B♭ (♪)
 *    2     2     2      1      1
 *
 * G (♩.)  E♭ (♪)  B♭ (♪) G (♩)  𝄾 (Eighth rest)
 *    3        1       1     2      1 
 *
 * D (♩) D (♩) D (♩) E♭ (♪) B♭ (♪)
 *    2     2     2      1      1 
 *
 * G (♩.)  E♭ (♪)  B♭ (♪) G (♩)  𝄾 (Eighth rest)
 *    3        1       1     2   1
 *
 * Looking at these timings, we have either 17, 35 or 52 or:
 *
 * 1 x 17
 * 2 x 17
 * 3 x 17
 *
 * We set the length of an eighth note in number of VDP frames and time the
 * tune accordingly.
 *
 * For the decay part we do:
 *
 * 104/60 = quarter
 * 104/60/2 = eighth
 *
 * env_freq = (256 * period)/clock
 * 0.87     = (256 * period)/1793712
 * 256*period = 0.87 * 1793712
 * period = (0.87 * 1793712) / 256
 *
 * period here is the value to plug into the envelope frequecy for
 * an envelope lasting one quarter note. = 6096
*/

#define LENGTH  20
#define EIGHTH  17  // Number of TMS 1/60 frames in an eighth note
#define VOL     15
#define DECAY   6096

uint8_t f;
uint8_t i;
uint8_t t;

uint8_t notes[LENGTH] = {
  35,
  35,
  35,
  31,
  38,

  35,
  31,
  38,
  35,
   0,

  30,
  30,
  30,
  31,
  38,

  35,
  31,
  38,
  35,
   0 
};

uint8_t waits[LENGTH] = {
  2,
  2,
  2,
  1,
  1,

  3,
  1,
  1,
  2,
  1,

  2,
  2,
  2,
  1,
  1,

  3,
  1,
  1,
  2,
  1
};

void wait(uint8_t d) {
  // sustain a note for 'd' x EIGHTH fames.
  for (i=0; i<d; i++) {
    f = EIGHTH;
    while (f-- > 0) {
      tms_wait();
    }
  }
  // silence for 1/16th between notes
  ay_write(8,0);
  for(i=0; i<EIGHTH/2; i++) {
    tms_wait();
  }
  // reset volume for next note
}


int main()
{
  ay_write(7,0x7E);       //enable channel A tone only 0b01111110
  ay_write(8,0x1F);         //set channel A volume to 12
  tms_init_g1(WHITE, BLACK, false, false); // need this to ensure VPD interrupt is set

  tms_load_pat(tms_patterns, 0x400);
  tms_load_col(tms_g1_colors, 32);
  memset(tms_buf, 0x20, 768);
  tms_wait();
  tms_g1flush(tms_buf);

  tms_puts_xy(2,6,"Solid notes, no decay");
  tms_wait();
  tms_g1flush(tms_buf);

  for (t=0; t < LENGTH; ++t)
  {
    ay_write(8,VOL);
    ay_play_note_delay(notes[t], 0, 0);
    wait(waits[t]);
  }
  ay_write(8,0);      // silence channel A

  tms_puts_xy(2,8,"Short intermission");
  tms_wait();
  tms_g1flush(tms_buf);

  for (t=0; t<120; t++) tms_wait();

  tms_puts_xy(2,10,"Now with decay notes");
  tms_wait();
  tms_g1flush(tms_buf);

  for (t=0; t<LENGTH; ++t)
  {
    ay_play_note_delay(notes[t], 0, waits[t]*DECAY);
    wait(waits[t]);
  }
  ay_write(8,0);      // silence channel A

  tms_puts_xy(2,12,"Exiting...");
  tms_wait();
  tms_g1flush(tms_buf);

  for (t=0; t<120; t++) tms_wait();
  return 0;
}

