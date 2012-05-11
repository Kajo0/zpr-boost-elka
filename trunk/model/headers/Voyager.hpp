#ifndef VOYAGER_HPP
#define VOYAGER_HPP

namespace zpr
	{
	class Point; // forward declaration

	/**
	 * Abstract base class for all objects moving around city.
	 */
	class Voyager
		{
		private:
		Point position_;
		double velocity_;
		double acceleration_;
		double weight_;
		double maxSpeed_;
		// size?
		// trasa
		boost::shared_ptr<Track> track_; // czemu tak? nie moze byc po prostu track bez shr_ptr?

		public:
		Voyager(double weight, double maxSpeed);
		virtual ~Voyager() = 0;
		
		const Point & Position() const;
		};
	}

#endif // VOYAGER_HPP