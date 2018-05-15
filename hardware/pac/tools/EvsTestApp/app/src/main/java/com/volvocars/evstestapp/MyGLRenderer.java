/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.evstestapp;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class MyGLRenderer implements GLSurfaceView.Renderer {

    private static final float VIDEO_CROP_VALUE = 0.46875f; // Calculated from information in specification about camera video resolution

    private int program;

    private RenderObject videoBackground;
    private RenderObject carImage;

    private TextureManager textureManager;

    public static boolean testingIfWeCanMakeTheCarInvisible = false;

    public MyGLRenderer(Context context) {
        super();

        textureManager = new TextureManager(context);
    }

    public void onSurfaceCreated(GL10 unused, EGLConfig config) {

        // create empty OpenGL ES Program
        program = GLES20.glCreateProgram();

        int vertexShader = ShaderManager.loadShader(GLES20.GL_VERTEX_SHADER,
                ShaderManager.VERTEX_SHADER_CODE);
        int fragmentShader = ShaderManager.loadShader(GLES20.GL_FRAGMENT_SHADER,
                ShaderManager.FRAGMENT_SHADER_CODE);

        // add the vertex shader to program
        GLES20.glAttachShader(program, vertexShader);

        // add the fragment shader to program
        GLES20.glAttachShader(program, fragmentShader);

        // creates OpenGL ES program executables
        GLES20.glLinkProgram(program);

        // This will be used when drawing the car image on top of the video
        GLES20.glBlendFunc (GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);

        // Since there is only one program it doesn't have to be switched to many times
        GLES20.glUseProgram(program);
    }

    public void onDrawFrame(GL10 unused) {

        Log.d(LogTags.EVSTEST, "Draw: " + NdkManager.refreshVideo());
        videoBackground.draw(program);

        if(!testingIfWeCanMakeTheCarInvisible) {
            textureManager.bindCarTexture();

            GLES20.glEnable(GLES20.GL_BLEND);
            carImage.draw(program);
            GLES20.glDisable(GLES20.GL_BLEND);
        }
    }

    public void onSurfaceChanged(GL10 unused, int width, int height) {
        GLES20.glViewport(0, 0, width, height);

        float[] projection = new float[16];
        Matrix.orthoM(projection, 0, 0, width, 0, height, -1, 1);

        float[] videoMp = MatrixManager.getModelProjectionMatrix(0 , 0, projection);
        Square videoSquare = new Square(width, height, 1, VIDEO_CROP_VALUE);

        videoBackground = new RenderObject(videoSquare, videoMp);

        int carWidth = textureManager.carFromTopDownView.getWidth();
        int carHeight = textureManager.carFromTopDownView.getHeight();

        // The car is supposed to be located in the centre of the viewport
        float[] carMp = MatrixManager.getModelProjectionMatrix(width / 2 - carWidth / 2,height / 2 - carHeight / 2, projection);
        Square carSquare = new Square(carWidth, carHeight, 1, 1);

        carImage = new RenderObject(carSquare, carMp);

        Log.d(LogTags.EVSTEST, NdkManager.generateVideoTexture());
        textureManager.genGlTextures();
    }
}
