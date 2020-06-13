#pragma once

#include "LXSystem.h"
#include "GFXUniformGroup.h"
#include "GL.h"

// todo jpp : This assumes all include files in shaders are in the same block without in between lines
/* todo jpp : Not sure this supports includes in includes.
// Should this be -> "ShaderFile" is any glsl file, it has a list of ShaderFileInclude, when loading a file resolve all includes from the bottom up
// e.g. : Basic
            Lighting
                Utils
            
First create a ShaderFile for Utils. Then create the ShaderFile for Lighting, using the resolved Utils.
Finally create Basic - concatenate somehow all the resolved shaderfiles.
*/

struct ShaderFileInclude
{
    ShaderFileInclude(const LXString& filename, int start, int numLines)
        : m_Filename(filename)
        , m_LineStart(start)
        , m_NumLines(numLines)
    {
    }
    LXString m_Filename = "";
    int m_LineStart = 0;
    int m_NumLines = 0;
};


class ProgramPipeline
{

public:
    class ShaderProgram
    {
    public:
        GLenum shaderType;
        GLuint glidShaderProgram;

        ShaderProgram();
        ~ShaderProgram() {}

		int m_NumLinesInInclude = 0;
		bool Initialize(GLenum shaderType, LXString& name, std::string srcFilenames, bool readSrcFilenamesAsSourceCode, LXVector<UniformGroupType>& uniformGroups);

        std::vector<ShaderFileInclude> m_FileIncludes;

    };

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

	bool m_IsValid = false;

	GFXUniformGroup* GetUniformGroup(UniformGroupType type);
private:
    std::string m_name;
    REGISTERCLASS(ProgramPipeline);

	//LXMap<LXString, GFXUniformGroup> m_UniformGroups;
    LXVector<GFXUniformGroup> m_UniformGroups;

};

