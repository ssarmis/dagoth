#include "opengl_layer.h"
#include <iostream>//for printing
#include <assert.h>

// TODO(Sarmis) move implementation
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static glm::mat4 camera;
static glm::mat4 transform;

static inline BufferId openglCreateVertexArray(u32 amount){
    assert(amount);

    BufferId result;
    glGenVertexArrays(amount, &result);

    assert(glGetError() == GL_NO_ERROR);

    return result;
}

static inline BufferId openglCreateBuffer(u32 amount){
    assert(amount);

    BufferId result;
    glGenBuffers(amount, &result);

    assert(glGetError() == GL_NO_ERROR);

    return result;
}

void openglUseVertexArray(BufferId bufferId){
    glBindVertexArray(bufferId);

    // assert(glGetError() == GL_NO_ERROR); Curious about this one
}

void openglUnuseVertexArray(){
    glBindVertexArray(0);

    // assert(glGetError() == GL_NO_ERROR);
}

void openglUseBuffer(BufferId bufferId, GLenum type){
    glBindBuffer(type, bufferId);

    // assert(glGetError() == GL_NO_ERROR);
}

void openglUnuseBuffer(GLenum type){
    glBindBuffer(type, 0);

    // assert(glGetError() == GL_NO_ERROR);
}

static inline void openglAllocateBufferSize(BufferId bufferId, u32 size, GLenum type){
    assert(size);

    openglUseBuffer(bufferId, type);
    glBufferData(type, size, NULL, GL_DYNAMIC_DRAW);
    openglUnuseBuffer(type);

    // assert(glGetError() == GL_NO_ERROR);
}

RenderingObject openglCreateRenderingObjectEmpty(u32 vertexBufferSize, u32 indicesBufferSize){
    RenderingObject result;

    result.bufferIsMapped = false;
    result.vertexBufferArrayId = openglCreateVertexArray(1);
    result.vertexBufferId = openglCreateBuffer(1);
    result.indicesBufferId = openglCreateBuffer(1);
    result.usedVertices = 0;
    result.usedIndices = 0;

    openglUseVertexArray(result.vertexBufferArrayId);

    openglAllocateBufferSize(result.vertexBufferId, vertexBufferSize, GL_ARRAY_BUFFER);
    openglAllocateBufferSize(result.indicesBufferId, indicesBufferSize, GL_ELEMENT_ARRAY_BUFFER);

    openglUseBuffer(result.vertexBufferId, GL_ARRAY_BUFFER);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)(3 * sizeof(r32)));
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)(6 * sizeof(r32)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    openglUnuseBuffer(GL_ARRAY_BUFFER);

    openglUnuseVertexArray();

    return result;
}

RenderingObject openglCreateRenderingObject(u32 vertexBufferSize, u32 indicesBufferSize){
    RenderingObject result;

    result.bufferIsMapped = false;
    result.vertexBufferArrayId = openglCreateVertexArray(1);
    result.vertexBufferId = openglCreateBuffer(1);
    result.indicesBufferId = openglCreateBuffer(1);
    result.usedVertices = vertexBufferSize / sizeof(Vertex);
    result.usedIndices = indicesBufferSize / sizeof(Index);

    openglUseVertexArray(result.vertexBufferArrayId);

    openglAllocateBufferSize(result.vertexBufferId, vertexBufferSize, GL_ARRAY_BUFFER);
    openglAllocateBufferSize(result.indicesBufferId, indicesBufferSize, GL_ELEMENT_ARRAY_BUFFER);

    openglUseBuffer(result.vertexBufferId, GL_ARRAY_BUFFER);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)(3 * sizeof(r32)));
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)(6 * sizeof(r32)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    openglUnuseBuffer(GL_ARRAY_BUFFER);

    openglUnuseVertexArray();

    return result;
}

