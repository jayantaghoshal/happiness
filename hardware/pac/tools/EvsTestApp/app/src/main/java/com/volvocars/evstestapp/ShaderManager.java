/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.evstestapp;

class ShaderManager {
    public static final String VERTEX_SHADER_CODE =

"attribute vec4 vPosition;" +
"attribute vec2 vTexCoords;" +
"varying vec2 texCoords;" +
"void main() {" +
"  texCoords = vTexCoords;" +
"  gl_Position = vPosition;" +
"}";

    public static final String FRAGMENT_SHADER_CODE =

"precision mediump float;" +
"uniform sampler2D diffSampler;" +
"varying vec2 texCoords;" +
"void main() {" +
"  gl_FragColor = texture2D(diffSampler, texCoords);" +
"}";

}