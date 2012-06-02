#include "Controller.hpp"
#include "Logger.hpp"
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <vector>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/bind/bind.hpp>
//TODO fabryczka jakos
#include "SmallCar.hpp"
#include "BigCar.hpp"
#include "Walker.hpp"
#include "Voyager.hpp"
#include "Track.hpp"
#include "Timer.hpp"
#include "Graph.hpp"

namespace zpr
{
	Controller::Controller(const boost::filesystem::path & path) : view_(*this, model_), run_(true)//, timer_(boost::chrono::milliseconds(10))
	{
		//logger tez bezie przechowywal przebieg dzialania aplikacji -> bledy jakie ludek zrobil w trakcie

		std::string paths[] = { "xml_data/dispatcher.xml", "xml_data/streets.xml", "xml_data/objects.xml" };
		std::string mesgs[] = { "Retype path (ex. '", "') or type 'q' to exit:\n\t", "No matching path like: " };
		
		// to na razie
		for (int i = 0; i < 3; ++i)
		{
			while ( !boost::filesystem::exists(path / paths[i]) )
			{
				std::cout<<mesgs[2]<<paths[i]<<std::endl;

				std::cout<<mesgs[0]<<paths[i]<<mesgs[1];
				std::cin>>paths[i];

				if (paths[i] == "q")
				{
					std::string msg = "Quit without starting application while trying to load ";
					msg.append(paths[i]);
					throw(std::exception(msg.c_str()));
				}
			}
		}

		// punkt wyjscia z programu jest jeden, zandych exit(0); stad wyjatki poleca do maina
		parseDispatcher(path / paths[0]);
		parseMap(path / paths[1]);
		parseObjects(path / paths[2]);

		model_.start();	// ustawienie na poczatku - tylko test pozycjonowania
		
		timer_.AddListener(TimerListener(boost::bind(&Model::scheduleUpdate, &model_, _1), boost::chrono::milliseconds(10)));
		timer_.AddListener(TimerListener(boost::bind(&View::scheduleRefresh, &view_, _1), boost::chrono::milliseconds(10)));
		timer_.AddListener(TimerListener(boost::bind(&Model::scheduleLog, &model_, _1), boost::chrono::milliseconds(1000)));
	}
	
	void Controller::scheduleEvent(const boost::shared_ptr<Event> & newEvent)
	{
		{
			boost::lock_guard<boost::mutex> lock(eventMutex_);
			eventQueue_.push(newEvent);
		}
		eventCondition_.notify_one();
	}
	
	void Controller::runThreads()
	{
		viewThread = boost::thread(boost::ref(view_));
		modelThread = boost::thread(boost::ref(model_));
		timerThread = boost::thread(boost::ref(timer_));
	}

	void Controller::endThreads()
	{
		viewThread.interrupt();
		timerThread.interrupt();
		modelThread.interrupt();
		
		viewThread.join();
		timerThread.join();
		modelThread.join();
	}

	void Controller::run()
	{
		runThreads();
		while(run_ && !boost::this_thread::interruption_requested())
		{
			
			boost::unique_lock<boost::mutex> lock(eventMutex_);
			while(eventQueue_.empty())
				eventCondition_.wait(lock);
					
			boost::shared_ptr<Event> ev = eventQueue_.front();
			eventQueue_.pop();
			ev->accept(*this);
		}
		endThreads();
	}

	void Controller::Process(EventStart&)
	{
		//timer_.start();
		model_.setActive(true);
	}

	void Controller::Process(EventStop&)
	{
		model_.setActive(false);
		//timer_.stop();
	}

	void Controller::Process(EventRestart&)
	{
		model_.start();
		view_.scheduleRefresh(0);
	}

	void Controller::Process(EventClose&)
	{
		run_ = false;
	}

	void Controller::Process(EventLoop&)
	{
		model_.switchLoop();
	}

	void Controller::parseDispatcher(const boost::filesystem::path & path)
	{
		using boost::property_tree::ptree;
		ptree pt;
		Dispatcher::PCamera cam;
	
		try
		{
			read_xml(path.string(), pt, boost::property_tree::xml_parser::no_comments);

			BOOST_FOREACH( ptree::value_type &v, pt.get_child("dispatcher") )
			{
				// get cam parameteres
				int id = v.second.get<int>("id");
				Point position( v.second.get<double>("position.<xmlattr>.x"), v.second.get<double>("position.<xmlattr>.y") );
				double direction = v.second.get<double>("direction");
				double angle = v.second.get<double>("angle");
				double range = v.second.get<double>("range");
				double precision = v.second.get<double>("precision");

				// add camera
				model_.addCamera( Dispatcher::PCamera(new Camera(id, position, direction, angle, range, precision)) );
			}
		}
		catch (boost::property_tree::ptree_bad_path &e)
		{
			std::string msg = e.what();
			msg.append(" : in file (");
			msg.append(path.string());
			msg.append(")");

			throw std::exception(msg.c_str());
		}
	}

