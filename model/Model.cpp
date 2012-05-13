#include "Model.hpp"
#include "Logger.hpp"
#include <boost\foreach.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>

namespace zpr
{
	Model::Model()
	{
		loadCameras(); // maybe tu lapac exceptiony odnosnie braku laku w xml ?!
		// to remove
		dispatcher_.tellMeSthAbout();
	}

	Model::~Model() {}

	void Model::loadCameras()
	{
		using boost::property_tree::ptree;
		ptree pt;
		PCamera cam;
	
		try
		{
			// standardowo ja lubie miec na pulpicie wiec z tamtad ciagne ;)
			// pewno na poczatku bedzie mozna okreslic patha , wiec model moze miec jakies modyfikowalne stringi do tego
			// czyt. czy to wszystko ladowac w konstruktorze czy moze jakos ciekawiej z kontrolera  skoro model zacznie sie
			// liczyc dopiero po jakims odpaleniu symulacji hmmm...
			read_xml("C:\\Users\\Acer\\Desktop\\cameras.xml", pt);

			BOOST_FOREACH( ptree::value_type &v, pt.get_child("dispatcher") )
			{
				cam.reset( new Camera( v.second.get<int>("id") ) );
			
				cam->setPosition( Point( v.second.get<double>("position.<xmlattr>.x"),
										 v.second.get<double>("position.<xmlattr>.y") ) );
				cam->setDirection( v.second.get<double>("direction") / 360.0 );	// czy na radiany od razu ?
				cam->setAngle( v.second.get<double>("angle") / 360 );
				cam->setRange( v.second.get<double>("range") );
				cam->setPrecision( v.second.get<double>("precision") );

				dispatcher_.addCamera(cam);
			}
		}	// TODO sprytnie jakos obsluzyc te exceptiony
		catch (boost::property_tree::xml_parser_error&)
		{
			Logger::getInstance().message("No files with cameras !!");
		}
		catch (boost::property_tree::ptree_bad_path&)
		{
			Logger::getInstance().message("Error while reading tree nodes: node doesn't exist !");
		}
		catch (std::exception &e)
		{
			// tu brakuje javowebo e.what().toString() + "dupa i moge co chce :/" - build stringa i append cos sensownego?
			// w ogole jak wejdzie do ktorego exceptiona to czy dalej w ogole jest sens cos robic :P
			Logger::getInstance().message(e.what());
		}
	}
}