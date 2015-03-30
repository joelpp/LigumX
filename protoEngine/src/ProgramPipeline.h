#ifndef PROGRAMPIPELINE_H
#define PROGRAMPIPELINE_H

#include "GL/glew.h"
//#include <initializer_list>
#include <string>

class ProgramPipeline
{
public:
    class ShaderProgram
    {
    public:
        GLenum shaderType;
        GLuint glidShaderProgram;
    public:
        ShaderProgram();
        ShaderProgram(GLenum shaderType,
                      //std::initializer_list<std::string> srcFilenames,
                      std::string srcFilename,
                      bool readSrcFilenamesAsSourceCode = false);
        ~ShaderProgram() {}
    };

public:
//private:
    ShaderProgram  *pVertexShader,
                   *pTessControlShader,
                   *pTessEvalShader,
                   *pGeometryShader,
                   *pFragmentShader,
                   *pComputeShader;
    GLuint glidProgramPipeline;
    GLuint glidVao;

public:
    ProgramPipeline();
//    void useShaders(std::initializer_list<ShaderProgram*> shaders);
    void useVertexShader(ShaderProgram* shader);
    void useFragmentShader(ShaderProgram* shader);
    void removeShader(GLenum shaderStage);
    static GLbitfield sShaderTypeEnumToBitField(GLenum shaderType);
    ShaderProgram* getShader(GLenum shaderType);

    void usePipeline();

};

#endif // PROGRAMPIPELINE_H
