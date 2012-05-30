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

namespace zpr
{
	Controller::Controller(const boost::filesystem::path & path) : view_(*this, model_), run_(true), timer_(boost::chrono::milliseconds(10))
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
		
		timer_.AddListener(boost::bind(&Model::scheduleUpdate, &model_, _1));
		timer_.AddListener(boost::bind(&View::scheduleRefresh, &view_, _1));
		
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
		viewThread.join();
		timerThread.interrupt();
		modelThread.interrupt();
		
		std::cout << "View thread joined." << std::endl;
		
		timerThread.join();
		std::cout << "Timer thread joined." << std::endl;

		modelThread.join();
		std::cout << "Model thread joined. Simulation ending." << std::endl;
	}

	void Controller::run()
	{
		runThreads();
		while(run_)
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
		std::cout << "Start event." << std::endl;
		timer_.start();
	}

	void Controller::Process(EventStop&)
	{
		std::cout << "Stop event." << std::endl;
		timer_.stop();
	}

	void Controller::Process(EventRestart&)
	{
		std::cout << "Restart event." << std::endl;
		timer_.step(0);
		model_.start();
	}

	void Controller::Process(EventClose&)
	{
		std::cout << "Close event." << std::endl;
		run_ = false;
	}

	void Controller::Process(EventLoop&)
	{
		std::cout << "Loop event." << std::endl;
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
				model_.dispatcher_.addCamera( Dispatcher::PCamera(new Camera(id, position, direction, angle, range, precision)) );
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
	
		try
		{
			read_xml(path.string(), pt, boost::property_tree::xml_parser::no_comments);
			//std::vector<std::pair<int, Point>> vertices;
			//std::vector<std::pair<int, int>> edges;

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

						model_.streets_.addVertex(id, position);
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

						model_.streets_.addEdge(from, to);
					}
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
						// jak nie powtorka to rob trase
					if ( model_.cars_.insert( std::pair<std::string, Model::PCar>( id, car ) ).second )
					{
						BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("track") )
						{
							Point point( vv.second.get<double>("<xmlattr>.x"),
										vv.second.get<double>("<xmlattr>.y") );

							track->addPoint(point);
						}
					}
				}
				else if ( v.second.get<std::string>("<xmlattr>.type") == "walker" )
				{
					std::string id = v.second.get<std::string>("id");
					double mspeed = v.second.get<double>("parameters.maxspeed");

					walker.reset( new Walker( id, mspeed ) );
					track.reset(new WalkerTrack());
					walker->track(track);
						// jak nie powtorka to rob trase
					if ( model_.walkers_.insert( std::pair<std::string, Model::PWalker>( id, walker ) ).second )
					{
						BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("track") )
						{
							Point point( vv.second.get<double>("<xmlattr>.x"),
										vv.second.get<double>("<xmlattr>.y") );
							
							track->addPoint(point);
						}
					}
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