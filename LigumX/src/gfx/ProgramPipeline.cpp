

#include "ProgramPipeline.h"
#include "renderer.h"
#include "ObjectManager.h"
#include "LXError.h"
#include "serializer.h"
#include "StringUtils.h"

#include "ShadowUniformGroup.h"
#include "LightingOptionsUniformGroup.h"



using namespace std;

const std::string g_IncludeMarker = "// Include";
const std::string g_ProviderIncludeMarker = "ProvidersMarker";

const std::string g_ProviderIncludeFilePath = g_PathShaders + "ProviderDefinitions.h";

const std::string g_UniformGroupMarker = "PROVIDER_";

const std::string g_LineNumberRegex = "\\([0-9]+\\)";

ProgramPipeline::ShaderProgram::ShaderProgram()
{
	
}

bool ProgramPipeline::ShaderProgram::Initialize(GLenum shaderType, LXString& name, string srcFilenames, bool readSrcFilenamesAsSourceCode, LXVector<LXString>& uniformGroups)
{
	StringList knownUniformGroups =
	{
		"LightingOptions",
		"ShadowMap"
	};

	this->shaderType = shaderType;

	int nbSources = 1;//int(srcFilenames.size());
	const char ** sourceCodes = new const char *[nbSources];
	int count = 0;
	StringList sourceCodeStrings;

	if (readSrcFilenamesAsSourceCode)
	{
		sourceCodes[count++] = srcFilenames.c_str();

	}
	else
	{

		sourceCodeStrings = StringList(nbSources);
		ifstream sourceCodeStream(srcFilenames, ios::in);

		int finalShaderLineNumber = 1; // in VS lines are counted starting at 1
		m_FileIncludes.push_back(ShaderFileInclude(name, finalShaderLineNumber, 1));

		//sourceCodeStrings[count] += "\b"
		m_NumLinesInInclude = 0;
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

					LXString path = g_PathShaders + toInclude;
					LXString name = toInclude;
					if (toInclude == g_ProviderIncludeMarker)
					{
						path = g_ProviderIncludeFilePath;
						name = "ProviderInclude";
					}

					LXString providerInclude = StringUtils::FromFile(path.c_str());
					sourceCodeStrings[count] += providerInclude + "\n";

					int linesInFile = StringUtils::Count(providerInclude, '\n');
					m_FileIncludes.push_back(ShaderFileInclude(name, finalShaderLineNumber, linesInFile));

					finalShaderLineNumber += linesInFile;
					m_NumLinesInInclude += linesInFile;

					continue;
				}

				bool uniformGroupStatement = StringUtils::StringContains(line, g_UniformGroupMarker);
				if (uniformGroupStatement)
				{
					StringList tokens = StringUtils::SplitString(line, '_');
					lxAssert(tokens.size() == 2);

					for (LXString& uniformGroupName : knownUniformGroups)
					{
						if (tokens[1] == uniformGroupName)
						{
							bool found = false;
							for (int i = 0; i < uniformGroups.size(); ++i)
							{
								if (uniformGroups[i] == uniformGroupName)
								{
									found = true;
									break;
								}
							}

							if (!found)
							{
								uniformGroups.push_back(uniformGroupName);
							}
						}
					}

				}

				sourceCodeStrings[count] += line + "\n";
				finalShaderLineNumber++;
			}

			ShaderFileInclude& lastFileInclude = m_FileIncludes[m_FileIncludes.size() - 1];
			int lastBlockStart = lastFileInclude.m_LineStart + lastFileInclude.m_NumLines;
			m_FileIncludes.push_back(ShaderFileInclude(name, lastBlockStart, finalShaderLineNumber - lastBlockStart));

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
		//lxLogMessage("Line numbers(adjusted for provider include file) :");

		// Find in which include files the errors happened
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

			int globalLineNumber = std::atoi(match_str.c_str());

			int includeIndex = m_FileIncludes.size() - 1;

			// find index
			for (int i = 0; i < m_FileIncludes.size() - 1; ++i)
			{
				ShaderFileInclude& inc = m_FileIncludes[i];
				if (globalLineNumber < inc.m_LineStart)
				{
					includeIndex = i - 1;
					break;
				}
			}

			ShaderFileInclude& inc = m_FileIncludes[includeIndex];
			int localLineNumber = globalLineNumber - inc.m_LineStart;

			// todo jpp get rid of this....
			int l2 = globalLineNumber - m_NumLinesInInclude;

			lxLogMessage(lxFormat("%s (%d) (%d)", inc.m_Filename.c_str(), localLineNumber, l2).c_str());

			++it;
		}
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
    
	LXVector<LXString> uniformGroups;

	bool success = false;
	if (isCompute)
	{

		ShaderProgram* cs = new ProgramPipeline::ShaderProgram();
		success = cs->Initialize(GL_COMPUTE_SHADER, name, path.str() + name + ".csh", false, uniformGroups);

		if (success)
		{
			useComputeShader(cs);
		}
	}
	else
	{
		ShaderProgram* vs = new ProgramPipeline::ShaderProgram();
		success = vs->Initialize(GL_VERTEX_SHADER, name, path.str() + name + ".vsh", false, uniformGroups);

		if (success)
		{
			useVertexShader(vs);

			ShaderProgram* ps = new ProgramPipeline::ShaderProgram();
			success &= ps->Initialize(GL_FRAGMENT_SHADER, name, path.str() + name + ".fsh", false, uniformGroups);
			if (success)
			{
				useFragmentShader(ps);
			}
		}

	}


	if (success)
	{
		// resolve uniform locations

		for (int g = 0; g < uniformGroups.size(); ++g)
		{
			LXString& groupName = uniformGroups[g];
			GFXUniformGroup uniformGroup;

			if (groupName == "ShadowMap") uniformGroup = ShadowUniformGroup();
			else if (groupName == "LightingOptions") uniformGroup = LightingOptionsUniformGroup();
			else lxAssert0();

			uniformGroup.GetLocationsFromShader(this);
			//m_UniformGroups.emplace(uniformGroup.GetGroupName(), uniformGroup);
			m_UniformGroups.push_back(uniformGroup);

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

GFXUniformGroup* ProgramPipeline::GetUniformGroup(const LXString& uniformGroupName)
{

	for (GFXUniformGroup& group : m_UniformGroups)
	{
		if (group.GetGroupName() == uniformGroupName)
		{
			return &group;
		}
	}
	
	return nullptr;
	//try
	//{
	//	GFXUniformGroup& group = m_UniformGroups.at(uniformGroupName);
	//	return &group;
	//}
	//catch (std::out_of_range e)
	//{
	//	return nullptr;
	//}
}