Vertex* openglMapBuffer(RenderingObject* object) {
    if (object->bufferIsMapped) {
        std::cerr << "Trying to map a buffer thats already mapped, returning nullptr" << std::endl;
        return nullptr;
    }
    object->bufferIsMapped = true;
    Vertex* result = static_cast<Vertex*>(glMapNamedBuffer(object->vertexBufferId, GL_WRITE_ONLY));
    object->buffer = result;
    return result;
}

void openglUnmapBuffer(RenderingObject* object) {
    if (!object->bufferIsMapped) {
        std::cerr << "Trying to unmap a buffer thats not mapped, return" << std::endl;
        return;
    }
    glUnmapNamedBuffer(object->vertexBufferId);
    object->bufferIsMapped = false;
    object->buffer = nullptr;
}

void openglBufferIndicesData(RenderingObject& renderingObject, u32 offset, Index* data, u32 numberOfIndices){
    glNamedBufferSubData(renderingObject.indicesBufferId, offset, numberOfIndices * sizeof(Index), (void*)data);
}

void openglBufferVertexData(RenderingObject& renderingObject, u32 offset, VertexData& data){
    glNamedBufferSubData(renderingObject.vertexBufferId, offset, data.numberOfVertecies * sizeof(Vertex), (void*)data.data);
}

void openglBufferVertex(RenderingObject& renderingObject, Vertex vertex){
    glNamedBufferSubData(renderingObject.vertexBufferId, sizeof(Vertex) * renderingObject.usedVertices, sizeof(Vertex), (void*)&vertex);
    ++renderingObject.usedVertices;
}

void openglBufferIndex(RenderingObject& renderingObject, Index index){
    glNamedBufferSubData(renderingObject.indicesBufferId, sizeof(Index) * renderingObject.usedIndices, sizeof(Index), (void*)&index);
    ++renderingObject.usedIndices;
}

void openglBufferLine(RenderingObject& renderingObject, v3 A, v3 B, v3 color){
    openglBufferVertex(renderingObject, Vertex(A, color));
    openglBufferVertex(renderingObject, Vertex(B, color));

    u32 clone = renderingObject.usedIndices;
    openglBufferIndex(renderingObject, clone);
    openglBufferIndex(renderingObject, clone + 1);
}

#include <stdio.h> // removeme

RenderingProgram openglCreateRenderProgram(){
    RenderingProgram result;

    result.vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    result.fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    
    const char* vertexShader = R"(
        #version 440 core
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec3 vertexColor;
        layout(location = 2) in vec3 vertexUV;

        uniform mat4 perspectiveMat;
        uniform mat4 transformMat;
        uniform mat4 viewMat;

        out vec3 outPosition;
        out vec3 outUV;
        out flat vec3 outColor;

        void main(){
            gl_Position = perspectiveMat * viewMat * transformMat * vec4(vertexPosition, 1.0);
            outPosition = vertexPosition;
            outUV = vertexUV;
            outColor = vertexColor;
        }
    )";

    const char *fragmentShader = R"(
        #version 440 core

        in vec3 outPosition;
        in vec3 outUV;
        in flat vec3 outColor;

        uniform sampler2D textureSampler;

        out vec4 finalFragmentColor;

        void main(){
            if(gl_FragCoord.x < 0 || gl_FragCoord.x >= 1400){
                discard;
            }
            vec2 uv2d = vec2(outUV.x, outUV.y);
            finalFragmentColor = texture2D(textureSampler, uv2d);
            // gl_FragColor = vec4(1, 0, 0, 1.0);
        }
    )";

    glShaderSource(result.vertexShaderId, 1, &vertexShader, NULL);
    glCompileShader(result.vertexShaderId);

    // TODO make the shader reports nicer

    GLint shaderStatus;
    glGetShaderiv(result.vertexShaderId, GL_COMPILE_STATUS, &shaderStatus);
    if (shaderStatus != GL_TRUE){
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetShaderInfoLog(result.vertexShaderId, 1024, &logLength, message);
        message[logLength] = 0;
        printf("%s\n", message);
    }

    glShaderSource(result.fragmentShaderId, 1, &fragmentShader, NULL);
    glCompileShader(result.fragmentShaderId);

    glGetShaderiv(result.fragmentShaderId, GL_COMPILE_STATUS, &shaderStatus);
    if (shaderStatus != GL_TRUE){
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetShaderInfoLog(result.fragmentShaderId, 1024, &logLength, message);
        message[logLength] = 0;
        printf("%s\n", message);
    }

    result.id = glCreateProgram();

    glAttachShader(result.id, result.vertexShaderId);
    glAttachShader(result.id, result.fragmentShaderId);
    glLinkProgram(result.id);

    GLint programLinked;
    glGetProgramiv(result.id, GL_LINK_STATUS, &programLinked);
    if (programLinked != GL_TRUE){
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetProgramInfoLog(result.id, 1024, &logLength, message);
        printf("%*.s\n", logLength, message);
    }


    // crash at any problem
    result.perspectiveMatrix = glGetUniformLocation(result.id, "perspectiveMat");
    assert(result.perspectiveMatrix != -1);

    result.transformMatrix = glGetUniformLocation(result.id, "transformMat");
    assert(result.transformMatrix != -1);

    result.viewMatrix = glGetUniformLocation(result.id, "viewMat");
    assert(result.viewMatrix != -1);
    
    return result;
}

