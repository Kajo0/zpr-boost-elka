#ifndef EVENTS_HPP
#define EVENTS_HPP

namespace zpr
{
	class Controller;
	
	struct Event
	{
		virtual void accept(Controller &) = 0;
	};

	struct EventStart : public Event
	{
		virtual void accept(Controller & c);
	};

	struct EventStop : public Event
	{
		virtual void accept(Controller & c);
	};

	struct EventRestart : public Event
	{
		virtual void accept(Controller & c);
	};

	struct EventClose : public Event
	{
		virtual void accept(Controller & c);
	};

	struct EventLoop : public Event
	{
		virtual void accept(Controller & c);
	};

}

#endif // EVENTS_HPP