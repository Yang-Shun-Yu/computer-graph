#include <iostream>

#include "context.h"
#include "program.h"
using namespace std;

bool BasicProgram::load() {
  programId = quickCreateProgram(vertProgramFile, fragProgramFIle);

  int num_model = (int)ctx->models.size();
  cout << "num_model : " << num_model << endl;
  VAO = new GLuint[num_model];

  /* TODO#2-2: Pass model vertex data to vertex buffer
   *           1. Generate and bind vertex array object (VAO) for each model
   *           2. Generate and bind three vertex buffer objects (VBOs) for each model
   *           3. Pass model positions, normals and texcoords to VBOs
   * Note:
   *           1. You may refer to Programs/example.cpp and example.vert
   *           2. You may checkout basic.vert to get location index first
   *           3. You can get model data from ctx->models[i]->{posisions,normals,texcoords}
   * Keywords:
   *           - glGenVertexArrays
   *           - glBindVertexArray
   *           - glGenBuffers
   *           - glBufferData
   *           - glEnableVertexAttribArray
   *           - glVertexAttribPointer
   */
  glGenVertexArrays(num_model, VAO);
  for (int i = 0; i < num_model; i++) {
    // bind VAO
    glBindVertexArray(VAO[i]);

    // get model
    Model* model = ctx->models[i];

    // combine positions, normals, textures to one vector
    std::vector<float> combined;
    for (int j = 0; j < model->numVertex; j++) {
      combined.push_back(model->positions[j * 3]);
      combined.push_back(model->positions[j * 3 + 1]);
      combined.push_back(model->positions[j * 3 + 2]);
      combined.push_back(model->normals[j * 3]);
      combined.push_back(model->normals[j * 3 + 1]);
      combined.push_back(model->normals[j * 3 + 2]);
      combined.push_back(model->texcoords[j * 2]);
      combined.push_back(model->texcoords[j * 2 + 1]);
    }

    // generate and bind VBO
    GLuint VBO[1];
    glGenBuffers(1, VBO);

    // pass data to buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * combined.size(), combined.data(), GL_STATIC_DRAW);

    // set attributes
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // texture
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  }


  return programId != 0;
}

void BasicProgram::doMainLoop() {
  /* TODO#2-3: Render objects with shader
   *           1. use and bind program (BasicProgram::programId)
   *           2. Iterate all objects (ctx->objects)
   *           3. Load currect model VAO for object
   *           4. Pass projection, view, model matrix to shaders
   *           5. Pass model texture to shaders
   *           6. Draw with glDrawArrays
   * Note:
   *           1. You may refer to Programs/example.cpp and example.vert
   *           2. You may checkout definition of Context class, Model class and Object class first
   *           3. You can get object class from ctx->objects[i]
   *           4. You can get texture index of Model::textures for current object by Object::textureIndex
   *           5. you may also need model->drawMode, model->numVertex
   * Keywords:
   *           - glUseProgram
   *           - glBindVertexArray
   *           - glGetUniformLocation
   *           - glUniformMatrix4fv
   *           - glActiveTexture
   *           - glBindTexture
   *           - glUniform1i
   *           - glDrawArrays
   */

  glUseProgram(programId);
  int obj_num = (int)ctx->objects.size();
  cout << "obj_num : " << obj_num << endl;
  
  for (int i = 0; i < obj_num; i++) {
    int modelIndex = ctx->objects[i]->modelIndex;
    glBindVertexArray(VAO[modelIndex]);

    Model* model = ctx->models[modelIndex];
    const float* p = ctx->camera->getProjectionMatrix();
    GLint pmatLoc = glGetUniformLocation(programId, "Projection");
    glUniformMatrix4fv(pmatLoc, 1, GL_FALSE, p);

    const float* v = ctx->camera->getViewMatrix();
    GLint vmatLoc = glGetUniformLocation(programId, "ViewMatrix");
    glUniformMatrix4fv(vmatLoc, 1, GL_FALSE, v);

    const float* m = glm::value_ptr(ctx->objects[i]->transformMatrix * model->modelMatrix);
    GLint mmatLoc = glGetUniformLocation(programId, "ModelMatrix");
    glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, m);

    glUniform1i(glGetUniformLocation(programId, "ourTexture"), 0);
    glBindTexture(GL_TEXTURE_2D, model->textures[ctx->objects[i]->textureIndex]);
    glDrawArrays(model->drawMode, 0, model->numVertex);


  }

  
  glUseProgram(0);
}