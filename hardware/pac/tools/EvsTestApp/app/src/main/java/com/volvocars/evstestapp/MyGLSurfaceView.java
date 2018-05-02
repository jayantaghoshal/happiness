/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.evstestapp;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

class MyGLSurfaceView extends GLSurfaceView {

    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);

        // Create an OpenGL ES 2.0 context
        setEGLContextClientVersion(2);

        final MyGLRenderer renderer = new MyGLRenderer();

        setRenderer(renderer);
    }
}
