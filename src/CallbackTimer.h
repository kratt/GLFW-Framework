#ifndef CALLBACKTIMER_H
#define CALLBACKTIMER_H

#include <iostream>
#include <agents.h>
#include <ppltasks.h>

template <class T>
void start_callback_timer(const T& callback, unsigned int timeInMs)
{
	concurrency::task_completion_event<void> tce;
	auto call = new concurrency::call<int>(
		[callback, tce](int)
	{
		callback();
		tce.set();
	});

	auto timer = new concurrency::timer<int>(timeInMs, 0, call, true);
	concurrency::task<void> event_set(tce);
	event_set.then([timer, call]()
	{
		/*delete call;
		delete timer;*/
	});

	timer->start();
}

#endif