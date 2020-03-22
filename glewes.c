
/*
 ** The OpenGLES Extension Wrangler Library
 ** Copyright (C) 2020, Stephane Denis
 ** All rights reserved.
 **
 ** Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are met:
 **
 ** * Redistributions of source code must retain the above copyright notice,
 **   this list of conditions and the following disclaimer.
 ** * Redistributions in binary form must reproduce the above copyright notice,
 **   this list of conditions and the following disclaimer in the documentation
 **   and/or other materials provided with the distribution.
 ** * The name of the author may be used to endorse or promote products
 **   derived from this software without specific prior written permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 ** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 ** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 ** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 ** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 ** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 ** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 ** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 ** THE POSSIBILITY OF SUCH DAMAGE.
 */

#define GLEW

#include "glewes.h"
void sysDebugPrint(const char* szFormat, ...);
static void * glewGetProcAddress(const GLubyte* name)
{
	void * ret = eglGetProcAddress((const char*)name);
	
	return ret;
}

static GLuint _glewStrLen (const GLubyte* s)
{
    GLuint i=0;
    if (s == NULL) return 0;
    while (s[i] != '\0') i++;
    return i;
}

static GLuint _glewStrCLen (const GLubyte* s, GLubyte c)
{
    GLuint i=0;
    if (s == NULL) return 0;
    while (s[i] != '\0' && s[i] != c) i++;
    return (s[i] == '\0' || s[i] == c) ? i : 0;
}

static GLboolean _glewStrSame (const GLubyte* a, const GLubyte* b, GLuint n)
{
    GLuint i=0;
    if(a == NULL || b == NULL)
        return (a == NULL && b == NULL && n == 0) ? GL_TRUE : GL_FALSE;
    while (i < n && a[i] != '\0' && b[i] != '\0' && a[i] == b[i]) i++;
    return i == n ? GL_TRUE : GL_FALSE;
}


static GLboolean _glewSearchExtension (const char* name, const GLubyte *start, const GLubyte *end)
{
    const GLubyte* p;
    GLuint len = _glewStrLen((const GLubyte*)name);
    p = start;
    while (p < end)
    {
        GLuint n = _glewStrCLen(p, ' ');
        if (len == n && _glewStrSame((const GLubyte*)name, p, n)) return GL_TRUE;
        p += n+1;
    }
    return GL_FALSE;
}


#define _glewResolveProc(s) s = (void*)eglGetProcAddress(#s);



/* Function pointer definitions */
GL_APICALL void (* GL_APIENTRY glReadBuffer)( GLenum mode );
GL_APICALL void (* GL_APIENTRY glDrawRangeElements)( GLenum mode,
                                                    GLuint start,
                                                    GLuint end,
                                                    GLsizei count,
                                                    GLenum type,
                                                    const GLvoid* indices );
GL_APICALL void (* GL_APIENTRY glTexImage3D)( GLenum target,
                                             GLint level,
                                             GLint internalformat,
                                             GLsizei width,
                                             GLsizei height,
                                             GLsizei depth,
                                             GLint border,
                                             GLenum format,
                                             GLenum type,
                                             const GLvoid* pixels );
GL_APICALL void (* GL_APIENTRY glTexSubImage3D)( GLenum target,
                                                GLint level,
                                                GLint xoffset,
                                                GLint yoffset,
                                                GLint zoffset,
                                                GLsizei width,
                                                GLsizei height,
                                                GLsizei depth,
                                                GLenum format,
                                                GLenum type,
                                                const GLvoid* pixels );
GL_APICALL void (* GL_APIENTRY glCopyTexSubImage3D)( GLenum target,
                                                    GLint level,
                                                    GLint xoffset,
                                                    GLint yoffset,
                                                    GLint zoffset,
                                                    GLint x,
                                                    GLint y,
                                                    GLsizei width,
                                                    GLsizei height );
GL_APICALL void (* GL_APIENTRY glCompressedTexImage3D)( GLenum target,
                                                       GLint level,
                                                       GLenum internalformat,
                                                       GLsizei width,
                                                       GLsizei height,
                                                       GLsizei depth,
                                                       GLint border,
                                                       GLsizei imageSize,
                                                       const GLvoid* data );
GL_APICALL void (* GL_APIENTRY glCompressedTexSubImage3D)( GLenum target,
                                                          GLint level,
                                                          GLint xoffset,
                                                          GLint yoffset,
                                                          GLint zoffset,
                                                          GLsizei width,
                                                          GLsizei height,
                                                          GLsizei depth,
                                                          GLenum format,
                                                          GLsizei imageSize,
                                                          const GLvoid* data );
GL_APICALL void (* GL_APIENTRY glGenQueries)( GLsizei n, GLuint* ids );
GL_APICALL void (* GL_APIENTRY glDeleteQueries)( GLsizei n, const GLuint* ids );
GL_APICALL GLboolean (* GL_APIENTRY glIsQuery)( GLuint id );
GL_APICALL void (* GL_APIENTRY glBeginQuery)( GLenum target, GLuint id );
GL_APICALL void (* GL_APIENTRY glEndQuery)( GLenum target );
GL_APICALL void (* GL_APIENTRY glGetQueryiv)( GLenum target, GLenum pname, GLint* params );
GL_APICALL void (* GL_APIENTRY glGetQueryObjectuiv)( GLuint id,
                                                    GLenum pname,
                                                    GLuint* params );
GL_APICALL GLboolean (* GL_APIENTRY glUnmapBuffer)( GLenum target );
GL_APICALL void (* GL_APIENTRY glGetBufferPointerv)( GLenum target,
                                                    GLenum pname,
                                                    GLvoid** params );
GL_APICALL void (* GL_APIENTRY glDrawBuffers)( GLsizei n, const GLenum* bufs );
GL_APICALL void (* GL_APIENTRY glUniformMatrix2x3fv)( GLint location,
                                                     GLsizei count,
                                                     GLboolean transpose,
                                                     const GLfloat* value );
GL_APICALL void (* GL_APIENTRY glUniformMatrix3x2fv)( GLint location,
                                                     GLsizei count,
                                                     GLboolean transpose,
                                                     const GLfloat* value );
GL_APICALL void (* GL_APIENTRY glUniformMatrix2x4fv)( GLint location,
                                                     GLsizei count,
                                                     GLboolean transpose,
                                                     const GLfloat* value );
GL_APICALL void (* GL_APIENTRY glUniformMatrix4x2fv)( GLint location,
                                                     GLsizei count,
                                                     GLboolean transpose,
                                                     const GLfloat* value );
GL_APICALL void (* GL_APIENTRY glUniformMatrix3x4fv)( GLint location,
                                                     GLsizei count,
                                                     GLboolean transpose,
                                                     const GLfloat* value );
GL_APICALL void (* GL_APIENTRY glUniformMatrix4x3fv)( GLint location,
                                                     GLsizei count,
                                                     GLboolean transpose,
                                                     const GLfloat* value );
GL_APICALL void (* GL_APIENTRY glBlitFramebuffer)( GLint srcX0,
                                                  GLint srcY0,
                                                  GLint srcX1,
                                                  GLint srcY1,
                                                  GLint dstX0,
                                                  GLint dstY0,
                                                  GLint dstX1,
                                                  GLint dstY1,
                                                  GLbitfield mask,
                                                  GLenum filter );
GL_APICALL void (* GL_APIENTRY glRenderbufferStorageMultisample)( GLenum target,
                                                                 GLsizei samples,
                                                                 GLenum internalformat,
                                                                 GLsizei width,
                                                                 GLsizei height );
GL_APICALL void (* GL_APIENTRY glFramebufferTextureLayer)( GLenum target,
                                                          GLenum attachment,
                                                          GLuint texture,
                                                          GLint level,
                                                          GLint layer );
GL_APICALL GLvoid* (* GL_APIENTRY glMapBufferRange)( GLenum target,
                                                    GLintptr offset,
                                                    GLsizeiptr length,
                                                    GLbitfield access );
GL_APICALL void (* GL_APIENTRY glFlushMappedBufferRange)( GLenum target,
                                                         GLintptr offset,
                                                         GLsizeiptr length );
GL_APICALL void (* GL_APIENTRY glBindVertexArray)( GLuint array );
GL_APICALL void (* GL_APIENTRY glDeleteVertexArrays)( GLsizei n, const GLuint* arrays );
GL_APICALL void (* GL_APIENTRY glGenVertexArrays)( GLsizei n, GLuint* arrays );
GL_APICALL GLboolean (* GL_APIENTRY glIsVertexArray)( GLuint array );
GL_APICALL void (* GL_APIENTRY glGetIntegeri_v)( GLenum target,
                                                GLuint index,
                                                GLint* data );
GL_APICALL void (* GL_APIENTRY glBeginTransformFeedback)( GLenum primitiveMode );
GL_APICALL void (* GL_APIENTRY glEndTransformFeedback)( void );
GL_APICALL void (* GL_APIENTRY glBindBufferRange)( GLenum target,
                                                  GLuint index,
                                                  GLuint buffer,
                                                  GLintptr offset,
                                                  GLsizeiptr size );
GL_APICALL void (* GL_APIENTRY glBindBufferBase)( GLenum target,
                                                 GLuint index,
                                                 GLuint buffer );
GL_APICALL void (* GL_APIENTRY glTransformFeedbackVaryings)( GLuint program,
                                                            GLsizei count,
                                                            const GLchar* const * varyings,
                                                            GLenum bufferMode );
GL_APICALL void (* GL_APIENTRY glGetTransformFeedbackVarying)( GLuint program,
                                                              GLuint index,
                                                              GLsizei bufSize,
                                                              GLsizei* length,
                                                              GLsizei* size,
                                                              GLenum* type,
                                                              GLchar* name );
