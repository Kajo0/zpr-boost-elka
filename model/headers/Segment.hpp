#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include "Base.hpp"

namespace zpr
{
	/**
	  * Abstract base class for representing track's segments.
	  */
	class Segment
	{
		protected:
		const Point begin_;
		const Point end_;

		public:
		Segment(const Point, const Point);
		virtual Point position(double) = 0;
		virtual ~Segment() = 0;
	};

	class BezierSegment : public Segment // Objects move on curves
	{
		Point control_;
		public:
			// tu pytanie czy robic , begin end controlPoint, czy po kolei begin controlPoint end ? 
		BezierSegment(const Point, const Point, const Point);
		~BezierSegment();

		Point position(double);
	};

	class StraightSegment : public Segment // Objects move on straight lines
	{
		public:
		StraightSegment(const Point, const Point);
		~StraightSegment();

		Point position(double);
	};
}

#endif // SEGMENT_HPP