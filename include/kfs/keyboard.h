#ifndef __KEYBOARD_H__
# define __KEYBOARD_H__

# include <stdint.h>

#define KEY_MAP_SIZE_1	94
#define KEY_MAP_SIZE_2	127

/*
* init_ps2_keyboard
* should be called once before using keykoard_loop.
* keyboard_loop behavior might be undefined otherwise
* this function actually init the whole controller
* rather than the keyboard only
* Might be move to ps2 controller file directly
*/
extern uint8_t	init_ps2_keyboard(void);
extern void		keyboard_loop(void);

#endif