GL_APICALL void (* GL_APIENTRY glVertexAttribIPointer)( GLuint index,
                                                       GLint size,
                                                       GLenum type,
                                                       GLsizei stride,
                                                       const GLvoid* pointer );
GL_APICALL void (* GL_APIENTRY glGetVertexAttribIiv)( GLuint index,
                                                     GLenum pname,
                                                     GLint* params );
GL_APICALL void (* GL_APIENTRY glGetVertexAttribIuiv)( GLuint index,
                                                      GLenum pname,
                                                      GLuint* params );
GL_APICALL void (* GL_APIENTRY glVertexAttribI4i)( GLuint index,
                                                  GLint x,
                                                  GLint y,
                                                  GLint z,
                                                  GLint w );
GL_APICALL void (* GL_APIENTRY glVertexAttribI4ui)( GLuint index,
                                                   GLuint x,
                                                   GLuint y,
                                                   GLuint z,
                                                   GLuint w );
GL_APICALL void (* GL_APIENTRY glVertexAttribI4iv)( GLuint index, const GLint* v );
GL_APICALL void (* GL_APIENTRY glVertexAttribI4uiv)( GLuint index, const GLuint* v );
GL_APICALL void (* GL_APIENTRY glGetUniformuiv)( GLuint program,
                                                GLint location,
                                                GLuint* params );
GL_APICALL GLint (* GL_APIENTRY glGetFragDataLocation)( GLuint program,
                                                       const GLchar *name );
GL_APICALL void (* GL_APIENTRY glUniform1ui)( GLint location, GLuint v0 );
GL_APICALL void (* GL_APIENTRY glUniform2ui)( GLint location, GLuint v0, GLuint v1 );
GL_APICALL void (* GL_APIENTRY glUniform3ui)( GLint location,
                                             GLuint v0,
                                             GLuint v1,
                                             GLuint v2 );
GL_APICALL void (* GL_APIENTRY glUniform4ui)( GLint location,
                                             GLuint v0,
                                             GLuint v1,
                                             GLuint v2,
                                             GLuint v3 );
GL_APICALL void (* GL_APIENTRY glUniform1uiv)( GLint location,
                                              GLsizei count,
                                              const GLuint* value );
GL_APICALL void (* GL_APIENTRY glUniform2uiv)( GLint location,
                                              GLsizei count,
                                              const GLuint* value );
GL_APICALL void (* GL_APIENTRY glUniform3uiv)( GLint location,
                                              GLsizei count,
                                              const GLuint* value );
GL_APICALL void (* GL_APIENTRY glUniform4uiv)( GLint location,
                                              GLsizei count,
                                              const GLuint* value );
GL_APICALL void (* GL_APIENTRY glClearBufferiv)( GLenum buffer,
                                                GLint drawbuffer,
                                                const GLint* value );
GL_APICALL void (* GL_APIENTRY glClearBufferuiv)( GLenum buffer,
                                                 GLint drawbuffer,
                                                 const GLuint* value );
GL_APICALL void (* GL_APIENTRY glClearBufferfv)( GLenum buffer,
                                                GLint drawbuffer,
                                                const GLfloat* value );
GL_APICALL void (* GL_APIENTRY glClearBufferfi)( GLenum buffer,
                                                GLint drawbuffer,
                                                GLfloat depth,
                                                GLint stencil );
GL_APICALL const GLubyte* (* GL_APIENTRY glGetStringi)( GLenum name, GLuint index );
GL_APICALL void (* GL_APIENTRY glCopyBufferSubData)( GLenum readTarget,
                                                    GLenum writeTarget,
                                                    GLintptr readOffset,
                                                    GLintptr writeOffset,
                                                    GLsizeiptr size );
GL_APICALL void (* GL_APIENTRY glGetUniformIndices)( GLuint program,
                                                    GLsizei uniformCount,
                                                    const GLchar* const * uniformNames,
                                                    GLuint* uniformIndices );
GL_APICALL void (* GL_APIENTRY glGetActiveUniformsiv)( GLuint program,
                                                      GLsizei uniformCount,
                                                      const GLuint* uniformIndices,
                                                      GLenum pname,
                                                      GLint* params );
GL_APICALL GLuint (* GL_APIENTRY glGetUniformBlockIndex)( GLuint program,
                                                         const GLchar* uniformBlockName );
GL_APICALL void (* GL_APIENTRY glGetActiveUniformBlockiv)( GLuint program,
                                                          GLuint uniformBlockIndex,
                                                          GLenum pname,
                                                          GLint* params );
GL_APICALL void (* GL_APIENTRY glGetActiveUniformBlockName)( GLuint program,
                                                            GLuint uniformBlockIndex,
                                                            GLsizei bufSize,
                                                            GLsizei* length,
                                                            GLchar* uniformBlockName );
GL_APICALL void (* GL_APIENTRY glUniformBlockBinding)( GLuint program,
                                                      GLuint uniformBlockIndex,
                                                      GLuint uniformBlockBinding );
GL_APICALL void (* GL_APIENTRY glDrawArraysInstanced)( GLenum mode,
                                                      GLint first,
                                                      GLsizei count,
                                                      GLsizei instanceCount );
GL_APICALL void (* GL_APIENTRY glDrawElementsInstanced)( GLenum mode,
                                                        GLsizei count,
                                                        GLenum type,
                                                        const GLvoid* indices,
                                                        GLsizei instanceCount );
GL_APICALL GLsync (* GL_APIENTRY glFenceSync)( GLenum condition, GLbitfield flags );
GL_APICALL GLboolean (* GL_APIENTRY glIsSync)( GLsync sync );
GL_APICALL void (* GL_APIENTRY glDeleteSync)( GLsync sync );
GL_APICALL GLenum (* GL_APIENTRY glClientWaitSync)( GLsync sync,
                                                   GLbitfield flags,
                                                   GLuint64 timeout );
GL_APICALL void (* GL_APIENTRY glWaitSync)( GLsync sync,
                                           GLbitfield flags,
                                           GLuint64 timeout );
GL_APICALL void (* GL_APIENTRY glGetInteger64v)( GLenum pname, GLint64* params );
GL_APICALL void (* GL_APIENTRY glGetSynciv)( GLsync sync,
                                            GLenum pname,
                                            GLsizei bufSize,
                                            GLsizei* length,
                                            GLint* values );
GL_APICALL void (* GL_APIENTRY glGetInteger64i_v)( GLenum target,
                                                  GLuint index,
                                                  GLint64* data );
GL_APICALL void (* GL_APIENTRY glGetBufferParameteri64v)( GLenum target,
                                                         GLenum pname,
                                                         GLint64* params );
GL_APICALL void (* GL_APIENTRY glGenSamplers)( GLsizei count, GLuint* samplers );
GL_APICALL void (* GL_APIENTRY glDeleteSamplers)( GLsizei count, const GLuint* samplers );
GL_APICALL GLboolean (* GL_APIENTRY glIsSampler)( GLuint sampler );
GL_APICALL void (* GL_APIENTRY glBindSampler)( GLuint unit, GLuint sampler );
GL_APICALL void (* GL_APIENTRY glSamplerParameteri)( GLuint sampler,
                                                    GLenum pname,
                                                    GLint param );
GL_APICALL void (* GL_APIENTRY glSamplerParameteriv)( GLuint sampler,
                                                     GLenum pname,
                                                     const GLint* param );
GL_APICALL void (* GL_APIENTRY glSamplerParameterf)( GLuint sampler,
                                                    GLenum pname,
                                                    GLfloat param );
GL_APICALL void (* GL_APIENTRY glSamplerParameterfv)( GLuint sampler,
                                                     GLenum pname,
                                                     const GLfloat* param );
GL_APICALL void (* GL_APIENTRY glGetSamplerParameteriv)( GLuint sampler,
                                                        GLenum pname,
                                                        GLint* params );
GL_APICALL void (* GL_APIENTRY glGetSamplerParameterfv)( GLuint sampler,
                                                        GLenum pname,
                                                        GLfloat* params );
GL_APICALL void (* GL_APIENTRY glVertexAttribDivisor)( GLuint index, GLuint divisor );
GL_APICALL void (* GL_APIENTRY glBindTransformFeedback)( GLenum target, GLuint id );
GL_APICALL void (* GL_APIENTRY glDeleteTransformFeedbacks)( GLsizei n, const GLuint* ids );
GL_APICALL void (* GL_APIENTRY glGenTransformFeedbacks)( GLsizei n, GLuint* ids );
GL_APICALL GLboolean (* GL_APIENTRY glIsTransformFeedback)( GLuint id );
GL_APICALL void (* GL_APIENTRY glPauseTransformFeedback)( void );
GL_APICALL void (* GL_APIENTRY glResumeTransformFeedback)( void );
GL_APICALL void (* GL_APIENTRY glGetProgramBinary)( GLuint program,
                                                   GLsizei bufSize,
                                                   GLsizei* length,
                                                   GLenum* binaryFormat,
                                                   GLvoid* binary );
GL_APICALL void (* GL_APIENTRY glProgramBinary)( GLuint program,
                                                GLenum binaryFormat,
                                                const GLvoid* binary,
                                                GLsizei length );
GL_APICALL void (* GL_APIENTRY glProgramParameteri)( GLuint program,
                                                    GLenum pname,
                                                    GLint value );
GL_APICALL void (* GL_APIENTRY glInvalidateFramebuffer)( GLenum target,
                                                        GLsizei numAttachments,
                                                        const GLenum* attachments );
GL_APICALL void (* GL_APIENTRY glInvalidateSubFramebuffer)( GLenum target,
                                                           GLsizei numAttachments,
                                                           const GLenum* attachments,
                                                           GLint x,
                                                           GLint y,
                                                           GLsizei width,
                                                           GLsizei height );
