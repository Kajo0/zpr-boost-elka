#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Base.hpp"
#include <boost\shared_ptr.hpp>
#include <map>
//#include <vector>

namespace zpr
{
	class Graph
	{
		friend class View; // tylko pomocnicze do view
		// Mialem kilka koncepcji, ale nie widzialem jak to ma wygladac i sie wkurzylem robieniem tego
		// i w ogole zostawiam to tak na razie, tgylko jedne krawedzie, bo w sumie tylko one sa nam potrzebne
		// i jak bedzie cos trzeba zmienic to potem, bo trzeba zaczac cos robic konkretnego
		
		struct Vertex;
		//struct Edge;

		public:
		typedef boost::shared_ptr<Vertex> PVertex;
		//typedef boost::shared_ptr<Edge> PEdge;
		typedef std::map<int, PVertex> MVertices;

		private:
		MVertices vertices_;

		public:
		Graph();
		virtual ~Graph();

		void addVertex(const int id, const Point position);
		void addEdge(const int from, const int to);

		private:
		struct Vertex
		{
			friend class View; // tylko pomocnicze do view
			const int id_;
			const Point position_;
			MVertices edges_;

			Vertex(const int id, const Point position);
			virtual ~Vertex();

			void addEdge(const PVertex);
		};

		//struct Edge
		//{
		//	const PVertex from_;
		//	const PVertex to_;
		//	
		//	Edge(const PVertex, const PVertex);
		//	virtual ~Edge();

		//	bool operator==(const Edge&);
		//};
	};

	
}

#endif // GRAPH_HPP