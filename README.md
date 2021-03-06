# picomesh
compact c++ header-only half-edge mesh library

## Features

I like small and portable bits of code that can be used without engineering dependencies and build files, so I often stick with small, self-contained bits of header-only code. With **picomesh**, I want to create a tiny library that supplies a fast but readable implementation of a *Half-Edge Mesh*, also known as a [Doubly Connected Edge List](https://en.wikipedia.org/wiki/Doubly_connected_edge_list) data structure.

Currently, the tiny thing is not feature complete but the interface is defined. So the completed and work-in-progress features are:

- [x] Half Edge triangle mesh with approachable interface
- [x] Attaching arbitrary data to Nodes, Edges or Faces
- [ ] Half Edge triangle mesh construction from triangle soup
- [ ] Comfortable interfaces for iterating/processing rings, stars on the mesh 
  
Additionally, I want to implement self-contained "support" headers that offer additional functionality, without depending on the base header (via template-based interface definitions, supplied by the user). Features planned for these are:

- [ ] Approximate Delaunification via edge flipping
- [ ] Constrained triangulation of point clouds
- [ ] Calculation of discrete differential operators (e.g. Laplace-Beltrami) or integrals and differentials (divergence, rotation, gradient)
- [ ] Quasi-Isometric Parameterization (heat method)
  
## Why?

Why make another library for dealing with triangle meshes? Large libraries can be either intimidating to pick up or take lots of time to extract the important features from. Dealing with the biolerplate that surrounds the useful parts can be a major time sink. Thus, I'd like to make small geometry processing 'modules' that can work together but can easily be fed data with STL containers.