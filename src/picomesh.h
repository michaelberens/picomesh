#ifndef PICOMESH_H_
#define PICOMESH_H_

#include <vector>
#include <memory>

namespace pmesh {


template<typename Index_T = std::size_t>        //< TODO: template metaprogramming check ::is_integer ::is_signed
class HalfEdgeMesh
{
public:
    //forward declarations
    class Node;
    class HalfEdge;
    class Face;

private:

    std::vector<Node>     m_nodes;         //< all nodes of the mesh
    std::vector<HalfEdge> m_edges;         //< all edge connection data
    std::vector<Face>     m_faces;         //< all face connection data

    struct DataRecord                       //< descriptor of data attached to the mesh graph
    {
        enum DataLocation : size_t          //< size_t for alignment reasons
        {
            PMESH_NODE_DATA = 0,
            PMESH_EDGE_DATA = 1,
            PMESH_FACE_DATA = 2
        };

        std::shared_ptr<void> m_data_ptr;  //< pointer to user supplied data
        DataLocation m_attach_at;           //< is data attached to nodes/edges/faces?
        size_t m_stride;                    //< sizeof one element of the data
        size_t m_size;                      //< size of the aux data in 'stride' elements
    };

    std::vector<DataRecord> m_data;        //< descriptors of data attached to elements
                                            //  of the half-edge mesh

    const Index_T outside_face = 0;

public:

    // contains a link back into the edge structure
    class Node
    {
    private:

        friend class HalfEdgeMesh<Index_T>;

        Index_T m_org_edge = 0;    //< one half edge originating from this vertex

        Index_T m_index = 0;    //< index of the node in m_nodes array

    public:

        // returns this half edge's own index
        Index_T index() { return this->m_index; }

        // returns the data attached to this half edge
        template<typename Data_T>
        Data_T data(Index_T data_idx)
        {
            return std::static_pointer_cast<Data_T>(m_data[data_idx].m_data_ptr)[m_index];
        }

        // returns a reference to an edge originating at the node
        HalfEdge& edge() { return m_edges[m_org_edge]; }
    };

    // contains data and methods related to traversing edges
    class HalfEdge
    {
    private:

        friend class HalfEdgeMesh<Index_T>;

        Index_T m_org = 0;         //< vertex the half edge originates from

        Index_T m_next = 0;        //< index of next half edge
        Index_T m_twin = 0;        //< index of twin half edge
        Index_T m_left = 0;        //< face 'left' of the edge (face edges CCW)

        Index_T m_index = 0;       //< index of the edge in m_edges array

        // returns this half edge's own index
        Index_T index() { return this->m_index; }

        // returns the data attached to this half edge
        template<typename Data_T>
        Data_T data(Index_T data_idx)
        {
            return std::static_pointer_cast<Data_T>(m_data[data_idx].m_data_ptr)[m_index];
        }

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

        // flip edge (no operation if edge or twin is boundary edge, or adjacent faces aren't triangles)
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

        Index_T m_face_edge = 0;        //< one inner edge of the face
        Index_T m_index = 0;            //< index of this face in m_faces

    public:

        // returns this face's own index
        Index_T index() { return m_index; };

        // returns data associated with the face
        template<typename Data_T>
        Data_T data(Index_T data_idx)
        {
            return std::static_pointer_cast<Data_T>(m_data[data_idx].m_data_ptr)[m_index];
        }

        // returns a reference to one of the face's inner edges
        HalfEdge& edge() { return m_edges[m_face_edge]; }

        // returns the number of edges that make up the face
        Index_T N_edges()
        {
            Index_T result = 0;

            HalfEdge sbegin = m_edges[m_face_edge];
            HalfEdge sedge  = m_edges[m_face_edge];

            do
            {
                sedge = sedge.next();
            }
            while (sedge != sbegin);

            return result;
        }
    };

    HalfEdgeMesh() 
    {

    }

    ~HalfEdgeMesh()
    {
    
    }

    // attach a data array to parts of the mesh (nodes, edges, faces), returns an index for the data
    template<typename Data_T>
    Index_T attach(std::shared_ptr<Data_T> data_ptr, Index_T data_size, typename DataRecord::DataLocation attach_at)
    {
        DataRecord tmp;
        tmp.m_data_ptr = data_ptr;
        tmp.m_attach_at = attach_at;
        tmp.m_stride = sizeof(Data_T);
        tmp.m_size = data_size;

        m_data.push_back(tmp);

        return m_data.size() - 1;
    }

    // return a shared pointer to the attached data array with index 'data_idx'
    template<typename Data_T>
    std::shared_ptr<Data_T> data(Index_T data_idx)
    {
        return std::static_pointer_cast<Data_T>(m_data[data_idx].m_data_ptr);
    }

    // detaches the data with index 'data_idx' from the object
    template<typename Data_T>
    void detach(Index_T data_idx)
    {
        // TODO: do proper cleanup
        m_data[data_idx].m_data_ptr.reset();
    }

private:



};

} // namespace pmesh

#endif  // PICOMESH_H_
