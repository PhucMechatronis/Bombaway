#include <stdint.h>
#include <stdlib.h>
#include "scr_game_play.h"
#include "screens_bitmap.h"
#include "screens.h"
#define GAME_PLAY_LINE1 (0)
#define GAME_PLAY_LINE2 (1)
#define GAME_PLAY_LINE3 (2)

#define MAX_TANK (8)
typedef struct 
{
	uint8_t TANK_LANE;
	uint8_t TANK_ALIVE;
	int16_t TANK_X;	
}TANK;
static TANK TANK_POOL[MAX_TANK];
static uint8_t TANK_TIMER_LANE[3];
static uint8_t TANK_WAIT_TIME=0;

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
	//Tank
	for(uint8_t i=0;i<MAX_TANK;i++)
	{
		if(TANK_POOL[i].TANK_ALIVE==1)
		{
			if(TANK_POOL[i].TANK_LANE==0)
			{
				view_render.drawBitmap(TANK_POOL[i].TANK_X,7,image_tank_bits,11,7,WHITE);
			}
			else if(TANK_POOL[i].TANK_LANE==1)
			{
				view_render.drawBitmap(TANK_POOL[i].TANK_X,28,image_tank_bits,11,7,WHITE);
			}
			else if(TANK_POOL[i].TANK_LANE==2)
			{
				view_render.drawBitmap(TANK_POOL[i].TANK_X,49,image_tank_bits,11,7,WHITE);
			}
		}
	}
}
void scr_game_play_handle(ak_msg_t *msg) {
	switch (msg->sig) {
		case SCREEN_ENTRY: {
			APP_DBG_SIG("SCREEN_ENTRY\n");
			srand(sys_ctrl_millis());
			for(uint8_t i=0;i<MAX_TANK;i++)
			{
				TANK_POOL[i].TANK_ALIVE=0;
			}
			game_play_cursor = GAME_PLAY_LINE2;
			timer_set(AC_TASK_DISPLAY_ID,AC_DISPLAY_GAME_PLAY_TICK,100 ,TIMER_PERIODIC);
			//Set time tank appear
			for(uint8_t i=0;i<3;i++)
			{
				TANK_TIMER_LANE[i]= 5 + (rand()%21);
			}
		} break;
		case AC_DISPLAY_GAME_PLAY_TICK: {
			APP_DBG_SIG("AC_DISPLAY_GAME_PLAY_TICK\n");
			//Plane
			if(plane_run==1)
			{
				active_plane_x +=3;
				if (active_plane_x >= 128) 
				{
    				plane_run = 0;
				}
			}
			//Tank
			for(uint8_t i=0;i<3;i++)
			{
				if(TANK_TIMER_LANE[i]>0)
				{
					TANK_TIMER_LANE[i]--;
				}
				else
				{
					TANK_TIMER_LANE[i]=5+(rand()%21);
					uint8_t TANK_ROAD_CLEAR=1;
					for(uint8_t k=0;k<MAX_TANK;k++)
					{
						if(TANK_POOL[k].TANK_ALIVE==1&&TANK_POOL[k].TANK_LANE==i&&TANK_POOL[k].TANK_X>102)
						{
							TANK_ROAD_CLEAR=0;
							break;
						}
					}
					if(TANK_ROAD_CLEAR==0)
					{

					}
					else 
					{
							for(uint8_t j=0;j<MAX_TANK;j++)
						{
							if(TANK_POOL[j].TANK_ALIVE==0)
							{
								TANK_POOL[j].TANK_ALIVE=1;
								TANK_POOL[j].TANK_LANE= i;
								TANK_POOL[j].TANK_X=117;
								break;
							}
						}
					}
				}
			}
			
			if(TANK_WAIT_TIME<3)
			{
				TANK_WAIT_TIME++;
			}
			else
			{
				TANK_WAIT_TIME=0;
				for(uint8_t i=0;i<MAX_TANK;i++)
				{
					if(TANK_POOL[i].TANK_ALIVE==1)
					{
						TANK_POOL[i].TANK_X -=3;
						if(TANK_POOL[i].TANK_X<=22)
						{
							TANK_POOL[i].TANK_ALIVE=0;
						}
					}
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