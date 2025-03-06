/*
 * MPScrollList.cpp
 *
 *  Created on: Mar 25, 2024
 *      Author: admin
 */

#include <DataServices.h>
#include <MPScrollList.hpp>
#include <mpapp.h>

MPScrollList::MPScrollList() {
	// TODO Auto-generated constructor stub
    allowHorizontalDrag(true);
//    allowVerticalDrag(true);
}

MPScrollList::~MPScrollList() {
	// TODO Auto-generated destructor stub
}

void MPScrollList::handleGestureEvent	(	const touchgfx::GestureEvent &	event	) {
	if( event.getType() == event.SWIPE_HORIZONTAL) {
		DS->setPause();
	}
	else {
		DS->setPause();
	}
}
