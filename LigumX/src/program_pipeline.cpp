#include "stdafx.h"

#include "program_pipeline.h"
#include "renderer.h"
using namespace std;

const std::string g_ProviderIncludeMarker = "// Include Providers Marker";

const std::string g_ProviderDefinition = R"(

#define PI 3.141592654f

#ifdef PROVIDER_Material
struct Material
{
	vec3		m_AmbientColor;
	vec3		m_DiffuseColor;
	vec3		m_SpecularColor;
	float		m_Shininess;
	sampler2D	m_DiffuseTexture;
	sampler2D	m_SpecularTexture;
	bool		m_DiffuseTextureEnabled;
	bool		m_SpecularTextureEnabled;
	bool		m_Unlit;
	float		m_EmissiveFactor;
	bool		m_IsGlass;
	float		m_RefractionIndex;
	bool		m_ReflectEnvironment;
	float		m_Metallic;
	float		m_Roughness;
	float		m_AO;
	bool		m_IsPBR;
};

uniform Material g_Material;
uniform int g_BlinnPhongShading;

#endif

#ifdef PROVIDER_Light
struct DirectionalLight
{
	vec3 m_Direction;
	vec3 m_DiffuseColor;
	vec3 m_AmbientColor;
	vec3 m_SpecularColor;
};

uniform DirectionalLight g_DirectionalLight;

struct PointLight
{
	vec3 m_Position;
	vec3 m_DiffuseColor;
	vec3 m_AmbientColor;
	vec3 m_SpecularColor;
};

uniform PointLight g_PointLight[8];

uniform int g_NumLights;

uniform int g_UseLighting;
uniform bool g_UseSkyLighting;
uniform bool g_UseShadows;
#endif

#ifdef PROVIDER_View
uniform vec3 g_CameraPosition;
uniform float g_CameraNearPlane;
uniform float g_CameraFarPlane;
#endif

#ifdef PROVIDER_Debug
uniform int g_DebugDiffuseEnabled;
uniform int g_DebugSpecularEnabled;
uniform int g_DebugAmbientEnabled;
uniform int g_DebugNormalsEnabled;
uniform int g_DebugDepthEnabled;
uniform int g_DebugUVEnabled;
uniform int g_DebugLinearizeDepth;
#endif

#ifdef PROVIDER_PostEffects
uniform int g_GammaCorrectionEnabled;
uniform float g_GammaCorrectionExponent;
uniform bool g_ToneMappingEnabled;
#endif


#ifdef PROVIDER_ShadowMap
uniform sampler2D g_DepthMapTexture;
#endif

#ifdef PROVIDER_Picking
uniform float g_PickingID;
#endif

#ifdef PROVIDER_Sky
uniform bool g_UseSkybox;
uniform samplerCube g_Skybox;
uniform float sunOrientation;
uniform float sunTime;
#endif

)";



