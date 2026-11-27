#include <stdint.h>
#include "scr_game_play.h"
#include "screens_bitmap.h"
#include "screens.h"
#define GAME_PLAY_LINE1 (0)
#define GAME_PLAY_LINE2 (1)
#define GAME_PLAY_LINE3 (2)
static uint8_t game_play_cursor=GAME_PLAY_LINE1;
static int16_t active_plane_x = 20;
static uint8_t plane_run=0;
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
		if(plane_run==0)
		{
		view_render.drawBitmap(2,3,image_plane_bits,16,16,WHITE);
		}
		else if(plane_run==1)
		{
			view_render.drawBitmap(active_plane_x,3,image_plane_bits,16,16,WHITE);
		}
	}
	else if(game_play_cursor==GAME_PLAY_LINE2)
	{
		if(plane_run==0)
		{
		view_render.drawBitmap(2,24,image_plane_bits,16,16,WHITE);
		}
		else if(plane_run==1)
		{
			view_render.drawBitmap(active_plane_x,24,image_plane_bits,16,16,WHITE);
		}
	}
	else if(game_play_cursor==GAME_PLAY_LINE3)
	{
		if(plane_run==0)
		{
		view_render.drawBitmap(2,45,image_plane_bits,16,16,WHITE);
		}
		else if(plane_run==1)
		{
			view_render.drawBitmap(active_plane_x,45,image_plane_bits,16,16,WHITE);
		}
	}
}
void scr_game_play_handle(ak_msg_t *msg) {
	switch (msg->sig) {
		case SCREEN_ENTRY: {
			APP_DBG_SIG("SCREEN_ENTRY\n");
			game_play_cursor = GAME_PLAY_LINE2;
			timer_set(AC_TASK_DISPLAY_ID,AC_DISPLAY_GAME_PLAY_TICK,100 ,TIMER_PERIODIC);
		} break;
		case AC_DISPLAY_GAME_PLAY_TICK: {
			APP_DBG_SIG("AC_DISPLAY_GAME_PLAY_TICK\n");
			if(plane_run==1)
			{
				active_plane_x +=3;
				if (active_plane_x >= 128) 
				{
    				plane_run = 0;
				}
			}
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
		case AC_DISPLAY_BUTON_MODE_PRESSED: {
			APP_DBG_SIG("AC_DISPLAY_BUTON_DOWN_PRESSED\n");
			if(plane_run==0)
			{
				plane_run=1;
				active_plane_x = 22;
			}
		}break;
}
}