/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.evstestapp;

class RenderObject {
    private Square square;
    private float[] mvpMatrix;

    public RenderObject(Square square, float[] mvpMatrix) {

        this.square = square;

        this.mvpMatrix = mvpMatrix;
    }

    public void draw(int program) {
        square.draw(program, mvpMatrix);
    }
}