ProgramPipeline::ShaderProgram::ShaderProgram(
        GLenum shaderType,
        //initializer_list<string> srcFilenames,
        string srcFilenames,
        bool readSrcFilenamesAsSourceCode)
{
    this->shaderType = shaderType;

    int nbSources = 1;//int(srcFilenames.size());
    const char ** sourceCodes = new const char *[nbSources];
    int count = 0;
    string* sourceCodeStrings;

    if(readSrcFilenamesAsSourceCode) {
        //for(initializer_list<string>::iterator iSrc = srcFilenames.begin();
        //        iSrc != srcFilenames.end(); iSrc++)
        //{
            //sourceCodes[count++] = iSrc->c_str();
        //}
        sourceCodes[count++] = srcFilenames.c_str();

    } else {
        // Read the Vertex Shader code from the files
        // based on http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/

        sourceCodeStrings = new string[nbSources];

        //for(initializer_list<string>::iterator iSrc = srcFilenames.begin();
        //        iSrc != srcFilenames.end(); iSrc++)
        //{
            //ifstream sourceCodeStream(*iSrc, ios::in);
            ifstream sourceCodeStream(srcFilenames, ios::in);
            if(sourceCodeStream.is_open())
            {
                string line = "";
                while(getline(sourceCodeStream, line)) {

					if (line == g_ProviderIncludeMarker)
					{
						sourceCodeStrings[count] += g_ProviderDefinition + "\n";
						continue;
					}

                    sourceCodeStrings[count] += line + "\n";

                }
                sourceCodeStream.close();
            } else {
                //cout << "Error loading shader : " << *iSrc << endl;
                cout << "Error loading shader : " << srcFilenames << endl;
				return;
            }
            sourceCodes[count] = sourceCodeStrings[count].c_str();
            count++;
        //}

    }

    GLint result;
    int infoLength;
    char* info;

    const GLuint shader = glCreateShader(shaderType);

    glShaderSource(shader, nbSources, sourceCodes, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	bool hadError = false;

    if(!result) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
        info = new char[infoLength];
        glGetShaderInfoLog(shader, infoLength, NULL, info);
        cerr << "Compiler error in shader :" << endl;
        cerr << info << endl;
		hadError = true;
	} else {
        glidShaderProgram = glCreateProgram();
        glProgramParameteri(glidShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
        glAttachShader(glidShaderProgram, shader);
        glLinkProgram(glidShaderProgram);
        glDetachShader(glidShaderProgram, shader);
        glDeleteShader(shader);

        glGetProgramiv(glidShaderProgram, GL_LINK_STATUS, &result);

        if(result != GL_TRUE) {
            glGetProgramiv(glidShaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
            info = new char[infoLength];
            glGetProgramInfoLog(glidShaderProgram, infoLength, NULL, info);
            cerr << "Linker error in shader :" << endl;
            cerr << info << endl;
			hadError = true;
        }
    }

	if (hadError)
	{
		assert(false);
	}
}

GLbitfield ProgramPipeline::sShaderTypeEnumToBitField(
        GLenum shaderType)
{
    GLbitfield shaderBit;
    switch(shaderType) {
    case GL_VERTEX_SHADER:
        shaderBit = GL_VERTEX_SHADER_BIT;
        break;
    case GL_TESS_CONTROL_SHADER:
        shaderBit = GL_TESS_CONTROL_SHADER_BIT;
        break;
    case GL_TESS_EVALUATION_SHADER:
        shaderBit = GL_TESS_EVALUATION_SHADER_BIT;
        break;
    case GL_GEOMETRY_SHADER:
        shaderBit = GL_GEOMETRY_SHADER_BIT;
        break;
    case GL_FRAGMENT_SHADER:
        shaderBit = GL_FRAGMENT_SHADER_BIT;
        break;
    case GL_COMPUTE_SHADER:
        shaderBit = GL_COMPUTE_SHADER_BIT;
        break;
    }
    return shaderBit;
}

ProgramPipeline::ShaderProgram *ProgramPipeline::getShader(GLenum shaderType)
{
    switch(shaderType) {
    case GL_VERTEX_SHADER:
        return pVertexShader;
        break;
    case GL_TESS_CONTROL_SHADER:
        return pTessControlShader;
        break;
    case GL_TESS_EVALUATION_SHADER:
        return pTessEvalShader;
        break;
    case GL_GEOMETRY_SHADER:
        return pGeometryShader;
        break;
    case GL_FRAGMENT_SHADER:
        return pFragmentShader;
        break;
    case GL_COMPUTE_SHADER:
        return pComputeShader;
        break;
    default:
        PRINT("ProgramPipeline::ShaderProgram::getShader(...) : Unknown shader type.");
        return NULL;
        break;
    }
}

void ProgramPipeline::usePipeline()
{
    glBindProgramPipeline(this->glidProgramPipeline);
    glBindVertexArray(this->glidVao);
}

ProgramPipeline::ProgramPipeline(std::string name, bool isCompute)
{
    pVertexShader = NULL;
    pTessControlShader = NULL;
    pTessEvalShader = NULL;
    pGeometryShader = NULL;
    pFragmentShader = NULL;
    pComputeShader = NULL;
    m_name = name;

    glGenProgramPipelines(1, &glidProgramPipeline);
    glBindProgramPipeline(glidProgramPipeline);
    glGenVertexArrays(1, &glidVao);
    glBindVertexArray(glidVao);

    std::stringstream path;
    path << ShadersPath;
    path << name << "/";
    
	if (isCompute)
	{
		useComputeShader(new ProgramPipeline::ShaderProgram(GL_COMPUTE_SHADER, path.str() + "compute.csh", false));

	}
	else
	{
		useVertexShader(new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER, path.str() + "vertex.vsh", false));
		useFragmentShader(new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER, path.str() + "fragment.fsh", false));
	}

	//useTessellationShader(new ProgramPipeline::ShaderProgram(GL_TESS_EVALUATION_SHADER, path.str() + "evaluation.tes", false), new ProgramPipeline::ShaderProgram(GL_TESS_CONTROL_SHADER, path.str() + "control.tcs", false));
	
}

