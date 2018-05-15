/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.evstestapp;

import android.opengl.Matrix;

class MatrixManager {

    public static float[] getModelProjectionMatrix(int posX, int posY, float[] projection) {

        float[] modelProjection = new float[16];

        Matrix.setIdentityM(modelProjection, 0);
        Matrix.translateM(modelProjection, 0, posX, posY, 0);

        Matrix.multiplyMM(modelProjection, 0, projection, 0, modelProjection, 0);

        return modelProjection;
    }
}
