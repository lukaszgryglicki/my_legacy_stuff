/************************************************************************
* *
* *
* Copyright (C) 1997 Diamond Multimedia Systems, Inc. *
* *
* *
************************************************************************/

#ifndef _STEREO_H
#define _STEREO_H

#define DIMD_STEREO_EXTENSION "GL_EXT_dimd_stereo"
#define GLQUERYSTEREOEXTPROC "glQueryStereoEXT"
#define GLSETSTEREOMODEEXTPROC "glSetStereoModeEXT"
#define GLENABLESTEREOEXTPROC "glEnableStereoEXT"
#define GLDISABLESTEREOEXTPROC "glDisableStereoEXT"
#define GLTOGGLESTEREOEXTPROC "glToggleStereoEXT"
#define GLDISPLAYSTEREOSUPPORTEXTPROC "glDisplayStereoSupportEXT"

#define STEREO_QUAD_BUFFER 1 // Currently supported by FIRE GL 1000 Pro
#define STEREO_ABOVE_BELOW 2 // Currently supported by FIRE 4000, 
// 1280x1024 True Colour 60Hz

typedef struct tagStereoDisplayContext
{
GLuint uiSize; // Must be set to sizeof(STEREODISPLAYCONTEXT).
GLuint uiWinWidth; // Width of window in pixel is returned.
GLuint uiWinHeight; // Height of window in pixel is returned.
GLuint uiWinAdjust; // Window adjustment between "above" and "below" is returned.
} STEREODISPLAYCONTEXT, *PSTEREODISPLAYCONTEXT;

// Note: All functions return GL_TRUE in case of success.
typedef GLboolean (APIENTRY * PFNGLQUERYSTEREOEXTPROC)(GLenum stereoCap);
typedef GLboolean (APIENTRY * PFNGLSETSTEREOMODEEXTPROC)(GLenum stereoMode, PSTEREODISPLAYCONTEXT stereoDisplayContext);
typedef GLboolean (APIENTRY * PFNGLENABLESTEREOEXTPROC)(void);
typedef GLboolean (APIENTRY * PFNGLDISABLESTEREOEXTPROC)(void);
typedef GLboolean (APIENTRY * PFNGLTOGGLESTEREOEXTPROC)(void);
typedef GLboolean (APIENTRY * PFNGLDISPLAYSTEREOSUPPORTEXTPROC)(GLuint uiWidth, GLuint uiHeight, GLuint uiBitsPerPixel, GLuint uiVFrequency);

#endif // _STEREO_H
