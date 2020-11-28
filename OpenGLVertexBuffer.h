/*
 * OpenGLVertexBuffer.h
 *
 *  Created on: 21.11.2020
 *      Author: Sniper
 */

#ifndef OPENGLVERTEXBUFFER_H_
#define OPENGLVERTEXBUFFER_H_

#include <GHSi.h>

ULONG create_vertex_buffer(void *data, ULONG size);
void delete_vertex_buffer(ULONG rendererID);

void set_vertex_buffer(ULONG rendererID, void *data, ULONG size);


#endif /* OPENGLVERTEXBUFFER_H_ */
