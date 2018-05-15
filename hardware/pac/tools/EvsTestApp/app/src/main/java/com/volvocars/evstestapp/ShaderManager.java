/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.evstestapp;

import android.opengl.GLES20;

class ShaderManager {

    public static final String VERTEX_SHADER_CODE =

"attribute vec4 position;" +
"attribute vec2 texCoords;" +
"varying vec2 fsTexCoords;" +
"uniform mat4 MP;" +
"void main() {" +
"  fsTexCoords = texCoords;" +
"  gl_Position = MP * position;" +
"}";

    public static final String FRAGMENT_SHADER_CODE =

"precision mediump float;" +
"uniform sampler2D sampler;" +
"varying vec2 fsTexCoords;" +
"void main() {" +
"  gl_FragColor = texture2D(sampler, fsTexCoords);" +
"}";

    public static int loadShader(int type, String shaderCode) {
        // create a vertex shader type (GLES20.GL_VERTEX_SHADER)
        // or a fragment shader type (GLES20.GL_FRAGMENT_SHADER)
        int shader = GLES20.glCreateShader(type);

        // add the source code to the shader and compile it
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);

        return shader;
    }
}