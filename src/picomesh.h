#ifndef PICOMESH_H_
#define PICOMESH_H_

#include <vector>


namespace pmesh {


/*
template<typename Index_T = std::size_t>
class HalfEdgeIterator
{
private:
	Index_T offset;						//< position in HalfEdgeMesh edge array
	HalfEdgeMesh &parent;				//< 

public:
	HalfEdgeIterator(HalfEdgeMesh)
};
*/

template<typename Index_T = std::size_t>		//< TODO: template metaprogramming check ::is_integer ::is_signed
class HalfEdgeMesh
{
public:
	//forward declaration
	class Node;
	class HalfEdge;
	class Face;

private:

	std::vector<HalfEdge> m_edges;		//< all edge connection data
	std::vector<Face> m_faces;			//< all face connection data

public:

	// contains a link back into the edge structure
	class Node
	{
		Index_T org_edge;	//< one half edge originating from this vertex
	};

	// contains data and methods related to traversing edges
	class HalfEdge
	{
	private:

		friend class HalfEdgeMesh<Index_T>;

		Index_T org_vertex;			//< vertex the half edge originates from

		Index_T next_edge;			//< index of next half edge
		Index_T twin_edge;			//< index of twin half edge
		Index_T left_face;			//< face 'left' of the edge (face edges CCW)

		// returns this half edge's own index
		Index_T index() { return this->twin()->twin_edge; }

	public:

		// return next half edge by reference
		HalfEdge& next() { return m_edges[next_edge]; }
		
		// return twin half edge by reference
		HalfEdge& twin() { return m_edges[twin_edge]; }

		// return previous half edge by reference
		HalfEdge& prev() { return this->twin().next().twin(); }

		bool operator==(HalfEdge& b) const
		{
			return (this->index() == b.index()) ? true : false;
		}
	};

	class Face
	{
		Index_T edge;
	};

	HalfEdgeMesh() 
	{

	}

};

} // namespace pmesh

#endif  // PICOMESH_H_
