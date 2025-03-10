#include <gui/containers/StatusNavigationBar.hpp>

#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>

#include <MPDataServices.h>

StatusNavigationBar::StatusNavigationBar()
{
	SetPlay();
}

void StatusNavigationBar::initialize()
{
    StatusNavigationBarBase::initialize();
}

void StatusNavigationBar::updateControl_PAUSE() {
	GetPause();
}

void StatusNavigationBar::SetPause() {
	DS->setPause();
}

//const uint16_t BITMAP_ICON_THEME_IMAGES_AV_PAUSE_24_24_3D3D3D_SVG_ID = 15;
//const uint16_t BITMAP_ICON_THEME_IMAGES_AV_PAUSE_24_24_4D99EB_SVG_ID = 16;
//const uint16_t BITMAP_ICON_THEME_IMAGES_AV_PLAY_ARROW_24_24_3D3D3D_SVG_ID = 17;
//const uint16_t BITMAP_ICON_THEME_IMAGES_AV_PLAY_ARROW_24_24_4D99EB_SVG_ID = 18;
//const uint16_t BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_LEFT_24_24_3D3D3D_SVG_ID = 26;
//const uint16_t BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_LEFT_24_24_4D99EB_SVG_ID = 27;
//const uint16_t BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_RIGHT_24_24_3D3D3D_SVG_ID = 28;
//const uint16_t BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_RIGHT_24_24_4D99EB_SVG_ID = 29;


void StatusNavigationBar::GetPause() {
	if( DS->getPause() == false)
	{
		Control_Play.setIconBitmaps(BITMAP_ICON_THEME_IMAGES_AV_PLAY_ARROW_24_24_4D99EB_SVG_ID, BITMAP_ICON_THEME_IMAGES_AV_PLAY_ARROW_24_24_3D3D3D_SVG_ID);
		Control_Pause.setIconBitmaps(BITMAP_ICON_THEME_IMAGES_AV_PAUSE_24_24_3D3D3D_SVG_ID, BITMAP_ICON_THEME_IMAGES_AV_PAUSE_24_24_4D99EB_SVG_ID);
	}
	else if( DS->getPause() == true)
	{
		Control_Play.setIconBitmaps(BITMAP_ICON_THEME_IMAGES_AV_PLAY_ARROW_24_24_3D3D3D_SVG_ID, BITMAP_ICON_THEME_IMAGES_AV_PLAY_ARROW_24_24_4D99EB_SVG_ID );
		Control_Pause.setIconBitmaps(BITMAP_ICON_THEME_IMAGES_AV_PAUSE_24_24_4D99EB_SVG_ID, BITMAP_ICON_THEME_IMAGES_AV_PAUSE_24_24_3D3D3D_SVG_ID);
	}

	Control_Pause.invalidate();
	Control_Play.invalidate();
}

void StatusNavigationBar::SetPlay() {
	DS->setPlay();
}

void StatusNavigationBar::GetPlay() {
	if( DS->getPlay() == false)
	{
		Control_Play.setIconBitmaps(BITMAP_ICON_THEME_IMAGES_AV_PLAY_ARROW_24_24_3D3D3D_SVG_ID, BITMAP_ICON_THEME_IMAGES_AV_PLAY_ARROW_24_24_4D99EB_SVG_ID );
//		Control_Pause.setIconBitmaps(BITMAP_ICON_THEME_IMAGES_AV_PAUSE_24_24_4D99EB_SVG_ID, BITMAP_ICON_THEME_IMAGES_AV_PAUSE_24_24_3D3D3D_SVG_ID);
	}
	else if( DS->getPlay() == true)
	{
		Control_Play.setIconBitmaps(BITMAP_ICON_THEME_IMAGES_AV_PLAY_ARROW_24_24_4D99EB_SVG_ID, BITMAP_ICON_THEME_IMAGES_AV_PLAY_ARROW_24_24_3D3D3D_SVG_ID);
//		Control_Pause.setIconBitmaps(BITMAP_ICON_THEME_IMAGES_AV_PAUSE_24_24_3D3D3D_SVG_ID, BITMAP_ICON_THEME_IMAGES_AV_PAUSE_24_24_4D99EB_SVG_ID);
	}

	Control_Play.invalidate();
	Control_Pause.invalidate();
}

//void StatusNavigationBar::updateStatus_IOT_CONNECTED(uint8_t value) {
//
//}
//
//void StatusNavigationBar::updateStatus_STORAGE(uint8_t value) {
//
//}

void StatusNavigationBar::updateStatus_APP(uint8_t value)
{
	if( value == APP_NOTOK)
	{
		Status_APP.setIconBitmaps(Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_CHECK_CIRCLE_OUTLINE_50_50_3D3D3D_SVG_ID), Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_CHECK_CIRCLE_OUTLINE_50_50_009C00_SVG_ID));
	}
	else if( value == APP_OK)
	{
		Status_APP.setIconBitmaps(Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_CHECK_CIRCLE_OUTLINE_50_50_009C00_SVG_ID), Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_CHECK_CIRCLE_OUTLINE_50_50_3D3D3D_SVG_ID));
	}

	Status_APP.invalidate();
}

//void StatusNavigationBar::updateStatus_ETH(uint8_t value)
//{
//	if( value == NET_NOTOK)
//	{
//		Status_ETH.setIconBitmaps(Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_SETTINGS_ETHERNET_50_50_3D3D3D_SVG_ID), Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_SETTINGS_ETHERNET_50_50_009C00_SVG_ID));
//	}
//	else if( value == NET_OK)
//	{
//		Status_ETH.setIconBitmaps(Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_SETTINGS_ETHERNET_50_50_009C00_SVG_ID), Bitmap(BITMAP_ICON_THEME_IMAGES_ACTION_SETTINGS_ETHERNET_50_50_3D3D3D_SVG_ID));
//	}
//
//	Status_ETH.invalidate();
//}
//
//void StatusNavigationBar::updateStatus_DATA(uint8_t value) {
//	if( value == CLOUD_NOTOK)
//	{
//		Status_DATA.setIconBitmaps(Bitmap(BITMAP_ICON_THEME_IMAGES_FILE_CLOUD_CIRCLE_50_50_3D3D3D_SVG_ID), Bitmap(BITMAP_ICON_THEME_IMAGES_FILE_CLOUD_CIRCLE_50_50_009C00_SVG_ID));
//	}
//	else if( value == CLOUD_OK)
//	{
//		Status_DATA.setIconBitmaps(Bitmap(BITMAP_ICON_THEME_IMAGES_FILE_CLOUD_CIRCLE_50_50_009C00_SVG_ID), Bitmap(BITMAP_ICON_THEME_IMAGES_FILE_CLOUD_CIRCLE_50_50_3D3D3D_SVG_ID));
//	}
//
//	Status_DATA.invalidate();
//}
//
//void StatusNavigationBar::updateStatus_IOT(uint8_t value) {
//}