ProgramPipeline::ProgramPipeline()
{
    pVertexShader = NULL;
    pTessControlShader = NULL;
    pTessEvalShader = NULL;
    pGeometryShader = NULL;
    pFragmentShader = NULL;
    pComputeShader = NULL;

    glGenProgramPipelines(1, &glidProgramPipeline);
    glBindProgramPipeline(glidProgramPipeline);
    glGenVertexArrays(1, &glidVao);
    glBindVertexArray(glidVao);
}

//void ProgramPipeline::useShaders(
//        std::initializer_list<ProgramPipeline::ShaderProgram *> shaders)
//{
//    for(initializer_list<ShaderProgram*>::iterator iShader = shaders.begin();
//            iShader != shaders.end(); iShader++)
//    {
//        GLbitfield shaderBit = sShaderTypeEnumToBitField((*iShader)->shaderType);

//        // make sure we don't mix compute shaders with other shader types.
//        if((*iShader)->shaderType == GL_COMPUTE_SHADER) {
//            glUseProgramStages(glidProgramPipeline, GL_ALL_SHADER_BITS, 0);
//        } else {
//            glUseProgramStages(glidProgramPipeline, GL_COMPUTE_SHADER_BIT, 0);
//        }

//        // store shader ID
//        switch((*iShader)->shaderType) {
//        case GL_VERTEX_SHADER:
//            pVertexShader = *iShader;
//            break;
//        case GL_TESS_CONTROL_SHADER:
//            pTessControlShader = *iShader;
//            break;
//        case GL_TESS_EVALUATION_SHADER:
//            pTessEvalShader = *iShader;
//            break;
//        case GL_GEOMETRY_SHADER:
//            pGeometryShader = *iShader;
//            break;
//        case GL_FRAGMENT_SHADER:
//            pFragmentShader = *iShader;
//            break;
//        case GL_COMPUTE_SHADER:
//            pComputeShader = *iShader;
//            break;
//        }


//        // attach shader shage
//        glUseProgramStages(glidProgramPipeline, shaderBit,
//                           (*iShader)->glidShaderProgram);

//    }

//    // check for validation errors.
//    GLint result;
//    int infoLength;
//    char* info;
//    glValidateProgramPipeline(glidProgramPipeline);
//    glGetProgramPipelineiv(glidProgramPipeline, GL_VALIDATE_STATUS, &result);
//    if(!result) {
//        glGetProgramPipelineiv(glidProgramPipeline, GL_INFO_LOG_LENGTH, &infoLength);
//        info = new char[infoLength];
//        glGetProgramPipelineInfoLog(glidProgramPipeline, infoLength, NULL, info);
//        cerr << "Validation error in program pipeline :" << endl;
//        cerr << info << endl;
//    }

//}

