###RULES####
All data is divided into sections, the names of the sections correspond to what is described in
these sections. There are three types of sections in the file.
1. Vertices is an array of coordinates of the vertices of points-nodes of the grid.
2. Edges – an array of grid edges. For each edge, the indexes of the vertices of the Vertices array that are included in it are stored.
3. Faces – an array of surfaces. All surfaces are triangles, there are three numbers in each line, the number shows the number of the edge from the Edges array
  forming the triangle. If the number is negative, it means that the edge
  it is reoriented, that is, instead of being drawn from 1 to 2 points, an edge is drawn from 2 to 1 point and its index is taken positive and
  one less.
  For example, in the cube file.grd in the Faces section there is such a surface: "-14 2 7". This means that edge 13 is taken first, and its start and end
  points are swapped, the other edges (2 and 7) are drawn as they should.
4. Locations – an array showing the position of the corresponding surface
  from the Faces array: 'i' means that the surface is located inside the shape,
  and 'e' means that it is outside.

###Functional
1. zoom in/out – press the "+"/"-" keys
2. Esc key – exit the program.
3. the program reads the data from the file and outputs to the console information about the found maximum and minimum values for all axes
4. Bounding guide shapes are drawn at the points of minimum and maximum coordinates. The color for the guide is blue for minimum values, red for maximum values.
5. When you press the "M" key, the drawing mode of the shape should change: instead of filling, the drawing is performed using the Edges array
6. Grid drawing is enabled/disabled by pressing the space bar.
7. F7 - alternate rotation around the axes
8. Esc - выход
