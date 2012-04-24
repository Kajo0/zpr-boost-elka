/*
	Tu sa moje lopatologiczne testy, np jak wczytac z xmla jest
	jak dla mnie w mainie sie sieknie watki modelu view i timera
	i bedzie sobie smigac tamto
*/

#include <iostream>
#include <string>
#include <boost\lexical_cast.hpp>
#include <boost\thread\thread.hpp>

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\foreach.hpp>

#include <vector>
#include <boost\shared_ptr.hpp>

#include "Logger.hpp"
using namespace std;

void loggerTest();
void thread();


class Car{
	typedef vector<pair<float, float>> VP;
	VP track_;
	const string registration_;
public:
	Car(string &reg): registration_(reg) {}
	void addPoint(float x, float y) {
		track_.push_back(make_pair<float, float>(x, y));
	}
	void printTrack() {
		cout<<registration_<<":\n";
		for (VP::const_iterator it = track_.begin(); it != track_.end(); ++it)
			cout<<"\tx: "<<it->first<<"\ty: "<<it->second<<"\n";
	}
	~Car() {
		cout<<"Niszczymy "<<registration_<<"!\n";
	}
};

typedef boost::shared_ptr<Car> PC;
vector<PC> loadCars();

int main() {
	

	//using boost::property_tree::ptree;

	//ptree pt;
	//string s, t;

	//try {
	//	read_xml("sample_xml/sample.xml", pt);
	//	s = pt.get<string>("debug.filename");
	//	t = pt.get<string>("debug.filename.<xmlattr>.sthValue");
	//	cout<<"\t"<<s<<" z atr: "<<t<<"\n";

	//	BOOST_FOREACH(ptree::value_type &v, pt.get_child("debug.modules")) {
	//		cout<<v.second.data()<<"\t";
	//		cout<<v.second.get("<xmlattr>.sth", 0)<<"\n";
	//	}

	//} catch (boost::property_tree::xml_parser_error &e) {
	//	cout<<"No file\n";
	//} catch (boost::property_tree::ptree_bad_path &e) {
	//	cout<<e.what()<<"\n\n";
	//}

	{
		vector<PC> v;
		v = loadCars();
		for (vector<PC>::const_iterator it = v.begin(); it != v.end(); ++it)
			(**it).printTrack();
	}
	


	system("PAUSE");

	return 0;
}

vector<PC> loadCars() {
	using boost::property_tree::ptree;

	ptree pt;
	string s, t;

	vector<PC> vc;
	PC c;

	try {
		read_xml("sample_xml/cars.xml", pt);


		BOOST_FOREACH(ptree::value_type &v, pt.get_child("cars")) {

			//cout<<v.second.get<string>("registration")<<":\n";
			c.reset(new Car(v.second.get<string>("registration")));

			BOOST_FOREACH(ptree::value_type &vt, v.second.get_child("track")) {

				//cout<<"\tx: "<<vt.second.get("<xmlattr>.x", 0.0f)
				//	<<"\ty: "<<vt.second.get("<xmlattr>.y", 0.0f)<<"\n";
				c->addPoint(vt.second.get("<xmlattr>.x", 0.0f),
							vt.second.get("<xmlattr>.y", 0.0f));

			}

			vc.push_back(c);

		}


		//s = pt.get<string>("cars");
		//t = pt.get<string>("debug.filename.<xmlattr>.sthValue");
		//cout<<"\t"<<s<<" z atr: "<<t<<"\n";

		//BOOST_FOREACH(ptree::value_type &v, pt.get_child("debug.modules")) {
		//	cout<<v.second.data()<<"\t";
		//	cout<<v.second.get("<xmlattr>.sth", 0)<<"\n";
		//}

	} catch (boost::property_tree::xml_parser_error &e) {
		cout<<"No file\n";
	} catch (boost::property_tree::ptree_bad_path &e) {
		cout<<"No node\n";
	} catch (exception &e) {
		cout<<e.what()<<"___other___\n";
	}


	return vc;
}

void loggerTest() {
	Logger::getInstance().message("Let's start!");
	Logger::getInstance().message("Let's start!");
	Logger::getInstance().message("Let's start!");
	Logger::getInstance().message("Let's start!");
	Logger::getInstance().message("Let's start!");

	string a = "y AJIu KoIIIKa :P";
	Logger::getInstance().message(a);

	//for (int i = 0; i <100000; i++)
	//	Logger::getInstance().message(boost::lexical_cast<string>(i));

	boost::thread t1(thread);
	boost::thread t2(thread);
	boost::thread t3(thread);
	boost::thread t4(thread);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
}

void thread() {
	std::string str;
	for (int i = 0; i < 5; ++i) { 
		str = "Thread ";
		str += boost::lexical_cast<string>(boost::this_thread::get_id());
		str += ": ";
		str += boost::lexical_cast<string>(i);
		str += "!";

		Logger::getInstance().message(str);
	}
}