#include "Track.hpp"
#include <math.h>

namespace zpr
{
	const double VehicleTrack::BEZIER_DIVISION = 10;

	Track::Track() : length_(0.0)
	{
	}

	Track::~Track() {}
	
	/**
	 * @return Begin track point
	 */
	Point Track::start()
	{
		return segments_.empty() ? Point() : segments_.front()->begin();
	}

	/**
	 * @return End track point
	 */
	Point Track::finish()
	{
		return segments_.empty() ? Point() : segments_.back()->end();
	}

	/**
	 * @return Actual track length
	 */
	double Track::length()
	{
		return length_;
	}

	/**
	 * Calculates position on track from given track percent
	 *
	 * @param percent Percent of track
	 * @return Request position, on bezier segment or not
	 */
	// zwraca pozycje i czy jest na bezierze czy nie
	std::pair<Point, double> Track::positionOnTrack(double percent)
	{
		if ( percent < 0 )
			percent = 0;
		else if ( percent > 1 )
			percent = 1;

		double traveled_length = length_ * percent;
		double temp_length = 0;

		for (DSegments::const_iterator it = segments_.begin(); it != segments_.end(); ++it)
		{
			temp_length += (*it)->length();

			if ( traveled_length <= temp_length )
			{
				// zwraca pozycje z wyliczonego procenta procenta segmetnu na ktorym sie znajduje na tym zadanym procencie
				return std::make_pair((*it)->position( 1 - ( ( temp_length - traveled_length ) / (*it)->length() ) ), (*it)->bezier());
			}
		}

		return std::make_pair(Point(), 0);
	}

	// zwraca procend trasy do najblizszego beziera i kat jego
	std::pair<double, double> Track::nextBezierDistanceAngle(double percent)
	{
		if ( percent < 0 )
			percent = 0;
		else if ( percent > 1 )
			percent = 1;
		
		double traveled_length = length_ * percent;
		double temp_length = 0;

		for (DSegments::const_iterator it = segments_.begin(); it != segments_.end(); ++it)
		{
			temp_length += (*it)->length();

			if ( traveled_length <= temp_length && (*it)->bezier() )
			{
				// zwraca pozycje z wyliczonego procenta procenta segmetnu na ktorym sie znajduje na tym zadanym procencie
				return std::make_pair((temp_length - (*it)->length()) / length_, (*it)->bezier());
			}
		}

		return std::make_pair(2, 0);
	}

	/**
	 * Calculates track length - sum of each segment length
	 */
	void Track::recalculateLength()
	{
		length_ = 0;

		for (DSegments::const_iterator it = segments_.begin(); it != segments_.end(); ++it)
			length_ += (*it)->length();
	}

	void VehicleTrack::addPoint(Point point)
	{
		if (segments_.empty())
			segments_.push_back( PSegment( new StraightSegment( point, point ) ) );
		else if ( segments_.size() == 1 && segments_.front()->begin() == segments_.front()->end() )
			segments_.front().reset( new StraightSegment( segments_.front()->begin(), point ) );
		else
		{
			if ( segments_.back()->end() == point )
				return;
			// Mowiac krotko jak jest jeden segment to jest StraightSegment
			// jak dodajemy trzeci punkt to juz mozna beziera zrobic, wiec jest : straight -> bezier -> straight
			// jak kolejny dajemy to jest : straight -> bezier -> straight(mozliwe ze o zerowej dlugosci jak nie wyrabia sie zakretowo) -> bezier -> straight

			Point realBegin = segments_.back()->begin();
			Point lastBegin = realBegin;
			Point lastEnd = segments_.back()->end();
			
			segments_.pop_back();
			
			if (!segments_.empty())
				realBegin = static_cast<BezierSegment*>(segments_.back().get())->control();

			Point p1, p2;
			std::pair<Point, Point> p_tmp = bothBezierBetween(lastBegin, point, lastEnd);
			p1 = p_tmp.first;
			p2 = p_tmp.second;	// do tej linii to jest dzielone rowno po dwoch stronach
			//Point p1 = bezierBetween( lastEnd, realBegin );	// tu jest nie wiem czy to prblem ale jedna czesc luku moze byc minimalizowana do polowy dlugosci odcinka a druga niekoniecznie i czy chcemy to wyeliminowac czy nie? trzeba by wymuszac rozstapienie minimalne
			//Point p2 = bezierBetween( lastEnd, point );

			// optymalizacja(jeszcze nie ma :P): jak begin i end na tym samym x'ie lub y'ku to tylko straight
			segments_.push_back( PSegment( new StraightSegment( lastBegin, p1 ) ) );
			segments_.push_back( PSegment( new BezierSegment( p1, p2, lastEnd ) ) );
			segments_.push_back( PSegment( new StraightSegment( p2, point ) ) );
		}

		recalculateLength();
	}