GL_APICALL void (* GL_APIENTRY glTexStorage2D)( GLenum target,
                                               GLsizei levels,
                                               GLenum internalformat,
                                               GLsizei width,
                                               GLsizei height );
GL_APICALL void (* GL_APIENTRY glTexStorage3D)( GLenum target,
                                               GLsizei levels,
                                               GLenum internalformat,
                                               GLsizei width,
                                               GLsizei height,
                                               GLsizei depth );
GL_APICALL void (* GL_APIENTRY glGetInternalformativ)( GLenum target,
                                                      GLenum internalformat,
                                                      GLenum pname,
                                                      GLsizei bufSize,
                                                      GLint* params );


PFNGLUSEPROGRAMSTAGESEXTPROC __glewUseProgramStagesEXT;
PFNGLACTIVESHADERPROGRAMEXTPROC __glewActiveShaderProgramEXT;
PFNGLCREATESHADERPROGRAMVEXTPROC __glewCreateShaderProgramvEXT;
PFNGLBINDPROGRAMPIPELINEEXTPROC __glewBindProgramPipelineEXT;
PFNGLDELETEPROGRAMPIPELINESEXTPROC __glewDeleteProgramPipelinesEXT;
PFNGLGENPROGRAMPIPELINESEXTPROC __glewGenProgramPipelinesEXT;
PFNGLISPROGRAMPIPELINEEXTPROC __glewIsProgramPipelineEXT;
PFNGLPROGRAMPARAMETERIEXTPROC __glewProgramParameteriEXT;
PFNGLGETPROGRAMPIPELINEIVEXTPROC __glewGetProgramPipelineivEXT;
PFNGLVALIDATEPROGRAMPIPELINEEXTPROC __glewValidateProgramPipelineEXT;
PFNGLGETPROGRAMPIPELINEINFOLOGEXTPROC __glewGetProgramPipelineInfoLogEXT;
PFNGLPROGRAMUNIFORM1IEXTPROC __glewProgramUniform1iEXT;
PFNGLPROGRAMUNIFORM2IEXTPROC __glewProgramUniform2iEXT;
PFNGLPROGRAMUNIFORM3IEXTPROC __glewProgramUniform3iEXT;
PFNGLPROGRAMUNIFORM4IEXTPROC __glewProgramUniform4iEXT;
PFNGLPROGRAMUNIFORM1FEXTPROC __glewProgramUniform1fEXT;
PFNGLPROGRAMUNIFORM2FEXTPROC __glewProgramUniform2fEXT;
PFNGLPROGRAMUNIFORM3FEXTPROC __glewProgramUniform3fEXT;
PFNGLPROGRAMUNIFORM4FEXTPROC __glewProgramUniform4fEXT;
PFNGLPROGRAMUNIFORM1IVEXTPROC __glewProgramUniform1ivEXT;
PFNGLPROGRAMUNIFORM2IVEXTPROC __glewProgramUniform2ivEXT;
PFNGLPROGRAMUNIFORM3IVEXTPROC __glewProgramUniform3ivEXT;
PFNGLPROGRAMUNIFORM4IVEXTPROC __glewProgramUniform4ivEXT;
PFNGLPROGRAMUNIFORM1FVEXTPROC __glewProgramUniform1fvEXT;
PFNGLPROGRAMUNIFORM2FVEXTPROC __glewProgramUniform2fvEXT;
PFNGLPROGRAMUNIFORM3FVEXTPROC __glewProgramUniform3fvEXT;
PFNGLPROGRAMUNIFORM4FVEXTPROC __glewProgramUniform4fvEXT;
PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC __glewProgramUniformMatrix2fvEXT;
PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC __glewProgramUniformMatrix3fvEXT;
PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC __glewProgramUniformMatrix4fvEXT;

