

#include "ProgramPipeline.h"
#include "renderer.h"
#include "ObjectManager.h"
#include "LXError.h"
#include "serializer.h"
#include "StringUtils.h"



using namespace std;

const std::string g_IncludeMarker = "// Include";
const std::string g_ProviderIncludeMarker = "ProvidersMarker";

const std::string g_ProviderIncludeFilePath = g_PathShaders + "ProviderDefinitions.h";

const std::string g_LineNumberRegex = "\\([0-9]+\\)";

ProgramPipeline::ShaderProgram::ShaderProgram()
{
	
}

bool ProgramPipeline::ShaderProgram::Initialize(GLenum shaderType, string srcFilenames,	bool readSrcFilenamesAsSourceCode)
{
	this->shaderType = shaderType;

	int nbSources = 1;//int(srcFilenames.size());
	const char ** sourceCodes = new const char *[nbSources];
	int count = 0;
	StringList sourceCodeStrings;

	m_NumLinesInInclude = 0;

	if (readSrcFilenamesAsSourceCode)
	{
		sourceCodes[count++] = srcFilenames.c_str();

	}
	else
	{

		sourceCodeStrings = StringList(nbSources);
		ifstream sourceCodeStream(srcFilenames, ios::in);
		if (sourceCodeStream.is_open())
		{
			string line;
			while (getline(sourceCodeStream, line))
			{
				bool includeStatement = StringUtils::StringContains(line, g_IncludeMarker);

				if (includeStatement)
				{
					std::vector<LXString> tokens = StringUtils::SplitString(line, ' ');
					lxAssert(tokens.size() == 3);
					
					LXString& toInclude = tokens[2];
					if (toInclude == g_ProviderIncludeMarker)
					{
						LXString providerInclude = StringUtils::FromFile(g_ProviderIncludeFilePath.c_str());
						m_NumLinesInInclude += StringUtils::Count(providerInclude, '\n');

						sourceCodeStrings[count] += providerInclude + "\n";
						continue;
					}
					else
					{
						LXString path = g_PathShaders + toInclude;
						LXString providerInclude = StringUtils::FromFile(path.c_str());
						m_NumLinesInInclude += StringUtils::Count(providerInclude, '\n');

						sourceCodeStrings[count] += providerInclude + "\n";
						continue;
					}
				}

				sourceCodeStrings[count] += line + "\n";

			}
			sourceCodeStream.close();
		}
		else
		{
			cout << "Error loading shader : " << srcFilenames << endl;
			lxAssert0();
			return false;
		}
		sourceCodes[count] = sourceCodeStrings[count].c_str();
		count++;

	}

	GLint result;
	int infoLength;
	char* info = nullptr;

	const GLuint shader = glCreateShader(shaderType);

	glShaderSource(shader, nbSources, sourceCodes, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	bool hadError = false;

	if (!result)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
		info = new char[infoLength];
		glGetShaderInfoLog(shader, infoLength, NULL, info);
		cerr << "Compiler error in shader :" << endl;
		hadError = true;
	}
	else
	{

		glidShaderProgram = glCreateProgram();
		glProgramParameteri(glidShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glAttachShader(glidShaderProgram, shader);
		glLinkProgram(glidShaderProgram);
		glDetachShader(glidShaderProgram, shader);
		glDeleteShader(shader);

		glGetProgramiv(glidShaderProgram, GL_LINK_STATUS, &result);

		if (result != GL_TRUE)
		{
			glGetProgramiv(glidShaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
			info = new char[infoLength];
			glGetProgramInfoLog(glidShaderProgram, infoLength, NULL, info);
			cerr << "Linker error in shader :" << endl;
			hadError = true;
		}
	}

	if (hadError)
	{
		lxLogMessage(lxFormat("%s", srcFilenames.c_str()).c_str());
		lxLogMessage(lxFormat("Shader error : %s", info).c_str());
		lxLogMessage("Line numbers(adjusted for provider include file) :");

		std::string infoString(info);
		std::regex lineNumberRegex(g_LineNumberRegex);

		sregex_iterator it_end;
		sregex_iterator it(infoString.begin(), infoString.end(), lineNumberRegex);
		while (it != it_end)
		{
			std::smatch match = *it;
			std::string match_str = match.str();

			const StringList parenthesisList = { "(", ")" };
			match_str = StringUtils::RemoveSubstrings(match_str, parenthesisList);

			int lineNumber = std::atoi(match_str.c_str()) - m_NumLinesInInclude;
			lxLogMessage(lxFormat("(%d)", lineNumber).c_str());

			++it;
		}

		lxLogMessage(lxFormat("Number of lines in provider include file : %d", m_NumLinesInInclude).c_str());
	}
	
	return !hadError;
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
    path << g_PathShaders;
    path << name << "/";
    
	bool success = false;
	if (isCompute)
	{

		ShaderProgram* cs = new ProgramPipeline::ShaderProgram();
		success = cs->Initialize(GL_COMPUTE_SHADER, path.str() + name + ".csh", false);

		if (success)
		{
			useComputeShader(cs);
		}
	}
	else
	{
		ShaderProgram* vs = new ProgramPipeline::ShaderProgram();
		success = vs->Initialize(GL_VERTEX_SHADER, path.str() + name + ".vsh", false);

		if (success)
		{
			useVertexShader(vs);

			ShaderProgram* ps = new ProgramPipeline::ShaderProgram();
			success &= ps->Initialize(GL_FRAGMENT_SHADER, path.str() + name + ".fsh", false);
			if (success)
			{
				useFragmentShader(ps);
			}
		}

	}

	//useTessellationShader(new ProgramPipeline::ShaderProgram(GL_TESS_EVALUATION_SHADER, path.str() + "evaluation.tes", false), new ProgramPipeline::ShaderProgram(GL_TESS_CONTROL_SHADER, path.str() + "control.tcs", false));
	m_IsValid = success;
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
			PRINTSTRING(m_name);
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
            PRINTSTRING(m_name);
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
            PRINTSTRING(m_name);
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