	// trzraba wybrac jedna z dwoch metod, zeby copy pase nie bylo ze jest ;p choc jest ;d
	std::pair<Point, Point> VehicleTrack::bothBezierBetween(const Point &start, const Point &final, const Point &control)
	{
		std::pair<Point, Point> tmp = std::make_pair(control, control);
		double p1_distance = Point::distance(control, start);
		double p2_distance = Point::distance(control, final);

		if ( p1_distance <= 2 * BEZIER_DIVISION || p2_distance <= 2 * BEZIER_DIVISION )
		{
			double min_distance = ( p1_distance < p2_distance ? p1_distance : p2_distance ) / 2;

			tmp.first.x_ += (start.x_ - control.x_) * ( min_distance / p1_distance );
			tmp.first.y_ += (start.y_ - control.y_) * ( min_distance / p1_distance );
			tmp.second.x_ += (final.x_ - control.x_) * ( min_distance / p2_distance );
			tmp.second.y_ += (final.y_ - control.y_) * ( min_distance / p2_distance );
		}
		else
		{
			tmp.first.x_ += (start.x_ - control.x_) * ( BEZIER_DIVISION / p1_distance );
			tmp.first.y_ += (start.y_ - control.y_) * ( BEZIER_DIVISION / p1_distance );
			tmp.second.x_ += (final.x_ - control.x_) * ( BEZIER_DIVISION / p2_distance );
			tmp.second.y_ += (final.y_ - control.y_) * ( BEZIER_DIVISION / p2_distance );
		}

		return tmp;
	}

	/**
	 * To rozsuniecie o ktorym mowilem - tylko ze dla pary tzn. majac kropki jako pkty, da wsp. punktu albo oddalonego
	 * o BEZIER_DIVISION ('promien' skretu aktualnie 2)
	 *
	 *         .
	 *
	 *     ,
	 *   .
	 */
	Point VehicleTrack::bezierBetween(const Point &control, const Point &final)
	{
		Point tmp = control;
		
		if ( Point::distance(control, final) <= 2 * BEZIER_DIVISION )
		{
			tmp.x_ += (final.x_ - control.x_) / 2;
			tmp.y_ += (final.y_ - control.y_) / 2;
		}
		else
		{
			tmp.x_ += (final.x_ - control.x_) * ( BEZIER_DIVISION / Point::distance(control, final) );
			tmp.y_ += (final.y_ - control.y_) * ( BEZIER_DIVISION / Point::distance(control, final) );
		}

		return tmp;
	}

	void WalkerTrack::addPoint(Point point)
	{
		if(segments_.empty())
			segments_.push_back(PSegment(new StraightSegment(point, point)));
		else if(segments_.size() == 1 && segments_.front()->begin() == segments_.front()->end())
			segments_.front().reset(new StraightSegment(segments_.front()->begin(), point));
		else
			segments_.push_back(PSegment(new StraightSegment(segments_.back()->end(), point)));

		recalculateLength();
	}
}