void openglUseRenderProgram(RenderingProgram& program){
    glUseProgram(program.id);
}

void openglUnuseRenderProgram(){
    glUseProgram(0);
}

void openglRenderObject(RenderingObject& object, RenderingProgram& program, u32 numberOfIndices, GLenum mode){
    openglUseVertexArray(object.vertexBufferArrayId);
    openglUseBuffer(object.indicesBufferId, GL_ELEMENT_ARRAY_BUFFER);
    openglUseRenderProgram(program);

    glDrawElements(mode, numberOfIndices, GL_UNSIGNED_INT, NULL); // well not always unsinged INT
    
    openglUnuseVertexArray();
    
    openglUnuseRenderProgram();
}

void openglPerspectiveSetup(RenderingProgram& program, u32 windowWidth, u32 windowHeight){
    openglUseRenderProgram(program);

    //glm::mat4 perspective = glm::perspective<r32>(glm::radians(80.0), 1.66, 0.01, 100000.0);
    glm::mat4 perspective = glm::ortho<r32>(0, windowWidth, windowHeight, 0, 0, 1000);
    glm::mat4 identity = glm::mat4(1.0);
    
    glUniformMatrix4fv(program.perspectiveMatrix, 1, false, &perspective[0][0]);
    // maybe setup the camera here ?
    glUniformMatrix4fv(program.transformMatrix, 1, false, &identity[0][0]);
    glUniformMatrix4fv(program.viewMatrix, 1, false, &identity[0][0]);
    
    openglUnuseRenderProgram();
}

void openglTranslate(RenderingProgram& program, r32 x, r32 y, r32 z){
    openglUseRenderProgram(program);

    transform = glm::translate<r32>(glm::mat4(1.0), glm::vec3(x, y, z));

    glUniformMatrix4fv(program.transformMatrix, 1, false, &transform[0][0]);
    
    openglUnuseRenderProgram();
}

void openglRotation(RenderingProgram& program, r32 angle, glm::vec3 axis){
    openglUseRenderProgram(program);

    transform = glm::rotate(transform, angle, axis);

    glUniformMatrix4fv(program.transformMatrix, 1, false, &transform[0][0]);
    
    openglUnuseRenderProgram();
}

void openglClearTransformations(RenderingProgram& program){
    openglUseRenderProgram(program);

    transform = glm::mat4(1.0);

    glUniformMatrix4fv(program.transformMatrix, 1, false, &transform[0][0]);
    
    openglUnuseRenderProgram();
}

