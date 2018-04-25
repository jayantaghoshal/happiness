/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.functions;

import com.volvocars.vehiclefunctions.R;

import java.util.ArrayList;
import java.util.List;

/**
 * Groups together several child Functions. Section itself inherits from Function to support
 * flattening into a single list, which is convenient when displaying in the UI.
 */
public class Section extends Function {
    private static final int COLUMNS_OCCUPIED = 2;

    private final List<Function> mFunctions;

    private Section(CharSequence title, List<Function> functions) {
        super(title, R.layout.vh_function_section, COLUMNS_OCCUPIED);
        mFunctions = functions;
    }

    public List<Function> getFunctions() {
        return mFunctions;
    }

    public static class Builder {
        private CharSequence mTitle;
        private List<Function> functions = new ArrayList<>();

        public Builder setTitle(CharSequence title) {
            mTitle = title;
            return this;
        }

        public Builder addFunction(Function function) {
            functions.add(function);
            return this;
        }

        public Section create() {
            return new Section(mTitle, functions);
        }
    }
}