	void Controller::parseMap(const boost::filesystem::path & path)
	{
		using boost::property_tree::ptree;
		ptree pt;
		Model::PGraph graph;
	
		try
		{
			read_xml(path.string(), pt, boost::property_tree::xml_parser::no_comments);
			//std::vector<std::pair<int, Point>> vertices;
			//std::vector<std::pair<int, int>> edges;

			graph.reset(new Graph());

			BOOST_FOREACH( ptree::value_type &v, pt.get_child("map") )
			{
				if ( v.first == "nodes" )
				{
					BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("") )
					{
						int id = vv.second.get<int>("");
						Point position( vv.second.get<double>("<xmlattr>.x"), vv.second.get<double>("<xmlattr>.y") );
						//std::cout<<"\tID: "<<id<<" x: "<<position.x_<<" y: "<<position.y_<<"\n";
						//vertices.push_back(std::make_pair(id, position));

						graph->addVertex(id, position);
					}
				}
				else if ( v.first == "edges" )
				{
					BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("") )
					{
						int from = vv.second.get<int>("<xmlattr>.from");
						int to = vv.second.get<int>("<xmlattr>.to");
						//std::cout<<"\tFrom: "<<from<<" To: "<<to<<"\n";
						//edges.push_back(std::make_pair(from, to));

						graph->addEdge(from, to);
					}
				}
			}

			model_.streets(graph);
		}
		catch (boost::property_tree::ptree_bad_path &e)
		{
			std::string msg = e.what();
			msg.append(" : in file (");
			msg.append(path.string());
			msg.append(")");

			throw std::exception(msg.c_str());
		}
	}

	void Controller::parseObjects(const boost::filesystem::path & path)
	{
		using boost::property_tree::ptree;
		ptree pt;
		Model::PCar car;
		Model::PWalker walker;
		Voyager::PTrack track;
	
		try
		{
			read_xml(path.string(), pt, boost::property_tree::xml_parser::no_comments);

			BOOST_FOREACH( ptree::value_type &v, pt.get_child("objects") )
			{
				if ( v.second.get<std::string>("<xmlattr>.type") == "car" )
				{
					std::string id = v.second.get<std::string>("id");
					double acceleration = v.second.get<double>("parameters.acceleration");
					double weight = v.second.get<double>("parameters.weight");
					std::string size = v.second.get<std::string>("parameters.size"); // potem enum jakis trza ogarnac
					double mspeed = v.second.get<double>("parameters.maxspeed");

					// TODO - tu wstaw fabryke jakos, ja tego nie widze za bardzo dla naszej aplikacyjki ale trzeba ladniej to zrobic :/
					// przyspieszenie jakos z masy i max predkosci wyliczamy czy kak ?
					if (size == "big")
						car.reset( new BigCar( id, acceleration, weight, mspeed ) );
					else if (size == "small")
						car.reset( new SmallCar( id, acceleration, weight, mspeed ) );
					
					track.reset(new VehicleTrack());
					car->track(track);

					BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("track") )
					{
						Point point( vv.second.get<double>("<xmlattr>.x"),
									vv.second.get<double>("<xmlattr>.y") );

						track->addPoint(point);
					}

					model_.addObject(car);
				}
				else if ( v.second.get<std::string>("<xmlattr>.type") == "walker" )
				{
					std::string id = v.second.get<std::string>("id");
					double mspeed = v.second.get<double>("parameters.maxspeed");

					walker.reset( new Walker( id, mspeed ) );
					track.reset(new WalkerTrack());
					walker->track(track);
					
					BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("track") )
					{
						Point point( vv.second.get<double>("<xmlattr>.x"),
									vv.second.get<double>("<xmlattr>.y") );
							
						track->addPoint(point);
					}

					model_.addObject(walker);
				}
			}
		}
		catch (boost::property_tree::ptree_bad_path &e)
		{
			std::string msg = e.what();
			msg.append(" : in file (");
			msg.append(path.string());
			msg.append(")");

			throw std::exception(msg.c_str());
		}
	}
}