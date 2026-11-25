#include <stdint.h>
#include "scr_menu.h"
#include "screens_bitmap.h"
#define MENU_ITEM_PLAY (0)
#define MENU_ITEM_SCORE (1)
#define MENU_ITEM_EXIT (2)
static uint8_t menu_cursor=MENU_ITEM_PLAY;
static void view_scr_menu();

view_dynamic_t dyn_view_menu = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_menu
};

view_screen_t scr_menu = {	
	&dyn_view_menu,
	ITEM_NULL,
	ITEM_NULL,
	.focus_item = 0,
};

void view_scr_menu() {
	view_render.clear();
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(31, 12);
	view_render.print("BOMB - GAME");
	view_render.drawLine(10, 16, 117, 16, WHITE);
	//PLAY
	if (menu_cursor == MENU_ITEM_PLAY) {
		view_render.fillRect(14, 28, 24, 24, WHITE);
		view_render.drawBitmap(18, 32, image_circle_play__1__bits, 16, 16, BLACK);
	} else {
		view_render.drawRect(14, 28, 24, 24, WHITE);
		view_render.drawBitmap(18, 32, image_circle_play__1__bits, 16, 16, WHITE);
	}
	//SCORE
	if (menu_cursor == MENU_ITEM_SCORE) {
		view_render.fillRect(52, 28, 24, 24, WHITE);
		view_render.drawBitmap(56, 32, image_trophy_bits, 16, 16, BLACK);
	} else {
		view_render.drawRect(52, 28, 24, 24, WHITE);
		view_render.drawBitmap(56, 32, image_trophy_bits, 16, 16, WHITE);
	}
	//EXIT
	if (menu_cursor == MENU_ITEM_EXIT) {
		view_render.fillRect(90, 28, 24, 24, WHITE);
		view_render.drawBitmap(94, 32, image_square_arrow_left__bits, 16, 16, BLACK);
	} else {
		view_render.drawRect(90, 28, 24, 24, WHITE);
		view_render.drawBitmap(94, 32, image_square_arrow_left__bits, 16, 16, WHITE);
	}
}

void scr_menu_handle(ak_msg_t *msg) {
	switch (msg->sig) {
	case AC_DISPLAY_BUTON_MODE_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_PRESSED\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_LOGO);
		SCREEN_TRAN(scr_qrcode_handle, &scr_qrcode);
	} break;

	case AC_DISPLAY_SHOW_LOGO: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_LOGO\n");
		SCREEN_TRAN(scr_qrcode_handle, &scr_qrcode);
	} break;

	case AC_DISPLAY_SHOW_IDLE: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_IDLE\n");
		SCREEN_TRAN(scr_idle_handle, &scr_idle);
	} break;

	default:
		break;
	}
}