void ProgramPipeline::useComputeShader(
	ProgramPipeline::ShaderProgram* shader)
{
	GLbitfield shaderBit = sShaderTypeEnumToBitField(shader->shaderType);

	pComputeShader = shader;

	// attach shader shage
	glUseProgramStages(glidProgramPipeline, GL_COMPUTE_SHADER_BIT,
		pComputeShader->glidShaderProgram);

	// check for validation errors.
	GLint result;
	int infoLength;
	char* info;
	glValidateProgramPipeline(glidProgramPipeline);
	glGetProgramPipelineiv(glidProgramPipeline, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramPipelineiv(glidProgramPipeline, GL_INFO_LOG_LENGTH, &infoLength);
		info = new char[infoLength];
		glGetProgramPipelineInfoLog(glidProgramPipeline, infoLength, NULL, info);
		if (std::string(info).find("compute") == string::npos)
		{
			PRINT(m_name);
			PRINT(info);
		}
	}

}

void ProgramPipeline::useVertexShader(
        ProgramPipeline::ShaderProgram* shader)
{
    GLbitfield shaderBit = sShaderTypeEnumToBitField(shader->shaderType);

    pVertexShader = shader;

    // attach shader shage
    glUseProgramStages(glidProgramPipeline, GL_VERTEX_SHADER_BIT,
                       pVertexShader->glidShaderProgram);

    // check for validation errors.
    GLint result;
    int infoLength;
    char* info;
    glValidateProgramPipeline(glidProgramPipeline);
    glGetProgramPipelineiv(glidProgramPipeline, GL_VALIDATE_STATUS, &result);
    if(!result) {
        glGetProgramPipelineiv(glidProgramPipeline, GL_INFO_LOG_LENGTH, &infoLength);
        info = new char[infoLength];
        glGetProgramPipelineInfoLog(glidProgramPipeline, infoLength, NULL, info);
        if (std::string(info).find("fragment") == string::npos)
        {
            PRINT(m_name);
            PRINT(info);
        }
    }
    
}

void ProgramPipeline::useGeometryShader(ProgramPipeline::ShaderProgram* shader)
{
    GLbitfield shaderBit = sShaderTypeEnumToBitField(shader->shaderType);

    // make sure we don't mix compute shaders with other shader types.
    if(shader->shaderType == GL_COMPUTE_SHADER) {
        glUseProgramStages(glidProgramPipeline, GL_ALL_SHADER_BITS, 0);
    } else {
        glUseProgramStages(glidProgramPipeline, GL_COMPUTE_SHADER_BIT, 0);
    }

    pGeometryShader = shader;

    // attach shader shage
    glUseProgramStages(glidProgramPipeline, shaderBit,
                       shader->glidShaderProgram);

    // check for validation errors.
    GLint result;
    int infoLength;
    char* info;
    glValidateProgramPipeline(glidProgramPipeline);
    glGetProgramPipelineiv(glidProgramPipeline, GL_VALIDATE_STATUS, &result);
    if(!result) {
        glGetProgramPipelineiv(glidProgramPipeline, GL_INFO_LOG_LENGTH, &infoLength);
        info = new char[infoLength];
        glGetProgramPipelineInfoLog(glidProgramPipeline, infoLength, NULL, info);
        if (std::string(info).find("fragment") == string::npos)
        {
            PRINT(m_name);
            PRINT(info);
        }
    }
}

