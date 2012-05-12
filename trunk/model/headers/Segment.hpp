#ifndef SEGMENT_HPP
#define SEGMENT_HPP

namespace zpr
{
	/**
	  * Abstract base class for representing track's segments.
	  */
	class Segment
	{
		private:
		// jakies dane odcinka, poczatek, koniec
		public:
		// jakas wfunkcja wyznaczenia kolejnej pozycji na segmencie na podstawie biezacego polozenia
		virtual ~Segment() = 0; // dodac puste cialo i plik cpp
	};

	class BezierSegment : public Segment // Objects move on curves
	{
		// dodatkowe punbkty kontrolne
	};

	class StraightSegment : public Segment // Objects move on straight lines
	{

	};
}

#endif // SEGMENT_HPP