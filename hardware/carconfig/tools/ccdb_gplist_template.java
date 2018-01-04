/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.carconfig;

import java.util.HashMap;

public class CarConfigEnums
{
    private static final HashMap<Class, HashMap<Integer, CarConfigEnumBase>> getValueData = new HashMap<>();
    private CarConfigEnums() {
    }

    public interface CarConfigEnumBase {
        int getValue();
        String getName();
        int getParam();
    }

    @SuppressWarnings("unchecked")
    public static <T extends CarConfigEnumBase> T getValue(Class clazz, int value){
        HashMap<Integer, CarConfigEnumBase> carconfigMap = getValueData.get(clazz);
        if (carconfigMap == null){
            return null;
        }
        return (T) carconfigMap.get(value);
    }

    @SuppressWarnings("unchecked")
    public static int getParamNumber(Class clazz){
        CarConfigEnumBase carConfigEnumBase = (CarConfigEnumBase) getValueData.get(clazz).values().toArray()[0];
        return carConfigEnumBase.getParam();
    }

    {% for param in paramlist %}
    private static HashMap<Integer, CarConfigEnumBase> map_CC_{{param.number}} = new HashMap<>();
    public enum CC_{{param.number}}_{{param.name}} implements CarConfigEnumBase
    {
        {% for value in param.values -%}
        {{value.desc}}(0x{{value.value}},"{{value.desc}}")
        {%- if not loop.last -%}
        ,
        {% else %};
        {%- endif %}

        {%- endfor %}

        public final int value;
        public final String name;
        public static final int ParamNumber = {{param.number}};

        CC_{{param.number}}_{{param.name}}(int value, String name){
            this.value=value;
            this.name=name;
        }
        public int getParam() {
            return ParamNumber;
        }
        public String getName(){
            return this.name;
        }
        public int getValue(){
            return this.value;
        }
    };

    static {
            {% for value in param.values -%}
            map_CC_{{param.number}}.put(0x{{value.value}}, CC_{{param.number}}_{{param.name}}.{{value.desc}});
            {% endfor -%}
            getValueData.put(CC_{{param.number}}_{{param.name}}.class, map_CC_{{param.number}});
    }
{% endfor %}
}