static GLboolean _glewInit_GL_EXT_separate_shader_objects()
{
	GLboolean r = GL_FALSE;

	r = ((glUseProgramStagesEXT = (PFNGLUSEPROGRAMSTAGESEXTPROC)glewGetProcAddress((const GLubyte*)"glUseProgramStagesEXT")) == NULL) || r;
	r = ((glActiveShaderProgramEXT = (PFNGLACTIVESHADERPROGRAMEXTPROC)glewGetProcAddress((const GLubyte*)"glActiveShaderProgramEXT")) == NULL) || r;
	r = ((glBindProgramPipelineEXT = (PFNGLBINDPROGRAMPIPELINEEXTPROC)glewGetProcAddress((const GLubyte*)"glBindProgramPipelineEXT")) == NULL) || r;
	r = ((glCreateShaderProgramvEXT = (PFNGLCREATESHADERPROGRAMVEXTPROC)glewGetProcAddress((const GLubyte*)"glCreateShaderProgramvEXT")) == NULL) || r;
	r = ((glDeleteProgramPipelinesEXT = (PFNGLDELETEPROGRAMPIPELINESEXTPROC)glewGetProcAddress((const GLubyte*)"glDeleteProgramPipelinesEXT")) == NULL) || r;
	r = ((glGenProgramPipelinesEXT = (PFNGLGENPROGRAMPIPELINESEXTPROC)glewGetProcAddress((const GLubyte*)"glGenProgramPipelinesEXT")) == NULL) || r;
	r = ((glGetProgramPipelineInfoLogEXT = (PFNGLGETPROGRAMPIPELINEINFOLOGEXTPROC)glewGetProcAddress((const GLubyte*)"glGetProgramPipelineInfoLogEXT")) == NULL) || r;
	r = ((glGetProgramPipelineivEXT = (PFNGLGETPROGRAMPIPELINEIVEXTPROC)glewGetProcAddress((const GLubyte*)"glGetProgramPipelineivEXT")) == NULL) || r;
	r = ((glIsProgramPipelineEXT = (PFNGLISPROGRAMPIPELINEEXTPROC)glewGetProcAddress((const GLubyte*)"glIsProgramPipelineEXT")) == NULL) || r;

	r = ((glProgramUniform1fEXT = (PFNGLPROGRAMUNIFORM1FEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform1fEXT")) == NULL) || r;
	r = ((glProgramUniform1fvEXT = (PFNGLPROGRAMUNIFORM1FVEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform1fvEXT")) == NULL) || r;
	r = ((glProgramUniform1iEXT = (PFNGLPROGRAMUNIFORM1IEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform1iEXT")) == NULL) || r;
	r = ((glProgramUniform1ivEXT = (PFNGLPROGRAMUNIFORM1IVEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform1ivEXT")) == NULL) || r;

	r = ((glProgramUniform2fEXT = (PFNGLPROGRAMUNIFORM2FEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform2fEXT")) == NULL) || r;
	r = ((glProgramUniform2fvEXT = (PFNGLPROGRAMUNIFORM2FVEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform2fvEXT")) == NULL) || r;
	r = ((glProgramUniform2iEXT = (PFNGLPROGRAMUNIFORM2IEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform2iEXT")) == NULL) || r;
	r = ((glProgramUniform2ivEXT = (PFNGLPROGRAMUNIFORM2IVEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform2ivEXT")) == NULL) || r;

	r = ((glProgramUniform3fEXT = (PFNGLPROGRAMUNIFORM3FEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform3fEXT")) == NULL) || r;
	r = ((glProgramUniform3fvEXT = (PFNGLPROGRAMUNIFORM3FVEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform3fvEXT")) == NULL) || r;
	r = ((glProgramUniform3iEXT = (PFNGLPROGRAMUNIFORM3IEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform3iEXT")) == NULL) || r;
	r = ((glProgramUniform3ivEXT = (PFNGLPROGRAMUNIFORM3IVEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform3ivEXT")) == NULL) || r;

	r = ((glProgramUniform4fEXT = (PFNGLPROGRAMUNIFORM4FEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform4fEXT")) == NULL) || r;
	r = ((glProgramUniform4fvEXT = (PFNGLPROGRAMUNIFORM4FVEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform4fvEXT")) == NULL) || r;
	r = ((glProgramUniform4iEXT = (PFNGLPROGRAMUNIFORM4IEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform4iEXT")) == NULL) || r;
	r = ((glProgramUniform4ivEXT = (PFNGLPROGRAMUNIFORM4IVEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniform4ivEXT")) == NULL) || r;

	r = ((glProgramUniformMatrix2fvEXT = (PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniformMatrix2fvEXT")) == NULL) || r;
	r = ((glProgramUniformMatrix3fvEXT = (PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniformMatrix3fvEXT")) == NULL) || r;
	r = ((glProgramUniformMatrix4fvEXT = (PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC)glewGetProcAddress((const GLubyte*)"glProgramUniformMatrix4fvEXT")) == NULL) || r;
	r = ((glUseProgramStagesEXT = (PFNGLUSEPROGRAMSTAGESEXTPROC)glewGetProcAddress((const GLubyte*)"glUseProgramStagesEXT")) == NULL) || r;
	r = ((glValidateProgramPipelineEXT = (PFNGLVALIDATEPROGRAMPIPELINEEXTPROC)glewGetProcAddress((const GLubyte*)"glValidateProgramPipelineEXT")) == NULL) || r;

	return r;
}

GLboolean GLEW_EXT_shader_io_blocks;
GLboolean GLEW_ANDROID_extension_pack_es31a;

GLboolean glewInit3()
{
	const GLubyte* extEnd;
	const GLubyte* extStart = glGetString(GL_EXTENSIONS);
	if (extStart == 0)
		extStart = (const GLubyte*)"";
	extEnd = extStart + _glewStrLen(extStart);

    _glewResolveProc( glReadBuffer );
    _glewResolveProc( glDrawRangeElements );
    _glewResolveProc( glTexImage3D );
    _glewResolveProc( glTexSubImage3D );
    _glewResolveProc( glCopyTexSubImage3D );
    _glewResolveProc( glCompressedTexImage3D );
    _glewResolveProc( glCompressedTexSubImage3D );
    _glewResolveProc( glGenQueries );
    _glewResolveProc( glDeleteQueries );
    _glewResolveProc( glIsQuery );
    _glewResolveProc( glBeginQuery );
    _glewResolveProc( glEndQuery );
    _glewResolveProc( glGetQueryiv );
    _glewResolveProc( glGetQueryObjectuiv );
    _glewResolveProc( glUnmapBuffer );
    _glewResolveProc( glGetBufferPointerv );
    _glewResolveProc( glDrawBuffers );
    _glewResolveProc( glUniformMatrix2x3fv );
    _glewResolveProc( glUniformMatrix3x2fv );
    _glewResolveProc( glUniformMatrix2x4fv );
    _glewResolveProc( glUniformMatrix4x2fv );
    _glewResolveProc( glUniformMatrix3x4fv );
    _glewResolveProc( glUniformMatrix4x3fv );
    _glewResolveProc( glBlitFramebuffer );
    _glewResolveProc( glRenderbufferStorageMultisample );
    _glewResolveProc( glFramebufferTextureLayer );
    _glewResolveProc( glMapBufferRange );
    _glewResolveProc( glFlushMappedBufferRange );
    _glewResolveProc( glBindVertexArray );
    _glewResolveProc( glDeleteVertexArrays );
    _glewResolveProc( glGenVertexArrays );
    _glewResolveProc( glIsVertexArray );
    _glewResolveProc( glGetIntegeri_v );
    _glewResolveProc( glBeginTransformFeedback );
    _glewResolveProc( glEndTransformFeedback );
    _glewResolveProc( glBindBufferRange );
    _glewResolveProc( glBindBufferBase );
    _glewResolveProc( glTransformFeedbackVaryings );
    _glewResolveProc( glGetTransformFeedbackVarying );
    _glewResolveProc( glVertexAttribIPointer );
    _glewResolveProc( glGetVertexAttribIiv );
    _glewResolveProc( glGetVertexAttribIuiv );
    _glewResolveProc( glVertexAttribI4i );
    _glewResolveProc( glVertexAttribI4ui );
    _glewResolveProc( glVertexAttribI4iv );
    _glewResolveProc( glVertexAttribI4uiv );
    _glewResolveProc( glGetUniformuiv );
    _glewResolveProc( glGetFragDataLocation );
    _glewResolveProc( glUniform1ui );
    _glewResolveProc( glUniform2ui );
    _glewResolveProc( glUniform3ui );
    _glewResolveProc( glUniform4ui );
    _glewResolveProc( glUniform1uiv );
    _glewResolveProc( glUniform2uiv );
    _glewResolveProc( glUniform3uiv );
    _glewResolveProc( glUniform4uiv );
    _glewResolveProc( glClearBufferiv );
    _glewResolveProc( glClearBufferuiv );
    _glewResolveProc( glClearBufferfv );
    _glewResolveProc( glClearBufferfi );
    _glewResolveProc( glGetStringi );
    _glewResolveProc( glCopyBufferSubData );
    _glewResolveProc( glGetUniformIndices );
    _glewResolveProc( glGetActiveUniformsiv );
    _glewResolveProc( glGetUniformBlockIndex );
    _glewResolveProc( glGetActiveUniformBlockiv );
    _glewResolveProc( glGetActiveUniformBlockName );
    _glewResolveProc( glUniformBlockBinding );
    _glewResolveProc( glDrawArraysInstanced );
    _glewResolveProc( glDrawElementsInstanced );
    _glewResolveProc( glFenceSync );
    _glewResolveProc( glIsSync );
    _glewResolveProc( glDeleteSync );
    _glewResolveProc( glClientWaitSync );
    _glewResolveProc( glWaitSync );
    _glewResolveProc( glGetInteger64v );
    _glewResolveProc( glGetSynciv );
    _glewResolveProc( glGetInteger64i_v );
    _glewResolveProc( glGetBufferParameteri64v );
    _glewResolveProc( glGenSamplers );
    _glewResolveProc( glDeleteSamplers );
    _glewResolveProc( glIsSampler );
    _glewResolveProc( glBindSampler );
    _glewResolveProc( glSamplerParameteri );
    _glewResolveProc( glSamplerParameteriv );
    _glewResolveProc( glSamplerParameterf );
    _glewResolveProc( glSamplerParameterfv );
    _glewResolveProc( glGetSamplerParameteriv );
    _glewResolveProc( glGetSamplerParameterfv );
    _glewResolveProc( glVertexAttribDivisor );
    _glewResolveProc( glBindTransformFeedback );
    _glewResolveProc( glDeleteTransformFeedbacks );
    _glewResolveProc( glGenTransformFeedbacks );
    _glewResolveProc( glIsTransformFeedback );
    _glewResolveProc( glPauseTransformFeedback );
    _glewResolveProc( glResumeTransformFeedback );
    _glewResolveProc( glGetProgramBinary );
    _glewResolveProc( glProgramBinary );
    _glewResolveProc( glProgramParameteri );
    _glewResolveProc( glInvalidateFramebuffer );
    _glewResolveProc( glInvalidateSubFramebuffer );
    _glewResolveProc( glTexStorage2D );
    _glewResolveProc( glTexStorage3D );
    _glewResolveProc( glGetInternalformativ );
    
    if( !glReadBuffer || !glDrawRangeElements || !glTexImage3D || !glTexSubImage3D
       || !glCopyTexSubImage3D || !glCompressedTexImage3D
       || !glCompressedTexSubImage3D || !glGenQueries || !glDeleteQueries
       || !glIsQuery || !glBeginQuery || !glEndQuery || !glGetQueryiv
       || !glGetQueryObjectuiv || !glUnmapBuffer || !glGetBufferPointerv
       || !glDrawBuffers || !glUniformMatrix2x3fv || !glUniformMatrix3x2fv
       || !glUniformMatrix2x4fv || !glUniformMatrix4x2fv || !glUniformMatrix3x4fv
       || !glUniformMatrix4x3fv || !glBlitFramebuffer
       || !glRenderbufferStorageMultisample || !glFramebufferTextureLayer
       || !glMapBufferRange || !glFlushMappedBufferRange || !glBindVertexArray
       || !glDeleteVertexArrays || !glGenVertexArrays || !glIsVertexArray
       || !glGetIntegeri_v || !glBeginTransformFeedback || !glEndTransformFeedback
       || !glBindBufferRange || !glBindBufferBase || !glTransformFeedbackVaryings
       || !glGetTransformFeedbackVarying || !glVertexAttribIPointer
       || !glGetVertexAttribIiv || !glGetVertexAttribIuiv || !glVertexAttribI4i
       || !glVertexAttribI4ui || !glVertexAttribI4iv || !glVertexAttribI4uiv
       || !glGetUniformuiv || !glGetFragDataLocation || !glUniform1ui
       || !glUniform2ui || !glUniform3ui || !glUniform4ui || !glUniform1uiv
       || !glUniform2uiv || !glUniform3uiv || !glUniform4uiv || !glClearBufferiv
       || !glClearBufferuiv || !glClearBufferfv || !glClearBufferfi || !glGetStringi
       || !glCopyBufferSubData || !glGetUniformIndices || !glGetActiveUniformsiv
       || !glGetUniformBlockIndex || !glGetActiveUniformBlockiv
       || !glGetActiveUniformBlockName || !glUniformBlockBinding
       || !glDrawArraysInstanced || !glDrawElementsInstanced || !glFenceSync
       || !glIsSync || !glDeleteSync || !glClientWaitSync || !glWaitSync
       || !glGetInteger64v || !glGetSynciv || !glGetInteger64i_v
       || !glGetBufferParameteri64v || !glGenSamplers || !glDeleteSamplers
       || !glIsSampler || !glBindSampler || !glSamplerParameteri
       || !glSamplerParameteriv || !glSamplerParameterf || !glSamplerParameterfv
       || !glGetSamplerParameteriv || !glGetSamplerParameterfv
       || !glVertexAttribDivisor || !glBindTransformFeedback
       || !glDeleteTransformFeedbacks || !glGenTransformFeedbacks
       || !glIsTransformFeedback || !glPauseTransformFeedback
       || !glResumeTransformFeedback || !glGetProgramBinary || !glProgramBinary
       || !glProgramParameteri || !glInvalidateFramebuffer
       || !glInvalidateSubFramebuffer || !glTexStorage2D || !glTexStorage3D
       || !glGetInternalformativ )
    {
        
        return GL_FALSE;
    }
    
	GLEW_EXT_separate_shader_objects = !_glewInit_GL_EXT_separate_shader_objects();
    GLEW_EXT_texture_filter_anisotropic = GL_TRUE;
    GLEW_ARB_ES2_compatibility = GL_TRUE;
    GLEW_ARB_ES3_compatibility = GL_TRUE;
	GLEW_EXT_packed_depth_stencil = GL_TRUE;
	GLEW_EXT_shader_io_blocks = _glewSearchExtension("GL_EXT_shader_io_blocks", extStart, extEnd);
	GLEW_OES_depth24 = GL_TRUE;
	GLEW_OES_depth32 = GL_TRUE;
	GLEW_EXT_shadow_samplers = GL_TRUE;
    
    return GL_TRUE;
}



/* Function pointer definitions */
GL_APICALL void (* GL_APIENTRY glDispatchCompute ) (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
GL_APICALL void (* GL_APIENTRY glDispatchComputeIndirect ) (GLintptr indirect);
GL_APICALL void (* GL_APIENTRY glDrawArraysIndirect ) (GLenum mode, const void *indirect);
GL_APICALL void (* GL_APIENTRY glDrawElementsIndirect ) (GLenum mode, GLenum type, const void *indirect);
GL_APICALL void (* GL_APIENTRY glFramebufferParameteri ) (GLenum target, GLenum pname, GLint param);
GL_APICALL void (* GL_APIENTRY glGetFramebufferParameteriv ) (GLenum target, GLenum pname, GLint *params);
GL_APICALL void (* GL_APIENTRY glGetProgramInterfaceiv ) (GLuint program, GLenum programInterface, GLenum pname, GLint *params);
GL_APICALL GLuint (* GL_APIENTRY glGetProgramResourceIndex ) (GLuint program, GLenum programInterface, const GLchar *name);
GL_APICALL void (* GL_APIENTRY glGetProgramResourceName ) (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
GL_APICALL void (* GL_APIENTRY glGetProgramResourceiv ) (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
GL_APICALL GLint (* GL_APIENTRY glGetProgramResourceLocation ) (GLuint program, GLenum programInterface, const GLchar *name);
GL_APICALL void (* GL_APIENTRY glUseProgramStages ) (GLuint pipeline, GLbitfield stages, GLuint program);
GL_APICALL void (* GL_APIENTRY glActiveShaderProgram ) (GLuint pipeline, GLuint program);
GL_APICALL GLuint (* GL_APIENTRY glCreateShaderProgramv ) (GLenum type, GLsizei count, const GLchar *const*strings);
GL_APICALL void (* GL_APIENTRY glBindProgramPipeline ) (GLuint pipeline);
GL_APICALL void (* GL_APIENTRY glDeleteProgramPipelines ) (GLsizei n, const GLuint *pipelines);
GL_APICALL void (* GL_APIENTRY glGenProgramPipelines ) (GLsizei n, GLuint *pipelines);
GL_APICALL GLboolean (* GL_APIENTRY glIsProgramPipeline ) (GLuint pipeline);
GL_APICALL void (* GL_APIENTRY glGetProgramPipelineiv ) (GLuint pipeline, GLenum pname, GLint *params);
GL_APICALL void (* GL_APIENTRY glProgramUniform1i ) (GLuint program, GLint location, GLint v0);
GL_APICALL void (* GL_APIENTRY glProgramUniform2i ) (GLuint program, GLint location, GLint v0, GLint v1);
GL_APICALL void (* GL_APIENTRY glProgramUniform3i ) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
GL_APICALL void (* GL_APIENTRY glProgramUniform4i ) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
GL_APICALL void (* GL_APIENTRY glProgramUniform1ui ) (GLuint program, GLint location, GLuint v0);
GL_APICALL void (* GL_APIENTRY glProgramUniform2ui ) (GLuint program, GLint location, GLuint v0, GLuint v1);
GL_APICALL void (* GL_APIENTRY glProgramUniform3ui ) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
GL_APICALL void (* GL_APIENTRY glProgramUniform4ui ) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
GL_APICALL void (* GL_APIENTRY glProgramUniform1f ) (GLuint program, GLint location, GLfloat v0);
GL_APICALL void (* GL_APIENTRY glProgramUniform2f ) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
GL_APICALL void (* GL_APIENTRY glProgramUniform3f ) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GL_APICALL void (* GL_APIENTRY glProgramUniform4f ) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GL_APICALL void (* GL_APIENTRY glProgramUniform1iv ) (GLuint program, GLint location, GLsizei count, const GLint *value);
GL_APICALL void (* GL_APIENTRY glProgramUniform2iv ) (GLuint program, GLint location, GLsizei count, const GLint *value);
GL_APICALL void (* GL_APIENTRY glProgramUniform3iv ) (GLuint program, GLint location, GLsizei count, const GLint *value);
GL_APICALL void (* GL_APIENTRY glProgramUniform4iv ) (GLuint program, GLint location, GLsizei count, const GLint *value);
GL_APICALL void (* GL_APIENTRY glProgramUniform1uiv ) (GLuint program, GLint location, GLsizei count, const GLuint *value);
GL_APICALL void (* GL_APIENTRY glProgramUniform2uiv ) (GLuint program, GLint location, GLsizei count, const GLuint *value);
GL_APICALL void (* GL_APIENTRY glProgramUniform3uiv ) (GLuint program, GLint location, GLsizei count, const GLuint *value);
GL_APICALL void (* GL_APIENTRY glProgramUniform4uiv ) (GLuint program, GLint location, GLsizei count, const GLuint *value);
GL_APICALL void (* GL_APIENTRY glProgramUniform1fv ) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniform2fv ) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniform3fv ) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniform4fv ) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniformMatrix2fv ) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniformMatrix3fv ) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniformMatrix4fv ) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniformMatrix2x3fv ) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniformMatrix3x2fv ) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniformMatrix2x4fv ) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniformMatrix4x2fv ) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniformMatrix3x4fv ) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glProgramUniformMatrix4x3fv ) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GL_APICALL void (* GL_APIENTRY glValidateProgramPipeline ) (GLuint pipeline);
GL_APICALL void (* GL_APIENTRY glGetProgramPipelineInfoLog ) (GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GL_APICALL void (* GL_APIENTRY glBindImageTexture ) (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
GL_APICALL void (* GL_APIENTRY glGetBooleani_v ) (GLenum target, GLuint index, GLboolean *data);
GL_APICALL void (* GL_APIENTRY glMemoryBarrier ) (GLbitfield barriers);
GL_APICALL void (* GL_APIENTRY glMemoryBarrierByRegion ) (GLbitfield barriers);
GL_APICALL void (* GL_APIENTRY glTexStorage2DMultisample ) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GL_APICALL void (* GL_APIENTRY glGetMultisamplefv ) (GLenum pname, GLuint index, GLfloat *val);
GL_APICALL void (* GL_APIENTRY glSampleMaski ) (GLuint maskNumber, GLbitfield mask);
GL_APICALL void (* GL_APIENTRY glGetTexLevelParameteriv ) (GLenum target, GLint level, GLenum pname, GLint *params);
GL_APICALL void (* GL_APIENTRY glGetTexLevelParameterfv ) (GLenum target, GLint level, GLenum pname, GLfloat *params);
GL_APICALL void (* GL_APIENTRY glBindVertexBuffer ) (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
GL_APICALL void (* GL_APIENTRY glVertexAttribFormat ) (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
GL_APICALL void (* GL_APIENTRY glVertexAttribIFormat ) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
GL_APICALL void (* GL_APIENTRY glVertexAttribBinding ) (GLuint attribindex, GLuint bindingindex);
GL_APICALL void (* GL_APIENTRY glVertexBindingDivisor ) (GLuint bindingindex, GLuint divisor);


GLboolean glewInit3_1()
{

	const GLubyte* extEnd;
	const GLubyte* extStart = glGetString(GL_EXTENSIONS);
	if (extStart == 0)
		extStart = (const GLubyte*)"";
	extEnd = extStart + _glewStrLen(extStart);

    _glewResolveProc (glDispatchCompute);
    _glewResolveProc (glDispatchComputeIndirect);
    _glewResolveProc (glDrawArraysIndirect);
    _glewResolveProc (glDrawElementsIndirect);
    _glewResolveProc (glFramebufferParameteri);
    _glewResolveProc (glGetFramebufferParameteriv);
    _glewResolveProc (glGetProgramInterfaceiv);
    _glewResolveProc (glGetProgramResourceIndex);
    _glewResolveProc (glGetProgramResourceName);
    _glewResolveProc (glGetProgramResourceiv);
    _glewResolveProc (glGetProgramResourceLocation);
    _glewResolveProc (glUseProgramStages);
    _glewResolveProc (glActiveShaderProgram);
    _glewResolveProc (glCreateShaderProgramv);
    _glewResolveProc (glBindProgramPipeline);
    _glewResolveProc (glDeleteProgramPipelines);
    _glewResolveProc (glGenProgramPipelines);
    _glewResolveProc (glIsProgramPipeline);
    _glewResolveProc (glGetProgramPipelineiv);
    _glewResolveProc (glProgramUniform1i);
    _glewResolveProc (glProgramUniform2i);
    _glewResolveProc (glProgramUniform3i);
    _glewResolveProc (glProgramUniform4i);
    _glewResolveProc (glProgramUniform1ui);
    _glewResolveProc (glProgramUniform2ui);
    _glewResolveProc (glProgramUniform3ui);
    _glewResolveProc (glProgramUniform4ui);
    _glewResolveProc (glProgramUniform1f);
    _glewResolveProc (glProgramUniform2f);
    _glewResolveProc (glProgramUniform3f);
    _glewResolveProc (glProgramUniform4f);
    _glewResolveProc (glProgramUniform1iv);
    _glewResolveProc (glProgramUniform2iv);
    _glewResolveProc (glProgramUniform3iv);
    _glewResolveProc (glProgramUniform4iv);
    _glewResolveProc (glProgramUniform1uiv);
    _glewResolveProc (glProgramUniform2uiv);
    _glewResolveProc (glProgramUniform3uiv);
    _glewResolveProc (glProgramUniform4uiv);
    _glewResolveProc (glProgramUniform1fv);
    _glewResolveProc (glProgramUniform2fv);
    _glewResolveProc (glProgramUniform3fv);
    _glewResolveProc (glProgramUniform4fv);
    _glewResolveProc (glProgramUniformMatrix2fv);
    _glewResolveProc (glProgramUniformMatrix3fv);
    _glewResolveProc (glProgramUniformMatrix4fv);
    _glewResolveProc (glProgramUniformMatrix2x3fv);
    _glewResolveProc (glProgramUniformMatrix3x2fv);
    _glewResolveProc (glProgramUniformMatrix2x4fv);
    _glewResolveProc (glProgramUniformMatrix4x2fv);
    _glewResolveProc (glProgramUniformMatrix3x4fv);
    _glewResolveProc (glProgramUniformMatrix4x3fv);
    _glewResolveProc (glValidateProgramPipeline);
    _glewResolveProc (glGetProgramPipelineInfoLog);
    _glewResolveProc (glBindImageTexture);
    _glewResolveProc (glGetBooleani_v);
    _glewResolveProc (glMemoryBarrier);
    _glewResolveProc (glMemoryBarrierByRegion);
    _glewResolveProc (glTexStorage2DMultisample);
    _glewResolveProc (glGetMultisamplefv);
    _glewResolveProc (glSampleMaski);
    _glewResolveProc (glGetTexLevelParameteriv);
    _glewResolveProc (glGetTexLevelParameterfv);
    _glewResolveProc (glBindVertexBuffer);
    _glewResolveProc (glVertexAttribFormat);
    _glewResolveProc (glVertexAttribIFormat);
    _glewResolveProc (glVertexAttribBinding);
    _glewResolveProc (glVertexBindingDivisor);
    
    GLEW_ARB_ES3_1_compatibility =  (glDispatchCompute) &&
    (glDispatchComputeIndirect) &&
    (glDrawArraysIndirect) &&
    (glDrawElementsIndirect) &&
    (glFramebufferParameteri) &&
    (glGetFramebufferParameteriv) &&
    (glGetProgramInterfaceiv) &&
    (glGetProgramResourceIndex) &&
    (glGetProgramResourceName) &&
    (glGetProgramResourceiv) &&
    (glGetProgramResourceLocation) &&
    (glUseProgramStages) &&
    (glActiveShaderProgram) &&
    (glCreateShaderProgramv) &&
    (glBindProgramPipeline) &&
    (glDeleteProgramPipelines) &&
    (glGenProgramPipelines) &&
    (glIsProgramPipeline) &&
    (glGetProgramPipelineiv) &&
    (glProgramUniform1i) &&
    (glProgramUniform2i) &&
    (glProgramUniform3i) &&
    (glProgramUniform4i) &&
    (glProgramUniform1ui) &&
    (glProgramUniform2ui) &&
    (glProgramUniform3ui) &&
    (glProgramUniform4ui) &&
    (glProgramUniform1f) &&
    (glProgramUniform2f) &&
    (glProgramUniform3f) &&
    (glProgramUniform4f) &&
    (glProgramUniform1iv) &&
    (glProgramUniform2iv) &&
    (glProgramUniform3iv) &&
    (glProgramUniform4iv) &&
    (glProgramUniform1uiv) &&
    (glProgramUniform2uiv) &&
    (glProgramUniform3uiv) &&
    (glProgramUniform4uiv) &&
    (glProgramUniform1fv) &&
    (glProgramUniform2fv) &&
    (glProgramUniform3fv) &&
    (glProgramUniform4fv) &&
    (glProgramUniformMatrix2fv) &&
    (glProgramUniformMatrix3fv) &&
    (glProgramUniformMatrix4fv) &&
    (glProgramUniformMatrix2x3fv) &&
    (glProgramUniformMatrix3x2fv) &&
    (glProgramUniformMatrix2x4fv) &&
    (glProgramUniformMatrix4x2fv) &&
    (glProgramUniformMatrix3x4fv) &&
    (glProgramUniformMatrix4x3fv) &&
    (glValidateProgramPipeline) &&
    (glGetProgramPipelineInfoLog) &&
    (glBindImageTexture) &&
    (glGetBooleani_v) &&
    (glMemoryBarrier) &&
    (glMemoryBarrierByRegion ) &&
    (glTexStorage2DMultisample) &&
    (glGetMultisamplefv) &&
    (glSampleMaski) &&
    (glGetTexLevelParameteriv) &&
    (glGetTexLevelParameterfv) &&
    (glBindVertexBuffer) &&
    (glVertexAttribFormat) &&
    (glVertexAttribIFormat) &&
    (glVertexAttribBinding) &&
    (glVertexBindingDivisor);
    
	GLEW_ANDROID_extension_pack_es31a = _glewSearchExtension("GL_ANDROID_extension_pack_es31a", extStart, extEnd);
    return glewInit3() && GLEW_ARB_ES3_1_compatibility;
}



GL_APICALL void (* GL_APIENTRY glBlendBarrier)(void);
GL_APICALL void (* GL_APIENTRY glCopyImageSubData)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
GL_APICALL void (* GL_APIENTRY glDebugMessageControl)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
GL_APICALL void (* GL_APIENTRY glDebugMessageInsert)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
GL_APICALL void (* GL_APIENTRY glDebugMessageCallback)(GLDEBUGPROC callback, const void *userParam);
GL_APICALL GLuint(* GL_APIENTRY glGetDebugMessageLog)(GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
GL_APICALL void (* GL_APIENTRY glPushDebugGroup)(GLenum source, GLuint id, GLsizei length, const GLchar *message);
GL_APICALL void (* GL_APIENTRY glPopDebugGroup)(void);
GL_APICALL void (* GL_APIENTRY glObjectLabel)(GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
GL_APICALL void (* GL_APIENTRY glGetObjectLabel)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
GL_APICALL void (* GL_APIENTRY glObjectPtrLabel)(const void *ptr, GLsizei length, const GLchar *label);
GL_APICALL void (* GL_APIENTRY glGetObjectPtrLabel)(const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
GL_APICALL void (* GL_APIENTRY glGetPointerv)(GLenum pname, void **params);
GL_APICALL void (* GL_APIENTRY glEnablei)(GLenum target, GLuint index);
GL_APICALL void (* GL_APIENTRY glDisablei)(GLenum target, GLuint index);
GL_APICALL void (* GL_APIENTRY glBlendEquationi)(GLuint buf, GLenum mode);
GL_APICALL void (* GL_APIENTRY glBlendEquationSeparatei)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
GL_APICALL void (* GL_APIENTRY glBlendFunci)(GLuint buf, GLenum src, GLenum dst);
GL_APICALL void (* GL_APIENTRY glBlendFuncSeparatei)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
GL_APICALL void (* GL_APIENTRY glColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
GL_APICALL GLboolean(* GL_APIENTRY glIsEnabledi)(GLenum target, GLuint index);
GL_APICALL void (* GL_APIENTRY glDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
GL_APICALL void (* GL_APIENTRY glDrawRangeElementsBaseVertex)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
GL_APICALL void (* GL_APIENTRY glDrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
GL_APICALL void (* GL_APIENTRY glFramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level);
GL_APICALL void (* GL_APIENTRY glPrimitiveBoundingBox)(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);
GL_APICALL GLenum(* GL_APIENTRY glGetGraphicsResetStatus)(void);
GL_APICALL void (* GL_APIENTRY glReadnPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
GL_APICALL void (* GL_APIENTRY glGetnUniformfv)(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
GL_APICALL void (* GL_APIENTRY glGetnUniformiv)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
GL_APICALL void (* GL_APIENTRY glGetnUniformuiv)(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
GL_APICALL void (* GL_APIENTRY glMinSampleShading)(GLfloat value);
GL_APICALL void (* GL_APIENTRY glPatchParameteri)(GLenum pname, GLint value);
GL_APICALL void (* GL_APIENTRY glTexParameterIiv)(GLenum target, GLenum pname, const GLint *params);
GL_APICALL void (* GL_APIENTRY glTexParameterIuiv)(GLenum target, GLenum pname, const GLuint *params);
GL_APICALL void (* GL_APIENTRY glGetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
GL_APICALL void (* GL_APIENTRY glGetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
GL_APICALL void (* GL_APIENTRY glSamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint *param);
GL_APICALL void (* GL_APIENTRY glSamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint *param);
GL_APICALL void (* GL_APIENTRY glGetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint *params);
GL_APICALL void (* GL_APIENTRY glGetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint *params);
GL_APICALL void (* GL_APIENTRY glTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer);
GL_APICALL void (* GL_APIENTRY glTexBufferRange)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
GL_APICALL void (* GL_APIENTRY glTexStorage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);


GLboolean glewInit3_2()
{
	_glewResolveProc(glBlendBarrier); // void);
	_glewResolveProc(glCopyImageSubData); // GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
	_glewResolveProc(glDebugMessageControl); // GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
	_glewResolveProc(glDebugMessageInsert); // GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
	_glewResolveProc(glDebugMessageCallback); // GLDEBUGPROC callback, const void *userParam);
	_glewResolveProc(glGetDebugMessageLog); // GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
	_glewResolveProc(glPushDebugGroup); // GLenum source, GLuint id, GLsizei length, const GLchar *message);
	_glewResolveProc(glPopDebugGroup); // void);
	_glewResolveProc(glObjectLabel); // GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
	_glewResolveProc(glGetObjectLabel); // GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
	_glewResolveProc(glObjectPtrLabel); // const void *ptr, GLsizei length, const GLchar *label);
	_glewResolveProc(glGetObjectPtrLabel); // const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
	_glewResolveProc(glGetPointerv); // GLenum pname, void **params);
	_glewResolveProc(glEnablei); // GLenum target, GLuint index);
	_glewResolveProc(glDisablei); // GLenum target, GLuint index);
	_glewResolveProc(glBlendEquationi); // GLuint buf, GLenum mode);
	_glewResolveProc(glBlendEquationSeparatei); // GLuint buf, GLenum modeRGB, GLenum modeAlpha);
	_glewResolveProc(glBlendFunci); // GLuint buf, GLenum src, GLenum dst);
	_glewResolveProc(glBlendFuncSeparatei); // GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
	_glewResolveProc(glColorMaski); // GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
	_glewResolveProc(glIsEnabledi); // GLenum target, GLuint index);
	_glewResolveProc(glDrawElementsBaseVertex); // GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
	_glewResolveProc(glDrawRangeElementsBaseVertex); // GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
	_glewResolveProc(glDrawElementsInstancedBaseVertex); // GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
	_glewResolveProc(glFramebufferTexture); // GLenum target, GLenum attachment, GLuint texture, GLint level);
	_glewResolveProc(glPrimitiveBoundingBox); // GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);
	_glewResolveProc(glGetGraphicsResetStatus); // void);
	_glewResolveProc(glReadnPixels); // GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
	_glewResolveProc(glGetnUniformfv); // GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
	_glewResolveProc(glGetnUniformiv); // GLuint program, GLint location, GLsizei bufSize, GLint *params);
	_glewResolveProc(glGetnUniformuiv); // GLuint program, GLint location, GLsizei bufSize, GLuint *params);
	_glewResolveProc(glMinSampleShading); // GLfloat value);
	_glewResolveProc(glPatchParameteri); // GLenum pname, GLint value);
	_glewResolveProc(glTexParameterIiv); // GLenum target, GLenum pname, const GLint *params);
	_glewResolveProc(glTexParameterIuiv); // GLenum target, GLenum pname, const GLuint *params);
	_glewResolveProc(glGetTexParameterIiv); // GLenum target, GLenum pname, GLint *params);
	_glewResolveProc(glGetTexParameterIuiv); // GLenum target, GLenum pname, GLuint *params);
	_glewResolveProc(glSamplerParameterIiv); // GLuint sampler, GLenum pname, const GLint *param);
	_glewResolveProc(glSamplerParameterIuiv); // GLuint sampler, GLenum pname, const GLuint *param);
	_glewResolveProc(glGetSamplerParameterIiv); // GLuint sampler, GLenum pname, GLint *params);
	_glewResolveProc(glGetSamplerParameterIuiv); // GLuint sampler, GLenum pname, GLuint *params);
	_glewResolveProc(glTexBuffer); // GLenum target, GLenum internalformat, GLuint buffer);
	_glewResolveProc(glTexBufferRange); // GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
	_glewResolveProc(glTexStorage3DMultisample); // GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);

	GLEW_ARB_ES3_2_compatibility =

		(glBlendBarrier) && // void) &&
		(glCopyImageSubData) && // GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth) &&
		(glDebugMessageControl) && // GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled) &&
		(glDebugMessageInsert) && // GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf) &&
		(glDebugMessageCallback) && // GLDEBUGPROC callback, const void *userParam) &&
		(glGetDebugMessageLog) && // GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog) &&
		(glPushDebugGroup) && // GLenum source, GLuint id, GLsizei length, const GLchar *message) &&
		(glPopDebugGroup) && // void) &&
		(glObjectLabel) && // GLenum identifier, GLuint name, GLsizei length, const GLchar *label) &&
		(glGetObjectLabel) && // GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label) &&
		(glObjectPtrLabel) && // const void *ptr, GLsizei length, const GLchar *label) &&
		(glGetObjectPtrLabel) && // const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label) &&
		(glGetPointerv) && // GLenum pname, void **params) &&
		(glEnablei) && // GLenum target, GLuint index) &&
		(glDisablei) && // GLenum target, GLuint index) &&
		(glBlendEquationi) && // GLuint buf, GLenum mode) &&
		(glBlendEquationSeparatei) && // GLuint buf, GLenum modeRGB, GLenum modeAlpha) &&
		(glBlendFunci) && // GLuint buf, GLenum src, GLenum dst) &&
		(glBlendFuncSeparatei) && // GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) &&
		(glColorMaski) && // GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) &&
		(glIsEnabledi) && // GLenum target, GLuint index) &&
		(glDrawElementsBaseVertex) && // GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex) &&
		(glDrawRangeElementsBaseVertex) && // GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex) &&
		(glDrawElementsInstancedBaseVertex) && // GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex) &&
		(glFramebufferTexture) && // GLenum target, GLenum attachment, GLuint texture, GLint level) &&
		(glPrimitiveBoundingBox) && // GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW) &&
		(glGetGraphicsResetStatus) && // void) &&
		(glReadnPixels) && // GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data) &&
		(glGetnUniformfv) && // GLuint program, GLint location, GLsizei bufSize, GLfloat *params) &&
		(glGetnUniformiv) && // GLuint program, GLint location, GLsizei bufSize, GLint *params) &&
		(glGetnUniformuiv) && // GLuint program, GLint location, GLsizei bufSize, GLuint *params) &&
		(glMinSampleShading) && // GLfloat value) &&
		(glPatchParameteri) && // GLenum pname, GLint value) &&
		(glTexParameterIiv) && // GLenum target, GLenum pname, const GLint *params) &&
		(glTexParameterIuiv) && // GLenum target, GLenum pname, const GLuint *params) &&
		(glGetTexParameterIiv) && // GLenum target, GLenum pname, GLint *params) &&
		(glGetTexParameterIuiv) && // GLenum target, GLenum pname, GLuint *params) &&
		(glSamplerParameterIiv) && // GLuint sampler, GLenum pname, const GLint *param) &&
		(glSamplerParameterIuiv) && // GLuint sampler, GLenum pname, const GLuint *param) &&
		(glGetSamplerParameterIiv) && // GLuint sampler, GLenum pname, GLint *params) &&
		(glGetSamplerParameterIuiv) && // GLuint sampler, GLenum pname, GLuint *params) &&
		(glTexBuffer) && // GLenum target, GLenum internalformat, GLuint buffer) &&
		(glTexBufferRange) && // GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) &&
		(glTexStorage3DMultisample);  // GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) &&


    return glewInit3_1() && GLEW_ARB_ES3_2_compatibility;
}



#define GLEW_CONTEXT_ARG_DEF_INIT void
/* This is a generic function pointer type, whose name indicates it must
 * be cast to the proper type *and calling convention* before use.
 */

static GLboolean _glewInit_GL_OES_vertex_array_object (GLEW_CONTEXT_ARG_DEF_INIT)
{
    GLboolean r = GL_FALSE;
    
    r = ((__glewBindVertexArrayOES = (PFNGLBINDVERTEXARRAYOESPROC)glewGetProcAddress((const GLubyte*)"glBindVertexArrayOES")) == NULL) || r;
    r = ((__glewDeleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSOESPROC)glewGetProcAddress((const GLubyte*)"glDeleteVertexArraysOES")) == NULL) || r;
    r = ((__glewGenVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC)glewGetProcAddress((const GLubyte*)"glGenVertexArraysOES")) == NULL) || r;
    r = ((__glewIsVertexArrayOES = (PFNGLISVERTEXARRAYOESPROC)glewGetProcAddress((const GLubyte*)"glIsVertexArrayOES")) == NULL) || r;
    
    
    
    return r;
}


static GLboolean _glewInit_GL_EXT_multi_draw_arrays (GLEW_CONTEXT_ARG_DEF_INIT)
{
    GLboolean r = GL_FALSE;
    
    r = ((__glewMultiDrawArraysEXT = (PFNGLMULTIDRAWARRAYSEXTPROC)glewGetProcAddress((const GLubyte*)"glMultiDrawArraysEXT")) == NULL) || r;
    r = ((__glewMultiDrawElementsEXT = (PFNGLMULTIDRAWELEMENTSEXTPROC)glewGetProcAddress((const GLubyte*)"glMultiDrawElementsEXT")) == NULL) || r;
    
    
    return r;
}



static GLboolean _glewInit_GL_EXT_discard_framebuffer (GLEW_CONTEXT_ARG_DEF_INIT)
{
    GLboolean r = GL_FALSE;
    
    r = ((__glewDiscardFrameBufferEXT = (PFNGLDISCARDFRAMEBUFFEREXTPROC)glewGetProcAddress((const GLubyte*)"glDiscardFramebufferEXT")) == NULL) || r;
    
    return r;
}


PFNGLLABELOBJECTEXTPROC __glewLabelObjectEXT;
PFNGLGETOBJECTLABELEXTPROC __glewGetObjectLabelEXT;
static GLboolean _glewInit_GL_EXT_debug_label(GLEW_CONTEXT_ARG_DEF_INIT)
{
	GLboolean r = GL_FALSE;
	

	r = ((__glewLabelObjectEXT = (PFNGLLABELOBJECTEXTPROC)glewGetProcAddress((const GLubyte*)"glLabelObjectEXT")) == NULL) || r;
	r = ((__glewGetObjectLabelEXT = (PFNGLGETOBJECTLABELEXTPROC)glewGetProcAddress((const GLubyte*)"glGetObjectLabelEXT")) == NULL) || r;	

	return r;
}


static GLboolean _glewInit_GL_OES_mapbuffer (GLEW_CONTEXT_ARG_DEF_INIT)
{
    GLboolean r = GL_FALSE;
    
    
    r = ((__glewMapBufferOES = (PFNGLMAPBUFFEROESPROC)glewGetProcAddress((const GLubyte*)"glMapBufferOES")) == NULL) || r;
    r = ((__glewUnmapBufferOES = (PFNGLUNMAPBUFFEROESPROC)glewGetProcAddress((const GLubyte*)"glUnmapBufferOES")) == NULL) || r;
    r = ((__glewGetBufferPointervOES = (PFNGLGETBUFFERPOINTERVOESPROC)glewGetProcAddress((const GLubyte*)"glGetBufferPointervOES")) == NULL) || r;
    
    return r;
}


static GLboolean _glewInit_GL_EXT_draw_instanced (GLEW_CONTEXT_ARG_DEF_INIT)
{
    GLboolean r = GL_FALSE;
    
    r = ((glDrawArraysInstancedEXT = (PFNGLDRAWARRAYSINSTANCEDEXTPROC)glewGetProcAddress((const GLubyte*)"glDrawArraysInstancedEXT")) == NULL) || r;
    r = ((glDrawElementsInstancedEXT = (PFNGLDRAWELEMENTSINSTANCEDEXTPROC)glewGetProcAddress((const GLubyte*)"glDrawElementsInstancedEXT")) == NULL) || r;
    
    return r;
}


static GLboolean _glewInit_GL_EXT_texture_storage (GLEW_CONTEXT_ARG_DEF_INIT)
{
    GLboolean r = GL_FALSE;
    
    r = ((__glewTexStorage1DEXT = (PFNGLTEXSTORAGE1DEXTPROC)glewGetProcAddress((const GLubyte*)"glTexStorage1DEXT")) == NULL) || r;
    r = ((__glewTexStorage2DEXT = (PFNGLTEXSTORAGE2DEXTPROC)glewGetProcAddress((const GLubyte*)"glTexStorage2DEXT")) == NULL) || r;
    r = ((__glewTexStorage3DEXT = (PFNGLTEXSTORAGE3DEXTPROC)glewGetProcAddress((const GLubyte*)"glTexStorage3DEXT")) == NULL) || r;
    return r;
}

static GLboolean _glewInit_GL_EXT_framebuffer_blit (GLEW_CONTEXT_ARG_DEF_INIT)
{
    GLboolean r = GL_FALSE;
    __glewBlitFramebufferEXT = (PFNGLBLITFRAMEBUFFEREXTPROC)glewGetProcAddress((const GLubyte*)"glBlitFramebufferEXT");
    
    if (__glewBlitFramebufferEXT == NULL)
        __glewBlitFramebufferEXT = (PFNGLBLITFRAMEBUFFEREXTPROC)glewGetProcAddress((const GLubyte*)"glBlitFramebufferNV");
    
    r = (__glewBlitFramebufferEXT == NULL) || r;
    return r;
}

PFNGLDELETEFENCESNVPROC __glewDeleteFencesNV;
PFNGLGENFENCESNVPROC __glewGenFencesNV;
PFNGLGETFENCEIVNVPROC __glewGetFenceivNV;
PFNGLISFENCENVPROC __glewIsFenceNV;
PFNGLFINISHFENCENVPROC __glewFinishFenceNV;
PFNGLSETFENCENVPROC __glewSetFenceNV;
PFNGLTESTFENCENVPROC __glewTestFenceNV;


static GLboolean _glewInit_GL_NV_fence (GLEW_CONTEXT_ARG_DEF_INIT)
{
    GLboolean r = GL_FALSE;
    
    r = ((__glewDeleteFencesNV = (PFNGLDELETEFENCESNVPROC)glewGetProcAddress((const GLubyte*)"glDeleteFencesNV")) == NULL) || r;
    r = ((__glewFinishFenceNV = (PFNGLFINISHFENCENVPROC)glewGetProcAddress((const GLubyte*)"glFinishFenceNV")) == NULL) || r;
    r = ((__glewGenFencesNV = (PFNGLGENFENCESNVPROC)glewGetProcAddress((const GLubyte*)"glGenFencesNV")) == NULL) || r;
    r = ((__glewGetFenceivNV = (PFNGLGETFENCEIVNVPROC)glewGetProcAddress((const GLubyte*)"glGetFenceivNV")) == NULL) || r;
    r = ((__glewIsFenceNV = (PFNGLISFENCENVPROC)glewGetProcAddress((const GLubyte*)"glIsFenceNV")) == NULL) || r;
    r = ((__glewSetFenceNV = (PFNGLSETFENCENVPROC)glewGetProcAddress((const GLubyte*)"glSetFenceNV")) == NULL) || r;
    r = ((__glewTestFenceNV = (PFNGLTESTFENCENVPROC)glewGetProcAddress((const GLubyte*)"glTestFenceNV")) == NULL) || r;
    
    return r;
}

PFNGLTEXSTORAGE1DEXTPROC __glewTexStorage1DEXT;
PFNGLTEXSTORAGE2DEXTPROC __glewTexStorage2DEXT;
PFNGLTEXSTORAGE3DEXTPROC __glewTexStorage3DEXT;

PFNGLDRAWARRAYSINSTANCEDEXTPROC __glewDrawArraysInstancedEXT;
PFNGLDRAWELEMENTSINSTANCEDEXTPROC __glewDrawElementsInstancedEXT;
PFNGLBLITFRAMEBUFFEREXTPROC __glewBlitFramebufferEXT;
PFNGLBINDVERTEXARRAYOESPROC __glewBindVertexArrayOES;
PFNGLDELETEVERTEXARRAYSOESPROC __glewDeleteVertexArraysOES;
PFNGLGENVERTEXARRAYSOESPROC __glewGenVertexArraysOES;
PFNGLISVERTEXARRAYOESPROC __glewIsVertexArrayOES;
PFNGLMULTIDRAWARRAYSEXTPROC __glewMultiDrawArraysEXT;
PFNGLMULTIDRAWELEMENTSEXTPROC __glewMultiDrawElementsEXT;
PFNGLDISCARDFRAMEBUFFEREXTPROC __glewDiscardFrameBufferEXT;
PFNGLMAPBUFFEROESPROC __glewMapBufferOES;
PFNGLUNMAPBUFFEROESPROC __glewUnmapBufferOES;
PFNGLGETBUFFERPOINTERVOESPROC __glewGetBufferPointervOES;

GLboolean GLEW_ARB_ES2_compatibility;
GLboolean GLEW_ARB_ES3_compatibility;
GLboolean GLEW_ARB_ES3_1_compatibility;
GLboolean GLEW_ARB_ES3_2_compatibility;

GLboolean GLEW_EXT_multi_draw_arrays;
GLboolean GLEW_EXT_discard_framebuffer;
GLboolean GLEW_OES_vertex_array_object;
GLboolean GLEW_OES_mapbuffer;
GLboolean GLEW_EXT_texture_storage;
GLboolean GLEW_EXT_draw_instanced;
GLboolean GLEW_EXT_framebuffer_blit;
GLboolean GLEW_EXT_texture_filter_anisotropic;
GLboolean GLEW_EXT_sRGB;
GLboolean GLEW_EXT_packed_depth_stencil;
GLboolean GLEW_OES_depth24;
GLboolean GLEW_OES_depth32;
GLboolean GLEW_EXT_shadow_samplers;
GLboolean GLEW_OES_vertex_half_float;
GLboolean GLEW_OES_texture_half_float;
GLboolean GLEW_NV_fence;
GLboolean GLEW_EXT_texture_format_BGRA8888;
GLboolean GLEW_EXT_separate_shader_objects;
GLboolean GLEW_EXT_debug_label;

GLenum glewInit2()
{
    const GLubyte* extEnd;
    const GLubyte* extStart = glGetString(GL_EXTENSIONS);
    if (extStart == 0)
        extStart = (const GLubyte*)"";
    extEnd = extStart + _glewStrLen(extStart);
    
    GLEW_EXT_multi_draw_arrays = !_glewInit_GL_EXT_multi_draw_arrays();
    GLEW_EXT_discard_framebuffer = !_glewInit_GL_EXT_discard_framebuffer();
    GLEW_OES_vertex_array_object = !_glewInit_GL_OES_vertex_array_object();
	GLEW_OES_mapbuffer = GL_FALSE; //  !_glewInit_GL_OES_mapbuffer();  [glUnmapBufferOES] 0x502
	GLEW_EXT_debug_label = !_glewInit_GL_EXT_debug_label();
    GLEW_EXT_texture_storage = !_glewInit_GL_EXT_texture_storage();
    GLEW_EXT_draw_instanced = !_glewInit_GL_EXT_draw_instanced();
    GLEW_EXT_framebuffer_blit = !_glewInit_GL_EXT_framebuffer_blit();
	
    GLEW_NV_fence = !_glewInit_GL_NV_fence();
    GLEW_EXT_texture_filter_anisotropic = _glewSearchExtension("GL_EXT_texture_filter_anisotropic", extStart, extEnd);
    
	GLEW_EXT_sRGB = _glewSearchExtension("GL_EXT_sRGB", extStart, extEnd);
	GLEW_EXT_packed_depth_stencil = _glewSearchExtension("GL_EXT_packed_depth_stencil", extStart, extEnd);
	GLEW_OES_depth24 = _glewSearchExtension("GL_OES_depth24", extStart, extEnd);
	GLEW_OES_depth32 = _glewSearchExtension("GL_OES_depth32", extStart, extEnd);
	GLEW_EXT_shadow_samplers = _glewSearchExtension("GL_EXT_shadow_samplers", extStart, extEnd);

    GLEW_OES_vertex_half_float = _glewSearchExtension("GL_OES_vertex_half_float", extStart, extEnd);
    GLEW_OES_texture_half_float = _glewSearchExtension("GL_OES_texture_half_float", extStart, extEnd);
	GLEW_EXT_texture_format_BGRA8888 = _glewSearchExtension("GL_EXT_texture_format_BGRA8888", extStart, extEnd);

    GLEW_ARB_ES2_compatibility = GL_TRUE;
    GLEW_ARB_ES3_compatibility = GL_FALSE;
    GLEW_ARB_ES3_1_compatibility = GL_FALSE;
    GLEW_ARB_ES3_2_compatibility = GL_FALSE;
    
    return (GLenum) 0;
}

GLenum glewInit(int version)
{
    return version >= 0x320 ?  glewInit3_2() : ( version == 0x310 ? glewInit3_1() : (version == 0x300 ? glewInit3() : glewInit2()));
}

