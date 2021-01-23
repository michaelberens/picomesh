# picomesh
compact c++ header-only half-edge mesh library

## Features

I like small and portable bits of code that can be used without engineering dependencies and build files, so I often stick with small, self-contained bits of header-only code. With **picomesh**, I want to create a tiny library that supplies a fast but readable implementation of a *Half-Edge Mesh*, also known as a [Doubly Connected Edge List](https://en.wikipedia.org/wiki/Doubly_connected_edge_list) data structure.

Currently, the tiny thing is not feature complete but the interface is defined. So the completed and work-in-progress features are:

- [x] Half Edge triangle mesh with approachable interface
- [ ] Half Edge triangle mesh construction from triangle soup
- [ ] Attaching arbitrary data to Nodes, Edges or Faces
- [ ] Comfortable interfaces for iterating/processing rings, stars on the mesh 
  
Additionally, I want to implement self-contained "support" headers that offer additional functionality, without depending on the base header (via template-based interface definitions, supplied by the user). Features planned for these are:

- [ ] Approximate Delaunification via edge flipping
- [ ] Constrained triangulation of point clouds
- [ ] Calculation of discrete differential operators (e.g. Laplace-Beltrami) or integrals and differentials (divergence, rotation, gradient)
- [ ] Quasi-Isometric Parameterization (heat method)
  
