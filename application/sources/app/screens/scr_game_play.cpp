#include <stdint.h>
#include "scr_game_play.h"
#include "screens_bitmap.h"
#include "screens.h"
#define GAME_PLAY_LINE1 (0)
#define GAME_PLAY_LINE2 (1)
#define GAME_PLAY_LINE3 (2)
static uint8_t game_play_cursor=GAME_PLAY_LINE1;
static void view_scr_game_play();

view_dynamic_t dyn_view_game_play = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_play
};

view_screen_t scr_game_play = {	
	&dyn_view_game_play,
	ITEM_NULL,
	ITEM_NULL,
	.focus_item = 0,
};

void view_scr_game_play() {
	view_render.clear();
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.fillRect(20,0,2,64,WHITE);
    for(uint8_t x=0;x<128;x+=10)
    {
        view_render.drawFastHLine(x,21,6,WHITE);
        view_render.drawFastHLine(x,42,6,WHITE);
    }
	if(game_play_cursor==GAME_PLAY_LINE1)
	{
		view_render.drawBitmap(2,3,image_plane_bits,16,16,WHITE);
	}
	else if(game_play_cursor==GAME_PLAY_LINE2)
	{
		view_render.drawBitmap(2,24,image_plane_bits,16,16,WHITE);
	}
	else if(game_play_cursor==GAME_PLAY_LINE3)
	{
		view_render.drawBitmap(2,45,image_plane_bits,16,16,WHITE);
	}

}

void scr_game_play_handle(ak_msg_t *msg) {
	switch (msg->sig) {
		case SCREEN_ENTRY: {
			APP_DBG_SIG("SCREEN_ENTRY\n");
			game_play_cursor = GAME_PLAY_LINE2;
			timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);
		} break;
		case AC_DISPLAY_BUTON_DOWN_PRESSED: {
			APP_DBG_SIG("AC_DISPLAY_BUTON_DOWN_PRESSED\n");
			if (game_play_cursor < GAME_PLAY_LINE3) {
				game_play_cursor++;
			}
			else {
				game_play_cursor = GAME_PLAY_LINE1;
			}
		} break;
		case AC_DISPLAY_BUTON_UP_PRESSED: {
			APP_DBG_SIG("AC_DISPLAY_BUTON_UP_PRESSED\n");
			if (game_play_cursor > GAME_PLAY_LINE1) {
				game_play_cursor--;
			}
			else {
				game_play_cursor = GAME_PLAY_LINE3;
			}
		} break;
}
}