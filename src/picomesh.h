#ifndef PICOMESH_H_
#define PICOMESH_H_

#include <vector>
#include <memory>

namespace pmesh {


template<typename Index_T = std::size_t>		//< TODO: template metaprogramming check ::is_integer ::is_signed
class HalfEdgeMesh
{
public:
	//forward declarations
	class Node;
	class HalfEdge;
	class Face;

private:

	struct DataRecord
	{
		std::shared_ptr<char> ptr;
		uint8_t dim;
        size_t stride;          //< 
        size_t size;            //< length of the aux data in 'stride' bytes
	};

	std::vector<Node>     m_nodes;			//< all nodes of the mesh
	std::vector<HalfEdge> m_edges;		    //< all edge connection data
	std::vector<Face>     m_faces;			//< all face connection data

    std::vector<DataRecord> m_data;         //< descriptors of data attached to elements
                                            //  of the half-edge mesh

    const Face outside;

public:

	// contains a link back into the edge structure
	class Node
	{
	private:

        friend class HalfEdgeMesh<Index_T>;

		Index_T m_org_edge;	//< one half edge originating from this vertex

    public:

        HalfEdge& edge() { return m_edges[m_org_edge]; }
	};

	// contains data and methods related to traversing edges
	class HalfEdge
	{
	private:

		friend class HalfEdgeMesh<Index_T>;

		Index_T m_org;			//< vertex the half edge originates from

		Index_T m_next;			//< index of next half edge
		Index_T m_twin;			//< index of twin half edge
		Index_T m_left;			//< face 'left' of the edge (face edges CCW)

		// returns this half edge's own index
		Index_T index() { return this->twin()->m_twin; }

	public:

		// return next half edge by reference
		HalfEdge& next() { return m_edges[m_next]; }
		
		// return twin half edge by reference
		HalfEdge& twin() { return m_edges[m_twin]; }

		// return previous half edge by reference
		HalfEdge& prev() { return this->twin().next().twin(); }

        // return node that the edge originates from by reference
		Node& org() { return m_nodes[org_vertex]; }

        // return node that the edge points to by reference
		Node& hed() { return next().org(); }

		bool operator==(HalfEdge& b) const
		{
			return (this->index() == b.index()) ? true : false;
		}

		// is this edge part of a mesh boundary?
		bool is_boundary()
		{
			return ((m_face[left_face] == outside_face) || (m_face[twin().left_face] == outside_face));
		}

        // flip edge (no operation if edge or twin is boundary edge)
        void flip()
        {
            if (this->is_boundary()) return;        

			// temporarily store source info (indices of faces and edges of both faces)
			Index_T face_ti = this->twin().m_left; Index_T face_i = this->m_left;
			Index_T e0 = this->index(); Index_T e1 = this->next().index(); Index_T e2 = this->next().next().index();
			Index_T f0 = this->twin().index(); Index_T f1 = this->twin().next().index(); Index_T f2 = this->twin().next().next().index();

			// reassign nodes on flipped edge
			m_edges[e0].m_org = m_edges[e2].m_org; m_edges[f0].m_org = m_edges[e1].m_org;

			// reassign next edges on the six participating half-edges
			m_edges[e0].m_next = f2; m_edges[e1].m_next = e0; m_edges[e2].m_next = f1;
			m_edges[f0].m_next = e2; m_edges[f1].m_next = f0; m_edges[f2].m_next = e1;

			// reassign adjacent faces to edges within the face
			m_faces[face_ti].m_face_edge = e0; m_faces[face_i].m_face_edge = f0;
        }

	};

	// contains a link back into the edge structure
	class Face
	{
    private:
        friend class HalfEdgeMesh<Index_T>;

		Index_T m_face_edge;

    public:

        HalfEdge& edge() { return m_edges[m_face_edge]; }


	};

	HalfEdgeMesh() 
	{

	}

	~HalfEdgeMesh()
	{

	}

    Index_T attach<Data_T>(std::shared_ptr<Data_T> data_ptr, Index_T data_size)
    {
        
    }

    void detach<Data_T>(Index_T location)
    {

    }

private:



};

} // namespace pmesh

#endif  // PICOMESH_H_
