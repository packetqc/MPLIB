/*
 * MPScrollList.h
 *
 *  Created on: Mar 25, 2024
 *      Author: admin
 */

#ifndef MPSCROLLLIST_HPP_
#define MPSCROLLLIST_HPP_

#include <touchgfx/containers/scrollers/ScrollList.hpp>
#include <touchgfx/events/GestureEvent.hpp>



class MPScrollList: public touchgfx::ScrollList {
public:
	MPScrollList();
	virtual ~MPScrollList();
	virtual void handleGestureEvent	(	const touchgfx::GestureEvent &	event	);


};

#endif /* MPSCROLLLIST_HPP_ */
