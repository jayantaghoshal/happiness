/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.evstestapp;

import android.opengl.GLES20;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

class Square {
    private FloatBuffer vertexBuffer;
    private FloatBuffer uvBuffer;
    private ShortBuffer drawListBuffer;

    private static final int COORDS_PER_VERTEX = 3;
    private static final int UV_COORDS_PER_VERTEX = 2;

    private static final int VERTEX_STRIDE = COORDS_PER_VERTEX * 4;
    private static final int TEX_COORD_STRIDE = UV_COORDS_PER_VERTEX * 4;

    private short drawOrder[] = { 0, 1, 2, 0, 2, 3 };

    public Square(int width, int height, float uvWidth, float uvHeight) {

        float squareCoords[] = {
                 0,     height, 0.0f,
                 0,     0,      0.0f,
                 width, 0,      0.0f,
                 width, height, 0.0f };

        // initialize vertex byte buffer for shape coordinates
        ByteBuffer vertexBb = ByteBuffer.allocateDirect(squareCoords.length * 4);
        vertexBb.order(ByteOrder.nativeOrder());
        vertexBuffer = vertexBb.asFloatBuffer();
        vertexBuffer.put(squareCoords);
        vertexBuffer.position(0);

        float uvCoords[] = {
                uvWidth, 0,
                uvWidth, uvHeight,
                0,       uvHeight,
                0,       0 };

        // byte buffer for texture coordinates
        ByteBuffer textureCoordsBb = ByteBuffer.allocateDirect(uvCoords.length * 4);
        textureCoordsBb.order(ByteOrder.nativeOrder());
        uvBuffer = textureCoordsBb.asFloatBuffer();
        uvBuffer.put(uvCoords);
        uvBuffer.position(0);

        // byte buffer for the draw list
        ByteBuffer drawListBb = ByteBuffer.allocateDirect(drawOrder.length * 2);
        drawListBb.order(ByteOrder.nativeOrder());
        drawListBuffer = drawListBb.asShortBuffer();
        drawListBuffer.put(drawOrder);
        drawListBuffer.position(0);
    }

    public void draw(int program, float[] mpMatrix) {
        // get handle to vertex shader's members
        int positionhandle = GLES20.glGetAttribLocation(program, "position");
        int uvPositionhandle = GLES20.glGetAttribLocation(program, "texCoords");

        // Enable a handle to the square vertices
        GLES20.glEnableVertexAttribArray(positionhandle);

        // Prepare the square coordinate data
        GLES20.glVertexAttribPointer(positionhandle, COORDS_PER_VERTEX,
                GLES20.GL_FLOAT, false,
                VERTEX_STRIDE, vertexBuffer);

        GLES20.glEnableVertexAttribArray(uvPositionhandle);
        GLES20.glVertexAttribPointer(uvPositionhandle, UV_COORDS_PER_VERTEX,
                GLES20.GL_FLOAT, false,
                TEX_COORD_STRIDE, uvBuffer);

        // Get handle to shape's transformation matrix
        int mpHandle = GLES20.glGetUniformLocation(program, "MP");

        // Pass the projection transformation to the shader
        GLES20.glUniformMatrix4fv(mpHandle, 1, false, mpMatrix, 0);

        // Draw the square
        GLES20.glDrawElements(GLES20.GL_TRIANGLES, drawOrder.length,
                GLES20.GL_UNSIGNED_SHORT, drawListBuffer);

        // Disable vertex array
        GLES20.glDisableVertexAttribArray(positionhandle);
        GLES20.glDisableVertexAttribArray(uvPositionhandle);
    }
}
