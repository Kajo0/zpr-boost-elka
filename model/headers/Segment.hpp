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
		Point begin_;
		Point end_;

		public:
		Segment(const Point, const Point);
		virtual ~Segment() = 0;
		
		virtual Point position(double percent) = 0;
		virtual double length() = 0;
		Point begin();
		Point end();
	};

	class BezierSegment : public Segment // Objects move on curves
	{
		Point control_;
		public:
			// tu pytanie czy robic , begin end controlPoint, czy po kolei begin controlPoint end ? 
			// m: czy to ma znaczenie? jak mam wybierac to niech bedzie b -> control -> e
		BezierSegment(const Point begin, const Point end, const Point control);
		~BezierSegment();

		Point position(double percent);
		double length();
		Point control();
	};

	class StraightSegment : public Segment // Objects move on straight lines
	{
		public:
		StraightSegment(const Point begin, const Point end);
		~StraightSegment();
		
		double length();
		Point position(double percent);
	};
}

#endif // SEGMENT_HPP