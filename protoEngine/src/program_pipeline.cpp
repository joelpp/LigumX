#include "program_pipeline.h"
#include "game.h"
#include <fstream>
#include <iostream>

using namespace std;

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
                    sourceCodeStrings[count] += line + "\n";
                }
                sourceCodeStream.close();
            } else {
                //cout << "Error loading shader : " << *iSrc << endl;
                cout << "Error loading shader : " << srcFilenames << endl;
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
    if(!result) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
        info = new char[infoLength];
        glGetShaderInfoLog(shader, infoLength, NULL, info);
        cerr << "Compiler error in shader :" << endl;
        cerr << info << endl;
    } else {
        glidShaderProgram = glCreateProgram();
        glProgramParameteri(glidShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
        glAttachShader(glidShaderProgram, shader);
        glLinkProgram(glidShaderProgram);
        glDetachShader(glidShaderProgram, shader);
        glDeleteShader(shader);

        glGetProgramiv(glidShaderProgram, GL_LINK_STATUS, &result);
        if(!result) {
            glGetProgramiv(glidShaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
            info = new char[infoLength];
            glGetProgramInfoLog(glidShaderProgram, infoLength, NULL, info);
            cerr << "Linker error in shader :" << endl;
            cerr << info << endl;
        }
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
        game->insertDebugMessage(
                "ProgramPipeline::ShaderProgram::getShader(...) : Unknown shader type.",
                GL_DEBUG_SEVERITY_LOW
        );
        return NULL;
        break;
    }
}

void ProgramPipeline::usePipeline()
{
    glBindProgramPipeline(this->glidProgramPipeline);
    glBindVertexArray(this->glidVao);
}

ProgramPipeline::ProgramPipeline()
{
    pVertexShader = NULL;
    pTessControlShader = NULL;
    pTessEvalShader = NULL;
    pGeometryShader = NULL;
    pFragmentShader = NULL;
    pComputeShader = NULL;

    // glCreateProgramPipelines(1, &glidProgramPipeline);
    // glCreateVertexArrays(1, &glidVao);

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

void ProgramPipeline::useVertexShader(
        ProgramPipeline::ShaderProgram* shader)
{
    GLbitfield shaderBit = sShaderTypeEnumToBitField(shader->shaderType);

    // make sure we don't mix compute shaders with other shader types.
    if(shader->shaderType == GL_COMPUTE_SHADER) {
        glUseProgramStages(glidProgramPipeline, GL_ALL_SHADER_BITS, 0);
    } else {
        glUseProgramStages(glidProgramPipeline, GL_COMPUTE_SHADER_BIT, 0);
    }

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
        cerr << "Validation error in useVertexShader:" << endl;
        cerr << info << endl;
    }
    PRINT(glGetAttribLocation(shader->glidShaderProgram, "pos"));

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
        cerr << "Validation error in program pipeline :" << endl;
        cerr << info << endl;
    }
}

void ProgramPipeline::useFragmentShader(ProgramPipeline::ShaderProgram *shader)
{

    GLbitfield shaderBit = sShaderTypeEnumToBitField(shader->shaderType);

    // make sure we don't mix compute shaders with other shader types.
    if(shader->shaderType == GL_COMPUTE_SHADER) {
        glUseProgramStages(glidProgramPipeline, GL_ALL_SHADER_BITS, 0);
    } else {
        glUseProgramStages(glidProgramPipeline, GL_COMPUTE_SHADER_BIT, 0);
    }

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

