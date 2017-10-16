#ifndef ProgramPipeline_H
#define ProgramPipeline_H

#include "GL.h"
#include "glm/glm.hpp"
#include "Logging.h"
#include <string>
#include <sstream>

class ProgramPipeline
{

public:
    class ShaderProgram
    {
    public:
        GLenum shaderType;
        GLuint glidShaderProgram;

        ShaderProgram();
        ShaderProgram(GLenum shaderType,
                      //std::initializer_list<std::string> srcFilenames,
                      std::string srcFilename,
                      bool readSrcFilenamesAsSourceCode = false);
        ~ShaderProgram() {}
    };

    static std::string ShadersPath;

    ShaderProgram  *pVertexShader,
                   *pTessControlShader,
                   *pTessEvalShader,
                   *pGeometryShader,
                   *pComputeShader;
    ShaderProgram *pFragmentShader;
    GLuint glidProgramPipeline;
    GLuint glidVao;

    ProgramPipeline();
    ProgramPipeline(std::string name, bool isCompute = false);
	void useComputeShader(ShaderProgram* shader);
	void useVertexShader(ShaderProgram* shader);
    void useGeometryShader(ShaderProgram* shader);
    void useFragmentShader(ShaderProgram* shader);
	void useTessellationShader(ProgramPipeline::ShaderProgram *evaluationShader, ProgramPipeline::ShaderProgram *controlShader);
    void removeShader(GLenum shaderStage);
    void setUniform(std::string name, glm::vec3 color);
    static GLbitfield sShaderTypeEnumToBitField(GLenum shaderType);
    ShaderProgram* getShader(GLenum shaderType);

    void usePipeline();

private:
    std::string m_name;
    REGISTERCLASS(ProgramPipeline);
};

#endif // ProgramPipeline_H
