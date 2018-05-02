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

    private int program;

    public Square() {
        float squareCoords[] = {
                -1f,  1f, 0.0f,
                -1f, -1f, 0.0f,
                 1f, -1f, 0.0f,
                 1f,  1f, 0.0f };

        // initialize vertex byte buffer for shape coordinates
        ByteBuffer vertexBb = ByteBuffer.allocateDirect(squareCoords.length * 4);
        vertexBb.order(ByteOrder.nativeOrder());
        vertexBuffer = vertexBb.asFloatBuffer();
        vertexBuffer.put(squareCoords);
        vertexBuffer.position(0);

        float uvCoords[] = {
                1, 0,
                1, 0.46875f,
                0, 0.46875f,
                0, 0 };

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

        int vertexShader = MyGLRenderer.loadShader(GLES20.GL_VERTEX_SHADER,
                ShaderManager.VERTEX_SHADER_CODE);
        int fragmentShader = MyGLRenderer.loadShader(GLES20.GL_FRAGMENT_SHADER,
                ShaderManager.FRAGMENT_SHADER_CODE);

        // create empty OpenGL ES Program
        program = GLES20.glCreateProgram();

        // add the vertex shader to program
        GLES20.glAttachShader(program, vertexShader);

        // add the fragment shader to program
        GLES20.glAttachShader(program, fragmentShader);

        // creates OpenGL ES program executables
        GLES20.glLinkProgram(program);
    }

    public void draw() {
        // Add program to OpenGL ES environment
        GLES20.glUseProgram(program);

        // get handle to vertex shader's vPosition member
        int positionhandle = GLES20.glGetAttribLocation(program, "vPosition");
        int uvPositionhandle = GLES20.glGetAttribLocation(program, "vTexCoords");

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

        // Draw the square
        GLES20.glDrawElements(GLES20.GL_TRIANGLES, drawOrder.length,
                GLES20.GL_UNSIGNED_SHORT, drawListBuffer);

        // Disable vertex array
        GLES20.glDisableVertexAttribArray(positionhandle);
    }

}
