/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.evstestapp;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;

class TextureManager {
    public final Bitmap carFromTopDownView;
    private int carFromTopDownId[] = { 0 };

    public TextureManager(Context context) {
        carFromTopDownView = BitmapFactory.decodeResource(context.getResources(), R.drawable.cam_360_v426);
    }

    void genGlTextures() {
        GLES20.glGenTextures(1, carFromTopDownId, 0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, carFromTopDownId[0]);
        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, carFromTopDownView, 0);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
    }

    void bindCarTexture() {
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, carFromTopDownId[0]);
    }
}
