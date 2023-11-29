#include "model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <glm/vec3.hpp>
using namespace std;

Model* Model::fromObjectFile(const char* obj_file) {
  Model* m = new Model();

  std::ifstream ObjFile(obj_file);

  if (!ObjFile.is_open()) {
    std::cout << "Can't open File !" << std::endl;
    return NULL;
  }

  /* TODO#1: Load model data from OBJ file
   *         You only need to handle v, vt, vn, f
   *         Other fields you can directly ignore
   *         Fill data into m->positions, m->texcoords m->normals and m->numVertex
   *         Data format:
   *           For positions and normals
   *         | 0    | 1    | 2    | 3    | 4    | 5    | 6    | 7    | 8    | 9    | 10   | 11   | ...
   *         | face 1                                                       | face 2               ...
   *         | v1x  | v1y  | v1z  | v2x  | v2y  | v2z  | v3x  | v3y  | v3z  | v1x  | v1y  | v1z  | ...
   *         | vn1x | vn1y | vn1z | vn1x | vn1y | vn1z | vn1x | vn1y | vn1z | vn1x | vn1y | vn1z | ...
   *           For texcoords
   *         | 0    | 1    | 2    | 3    | 4    | 5    | 6    | 7    | ...
   *         | face 1                                  | face 2        ...
   *         | v1x  | v1y  | v2x  | v2y  | v3x  | v3y  | v1x  | v1y  | ...
   * Note:
   *        OBJ File Format (https://en.wikipedia.org/wiki/Wavefront_.obj_file)
   *        Vertex per face = 3 or 4
   */
  string line = "";// ues to store every line read from the ObjFile
  string prefix = "";// use to store the prefix for ever line  
  stringstream ss;
  vector<float> v, vt, vn;
  // Read each line from ObjFile
  while (getline(ObjFile, line)) {
    ss.clear();
    ss.str(line);
    ss >> prefix;

    // Process vertex coordinates
    if (prefix == "v") {
      float vx, vy, vz;
      ss >> vx >> vy >> vz;
      v.push_back(vx);
      v.push_back(vy);
      v.push_back(vz);
      // Process texture coordinates
    } else if (prefix == "vt") {
      float vtx, vty;
      ss >> vtx >> vty;
      vt.push_back(vtx);
      vt.push_back(vty);
      // Process normal vectors
    } else if (prefix == "vn") {
      float vnx, vny, vnz;
      ss >> vnx >> vny >> vnz;
      vn.push_back(vnx);
      vn.push_back(vny);
      vn.push_back(vnz);
      // Process face data
    } else if (prefix == "f") {
      string tmp;
      while (ss >> tmp) {
        istringstream iss(tmp);
        int v_index, vt_index, vn_index;
        char delimiter;
        if (iss >> v_index >> delimiter >> vt_index >> delimiter >> vn_index) {
          m->positions.push_back(v[(v_index - 1) * 3]);
          m->positions.push_back(v[(v_index - 1) * 3 + 1]);
          m->positions.push_back(v[(v_index - 1) * 3 + 2]);
          m->texcoords.push_back(vt[(vt_index - 1) * 2]);
          m->texcoords.push_back(vt[(vt_index - 1) * 2 + 1]);
          m->normals.push_back(vn[(vn_index - 1) * 3]);
          m->normals.push_back(vn[(vn_index - 1) * 3 + 1]);
          m->normals.push_back(vn[(vn_index - 1) * 3 + 2]);
          m->numVertex++;
        }
      }
    }
  }


  if (ObjFile.is_open()) ObjFile.close();

  return m;
}
