#include "VehicleTrack.hpp"

namespace zpr
{
	const double VehicleTrack::BEZIER_DIVISION = 50;

	VehicleTrack::VehicleTrack()
	{
	}

	VehicleTrack::~VehicleTrack() {}

	void VehicleTrack::addPoint(Point point)
	{
		if (segments_.empty())
		{
			segments_.push_back( PSegment( new StraightSegment( point, point ) ) );
		}
		else if ( segments_.size() == 1 && segments_.front()->begin() == segments_.front()->end() )
		{
			segments_.front().reset( new StraightSegment( segments_.front()->begin(), point ) );
		}
		else
		{
			// Mowiac krotko jak jest jeden segment to jest StraightSegment
			// jak dodajemy trzeci punkt to juz mozna beziera zrobic, wiec jest : straight -> bezier -> straight
			// jak kolejny dajemy to jest : straight -> bezier -> straight(mozliwe ze o zerowej dlugosci jak nie wyrabia sie zakretowo) -> bezier -> straight

			Point realBegin = segments_.back()->begin();
			Point lastBegin = realBegin;
			Point lastEnd = segments_.back()->end();
			
			segments_.pop_back();
			
			if (!segments_.empty())
			{
				realBegin = static_cast<BezierSegment*>(segments_.back().get())->control();
			}

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
}