/*
 * OpenGLIndexBuffer.C
 *
 *  Created on: 28.11.2020
 *      Author: Sniper
 */

#ifndef OPENGLINDEXBUFFER_H
#define OPENGLINDEXBUFFER_H

#include <GHSi.h>

ULONG create_index_buffer(void *data, ULONG size);
void delete_index_buffer(ULONG rendererID);

void set_index_buffer(ULONG rendererID, void *data, ULONG size);


#endif /* OPENGLINDEXBUFFER_H */