void ProgramPipeline::useFragmentShader(ProgramPipeline::ShaderProgram *shader)
{

    GLbitfield shaderBit = sShaderTypeEnumToBitField(shader->shaderType);

    // make sure we don't mix compute shaders with other shader types.
//    if(shader->shaderType == GL_COMPUTE_SHADER) {
//        glUseProgramStages(glidProgramPipeline, GL_ALL_SHADER_BITS, 0);
//    } else {
//        glUseProgramStages(glidProgramPipeline, GL_COMPUTE_SHADER_BIT, 0);
//    }

    pFragmentShader = shader;

    // attach shader shage
    glUseProgramStages(glidProgramPipeline, GL_FRAGMENT_SHADER_BIT,
                       pFragmentShader->glidShaderProgram);

    // check for validation errors.
    GLint result;
    int infoLength;
    char* info;
    glValidateProgramPipeline(glidProgramPipeline);
    glGetProgramPipelineiv(glidProgramPipeline, GL_VALIDATE_STATUS, &result);
    if(!result) {
        glGetProgramPipelineiv(glidProgramPipeline, GL_INFO_LOG_LENGTH, &infoLength);
        info = new char[infoLength];
        glGetProgramPipelineInfoLog(glidProgramPipeline, infoLength, NULL, info);
        cerr << "Validation error in useFragmentShader:" << endl;
        cerr << info << endl;
    }

}

void ProgramPipeline::useTessellationShader(ProgramPipeline::ShaderProgram *evaluationShader, ProgramPipeline::ShaderProgram *controlShader)
{

	GLbitfield evaluationShaderBit = sShaderTypeEnumToBitField(evaluationShader->shaderType);
	GLbitfield controlShaderBit = sShaderTypeEnumToBitField(controlShader->shaderType);

	// make sure we don't mix compute shaders with other shader types.
	//    if(shader->shaderType == GL_COMPUTE_SHADER) {
	//        glUseProgramStages(glidProgramPipeline, GL_ALL_SHADER_BITS, 0);
	//    } else {
	//        glUseProgramStages(glidProgramPipeline, GL_COMPUTE_SHADER_BIT, 0);
	//    }

	pTessControlShader = controlShader;
	pTessEvalShader = evaluationShader;

	// attach shader shage
	glUseProgramStages(glidProgramPipeline, evaluationShaderBit,
		pTessEvalShader->glidShaderProgram);

	// check for validation errors.
	GLint result;
	int infoLength;
	char* info;
	glValidateProgramPipeline(glidProgramPipeline);
	glGetProgramPipelineiv(glidProgramPipeline, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramPipelineiv(glidProgramPipeline, GL_INFO_LOG_LENGTH, &infoLength);
		info = new char[infoLength];
		glGetProgramPipelineInfoLog(glidProgramPipeline, infoLength, NULL, info);
		cerr << "Validation error in useFragmentShader:" << endl;
		cerr << info << endl;
	}

	// attach shader shage
	glUseProgramStages(glidProgramPipeline, controlShaderBit,
		pTessControlShader->glidShaderProgram);

	// check for validation errors.
	glValidateProgramPipeline(glidProgramPipeline);
	glGetProgramPipelineiv(glidProgramPipeline, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramPipelineiv(glidProgramPipeline, GL_INFO_LOG_LENGTH, &infoLength);
		info = new char[infoLength];
		glGetProgramPipelineInfoLog(glidProgramPipeline, infoLength, NULL, info);
		cerr << "Validation error in useFragmentShader:" << endl;
		cerr << info << endl;
	}

}

void ProgramPipeline::removeShader(GLenum shaderStage)
{
    glUseProgramStages(glidProgramPipeline,
                       sShaderTypeEnumToBitField(shaderStage), 0);

    // store shader ID
    switch(shaderStage) {
    case GL_VERTEX_SHADER:
        pVertexShader = 0;
        break;
    case GL_TESS_CONTROL_SHADER:
        pTessControlShader = 0;
        break;
    case GL_TESS_EVALUATION_SHADER:
        pTessEvalShader = 0;
        break;
    case GL_GEOMETRY_SHADER:
        pGeometryShader = 0;
        break;
    case GL_FRAGMENT_SHADER:
        pFragmentShader = 0;
        break;
    case GL_COMPUTE_SHADER:
        pComputeShader = 0;
        break;
    }
}

//             
void ProgramPipeline::setUniform(std::string name, glm::vec3 color)
{
    glProgramUniform3f(this->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(this->getShader(GL_VERTEX_SHADER)->glidShaderProgram, name.c_str()), color.x, color.y, color.z);

}
