
#include "types.h"
#include "color.h"
#include "lib.h"
#include "keyboard.h"
#include "display.h"
#include "idt.h"

void keyboard_handler(proc_ctx_t* this_context)
{
  uint8_t key_char;
  uint8_t status;

  status = inb(0x64);

  if(status & 0x01)
  {
    key_char = inb(0x60);
    
    if(key_char < 0)
    {
        return;
    }
  }

  printk_color(red, cyan, "%c", keymap[key_char]);

}

void init_keyboard()
{
  register_interrupt_handler(IRQ1, keyboard_handler);

  keymap_setup(keymap);
}

void keymap_setup(uint8_t * keymap)
{
  // a to z
  keymap[30] = 'a';
  keymap[48] = 'b';
  keymap[46] = 'c';
  keymap[32] = 'd';
  keymap[18] = 'e';
  keymap[33] = 'f';
  keymap[34] = 'g';
  keymap[35] = 'h';
  keymap[23] = 'i';
  keymap[36] = 'j';
  keymap[37] = 'k';
  keymap[38] = 'l';
  keymap[50] = 'm';
  keymap[49] = 'n';
  keymap[24] = 'o';
  keymap[25] = 'p';
  keymap[16] = 'q';
  keymap[19] = 'r';
  keymap[31] = 's';
  keymap[20] = 't';
  keymap[22] = 'u';
  keymap[47] = 'v';
  keymap[17] = 'w';
  keymap[45] = 'x';
  keymap[21] = 'y';
  keymap[44] = 'z';

  /* A to Z
  keymap[30] = 'a';
  keymap[48] = 'b';
  keymap[46] = 'c';
  keymap[32] = 'd';
  keymap[18] = 'e';
  keymap[33] = 'f';
  keymap[34] = 'g';
  keymap[35] = 'h';
  keymap[23] = 'i';
  keymap[36] = 'j';
  keymap[37] = 'k';
  keymap[38] = 'l';
  keymap[50] = 'm';
  keymap[49] = 'n';
  keymap[24] = 'o';
  keymap[25] = 'p';
  keymap[16] = 'q';
  keymap[19] = 'r';
  keymap[31] = 's';
  keymap[20] = 't';
  keymap[22] = 'u';
  keymap[47] = 'v';
  keymap[17] = 'w';
  keymap[45] = 'x';
  keymap[21] = 'y';
  keymap[44] = 'z';
  */

  //0 - 9
  keymap[11] = '0';
  keymap[2] = '1';
  keymap[3] = '2';
  keymap[4] = '3';
  keymap[5] = '4';
  keymap[6] = '5';
  keymap[7] = '6';
  keymap[8] = '7';
  keymap[9] = '8';
  keymap[10] = '9';

  //symbol
  keymap[39] = ';';
  keymap[40] = '\'';
  keymap[51] = ',';
  keymap[52] = '.';
  keymap[39] = ';';
  keymap[28] = '\n';
  keymap[13] = '=';
  keymap[12] = '-';
  keymap[43] = '\\';
  keymap[53] = '/';
  keymap[26] = '[';
  keymap[27] = ']';
  keymap[41] = '~';
  keymap[15] = '\t';
  keymap[57] = ' ';
  keymap[14] = '\b';
}