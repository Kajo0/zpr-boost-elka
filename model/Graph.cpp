#include "Graph.hpp"

namespace zpr
{
	/************
	*	Graph	*
	************/
	Graph::Graph() {}

	Graph::~Graph() {}

	void Graph::addVertex(const int id, const Point position)
	{
		vertices_.insert( std::pair<int, PVertex>( id, PVertex( new Vertex(id, position) ) ) );
	}

	void Graph::addEdge(const int from, const int to)
	{
		if ( vertices_.find(from) != vertices_.end() && vertices_.find(to) != vertices_.end() )
		{
			vertices_[from]->addEdge( vertices_[to] );
			//vertices_[to]->addEdge( vertices_[from] );
		}
	}

	void Graph::printAllData()
	{
		for (MVertices::const_iterator it = vertices_.begin(); it != vertices_.end(); ++it)
		{
			std::cout<<"Id: "<<it->second->id_<<" Pos: ("<<it->second->position_.x_<<", "<<it->second->position_.y_<<")\n";

			for (MVertices::const_iterator i = it->second->edges_.begin(); i != it->second->edges_.end(); ++i)
			{
				std::cout<<"\tEdge to: "<<i->second->id_<<"\n";
			}
		}
	}
	
	/************
	*	Vertex	*
	************/
	Graph::Vertex::Vertex(const int id, const Point position): id_(id), position_(position)
	{
	}

	Graph::Vertex::~Vertex() {}

	void Graph::Vertex::addEdge(const Graph::PVertex vertex)
	{
		edges_.insert( std::pair<int, PVertex>(vertex->id_, vertex) );
	}

	/************
	*	Edge	*
	************/
	//Graph::Edge::Edge(const Graph::PVertex from, const Graph::PVertex to): from_(from), to_(to)
	//{
	//}

	//Graph::Edge::~Edge() {}
}