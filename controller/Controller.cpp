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

namespace zpr
{
	Timer::Timer(const Timer::Duration & frequency) : frequency_(frequency)
	{}

	void Timer::AddListener(boost::function<void ()> listener)
	{
		listeners_.push_back(listener);
	}

	Timer::TimePoint Timer::Now() const
	{
		return boost::chrono::high_resolution_clock::now();
	}

	Timer::Duration Timer::Elapsed(const TimePoint & since) const
	{
		return Now() - since;
	}

	void Timer::operator()()
	{
		/*DEBUG*/ std::cout << "Timer starts." << std::endl;
		while(!boost::this_thread::interruption_requested())
		{
			/**/ TimePoint prev = Now();
			TimePoint nextTick = Now() + frequency_;
			while(nextTick > Now())
			{
				boost::this_thread::yield();
				boost::this_thread::sleep(boost::posix_time::milliseconds(1)); // DEBUG! tylko zeby zmniejszyc zuzycie procka, trzeba ladniej wymyslic
			}

			/*DEBUG*/// std::cout << "Timer:\t" << boost::chrono::duration_cast<boost::chrono::microseconds>(Now() - prev) << " -> " << Now() << std::endl;

			BOOST_FOREACH(boost::function<void ()> & current, listeners_)
				current();
		}
		/*DEBUG*/ std::cout << "Timer gracefully ends." << std::endl;
	}

	Controller::Controller(const boost::filesystem::path & path) : view_(model_)
	{
		//logger tez bezie przechowywal przebieg dzialania aplikacji -> bledy jakie ludek zrobil w trakcie

		std::string paths[] = { "xml_data/dispatcher.xml", "xml_data/streets.xml", "xml_data/objects.xml" };
		std::string mesgs[] = { "Retype path (ex. '", "') or type 'q' to exit:\n\t", "No matching path like: " };
		
		// to na razie
		for (int i = 0; i < 3; ++i)
		{
			while ( !boost::filesystem::exists(path / paths[i]) )
			{
				std::string msg = mesgs[2];
				msg.append(paths[i]);

				Logger::getInstance().message(msg);

				std::cout<<mesgs[0]<<paths[i]<<mesgs[1];
				std::cin>>paths[i];

				if (paths[i] == "q")
				{
					msg = "Quit without starting application while trying to load ";
					msg.append(paths[i]);
					throw(std::exception(msg.c_str()));
				}
			}
		}

		// punkt wyjscia z programu jest jeden, zandych exit(0); stad wyjatki poleca do maina
		parseDispatcher(path / paths[0]);
		parseMap(path / paths[1]);
		parseObjects(path / paths[2]);
				
		mainLoop();
	}

	void Controller::mainLoop()
	{
		//// tu mamy wczytane niby wsio, trza upieknic komunikacje pomiedzy modulami,
		//// wypada uruchomic jakos timera i zeby rozpoczynal symulacje gdzies itp
		//// ogolnie to czytanie tych xmli , fabryka, tworzenie obiektow do modelu trzeba pokminic i ladnie to zrobic

		// po wczytaniu recznym mozliwosc wyklikania wlasego i start aplikacji -> tak to widze zeby nie komplikowac
		// a wczytanie zapetlic az beda jakies obiekty

		model_.start();	// ustawienie na poczatku - tylko test pozycjonowania
		//view_.model(&model_);
		
		//View view(model_);
		viewThread = boost::thread(boost::ref(view_));
		modelThread = boost::thread(boost::ref(model_));
				
		Timer mainTimer(boost::chrono::milliseconds(10));
		mainTimer.AddListener(boost::bind(&Model::scheduleUpdate, &model_));
		mainTimer.AddListener(boost::bind(&View::scheduleRefresh, &view_));
		timer = boost::thread(mainTimer);

		viewThread.join(); //// ends with user request
		std::cout << "View thread joined." << std::endl;
		timer.interrupt();
		timer.join();
		std::cout << "Timer thread joined." << std::endl;
		modelThread.interrupt();
		modelThread.join();
		std::cout << "Model thread joined. Simulation ending." << std::endl;
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

					// jak nie powtorka to rob trase
					if ( model_.cars_.insert( std::pair<std::string, Model::PCar>( id, car ) ).second )
					{
						BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("track") )
						{
							Point point( vv.second.get<double>("<xmlattr>.x"),
										vv.second.get<double>("<xmlattr>.y") );

							model_.cars_[id]->addTrackPoint(point);
						}
					}
				}
				else if ( v.second.get<std::string>("<xmlattr>.type") == "walker" )
				{
					std::string id = v.second.get<std::string>("id");
					double mspeed = v.second.get<double>("parameters.maxspeed");

					walker.reset( new Walker( id, mspeed ) );
					// jak nie powtorka to rob trase
					if ( model_.walkers_.insert( std::pair<std::string, Model::PWalker>( id, walker ) ).second )
					{
						BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("track") )
						{
							Point point( vv.second.get<double>("<xmlattr>.x"),
										vv.second.get<double>("<xmlattr>.y") );

							model_.walkers_[id]->addTrackPoint(point);
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