void openglCameraSetup(RenderingProgram& program){
    openglUseRenderProgram(program);

    /*
    camera = glm::lookAt<r32>(glm::vec3(0, 0, -1),
                              glm::vec3(0, 0, 0),
                              glm::vec3(0, 1, 0));
    */
    camera = glm::mat4();

    glUniformMatrix4fv(program.viewMatrix, 1, false, &camera[0][0]);
    
    openglUnuseRenderProgram();
}

void openglCameraRotate(RenderingProgram& program, r32 angle, glm::vec3 axis){
    openglUseRenderProgram(program);

    camera = glm::rotate(camera, angle, axis);

    glUniformMatrix4fv(program.viewMatrix, 1, false, &camera[0][0]);
    
    openglUnuseRenderProgram();
}

void openglCameraTranslate(RenderingProgram& program, glm::vec3 translation){
    openglUseRenderProgram(program);

    camera = glm::translate(camera, translation);

    glUniformMatrix4fv(program.viewMatrix, 1, false, &camera[0][0]);
    
    openglUnuseRenderProgram();
}

void openglClearCameraTransformations(RenderingProgram& program) {
    openglUseRenderProgram(program);

    camera = glm::mat4(1.0);

    glUniformMatrix4fv(program.viewMatrix, 1, false, &camera[0][0]);

    openglUnuseRenderProgram();
}

Texture openglCreateTexture(u8* data, u32 width, u32 height) {
    Texture result = {};

    glGenTextures(1, &result.textureId);
    glBindTexture(GL_TEXTURE_2D, result.textureId);
     
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return result;
}

void openglBindTexture(u32 target, Texture texture, GLuint textureSlot) {
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(target, texture.textureId);
}
#include "global_settings.hpp"
void RenderingObjectPushQuad(RenderingObject* object, glm::vec2 position, glm::vec2 texture, glm::vec2 size, u32 atlasWidth, u32 atlasHeight, glm::vec2 tileSize,  bool mirrorX, bool mirrorY) {
    r32 ax = (r32)(tileSize.x) / (r32)atlasWidth;
    r32 ay = (r32)(tileSize.y) / (r32)atlasHeight;

    r32 u0 = texture.x * ax;
    r32 u1 = u0 + ax;
    r32 v0 = texture.y * ay;
    r32 v1 = v0 + ay;
    
    if (mirrorX) {
        r32 aux = u0;
        u0 = u1;
        u1 = aux;
    }

    if (mirrorY) {
        r32 aux = v0;
        v0 = v1;
        v1 = aux;
    }

    u32 offset = object->usedVertices;

    object->buffer[offset + 0] = Vertex(position.x, position.y, 0, 1, 1, 1, u0, v0, 0);
    object->buffer[offset + 1] = Vertex(position.x, position.y + size.y, 0, 1, 1, 1, u0, v1, 0);
    object->buffer[offset + 2] = Vertex(position.x + size.x, position.y, 0, 1, 1, 1, u1, v0, 0);
    object->buffer[offset + 3] = Vertex(position.x + size.x, position.y + size.y, 0, 1, 1, 1, u1, v1, 0);

    object->usedVertices += 4;
}

Texture openglCreateTextureFromFile(const char* filename) {
    // TODO(Sarmis) add error handling
    int width;
    int height;
    int components;
    u8* data = stbi_load(filename, &width, &height, &components, 0);

    Texture result = openglCreateTexture(data, width, height);
    
    result.width = width;
    result.height = height;

    stbi_image_free(data);
    return result;
}


Index* openglGenerateQuadInGridIndices(u32 indicesAmount) {
    Index* result = new Index[indicesAmount];
    u32 offset = 0;
    for (int i = 0; i < indicesAmount; i += 6) {
        result[i + 0] = offset + 0;
        result[i + 1] = offset + 1;
        result[i + 2] = offset + 2;
        result[i + 3] = offset + 1;
        result[i + 4] = offset + 3;
        result[i + 5] = offset + 2;

        offset += 4;
    }
    return result;
}