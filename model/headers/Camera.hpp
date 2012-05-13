#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Base.hpp"

namespace zpr
{
	/**
	 * Class representing camera object, contains essential information about
	 */
	class Camera
	{
		const int id_;
		Point position_;
		double direction_;
		double angle_;
		double range_;
		double precision_;

		public:
		Camera(int);
		virtual ~Camera();

		// pytanie czy odwolnaia robimy like setX getX czy po prostu dzialamy np void x(int x); int x(); ?!
		// ja to chyba jednak preferowalbym x() ale juz to napislem wiec nei zmienie do czasu konsultacji ;d
		// wszystko wyrzucamy do .cpp, te pierdoly tez ?
		void setPosition(const Point position)	{ position_ = position; }
		void setDirection(double direction)		{ direction_ = direction; }
		void setAngle(double angle)				{ angle_ = angle; }
		void setRange(double range)				{ range_ = range; }
		void setPrecision(double precision)		{ precision_ = precision; }

		int getId()				{ return id_; }
		Point getPosition()		{ return position_; }
		double getDirection()	{ return direction_; }
		double getAngle()		{ return angle_; }
		double getRange()		{ return range_; }
		double getPrecision()	{ return precision_; }


		// TODO delete it - test function
		void tellMeSthAbout();
	};
}

#endif // CAMERA_HPP