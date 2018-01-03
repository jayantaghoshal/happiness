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


    private static HashMap<Integer, CarConfigEnumBase> map_CC_1 = new HashMap<>();
    public enum CC_1_VehicleType implements CarConfigEnumBase
    {
        L541(0x01,"L541"),
        V542(0x02,"V542"),
        V543(0x03,"V543"),
        V526(0x04,"V526"),
        V426(0x05,"V426"),
        V541(0x06,"V541"),
        V316(0x07,"V316"),
        V320(0x08,"V320"),
        V323(0x09,"V323"),
        V431(0x0A,"V431"),
        V432(0x0B,"V432"),
        L431(0x0C,"L431"),
        V433(0x0D,"V433"),
        PS14(0x0E,"PS14"),
        V331(0x0F,"V331"),
        CX11(0x80,"CX11"),
        CS11(0x81,"CS11"),
        CH11(0x82,"CH11"),
        CC11(0x83,"CC11"),
        CS12(0x84,"CS12"),
        DCY11(0x85,"DCY11"),
        HRE1(0xE0,"HRE1");

        public final int value;
        public final String name;
        public static final int ParamNumber = 1;

        CC_1_VehicleType(int value, String name){
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
            map_CC_1.put(0x01, CC_1_VehicleType.L541);
            map_CC_1.put(0x02, CC_1_VehicleType.V542);
            map_CC_1.put(0x03, CC_1_VehicleType.V543);
            map_CC_1.put(0x04, CC_1_VehicleType.V526);
            map_CC_1.put(0x05, CC_1_VehicleType.V426);
            map_CC_1.put(0x06, CC_1_VehicleType.V541);
            map_CC_1.put(0x07, CC_1_VehicleType.V316);
            map_CC_1.put(0x08, CC_1_VehicleType.V320);
            map_CC_1.put(0x09, CC_1_VehicleType.V323);
            map_CC_1.put(0x0A, CC_1_VehicleType.V431);
            map_CC_1.put(0x0B, CC_1_VehicleType.V432);
            map_CC_1.put(0x0C, CC_1_VehicleType.L431);
            map_CC_1.put(0x0D, CC_1_VehicleType.V433);
            map_CC_1.put(0x0E, CC_1_VehicleType.PS14);
            map_CC_1.put(0x0F, CC_1_VehicleType.V331);
            map_CC_1.put(0x80, CC_1_VehicleType.CX11);
            map_CC_1.put(0x81, CC_1_VehicleType.CS11);
            map_CC_1.put(0x82, CC_1_VehicleType.CH11);
            map_CC_1.put(0x83, CC_1_VehicleType.CC11);
            map_CC_1.put(0x84, CC_1_VehicleType.CS12);
            map_CC_1.put(0x85, CC_1_VehicleType.DCY11);
            map_CC_1.put(0xE0, CC_1_VehicleType.HRE1);
            getValueData.put(CC_1_VehicleType.class, map_CC_1);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_3 = new HashMap<>();
    public enum CC_3_TransmissionDriveline implements CarConfigEnumBase
    {
        Two_wheel_drive(0x01,"Two_wheel_drive"),
        AWD_All_wheel_drive(0x02,"AWD_All_wheel_drive");

        public final int value;
        public final String name;
        public static final int ParamNumber = 3;

        CC_3_TransmissionDriveline(int value, String name){
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
            map_CC_3.put(0x01, CC_3_TransmissionDriveline.Two_wheel_drive);
            map_CC_3.put(0x02, CC_3_TransmissionDriveline.AWD_All_wheel_drive);
            getValueData.put(CC_3_TransmissionDriveline.class, map_CC_3);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_4 = new HashMap<>();
    public enum CC_4_ElectricDriveline implements CarConfigEnumBase
    {
        Without_Power_Flow(0x01,"Without_Power_Flow"),
        Combustion_Engine_FWD_and_Electric_Motor_FWD(0x02,"Combustion_Engine_FWD_and_Electric_Motor_FWD"),
        Combustion_Engine_FWD_and_Electric_Motor_RWD(0x03,"Combustion_Engine_FWD_and_Electric_Motor_RWD"),
        Combustion_Engine_RWD_and_Electric_Motor_FWD(0x04,"Combustion_Engine_RWD_and_Electric_Motor_FWD"),
        Combustion_Engine_RWD_and_Electric_Motor_RWD(0x05,"Combustion_Engine_RWD_and_Electric_Motor_RWD"),
        Electric_Motor_AWD(0x06,"Electric_Motor_AWD"),
        Electric_Motor_FWD(0x07,"Electric_Motor_FWD"),
        Electric_Motor_RWD(0x08,"Electric_Motor_RWD");

        public final int value;
        public final String name;
        public static final int ParamNumber = 4;

        CC_4_ElectricDriveline(int value, String name){
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
            map_CC_4.put(0x01, CC_4_ElectricDriveline.Without_Power_Flow);
            map_CC_4.put(0x02, CC_4_ElectricDriveline.Combustion_Engine_FWD_and_Electric_Motor_FWD);
            map_CC_4.put(0x03, CC_4_ElectricDriveline.Combustion_Engine_FWD_and_Electric_Motor_RWD);
            map_CC_4.put(0x04, CC_4_ElectricDriveline.Combustion_Engine_RWD_and_Electric_Motor_FWD);
            map_CC_4.put(0x05, CC_4_ElectricDriveline.Combustion_Engine_RWD_and_Electric_Motor_RWD);
            map_CC_4.put(0x06, CC_4_ElectricDriveline.Electric_Motor_AWD);
            map_CC_4.put(0x07, CC_4_ElectricDriveline.Electric_Motor_FWD);
            map_CC_4.put(0x08, CC_4_ElectricDriveline.Electric_Motor_RWD);
            getValueData.put(CC_4_ElectricDriveline.class, map_CC_4);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_5 = new HashMap<>();
    public enum CC_5_Engine implements CarConfigEnumBase
    {
        VED4_2_0L_225_470(0x01,"VED4_2_0L_225_470"),
        VEP4_2_0L_190_300(0x02,"VEP4_2_0L_190_300"),
        VED4_2_0L_190_400(0x03,"VED4_2_0L_190_400"),
        VEP4_2_0L_249_350(0x04,"VEP4_2_0L_249_350"),
        VEP4_2_0L_254_350(0x05,"VEP4_2_0L_254_350"),
        VEP4_2_0L_260_350_PZEV(0x06,"VEP4_2_0L_260_350_PZEV"),
        VEP4_2_0L_233_350(0x07,"VEP4_2_0L_233_350"),
        VEP4_2_0L_230_350_PZEV(0x08,"VEP4_2_0L_230_350_PZEV"),
        VED4_2_0L_150_320(0x09,"VED4_2_0L_150_320"),
        VEP4_2_0L_320_400(0x0A,"VEP4_2_0L_320_400"),
        VEP4_2_0L_306_380_H_PZEV(0x0B,"VEP4_2_0L_306_380_H_PZEV"),
        VEP4_2_0L_266_380(0x0C,"VEP4_2_0L_266_380"),
        VEP4_2_0L_266_380_PZEV(0x0D,"VEP4_2_0L_266_380_PZEV"),
        VED4_2_0L_190_420(0x0E,"VED4_2_0L_190_420"),
        VEP4_2_0L_197_330(0x0F,"VEP4_2_0L_197_330"),
        VED4_2_0L_231_480(0x10,"VED4_2_0L_231_480"),
        VED4_2_0L_150_350(0x11,"VED4_2_0L_150_350"),
        VEP4_2_0L_156_265(0x12,"VEP4_2_0L_156_265"),
        VEP4_2_0L_320_400_SULEV(0x13,"VEP4_2_0L_320_400_SULEV"),
        VEP4_2_0L_306_380_H(0x14,"VEP4_2_0L_306_380_H"),
        VED4_2_0L_190_400_X(0x15,"VED4_2_0L_190_400_X"),
        VED4_2_0L_233_470(0x16,"VED4_2_0L_233_470"),
        VEP4_2_0L_254_350_H(0x17,"VEP4_2_0L_254_350_H"),
        GEP3_MP_1_5L_156_265(0x18,"GEP3_MP_1_5L_156_265"),
        GEP3_MP_1_5L_150_265(0x19,"GEP3_MP_1_5L_150_265"),
        GEP3_MP_1_5L_180_265_H(0x1A,"GEP3_MP_1_5L_180_265_H"),
        VEP4_2_0L_197_300(0x1B,"VEP4_2_0L_197_300"),
        VED4_2_0L_238_480(0x1C,"VED4_2_0L_238_480"),
        VED4_2_0L_156_330(0x1D,"VED4_2_0L_156_330"),
        VED4_2_0L_197_420(0x1E,"VED4_2_0L_197_420"),
        GEP3_LP_1_5L_129_245(0x1F,"GEP3_LP_1_5L_129_245"),
        VED4_2_0L_122_280(0x20,"VED4_2_0L_122_280"),
        VED4_2_0L_156_330_X(0x21,"VED4_2_0L_156_330_X"),
        GEP3_HP_1_5L_190_270(0x22,"GEP3_HP_1_5L_190_270"),
        VEP4_2_0L_354_450_H(0x23,"VEP4_2_0L_354_450_H"),
        VEP4_2_0L_340_420(0x24,"VEP4_2_0L_340_420"),
        VEP4_2_0L_258_350(0x25,"VEP4_2_0L_258_350"),
        VEP4_2_0L_247_350(0x26,"VEP4_2_0L_247_350"),
        VEP4_2_0L_252_350(0x27,"VEP4_2_0L_252_350"),
        VEP4_2_0L_345_400_H(0x28,"VEP4_2_0L_345_400_H"),
        VEP4_2_0L_345_400_H_PZEV(0x29,"VEP4_2_0L_345_400_H_PZEV"),
        GEP3_HP_1_5L_197_285_H(0x2A,"GEP3_HP_1_5L_197_285_H"),
        GEM3_1_0_122_180(0x80,"GEM3_1_0_122_180"),
        GEP_180_265_Non_VCC(0xE0,"GEP_180_265_Non_VCC"),
        Without_cumbustion_engine_for_propulsion(0xFD,"Without_cumbustion_engine_for_propulsion");

        public final int value;
        public final String name;
        public static final int ParamNumber = 5;

        CC_5_Engine(int value, String name){
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
            map_CC_5.put(0x01, CC_5_Engine.VED4_2_0L_225_470);
            map_CC_5.put(0x02, CC_5_Engine.VEP4_2_0L_190_300);
            map_CC_5.put(0x03, CC_5_Engine.VED4_2_0L_190_400);
            map_CC_5.put(0x04, CC_5_Engine.VEP4_2_0L_249_350);
            map_CC_5.put(0x05, CC_5_Engine.VEP4_2_0L_254_350);
            map_CC_5.put(0x06, CC_5_Engine.VEP4_2_0L_260_350_PZEV);
            map_CC_5.put(0x07, CC_5_Engine.VEP4_2_0L_233_350);
            map_CC_5.put(0x08, CC_5_Engine.VEP4_2_0L_230_350_PZEV);
            map_CC_5.put(0x09, CC_5_Engine.VED4_2_0L_150_320);
            map_CC_5.put(0x0A, CC_5_Engine.VEP4_2_0L_320_400);
            map_CC_5.put(0x0B, CC_5_Engine.VEP4_2_0L_306_380_H_PZEV);
            map_CC_5.put(0x0C, CC_5_Engine.VEP4_2_0L_266_380);
            map_CC_5.put(0x0D, CC_5_Engine.VEP4_2_0L_266_380_PZEV);
            map_CC_5.put(0x0E, CC_5_Engine.VED4_2_0L_190_420);
            map_CC_5.put(0x0F, CC_5_Engine.VEP4_2_0L_197_330);
            map_CC_5.put(0x10, CC_5_Engine.VED4_2_0L_231_480);
            map_CC_5.put(0x11, CC_5_Engine.VED4_2_0L_150_350);
            map_CC_5.put(0x12, CC_5_Engine.VEP4_2_0L_156_265);
            map_CC_5.put(0x13, CC_5_Engine.VEP4_2_0L_320_400_SULEV);
            map_CC_5.put(0x14, CC_5_Engine.VEP4_2_0L_306_380_H);
            map_CC_5.put(0x15, CC_5_Engine.VED4_2_0L_190_400_X);
            map_CC_5.put(0x16, CC_5_Engine.VED4_2_0L_233_470);
            map_CC_5.put(0x17, CC_5_Engine.VEP4_2_0L_254_350_H);
            map_CC_5.put(0x18, CC_5_Engine.GEP3_MP_1_5L_156_265);
            map_CC_5.put(0x19, CC_5_Engine.GEP3_MP_1_5L_150_265);
            map_CC_5.put(0x1A, CC_5_Engine.GEP3_MP_1_5L_180_265_H);
            map_CC_5.put(0x1B, CC_5_Engine.VEP4_2_0L_197_300);
            map_CC_5.put(0x1C, CC_5_Engine.VED4_2_0L_238_480);
            map_CC_5.put(0x1D, CC_5_Engine.VED4_2_0L_156_330);
            map_CC_5.put(0x1E, CC_5_Engine.VED4_2_0L_197_420);
            map_CC_5.put(0x1F, CC_5_Engine.GEP3_LP_1_5L_129_245);
            map_CC_5.put(0x20, CC_5_Engine.VED4_2_0L_122_280);
            map_CC_5.put(0x21, CC_5_Engine.VED4_2_0L_156_330_X);
            map_CC_5.put(0x22, CC_5_Engine.GEP3_HP_1_5L_190_270);
            map_CC_5.put(0x23, CC_5_Engine.VEP4_2_0L_354_450_H);
            map_CC_5.put(0x24, CC_5_Engine.VEP4_2_0L_340_420);
            map_CC_5.put(0x25, CC_5_Engine.VEP4_2_0L_258_350);
            map_CC_5.put(0x26, CC_5_Engine.VEP4_2_0L_247_350);
            map_CC_5.put(0x27, CC_5_Engine.VEP4_2_0L_252_350);
            map_CC_5.put(0x28, CC_5_Engine.VEP4_2_0L_345_400_H);
            map_CC_5.put(0x29, CC_5_Engine.VEP4_2_0L_345_400_H_PZEV);
            map_CC_5.put(0x2A, CC_5_Engine.GEP3_HP_1_5L_197_285_H);
            map_CC_5.put(0x80, CC_5_Engine.GEM3_1_0_122_180);
            map_CC_5.put(0xE0, CC_5_Engine.GEP_180_265_Non_VCC);
            map_CC_5.put(0xFD, CC_5_Engine.Without_cumbustion_engine_for_propulsion);
            getValueData.put(CC_5_Engine.class, map_CC_5);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_6 = new HashMap<>();
    public enum CC_6_Fuel implements CarConfigEnumBase
    {
        Petrol(0x01,"Petrol"),
        Diesel(0x02,"Diesel");

        public final int value;
        public final String name;
        public static final int ParamNumber = 6;

        CC_6_Fuel(int value, String name){
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
            map_CC_6.put(0x01, CC_6_Fuel.Petrol);
            map_CC_6.put(0x02, CC_6_Fuel.Diesel);
            getValueData.put(CC_6_Fuel.class, map_CC_6);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_8 = new HashMap<>();
    public enum CC_8_SteeringWheelPosition implements CarConfigEnumBase
    {
        Left_Hand_Drive(0x01,"Left_Hand_Drive"),
        Right_Hand_Drive(0x02,"Right_Hand_Drive");

        public final int value;
        public final String name;
        public static final int ParamNumber = 8;

        CC_8_SteeringWheelPosition(int value, String name){
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
            map_CC_8.put(0x01, CC_8_SteeringWheelPosition.Left_Hand_Drive);
            map_CC_8.put(0x02, CC_8_SteeringWheelPosition.Right_Hand_Drive);
            getValueData.put(CC_8_SteeringWheelPosition.class, map_CC_8);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_9 = new HashMap<>();
    public enum CC_9_Gearbox implements CarConfigEnumBase
    {
        AWF22FWD_Automatic_8_speed_FWD(0x01,"AWF22FWD_Automatic_8_speed_FWD"),
        M66FWD_Manual_6_speed_FWD(0x02,"M66FWD_Manual_6_speed_FWD"),
        C6FWD_Manual_6_speed_FWD(0x03,"C6FWD_Manual_6_speed_FWD"),
        AWF22AWD_Automatic_8_speed_AWD(0x04,"AWF22AWD_Automatic_8_speed_AWD"),
        M66AWD_Manual_6_speed_AWD(0x05,"M66AWD_Manual_6_speed_AWD"),
        AWF21FWD_Automatic_6_speed_FWD(0x06,"AWF21FWD_Automatic_6_speed_FWD"),
        SevenDCTFWD_Dual_Clutch_7_speed_FWD(0x07,"SevenDCTFWD_Dual_Clutch_7_speed_FWD"),
        M76FWD_Manual_6_speed_FWD(0x08,"M76FWD_Manual_6_speed_FWD"),
        SevenDCTAWD_Dual_Clutch_7_speed_AWD(0x09,"SevenDCTAWD_Dual_Clutch_7_speed_AWD"),
        AW8GFWD_Automatic_8_speed_FWD(0x0A,"AW8GFWD_Automatic_8_speed_FWD"),
        AW8GAWD_Automatic_8_speed_AWD(0x0B,"AW8GAWD_Automatic_8_speed_AWD"),
        No_Gearbox(0x7F,"No_Gearbox"),
        MX65FWD_Manual_6_Speed_FWD(0x80,"MX65FWD_Manual_6_Speed_FWD");

        public final int value;
        public final String name;
        public static final int ParamNumber = 9;

        CC_9_Gearbox(int value, String name){
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
            map_CC_9.put(0x01, CC_9_Gearbox.AWF22FWD_Automatic_8_speed_FWD);
            map_CC_9.put(0x02, CC_9_Gearbox.M66FWD_Manual_6_speed_FWD);
            map_CC_9.put(0x03, CC_9_Gearbox.C6FWD_Manual_6_speed_FWD);
            map_CC_9.put(0x04, CC_9_Gearbox.AWF22AWD_Automatic_8_speed_AWD);
            map_CC_9.put(0x05, CC_9_Gearbox.M66AWD_Manual_6_speed_AWD);
            map_CC_9.put(0x06, CC_9_Gearbox.AWF21FWD_Automatic_6_speed_FWD);
            map_CC_9.put(0x07, CC_9_Gearbox.SevenDCTFWD_Dual_Clutch_7_speed_FWD);
            map_CC_9.put(0x08, CC_9_Gearbox.M76FWD_Manual_6_speed_FWD);
            map_CC_9.put(0x09, CC_9_Gearbox.SevenDCTAWD_Dual_Clutch_7_speed_AWD);
            map_CC_9.put(0x0A, CC_9_Gearbox.AW8GFWD_Automatic_8_speed_FWD);
            map_CC_9.put(0x0B, CC_9_Gearbox.AW8GAWD_Automatic_8_speed_AWD);
            map_CC_9.put(0x7F, CC_9_Gearbox.No_Gearbox);
            map_CC_9.put(0x80, CC_9_Gearbox.MX65FWD_Manual_6_Speed_FWD);
            getValueData.put(CC_9_Gearbox.class, map_CC_9);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_10 = new HashMap<>();
    public enum CC_10_GearboxType implements CarConfigEnumBase
    {
        Manual_gearbox(0x01,"Manual_gearbox"),
        Automatic_gearbox(0x02,"Automatic_gearbox");

        public final int value;
        public final String name;
        public static final int ParamNumber = 10;

        CC_10_GearboxType(int value, String name){
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
            map_CC_10.put(0x01, CC_10_GearboxType.Manual_gearbox);
            map_CC_10.put(0x02, CC_10_GearboxType.Automatic_gearbox);
            getValueData.put(CC_10_GearboxType.class, map_CC_10);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_11 = new HashMap<>();
    public enum CC_11_FuelTankVolume implements CarConfigEnumBase
    {
        Volume_71_Litre(0x01,"Volume_71_Litre"),
        Volume_60_Litre(0x02,"Volume_60_Litre"),
        Volume_50_Litre(0x03,"Volume_50_Litre"),
        Volume_55_Litre(0x04,"Volume_55_Litre"),
        Volume_36_Litre(0x05,"Volume_36_Litre"),
        Volume_47_Litre(0x06,"Volume_47_Litre"),
        Volume_54_Litre(0x07,"Volume_54_Litre"),
        Volume_49_Litre(0x08,"Volume_49_Litre"),
        Volume_0_Litre_No_fuel_tank_(0x09,"Volume_0_Litre_No_fuel_tank_"),
        Volume_70_Litre(0x0A,"Volume_70_Litre"),
        Volume_40_Litre(0x80,"Volume_40_Litre");

        public final int value;
        public final String name;
        public static final int ParamNumber = 11;

        CC_11_FuelTankVolume(int value, String name){
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
            map_CC_11.put(0x01, CC_11_FuelTankVolume.Volume_71_Litre);
            map_CC_11.put(0x02, CC_11_FuelTankVolume.Volume_60_Litre);
            map_CC_11.put(0x03, CC_11_FuelTankVolume.Volume_50_Litre);
            map_CC_11.put(0x04, CC_11_FuelTankVolume.Volume_55_Litre);
            map_CC_11.put(0x05, CC_11_FuelTankVolume.Volume_36_Litre);
            map_CC_11.put(0x06, CC_11_FuelTankVolume.Volume_47_Litre);
            map_CC_11.put(0x07, CC_11_FuelTankVolume.Volume_54_Litre);
            map_CC_11.put(0x08, CC_11_FuelTankVolume.Volume_49_Litre);
            map_CC_11.put(0x09, CC_11_FuelTankVolume.Volume_0_Litre_No_fuel_tank_);
            map_CC_11.put(0x0A, CC_11_FuelTankVolume.Volume_70_Litre);
            map_CC_11.put(0x80, CC_11_FuelTankVolume.Volume_40_Litre);
            getValueData.put(CC_11_FuelTankVolume.class, map_CC_11);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_13 = new HashMap<>();
    public enum CC_13_PropulsionType implements CarConfigEnumBase
    {
        Combustion_engine_only(0x01,"Combustion_engine_only"),
        Hybrid_vehicle(0x02,"Hybrid_vehicle"),
        Plugin_hybrid_vehicle(0x03,"Plugin_hybrid_vehicle"),
        Battery_electric_vehicle(0x04,"Battery_electric_vehicle");

        public final int value;
        public final String name;
        public static final int ParamNumber = 13;

        CC_13_PropulsionType(int value, String name){
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
            map_CC_13.put(0x01, CC_13_PropulsionType.Combustion_engine_only);
            map_CC_13.put(0x02, CC_13_PropulsionType.Hybrid_vehicle);
            map_CC_13.put(0x03, CC_13_PropulsionType.Plugin_hybrid_vehicle);
            map_CC_13.put(0x04, CC_13_PropulsionType.Battery_electric_vehicle);
            getValueData.put(CC_13_PropulsionType.class, map_CC_13);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_14 = new HashMap<>();
    public enum CC_14_RemoteMonitoringSystem implements CarConfigEnumBase
    {
        RMS_Disactivated(0x01,"RMS_Disactivated"),
        RMS_Activated(0x02,"RMS_Activated");

        public final int value;
        public final String name;
        public static final int ParamNumber = 14;

        CC_14_RemoteMonitoringSystem(int value, String name){
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
            map_CC_14.put(0x01, CC_14_RemoteMonitoringSystem.RMS_Disactivated);
            map_CC_14.put(0x02, CC_14_RemoteMonitoringSystem.RMS_Activated);
            getValueData.put(CC_14_RemoteMonitoringSystem.class, map_CC_14);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_15 = new HashMap<>();
    public enum CC_15_TurboConfiguration implements CarConfigEnumBase
    {
        Medium_performance(0x01,"Medium_performance"),
        High_performance(0x02,"High_performance"),
        Low_performance(0x03,"Low_performance"),
        Entry_performance(0x04,"Entry_performance");

        public final int value;
        public final String name;
        public static final int ParamNumber = 15;

        CC_15_TurboConfiguration(int value, String name){
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
            map_CC_15.put(0x01, CC_15_TurboConfiguration.Medium_performance);
            map_CC_15.put(0x02, CC_15_TurboConfiguration.High_performance);
            map_CC_15.put(0x03, CC_15_TurboConfiguration.Low_performance);
            map_CC_15.put(0x04, CC_15_TurboConfiguration.Entry_performance);
            getValueData.put(CC_15_TurboConfiguration.class, map_CC_15);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_16 = new HashMap<>();
    public enum CC_16_PowerSteeringPersonalisation implements CarConfigEnumBase
    {
        Without_Electro_Power_Steering_Personalisation(0x01,"Without_Electro_Power_Steering_Personalisation"),
        With_Electro_Power_Steering_Personalisation(0x02,"With_Electro_Power_Steering_Personalisation");

        public final int value;
        public final String name;
        public static final int ParamNumber = 16;

        CC_16_PowerSteeringPersonalisation(int value, String name){
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
            map_CC_16.put(0x01, CC_16_PowerSteeringPersonalisation.Without_Electro_Power_Steering_Personalisation);
            map_CC_16.put(0x02, CC_16_PowerSteeringPersonalisation.With_Electro_Power_Steering_Personalisation);
            getValueData.put(CC_16_PowerSteeringPersonalisation.class, map_CC_16);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_18 = new HashMap<>();
    public enum CC_18_TyreRollingCircumference implements CarConfigEnumBase
    {
        Rolling_circumference_1900mm(0x28,"Rolling_circumference_1900mm"),
        Rolling_circumference_1905mm(0x29,"Rolling_circumference_1905mm"),
        Rolling_circumference_1910mm(0x2A,"Rolling_circumference_1910mm"),
        Rolling_circumference_1915mm(0x2B,"Rolling_circumference_1915mm"),
        Rolling_circumference_1920mm(0x2C,"Rolling_circumference_1920mm"),
        Rolling_circumference_1925mm(0x2D,"Rolling_circumference_1925mm"),
        Rolling_circumference_1930mm(0x2E,"Rolling_circumference_1930mm"),
        Rolling_circumference_1935mm(0x2F,"Rolling_circumference_1935mm"),
        Rolling_circumference_1940mm(0x30,"Rolling_circumference_1940mm"),
        Rolling_circumference_1945mm(0x31,"Rolling_circumference_1945mm"),
        Rolling_circumference_1950mm(0x32,"Rolling_circumference_1950mm"),
        Rolling_circumference_1955mm(0x33,"Rolling_circumference_1955mm"),
        Rolling_circumference_1960mm(0x34,"Rolling_circumference_1960mm"),
        Rolling_circumference_1965mm(0x35,"Rolling_circumference_1965mm"),
        Rolling_circumference_1970mm(0x36,"Rolling_circumference_1970mm"),
        Rolling_circumference_1975mm(0x37,"Rolling_circumference_1975mm"),
        Rolling_circumference_1980mm(0x38,"Rolling_circumference_1980mm"),
        Rolling_circumference_1985mm(0x39,"Rolling_circumference_1985mm"),
        Rolling_circumference_1990mm(0x3A,"Rolling_circumference_1990mm"),
        Rolling_circumference_1995mm(0x3B,"Rolling_circumference_1995mm"),
        Rolling_circumference_2000mm(0x3C,"Rolling_circumference_2000mm"),
        Rolling_circumference_2005mm(0x3D,"Rolling_circumference_2005mm"),
        Rolling_circumference_2010mm(0x3E,"Rolling_circumference_2010mm"),
        Rolling_circumference_2015mm(0x3F,"Rolling_circumference_2015mm"),
        Rolling_circumference_2020mm(0x40,"Rolling_circumference_2020mm"),
        Rolling_circumference_2025mm(0x41,"Rolling_circumference_2025mm"),
        Rolling_circumference_2030mm(0x42,"Rolling_circumference_2030mm"),
        Rolling_circumference_2035mm(0x43,"Rolling_circumference_2035mm"),
        Rolling_circumference_2040mm(0x44,"Rolling_circumference_2040mm"),
        Rolling_circumference_2045mm(0x45,"Rolling_circumference_2045mm"),
        Rolling_circumference_2050mm(0x46,"Rolling_circumference_2050mm"),
        Rolling_circumference_2055mm(0x47,"Rolling_circumference_2055mm"),
        Rolling_circumference_2060mm(0x48,"Rolling_circumference_2060mm"),
        Rolling_circumference_2065mm(0x49,"Rolling_circumference_2065mm"),
        Rolling_circumference_2070mm(0x4A,"Rolling_circumference_2070mm"),
        Rolling_circumference_2075mm(0x4B,"Rolling_circumference_2075mm"),
        Rolling_circumference_2080mm(0x4C,"Rolling_circumference_2080mm"),
        Rolling_circumference_2085mm(0x4D,"Rolling_circumference_2085mm"),
        Rolling_circumference_2090mm(0x4E,"Rolling_circumference_2090mm"),
        Rolling_circumference_2095mm(0x4F,"Rolling_circumference_2095mm"),
        Rolling_circumference_2100mm(0x50,"Rolling_circumference_2100mm"),
        Rolling_circumference_2105mm(0x51,"Rolling_circumference_2105mm"),
        Rolling_circumference_2110mm(0x52,"Rolling_circumference_2110mm"),
        Rolling_circumference_2115mm(0x53,"Rolling_circumference_2115mm"),
        Rolling_circumference_2120mm(0x54,"Rolling_circumference_2120mm"),
        Rolling_circumference_2125mm(0x55,"Rolling_circumference_2125mm"),
        Rolling_circumference_2130mm(0x56,"Rolling_circumference_2130mm"),
        Rolling_circumference_2135mm(0x57,"Rolling_circumference_2135mm"),
        Rolling_circumference_2140mm(0x58,"Rolling_circumference_2140mm"),
        Rolling_circumference_2145mm(0x59,"Rolling_circumference_2145mm"),
        Rolling_circumference_2150mm(0x5A,"Rolling_circumference_2150mm"),
        Rolling_circumference_2155mm(0x5B,"Rolling_circumference_2155mm"),
        Rolling_circumference_2160mm(0x5C,"Rolling_circumference_2160mm"),
        Rolling_circumference_2165mm(0x5D,"Rolling_circumference_2165mm"),
        Rolling_circumference_2170mm(0x5E,"Rolling_circumference_2170mm"),
        Rolling_circumference_2175mm(0x5F,"Rolling_circumference_2175mm"),
        Rolling_circumference_2180mm(0x60,"Rolling_circumference_2180mm"),
        Rolling_circumference_2185mm(0x61,"Rolling_circumference_2185mm"),
        Rolling_circumference_2190mm(0x62,"Rolling_circumference_2190mm"),
        Rolling_circumference_2195mm(0x63,"Rolling_circumference_2195mm"),
        Rolling_circumference_2200mm(0x64,"Rolling_circumference_2200mm"),
        Rolling_circumference_2205mm(0x65,"Rolling_circumference_2205mm"),
        Rolling_circumference_2210mm(0x66,"Rolling_circumference_2210mm"),
        Rolling_circumference_2215mm(0x67,"Rolling_circumference_2215mm"),
        Rolling_circumference_2220mm(0x68,"Rolling_circumference_2220mm"),
        Rolling_circumference_2225mm(0x69,"Rolling_circumference_2225mm"),
        Rolling_circumference_2230mm(0x6A,"Rolling_circumference_2230mm"),
        Rolling_circumference_2235mm(0x6B,"Rolling_circumference_2235mm"),
        Rolling_circumference_2240mm(0x6C,"Rolling_circumference_2240mm"),
        Rolling_circumference_2245mm(0x6D,"Rolling_circumference_2245mm"),
        Rolling_circumference_2250mm(0x6E,"Rolling_circumference_2250mm"),
        Rolling_circumference_2255mm(0x6F,"Rolling_circumference_2255mm"),
        Rolling_circumference_2260mm(0x70,"Rolling_circumference_2260mm"),
        Rolling_circumference_2265mm(0x71,"Rolling_circumference_2265mm"),
        Rolling_circumference_2270mm(0x72,"Rolling_circumference_2270mm"),
        Rolling_circumference_2275mm(0x73,"Rolling_circumference_2275mm"),
        Rolling_circumference_2280mm(0x74,"Rolling_circumference_2280mm"),
        Rolling_circumference_2285mm(0x75,"Rolling_circumference_2285mm"),
        Rolling_circumference_2290mm(0x76,"Rolling_circumference_2290mm"),
        Rolling_circumference_2295mm(0x77,"Rolling_circumference_2295mm"),
        Rolling_circumference_2300mm(0x78,"Rolling_circumference_2300mm"),
        Rolling_circumference_2305mm(0x79,"Rolling_circumference_2305mm"),
        Rolling_circumference_2310mm(0x7A,"Rolling_circumference_2310mm"),
        Rolling_circumference_2315mm(0x7B,"Rolling_circumference_2315mm"),
        Rolling_circumference_2320mm(0x7C,"Rolling_circumference_2320mm"),
        Rolling_circumference_2325mm(0x7D,"Rolling_circumference_2325mm"),
        Rolling_circumference_2330mm(0x7E,"Rolling_circumference_2330mm"),
        Rolling_circumference_2335mm(0x7F,"Rolling_circumference_2335mm"),
        Rolling_circumference_2340mm(0x80,"Rolling_circumference_2340mm"),
        Rolling_circumference_2345mm(0x81,"Rolling_circumference_2345mm"),
        Rolling_circumference_2350mm(0x82,"Rolling_circumference_2350mm"),
        Rolling_circumference_2355mm(0x83,"Rolling_circumference_2355mm"),
        Rolling_circumference_2360mm(0x84,"Rolling_circumference_2360mm"),
        Rolling_circumference_2365mm(0x85,"Rolling_circumference_2365mm"),
        Rolling_circumference_2370mm(0x86,"Rolling_circumference_2370mm"),
        Rolling_circumference_2375mm(0x87,"Rolling_circumference_2375mm"),
        Rolling_circumference_2380mm(0x88,"Rolling_circumference_2380mm"),
        Rolling_circumference_2385mm(0x89,"Rolling_circumference_2385mm"),
        Rolling_circumference_2390mm(0x8A,"Rolling_circumference_2390mm"),
        Rolling_circumference_2395mm(0x8B,"Rolling_circumference_2395mm"),
        Rolling_circumference_2400mm(0x8C,"Rolling_circumference_2400mm");

        public final int value;
        public final String name;
        public static final int ParamNumber = 18;

        CC_18_TyreRollingCircumference(int value, String name){
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
            map_CC_18.put(0x28, CC_18_TyreRollingCircumference.Rolling_circumference_1900mm);
            map_CC_18.put(0x29, CC_18_TyreRollingCircumference.Rolling_circumference_1905mm);
            map_CC_18.put(0x2A, CC_18_TyreRollingCircumference.Rolling_circumference_1910mm);
            map_CC_18.put(0x2B, CC_18_TyreRollingCircumference.Rolling_circumference_1915mm);
            map_CC_18.put(0x2C, CC_18_TyreRollingCircumference.Rolling_circumference_1920mm);
            map_CC_18.put(0x2D, CC_18_TyreRollingCircumference.Rolling_circumference_1925mm);
            map_CC_18.put(0x2E, CC_18_TyreRollingCircumference.Rolling_circumference_1930mm);
            map_CC_18.put(0x2F, CC_18_TyreRollingCircumference.Rolling_circumference_1935mm);
            map_CC_18.put(0x30, CC_18_TyreRollingCircumference.Rolling_circumference_1940mm);
            map_CC_18.put(0x31, CC_18_TyreRollingCircumference.Rolling_circumference_1945mm);
            map_CC_18.put(0x32, CC_18_TyreRollingCircumference.Rolling_circumference_1950mm);
            map_CC_18.put(0x33, CC_18_TyreRollingCircumference.Rolling_circumference_1955mm);
            map_CC_18.put(0x34, CC_18_TyreRollingCircumference.Rolling_circumference_1960mm);
            map_CC_18.put(0x35, CC_18_TyreRollingCircumference.Rolling_circumference_1965mm);
            map_CC_18.put(0x36, CC_18_TyreRollingCircumference.Rolling_circumference_1970mm);
            map_CC_18.put(0x37, CC_18_TyreRollingCircumference.Rolling_circumference_1975mm);
            map_CC_18.put(0x38, CC_18_TyreRollingCircumference.Rolling_circumference_1980mm);
            map_CC_18.put(0x39, CC_18_TyreRollingCircumference.Rolling_circumference_1985mm);
            map_CC_18.put(0x3A, CC_18_TyreRollingCircumference.Rolling_circumference_1990mm);
            map_CC_18.put(0x3B, CC_18_TyreRollingCircumference.Rolling_circumference_1995mm);
            map_CC_18.put(0x3C, CC_18_TyreRollingCircumference.Rolling_circumference_2000mm);
            map_CC_18.put(0x3D, CC_18_TyreRollingCircumference.Rolling_circumference_2005mm);
            map_CC_18.put(0x3E, CC_18_TyreRollingCircumference.Rolling_circumference_2010mm);
            map_CC_18.put(0x3F, CC_18_TyreRollingCircumference.Rolling_circumference_2015mm);
            map_CC_18.put(0x40, CC_18_TyreRollingCircumference.Rolling_circumference_2020mm);
            map_CC_18.put(0x41, CC_18_TyreRollingCircumference.Rolling_circumference_2025mm);
            map_CC_18.put(0x42, CC_18_TyreRollingCircumference.Rolling_circumference_2030mm);
            map_CC_18.put(0x43, CC_18_TyreRollingCircumference.Rolling_circumference_2035mm);
            map_CC_18.put(0x44, CC_18_TyreRollingCircumference.Rolling_circumference_2040mm);
            map_CC_18.put(0x45, CC_18_TyreRollingCircumference.Rolling_circumference_2045mm);
            map_CC_18.put(0x46, CC_18_TyreRollingCircumference.Rolling_circumference_2050mm);
            map_CC_18.put(0x47, CC_18_TyreRollingCircumference.Rolling_circumference_2055mm);
            map_CC_18.put(0x48, CC_18_TyreRollingCircumference.Rolling_circumference_2060mm);
            map_CC_18.put(0x49, CC_18_TyreRollingCircumference.Rolling_circumference_2065mm);
            map_CC_18.put(0x4A, CC_18_TyreRollingCircumference.Rolling_circumference_2070mm);
            map_CC_18.put(0x4B, CC_18_TyreRollingCircumference.Rolling_circumference_2075mm);
            map_CC_18.put(0x4C, CC_18_TyreRollingCircumference.Rolling_circumference_2080mm);
            map_CC_18.put(0x4D, CC_18_TyreRollingCircumference.Rolling_circumference_2085mm);
            map_CC_18.put(0x4E, CC_18_TyreRollingCircumference.Rolling_circumference_2090mm);
            map_CC_18.put(0x4F, CC_18_TyreRollingCircumference.Rolling_circumference_2095mm);
            map_CC_18.put(0x50, CC_18_TyreRollingCircumference.Rolling_circumference_2100mm);
            map_CC_18.put(0x51, CC_18_TyreRollingCircumference.Rolling_circumference_2105mm);
            map_CC_18.put(0x52, CC_18_TyreRollingCircumference.Rolling_circumference_2110mm);
            map_CC_18.put(0x53, CC_18_TyreRollingCircumference.Rolling_circumference_2115mm);
            map_CC_18.put(0x54, CC_18_TyreRollingCircumference.Rolling_circumference_2120mm);
            map_CC_18.put(0x55, CC_18_TyreRollingCircumference.Rolling_circumference_2125mm);
            map_CC_18.put(0x56, CC_18_TyreRollingCircumference.Rolling_circumference_2130mm);
            map_CC_18.put(0x57, CC_18_TyreRollingCircumference.Rolling_circumference_2135mm);
            map_CC_18.put(0x58, CC_18_TyreRollingCircumference.Rolling_circumference_2140mm);
            map_CC_18.put(0x59, CC_18_TyreRollingCircumference.Rolling_circumference_2145mm);
            map_CC_18.put(0x5A, CC_18_TyreRollingCircumference.Rolling_circumference_2150mm);
            map_CC_18.put(0x5B, CC_18_TyreRollingCircumference.Rolling_circumference_2155mm);
            map_CC_18.put(0x5C, CC_18_TyreRollingCircumference.Rolling_circumference_2160mm);
            map_CC_18.put(0x5D, CC_18_TyreRollingCircumference.Rolling_circumference_2165mm);
            map_CC_18.put(0x5E, CC_18_TyreRollingCircumference.Rolling_circumference_2170mm);
            map_CC_18.put(0x5F, CC_18_TyreRollingCircumference.Rolling_circumference_2175mm);
            map_CC_18.put(0x60, CC_18_TyreRollingCircumference.Rolling_circumference_2180mm);
            map_CC_18.put(0x61, CC_18_TyreRollingCircumference.Rolling_circumference_2185mm);
            map_CC_18.put(0x62, CC_18_TyreRollingCircumference.Rolling_circumference_2190mm);
            map_CC_18.put(0x63, CC_18_TyreRollingCircumference.Rolling_circumference_2195mm);
            map_CC_18.put(0x64, CC_18_TyreRollingCircumference.Rolling_circumference_2200mm);
            map_CC_18.put(0x65, CC_18_TyreRollingCircumference.Rolling_circumference_2205mm);
            map_CC_18.put(0x66, CC_18_TyreRollingCircumference.Rolling_circumference_2210mm);
            map_CC_18.put(0x67, CC_18_TyreRollingCircumference.Rolling_circumference_2215mm);
            map_CC_18.put(0x68, CC_18_TyreRollingCircumference.Rolling_circumference_2220mm);
            map_CC_18.put(0x69, CC_18_TyreRollingCircumference.Rolling_circumference_2225mm);
            map_CC_18.put(0x6A, CC_18_TyreRollingCircumference.Rolling_circumference_2230mm);
            map_CC_18.put(0x6B, CC_18_TyreRollingCircumference.Rolling_circumference_2235mm);
            map_CC_18.put(0x6C, CC_18_TyreRollingCircumference.Rolling_circumference_2240mm);
            map_CC_18.put(0x6D, CC_18_TyreRollingCircumference.Rolling_circumference_2245mm);
            map_CC_18.put(0x6E, CC_18_TyreRollingCircumference.Rolling_circumference_2250mm);
            map_CC_18.put(0x6F, CC_18_TyreRollingCircumference.Rolling_circumference_2255mm);
            map_CC_18.put(0x70, CC_18_TyreRollingCircumference.Rolling_circumference_2260mm);
            map_CC_18.put(0x71, CC_18_TyreRollingCircumference.Rolling_circumference_2265mm);
            map_CC_18.put(0x72, CC_18_TyreRollingCircumference.Rolling_circumference_2270mm);
            map_CC_18.put(0x73, CC_18_TyreRollingCircumference.Rolling_circumference_2275mm);
            map_CC_18.put(0x74, CC_18_TyreRollingCircumference.Rolling_circumference_2280mm);
            map_CC_18.put(0x75, CC_18_TyreRollingCircumference.Rolling_circumference_2285mm);
            map_CC_18.put(0x76, CC_18_TyreRollingCircumference.Rolling_circumference_2290mm);
            map_CC_18.put(0x77, CC_18_TyreRollingCircumference.Rolling_circumference_2295mm);
            map_CC_18.put(0x78, CC_18_TyreRollingCircumference.Rolling_circumference_2300mm);
            map_CC_18.put(0x79, CC_18_TyreRollingCircumference.Rolling_circumference_2305mm);
            map_CC_18.put(0x7A, CC_18_TyreRollingCircumference.Rolling_circumference_2310mm);
            map_CC_18.put(0x7B, CC_18_TyreRollingCircumference.Rolling_circumference_2315mm);
            map_CC_18.put(0x7C, CC_18_TyreRollingCircumference.Rolling_circumference_2320mm);
            map_CC_18.put(0x7D, CC_18_TyreRollingCircumference.Rolling_circumference_2325mm);
            map_CC_18.put(0x7E, CC_18_TyreRollingCircumference.Rolling_circumference_2330mm);
            map_CC_18.put(0x7F, CC_18_TyreRollingCircumference.Rolling_circumference_2335mm);
            map_CC_18.put(0x80, CC_18_TyreRollingCircumference.Rolling_circumference_2340mm);
            map_CC_18.put(0x81, CC_18_TyreRollingCircumference.Rolling_circumference_2345mm);
            map_CC_18.put(0x82, CC_18_TyreRollingCircumference.Rolling_circumference_2350mm);
            map_CC_18.put(0x83, CC_18_TyreRollingCircumference.Rolling_circumference_2355mm);
            map_CC_18.put(0x84, CC_18_TyreRollingCircumference.Rolling_circumference_2360mm);
            map_CC_18.put(0x85, CC_18_TyreRollingCircumference.Rolling_circumference_2365mm);
            map_CC_18.put(0x86, CC_18_TyreRollingCircumference.Rolling_circumference_2370mm);
            map_CC_18.put(0x87, CC_18_TyreRollingCircumference.Rolling_circumference_2375mm);
            map_CC_18.put(0x88, CC_18_TyreRollingCircumference.Rolling_circumference_2380mm);
            map_CC_18.put(0x89, CC_18_TyreRollingCircumference.Rolling_circumference_2385mm);
            map_CC_18.put(0x8A, CC_18_TyreRollingCircumference.Rolling_circumference_2390mm);
            map_CC_18.put(0x8B, CC_18_TyreRollingCircumference.Rolling_circumference_2395mm);
            map_CC_18.put(0x8C, CC_18_TyreRollingCircumference.Rolling_circumference_2400mm);
            getValueData.put(CC_18_TyreRollingCircumference.class, map_CC_18);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_19 = new HashMap<>();
    public enum CC_19_TyrePressureMonitoringSystem implements CarConfigEnumBase
    {
        Without_Monitoring_system_TPMS(0x01,"Without_Monitoring_system_TPMS"),
        TPMS_WBA_US_algorithm(0x02,"TPMS_WBA_US_algorithm"),
        TPMS_WBL_US_algorithm(0x03,"TPMS_WBL_US_algorithm"),
        TPMS_WBA_EU_algorithm(0x04,"TPMS_WBA_EU_algorithm"),
        TPMS_WBL_EU_algorithm(0x05,"TPMS_WBL_EU_algorithm");

        public final int value;
        public final String name;
        public static final int ParamNumber = 19;

        CC_19_TyrePressureMonitoringSystem(int value, String name){
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
            map_CC_19.put(0x01, CC_19_TyrePressureMonitoringSystem.Without_Monitoring_system_TPMS);
            map_CC_19.put(0x02, CC_19_TyrePressureMonitoringSystem.TPMS_WBA_US_algorithm);
            map_CC_19.put(0x03, CC_19_TyrePressureMonitoringSystem.TPMS_WBL_US_algorithm);
            map_CC_19.put(0x04, CC_19_TyrePressureMonitoringSystem.TPMS_WBA_EU_algorithm);
            map_CC_19.put(0x05, CC_19_TyrePressureMonitoringSystem.TPMS_WBL_EU_algorithm);
            getValueData.put(CC_19_TyrePressureMonitoringSystem.class, map_CC_19);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_20 = new HashMap<>();
    public enum CC_20_TpmsMenuFunctionsInIhu implements CarConfigEnumBase
    {
        No_TPMS(0x01,"No_TPMS"),
        TPMS_Reset_menu(0x02,"TPMS_Reset_menu"),
        TPMS_ShutOff_menu(0x03,"TPMS_ShutOff_menu"),
        TPMS_Show_Pressures_menu(0x04,"TPMS_Show_Pressures_menu"),
        TPMS_Reset_and_Show_Pressures_menu(0x05,"TPMS_Reset_and_Show_Pressures_menu"),
        TPMS_Reset_and_ShutOff_menu(0x06,"TPMS_Reset_and_ShutOff_menu"),
        TPMS_ShutOff_and_Show_Pressures_menu(0x07,"TPMS_ShutOff_and_Show_Pressures_menu"),
        TPMS_ShutOff_Show_Pressures_and_Reset_Menu(0x08,"TPMS_ShutOff_Show_Pressures_and_Reset_Menu");

        public final int value;
        public final String name;
        public static final int ParamNumber = 20;

        CC_20_TpmsMenuFunctionsInIhu(int value, String name){
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
            map_CC_20.put(0x01, CC_20_TpmsMenuFunctionsInIhu.No_TPMS);
            map_CC_20.put(0x02, CC_20_TpmsMenuFunctionsInIhu.TPMS_Reset_menu);
            map_CC_20.put(0x03, CC_20_TpmsMenuFunctionsInIhu.TPMS_ShutOff_menu);
            map_CC_20.put(0x04, CC_20_TpmsMenuFunctionsInIhu.TPMS_Show_Pressures_menu);
            map_CC_20.put(0x05, CC_20_TpmsMenuFunctionsInIhu.TPMS_Reset_and_Show_Pressures_menu);
            map_CC_20.put(0x06, CC_20_TpmsMenuFunctionsInIhu.TPMS_Reset_and_ShutOff_menu);
            map_CC_20.put(0x07, CC_20_TpmsMenuFunctionsInIhu.TPMS_ShutOff_and_Show_Pressures_menu);
            map_CC_20.put(0x08, CC_20_TpmsMenuFunctionsInIhu.TPMS_ShutOff_Show_Pressures_and_Reset_Menu);
            getValueData.put(CC_20_TpmsMenuFunctionsInIhu.class, map_CC_20);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_23 = new HashMap<>();
    public enum CC_23_CruiseControl implements CarConfigEnumBase
    {
        No_Cruise_control(0x01,"No_Cruise_control"),
        Cruise_control(0x02,"Cruise_control"),
        Adaptive_Cruise_Control_Level1(0x03,"Adaptive_Cruise_Control_Level1"),
        Adaptive_Cruise_Control_Stop_Go_Level1(0x04,"Adaptive_Cruise_Control_Stop_Go_Level1"),
        Not_in_use(0x05,"Not_in_use"),
        Adaptive_Cruise_Control_Level2(0x06,"Adaptive_Cruise_Control_Level2"),
        Adaptive_Cruise_Control_Stop_Go_Level2(0x07,"Adaptive_Cruise_Control_Stop_Go_Level2"),
        Adaptive_Cruise_Control_Level3(0x08,"Adaptive_Cruise_Control_Level3"),
        Adaptive_Cruise_Control_Stop_Go_Level3(0x09,"Adaptive_Cruise_Control_Stop_Go_Level3");

        public final int value;
        public final String name;
        public static final int ParamNumber = 23;

        CC_23_CruiseControl(int value, String name){
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
            map_CC_23.put(0x01, CC_23_CruiseControl.No_Cruise_control);
            map_CC_23.put(0x02, CC_23_CruiseControl.Cruise_control);
            map_CC_23.put(0x03, CC_23_CruiseControl.Adaptive_Cruise_Control_Level1);
            map_CC_23.put(0x04, CC_23_CruiseControl.Adaptive_Cruise_Control_Stop_Go_Level1);
            map_CC_23.put(0x05, CC_23_CruiseControl.Not_in_use);
            map_CC_23.put(0x06, CC_23_CruiseControl.Adaptive_Cruise_Control_Level2);
            map_CC_23.put(0x07, CC_23_CruiseControl.Adaptive_Cruise_Control_Stop_Go_Level2);
            map_CC_23.put(0x08, CC_23_CruiseControl.Adaptive_Cruise_Control_Level3);
            map_CC_23.put(0x09, CC_23_CruiseControl.Adaptive_Cruise_Control_Stop_Go_Level3);
            getValueData.put(CC_23_CruiseControl.class, map_CC_23);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_24 = new HashMap<>();
    public enum CC_24_Seats implements CarConfigEnumBase
    {
        Not_applicable_variant_no_parts_allowed(0x01,"Not_applicable_variant_no_parts_allowed"),
        Fiveseat(0x02,"Fiveseat"),
        Sevenseat(0x03,"Sevenseat"),
        Fourseat(0x04,"Fourseat"),
        Threeseat(0x05,"Threeseat");

        public final int value;
        public final String name;
        public static final int ParamNumber = 24;

        CC_24_Seats(int value, String name){
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
            map_CC_24.put(0x01, CC_24_Seats.Not_applicable_variant_no_parts_allowed);
            map_CC_24.put(0x02, CC_24_Seats.Fiveseat);
            map_CC_24.put(0x03, CC_24_Seats.Sevenseat);
            map_CC_24.put(0x04, CC_24_Seats.Fourseat);
            map_CC_24.put(0x05, CC_24_Seats.Threeseat);
            getValueData.put(CC_24_Seats.class, map_CC_24);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_28 = new HashMap<>();
    public enum CC_28_RadioAmpSpeakerSystem implements CarConfigEnumBase
    {
        Performance_audio(0x01,"Performance_audio"),
        High_Performance_audio(0x02,"High_Performance_audio"),
        Premium_Sound_audio(0x03,"Premium_Sound_audio"),
        High_Performance_Plus_audio(0x04,"High_Performance_Plus_audio");

        public final int value;
        public final String name;
        public static final int ParamNumber = 28;

        CC_28_RadioAmpSpeakerSystem(int value, String name){
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
            map_CC_28.put(0x01, CC_28_RadioAmpSpeakerSystem.Performance_audio);
            map_CC_28.put(0x02, CC_28_RadioAmpSpeakerSystem.High_Performance_audio);
            map_CC_28.put(0x03, CC_28_RadioAmpSpeakerSystem.Premium_Sound_audio);
            map_CC_28.put(0x04, CC_28_RadioAmpSpeakerSystem.High_Performance_Plus_audio);
            getValueData.put(CC_28_RadioAmpSpeakerSystem.class, map_CC_28);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_36 = new HashMap<>();
    public enum CC_36_AdjustableSpeedLimiter implements CarConfigEnumBase
    {
        Without_Adjustable_speed_limiter(0x01,"Without_Adjustable_speed_limiter"),
        Adjustable_speed_limiter(0x02,"Adjustable_speed_limiter"),
        ASL_and_AVSL(0x03,"ASL_and_AVSL"),
        AVSL(0x04,"AVSL");

        public final int value;
        public final String name;
        public static final int ParamNumber = 36;

        CC_36_AdjustableSpeedLimiter(int value, String name){
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
            map_CC_36.put(0x01, CC_36_AdjustableSpeedLimiter.Without_Adjustable_speed_limiter);
            map_CC_36.put(0x02, CC_36_AdjustableSpeedLimiter.Adjustable_speed_limiter);
            map_CC_36.put(0x03, CC_36_AdjustableSpeedLimiter.ASL_and_AVSL);
            map_CC_36.put(0x04, CC_36_AdjustableSpeedLimiter.AVSL);
            getValueData.put(CC_36_AdjustableSpeedLimiter.class, map_CC_36);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_39 = new HashMap<>();
    public enum CC_39_ShiftByWire implements CarConfigEnumBase
    {
        Without_Shift_By_Wire(0x01,"Without_Shift_By_Wire"),
        With_Shift_By_Wire(0x02,"With_Shift_By_Wire"),
        SHIFT_BY_WIRE_pattern_type_2_(0x03,"SHIFT_BY_WIRE_pattern_type_2_");

        public final int value;
        public final String name;
        public static final int ParamNumber = 39;

        CC_39_ShiftByWire(int value, String name){
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
            map_CC_39.put(0x01, CC_39_ShiftByWire.Without_Shift_By_Wire);
            map_CC_39.put(0x02, CC_39_ShiftByWire.With_Shift_By_Wire);
            map_CC_39.put(0x03, CC_39_ShiftByWire.SHIFT_BY_WIRE_pattern_type_2_);
            getValueData.put(CC_39_ShiftByWire.class, map_CC_39);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_43 = new HashMap<>();
    public enum CC_43_MaxVehicleSpeedLimitation implements CarConfigEnumBase
    {
        Without_speed_limit(0x01,"Without_speed_limit"),
        Speed_limit_100_km_h(0x02,"Speed_limit_100_km_h"),
        Speed_limit_105_km_h(0x03,"Speed_limit_105_km_h"),
        Speed_limit_110_km_h(0x04,"Speed_limit_110_km_h"),
        Speed_limit_115_km_h(0x05,"Speed_limit_115_km_h"),
        Speed_limit_120_km_h(0x06,"Speed_limit_120_km_h"),
        Speed_limit_125_km_h(0x07,"Speed_limit_125_km_h"),
        Speed_limit_130_km_h(0x08,"Speed_limit_130_km_h"),
        Speed_limit_135_km_h(0x09,"Speed_limit_135_km_h"),
        Speed_limit_140_km_h(0x0A,"Speed_limit_140_km_h"),
        Speed_limit_145_km_h(0x0B,"Speed_limit_145_km_h"),
        Speed_limit_150_km_h(0x0C,"Speed_limit_150_km_h"),
        Speed_limit_155_km_h(0x0D,"Speed_limit_155_km_h"),
        Speed_limit_160_km_h(0x0E,"Speed_limit_160_km_h"),
        Speed_limit_165_km_h(0x0F,"Speed_limit_165_km_h"),
        Speed_limit_170_km_h(0x10,"Speed_limit_170_km_h"),
        Speed_limit_175_km_h(0x11,"Speed_limit_175_km_h"),
        Speed_limit_180_km_h(0x12,"Speed_limit_180_km_h"),
        Speed_limit_185_km_h(0x13,"Speed_limit_185_km_h"),
        Speed_limit_190_km_h(0x14,"Speed_limit_190_km_h"),
        Speed_limit_195_km_h(0x15,"Speed_limit_195_km_h"),
        Speed_limit_200_km_h(0x16,"Speed_limit_200_km_h"),
        Speed_limit_205_km_h(0x17,"Speed_limit_205_km_h"),
        Speed_limit_210_km_h(0x18,"Speed_limit_210_km_h"),
        Speed_limit_215_km_h(0x19,"Speed_limit_215_km_h"),
        Speed_limit_220_km_h(0x1A,"Speed_limit_220_km_h"),
        Speed_limit_225_km_h(0x1B,"Speed_limit_225_km_h"),
        Speed_limit_230_km_h(0x1C,"Speed_limit_230_km_h"),
        Speed_limit_235_km_h(0x1D,"Speed_limit_235_km_h"),
        Speed_limit_240_km_h(0x1E,"Speed_limit_240_km_h"),
        Speed_limit_245_km_h(0x1F,"Speed_limit_245_km_h"),
        Speed_limit_250_km_h(0x20,"Speed_limit_250_km_h"),
        Speed_limit_255_km_h(0x21,"Speed_limit_255_km_h"),
        Speed_limit_260_km_h(0x22,"Speed_limit_260_km_h"),
        Speed_limit_265_km_h(0x23,"Speed_limit_265_km_h"),
        Speed_limit_270_km_h(0x24,"Speed_limit_270_km_h"),
        Speed_limit_275_km_h(0x25,"Speed_limit_275_km_h"),
        Speed_limit_280_km_h(0x26,"Speed_limit_280_km_h"),
        Speed_limit_285_km_h(0x27,"Speed_limit_285_km_h"),
        Speed_limit_290_km_h(0x28,"Speed_limit_290_km_h"),
        Speed_limit_295_km_h(0x29,"Speed_limit_295_km_h"),
        Speed_limit_300_km_h(0x2A,"Speed_limit_300_km_h"),
        Speed_limit_60_mph(0x2B,"Speed_limit_60_mph"),
        Speed_limit_65_mph(0x2C,"Speed_limit_65_mph"),
        Speed_limit_70_mph(0x2D,"Speed_limit_70_mph"),
        Speed_limit_75_mph(0x2E,"Speed_limit_75_mph"),
        Speed_limit_80_mph(0x2F,"Speed_limit_80_mph"),
        Speed_limit_85_mph(0x30,"Speed_limit_85_mph"),
        Speed_limit_90_mph(0x31,"Speed_limit_90_mph"),
        Speed_limit_95_mph(0x32,"Speed_limit_95_mph"),
        Speed_limit_100_mph(0x33,"Speed_limit_100_mph"),
        Speed_limit_105_mph(0x34,"Speed_limit_105_mph"),
        Speed_limit_110_mph(0x35,"Speed_limit_110_mph"),
        Speed_limit_115_mph(0x36,"Speed_limit_115_mph"),
        Speed_limit_120_mph(0x37,"Speed_limit_120_mph"),
        Speed_limit_125_mph(0x38,"Speed_limit_125_mph"),
        Speed_limit_130_mph(0x39,"Speed_limit_130_mph"),
        Speed_limit_135_mph(0x3A,"Speed_limit_135_mph"),
        Speed_limit_140_mph(0x3B,"Speed_limit_140_mph"),
        Speed_limit_145_mph(0x3C,"Speed_limit_145_mph"),
        Speed_limit_150_mph(0x3D,"Speed_limit_150_mph"),
        Speed_limit_155_mph(0x3E,"Speed_limit_155_mph"),
        Speed_limit_160_mph(0x3F,"Speed_limit_160_mph"),
        Speed_limit_165_mph(0x40,"Speed_limit_165_mph"),
        Speed_limit_170_mph(0x41,"Speed_limit_170_mph"),
        Speed_limit_175_mph(0x42,"Speed_limit_175_mph"),
        Speed_limit_180_mph(0x43,"Speed_limit_180_mph"),
        Speed_limit_185_mph(0x44,"Speed_limit_185_mph"),
        Speed_limit_190_mph(0x45,"Speed_limit_190_mph");

        public final int value;
        public final String name;
        public static final int ParamNumber = 43;

        CC_43_MaxVehicleSpeedLimitation(int value, String name){
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
            map_CC_43.put(0x01, CC_43_MaxVehicleSpeedLimitation.Without_speed_limit);
            map_CC_43.put(0x02, CC_43_MaxVehicleSpeedLimitation.Speed_limit_100_km_h);
            map_CC_43.put(0x03, CC_43_MaxVehicleSpeedLimitation.Speed_limit_105_km_h);
            map_CC_43.put(0x04, CC_43_MaxVehicleSpeedLimitation.Speed_limit_110_km_h);
            map_CC_43.put(0x05, CC_43_MaxVehicleSpeedLimitation.Speed_limit_115_km_h);
            map_CC_43.put(0x06, CC_43_MaxVehicleSpeedLimitation.Speed_limit_120_km_h);
            map_CC_43.put(0x07, CC_43_MaxVehicleSpeedLimitation.Speed_limit_125_km_h);
            map_CC_43.put(0x08, CC_43_MaxVehicleSpeedLimitation.Speed_limit_130_km_h);
            map_CC_43.put(0x09, CC_43_MaxVehicleSpeedLimitation.Speed_limit_135_km_h);
            map_CC_43.put(0x0A, CC_43_MaxVehicleSpeedLimitation.Speed_limit_140_km_h);
            map_CC_43.put(0x0B, CC_43_MaxVehicleSpeedLimitation.Speed_limit_145_km_h);
            map_CC_43.put(0x0C, CC_43_MaxVehicleSpeedLimitation.Speed_limit_150_km_h);
            map_CC_43.put(0x0D, CC_43_MaxVehicleSpeedLimitation.Speed_limit_155_km_h);
            map_CC_43.put(0x0E, CC_43_MaxVehicleSpeedLimitation.Speed_limit_160_km_h);
            map_CC_43.put(0x0F, CC_43_MaxVehicleSpeedLimitation.Speed_limit_165_km_h);
            map_CC_43.put(0x10, CC_43_MaxVehicleSpeedLimitation.Speed_limit_170_km_h);
            map_CC_43.put(0x11, CC_43_MaxVehicleSpeedLimitation.Speed_limit_175_km_h);
            map_CC_43.put(0x12, CC_43_MaxVehicleSpeedLimitation.Speed_limit_180_km_h);
            map_CC_43.put(0x13, CC_43_MaxVehicleSpeedLimitation.Speed_limit_185_km_h);
            map_CC_43.put(0x14, CC_43_MaxVehicleSpeedLimitation.Speed_limit_190_km_h);
            map_CC_43.put(0x15, CC_43_MaxVehicleSpeedLimitation.Speed_limit_195_km_h);
            map_CC_43.put(0x16, CC_43_MaxVehicleSpeedLimitation.Speed_limit_200_km_h);
            map_CC_43.put(0x17, CC_43_MaxVehicleSpeedLimitation.Speed_limit_205_km_h);
            map_CC_43.put(0x18, CC_43_MaxVehicleSpeedLimitation.Speed_limit_210_km_h);
            map_CC_43.put(0x19, CC_43_MaxVehicleSpeedLimitation.Speed_limit_215_km_h);
            map_CC_43.put(0x1A, CC_43_MaxVehicleSpeedLimitation.Speed_limit_220_km_h);
            map_CC_43.put(0x1B, CC_43_MaxVehicleSpeedLimitation.Speed_limit_225_km_h);
            map_CC_43.put(0x1C, CC_43_MaxVehicleSpeedLimitation.Speed_limit_230_km_h);
            map_CC_43.put(0x1D, CC_43_MaxVehicleSpeedLimitation.Speed_limit_235_km_h);
            map_CC_43.put(0x1E, CC_43_MaxVehicleSpeedLimitation.Speed_limit_240_km_h);
            map_CC_43.put(0x1F, CC_43_MaxVehicleSpeedLimitation.Speed_limit_245_km_h);
            map_CC_43.put(0x20, CC_43_MaxVehicleSpeedLimitation.Speed_limit_250_km_h);
            map_CC_43.put(0x21, CC_43_MaxVehicleSpeedLimitation.Speed_limit_255_km_h);
            map_CC_43.put(0x22, CC_43_MaxVehicleSpeedLimitation.Speed_limit_260_km_h);
            map_CC_43.put(0x23, CC_43_MaxVehicleSpeedLimitation.Speed_limit_265_km_h);
            map_CC_43.put(0x24, CC_43_MaxVehicleSpeedLimitation.Speed_limit_270_km_h);
            map_CC_43.put(0x25, CC_43_MaxVehicleSpeedLimitation.Speed_limit_275_km_h);
            map_CC_43.put(0x26, CC_43_MaxVehicleSpeedLimitation.Speed_limit_280_km_h);
            map_CC_43.put(0x27, CC_43_MaxVehicleSpeedLimitation.Speed_limit_285_km_h);
            map_CC_43.put(0x28, CC_43_MaxVehicleSpeedLimitation.Speed_limit_290_km_h);
            map_CC_43.put(0x29, CC_43_MaxVehicleSpeedLimitation.Speed_limit_295_km_h);
            map_CC_43.put(0x2A, CC_43_MaxVehicleSpeedLimitation.Speed_limit_300_km_h);
            map_CC_43.put(0x2B, CC_43_MaxVehicleSpeedLimitation.Speed_limit_60_mph);
            map_CC_43.put(0x2C, CC_43_MaxVehicleSpeedLimitation.Speed_limit_65_mph);
            map_CC_43.put(0x2D, CC_43_MaxVehicleSpeedLimitation.Speed_limit_70_mph);
            map_CC_43.put(0x2E, CC_43_MaxVehicleSpeedLimitation.Speed_limit_75_mph);
            map_CC_43.put(0x2F, CC_43_MaxVehicleSpeedLimitation.Speed_limit_80_mph);
            map_CC_43.put(0x30, CC_43_MaxVehicleSpeedLimitation.Speed_limit_85_mph);
            map_CC_43.put(0x31, CC_43_MaxVehicleSpeedLimitation.Speed_limit_90_mph);
            map_CC_43.put(0x32, CC_43_MaxVehicleSpeedLimitation.Speed_limit_95_mph);
            map_CC_43.put(0x33, CC_43_MaxVehicleSpeedLimitation.Speed_limit_100_mph);
            map_CC_43.put(0x34, CC_43_MaxVehicleSpeedLimitation.Speed_limit_105_mph);
            map_CC_43.put(0x35, CC_43_MaxVehicleSpeedLimitation.Speed_limit_110_mph);
            map_CC_43.put(0x36, CC_43_MaxVehicleSpeedLimitation.Speed_limit_115_mph);
            map_CC_43.put(0x37, CC_43_MaxVehicleSpeedLimitation.Speed_limit_120_mph);
            map_CC_43.put(0x38, CC_43_MaxVehicleSpeedLimitation.Speed_limit_125_mph);
            map_CC_43.put(0x39, CC_43_MaxVehicleSpeedLimitation.Speed_limit_130_mph);
            map_CC_43.put(0x3A, CC_43_MaxVehicleSpeedLimitation.Speed_limit_135_mph);
            map_CC_43.put(0x3B, CC_43_MaxVehicleSpeedLimitation.Speed_limit_140_mph);
            map_CC_43.put(0x3C, CC_43_MaxVehicleSpeedLimitation.Speed_limit_145_mph);
            map_CC_43.put(0x3D, CC_43_MaxVehicleSpeedLimitation.Speed_limit_150_mph);
            map_CC_43.put(0x3E, CC_43_MaxVehicleSpeedLimitation.Speed_limit_155_mph);
            map_CC_43.put(0x3F, CC_43_MaxVehicleSpeedLimitation.Speed_limit_160_mph);
            map_CC_43.put(0x40, CC_43_MaxVehicleSpeedLimitation.Speed_limit_165_mph);
            map_CC_43.put(0x41, CC_43_MaxVehicleSpeedLimitation.Speed_limit_170_mph);
            map_CC_43.put(0x42, CC_43_MaxVehicleSpeedLimitation.Speed_limit_175_mph);
            map_CC_43.put(0x43, CC_43_MaxVehicleSpeedLimitation.Speed_limit_180_mph);
            map_CC_43.put(0x44, CC_43_MaxVehicleSpeedLimitation.Speed_limit_185_mph);
            map_CC_43.put(0x45, CC_43_MaxVehicleSpeedLimitation.Speed_limit_190_mph);
            getValueData.put(CC_43_MaxVehicleSpeedLimitation.class, map_CC_43);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_47 = new HashMap<>();
    public enum CC_47_SelectiveCatalyticReduction implements CarConfigEnumBase
    {
        Without_UREA(0x01,"Without_UREA"),
        With_UREA(0x02,"With_UREA");

        public final int value;
        public final String name;
        public static final int ParamNumber = 47;

        CC_47_SelectiveCatalyticReduction(int value, String name){
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
            map_CC_47.put(0x01, CC_47_SelectiveCatalyticReduction.Without_UREA);
            map_CC_47.put(0x02, CC_47_SelectiveCatalyticReduction.With_UREA);
            getValueData.put(CC_47_SelectiveCatalyticReduction.class, map_CC_47);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_48 = new HashMap<>();
    public enum CC_48_OilLevelSensor implements CarConfigEnumBase
    {
        Without_oil_level_sensor(0x01,"Without_oil_level_sensor"),
        Oil_level_sensor(0x02,"Oil_level_sensor");

        public final int value;
        public final String name;
        public static final int ParamNumber = 48;

        CC_48_OilLevelSensor(int value, String name){
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
            map_CC_48.put(0x01, CC_48_OilLevelSensor.Without_oil_level_sensor);
            map_CC_48.put(0x02, CC_48_OilLevelSensor.Oil_level_sensor);
            getValueData.put(CC_48_OilLevelSensor.class, map_CC_48);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_50 = new HashMap<>();
    public enum CC_50_ChassisType implements CarConfigEnumBase
    {
        Chassis_STD(0x01,"Chassis_STD"),
        Chassis_SPORT_std_height(0x02,"Chassis_SPORT_std_height"),
        Chassis_DYNAMIC(0x03,"Chassis_DYNAMIC"),
        Chassis_CCD(0x04,"Chassis_CCD"),
        Chassi_P_Police(0x05,"Chassi_P_Police"),
        Chassi_T_Taxi(0x06,"Chassi_T_Taxi"),
        Chassis_TOURING(0x07,"Chassis_TOURING"),
        Chassis_SPORT_low(0x08,"Chassis_SPORT_low");

        public final int value;
        public final String name;
        public static final int ParamNumber = 50;

        CC_50_ChassisType(int value, String name){
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
            map_CC_50.put(0x01, CC_50_ChassisType.Chassis_STD);
            map_CC_50.put(0x02, CC_50_ChassisType.Chassis_SPORT_std_height);
            map_CC_50.put(0x03, CC_50_ChassisType.Chassis_DYNAMIC);
            map_CC_50.put(0x04, CC_50_ChassisType.Chassis_CCD);
            map_CC_50.put(0x05, CC_50_ChassisType.Chassi_P_Police);
            map_CC_50.put(0x06, CC_50_ChassisType.Chassi_T_Taxi);
            map_CC_50.put(0x07, CC_50_ChassisType.Chassis_TOURING);
            map_CC_50.put(0x08, CC_50_ChassisType.Chassis_SPORT_low);
            getValueData.put(CC_50_ChassisType.class, map_CC_50);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_51 = new HashMap<>();
    public enum CC_51_TrailerStabilityAssist implements CarConfigEnumBase
    {
        Without_TRaiLer_STaBility_Assist(0x01,"Without_TRaiLer_STaBility_Assist"),
        TRaiLer_STaBility_Assist(0x02,"TRaiLer_STaBility_Assist");

        public final int value;
        public final String name;
        public static final int ParamNumber = 51;

        CC_51_TrailerStabilityAssist(int value, String name){
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
            map_CC_51.put(0x01, CC_51_TrailerStabilityAssist.Without_TRaiLer_STaBility_Assist);
            map_CC_51.put(0x02, CC_51_TrailerStabilityAssist.TRaiLer_STaBility_Assist);
            getValueData.put(CC_51_TrailerStabilityAssist.class, map_CC_51);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_53 = new HashMap<>();
    public enum CC_53_HillDescentControl implements CarConfigEnumBase
    {
        Without_Hill_Descent_Control(0x01,"Without_Hill_Descent_Control"),
        Hill_Descent_Control(0x02,"Hill_Descent_Control");

        public final int value;
        public final String name;
        public static final int ParamNumber = 53;

        CC_53_HillDescentControl(int value, String name){
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
            map_CC_53.put(0x01, CC_53_HillDescentControl.Without_Hill_Descent_Control);
            map_CC_53.put(0x02, CC_53_HillDescentControl.Hill_Descent_Control);
            getValueData.put(CC_53_HillDescentControl.class, map_CC_53);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_57 = new HashMap<>();
    public enum CC_57_DriveMode implements CarConfigEnumBase
    {
        Without_drive_mode_switch(0x01,"Without_drive_mode_switch"),
        With_drive_mode_switch(0x02,"With_drive_mode_switch");

        public final int value;
        public final String name;
        public static final int ParamNumber = 57;

        CC_57_DriveMode(int value, String name){
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
            map_CC_57.put(0x01, CC_57_DriveMode.Without_drive_mode_switch);
            map_CC_57.put(0x02, CC_57_DriveMode.With_drive_mode_switch);
            getValueData.put(CC_57_DriveMode.class, map_CC_57);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_58 = new HashMap<>();
    public enum CC_58_AirSuspension implements CarConfigEnumBase
    {
        Without_air_suspension(0x01,"Without_air_suspension"),
        Air_suspension_2_corner(0x02,"Air_suspension_2_corner"),
        Air_suspension_4_corner(0x03,"Air_suspension_4_corner");

        public final int value;
        public final String name;
        public static final int ParamNumber = 58;

        CC_58_AirSuspension(int value, String name){
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
            map_CC_58.put(0x01, CC_58_AirSuspension.Without_air_suspension);
            map_CC_58.put(0x02, CC_58_AirSuspension.Air_suspension_2_corner);
            map_CC_58.put(0x03, CC_58_AirSuspension.Air_suspension_4_corner);
            getValueData.put(CC_58_AirSuspension.class, map_CC_58);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_59 = new HashMap<>();
    public enum CC_59_ActiveSuspension implements CarConfigEnumBase
    {
        Without_Continious_Controlled_Chassi(0x01,"Without_Continious_Controlled_Chassi"),
        Continious_Controlled_Chassi(0x02,"Continious_Controlled_Chassi");

        public final int value;
        public final String name;
        public static final int ParamNumber = 59;

        CC_59_ActiveSuspension(int value, String name){
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
            map_CC_59.put(0x01, CC_59_ActiveSuspension.Without_Continious_Controlled_Chassi);
            map_CC_59.put(0x02, CC_59_ActiveSuspension.Continious_Controlled_Chassi);
            getValueData.put(CC_59_ActiveSuspension.class, map_CC_59);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_65 = new HashMap<>();
    public enum CC_65_InteriorMotionSensor implements CarConfigEnumBase
    {
        Without_Interior_Motion_Sensor(0x01,"Without_Interior_Motion_Sensor"),
        Interior_Motion_Sensor(0x02,"Interior_Motion_Sensor");

        public final int value;
        public final String name;
        public static final int ParamNumber = 65;

        CC_65_InteriorMotionSensor(int value, String name){
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
            map_CC_65.put(0x01, CC_65_InteriorMotionSensor.Without_Interior_Motion_Sensor);
            map_CC_65.put(0x02, CC_65_InteriorMotionSensor.Interior_Motion_Sensor);
            getValueData.put(CC_65_InteriorMotionSensor.class, map_CC_65);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_66 = new HashMap<>();
    public enum CC_66_InclinationSensor implements CarConfigEnumBase
    {
        Without_inclination_sensor(0x01,"Without_inclination_sensor"),
        With_inclination_sensor(0x02,"With_inclination_sensor");

        public final int value;
        public final String name;
        public static final int ParamNumber = 66;

        CC_66_InclinationSensor(int value, String name){
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
            map_CC_66.put(0x01, CC_66_InclinationSensor.Without_inclination_sensor);
            map_CC_66.put(0x02, CC_66_InclinationSensor.With_inclination_sensor);
            getValueData.put(CC_66_InclinationSensor.class, map_CC_66);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_70 = new HashMap<>();
    public enum CC_70_PassiveArming implements CarConfigEnumBase
    {
        Without_passive_arming(0x01,"Without_passive_arming"),
        With_passive_arming(0x02,"With_passive_arming");

        public final int value;
        public final String name;
        public static final int ParamNumber = 70;

        CC_70_PassiveArming(int value, String name){
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
            map_CC_70.put(0x01, CC_70_PassiveArming.Without_passive_arming);
            map_CC_70.put(0x02, CC_70_PassiveArming.With_passive_arming);
            getValueData.put(CC_70_PassiveArming.class, map_CC_70);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_71 = new HashMap<>();
    public enum CC_71_ForeignComponentDetection implements CarConfigEnumBase
    {
        Without_Foreign_Component_Detection(0x01,"Without_Foreign_Component_Detection"),
        Foreign_Component_Detection(0x02,"Foreign_Component_Detection");

        public final int value;
        public final String name;
        public static final int ParamNumber = 71;

        CC_71_ForeignComponentDetection(int value, String name){
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
            map_CC_71.put(0x01, CC_71_ForeignComponentDetection.Without_Foreign_Component_Detection);
            map_CC_71.put(0x02, CC_71_ForeignComponentDetection.Foreign_Component_Detection);
            getValueData.put(CC_71_ForeignComponentDetection.class, map_CC_71);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_85 = new HashMap<>();
    public enum CC_85_CentralLockType implements CarConfigEnumBase
    {
        Without_blocked_lock_mode(0x01,"Without_blocked_lock_mode"),
        Blocked_lock_mode(0x02,"Blocked_lock_mode");

        public final int value;
        public final String name;
        public static final int ParamNumber = 85;

        CC_85_CentralLockType(int value, String name){
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
            map_CC_85.put(0x01, CC_85_CentralLockType.Without_blocked_lock_mode);
            map_CC_85.put(0x02, CC_85_CentralLockType.Blocked_lock_mode);
            getValueData.put(CC_85_CentralLockType.class, map_CC_85);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_88 = new HashMap<>();
    public enum CC_88_PrivateLockingLuggageComp implements CarConfigEnumBase
    {
        Private_locking_disabled(0x01,"Private_locking_disabled"),
        Private_locking_enabled(0x02,"Private_locking_enabled");

        public final int value;
        public final String name;
        public static final int ParamNumber = 88;

        CC_88_PrivateLockingLuggageComp(int value, String name){
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
            map_CC_88.put(0x01, CC_88_PrivateLockingLuggageComp.Private_locking_disabled);
            map_CC_88.put(0x02, CC_88_PrivateLockingLuggageComp.Private_locking_enabled);
            getValueData.put(CC_88_PrivateLockingLuggageComp.class, map_CC_88);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_90 = new HashMap<>();
    public enum CC_90_AutomaticLockingBySpeed implements CarConfigEnumBase
    {
        Automatic_Locking_by_speed_always_ON_disabled_in_menu(0x01,"Automatic_Locking_by_speed_always_ON_disabled_in_menu"),
        Automatic_Locking_by_speed_enabled_default_OFF(0x02,"Automatic_Locking_by_speed_enabled_default_OFF"),
        Automatic_Locking_by_speed_enabled_default_ON(0x03,"Automatic_Locking_by_speed_enabled_default_ON");

        public final int value;
        public final String name;
        public static final int ParamNumber = 90;

        CC_90_AutomaticLockingBySpeed(int value, String name){
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
            map_CC_90.put(0x01, CC_90_AutomaticLockingBySpeed.Automatic_Locking_by_speed_always_ON_disabled_in_menu);
            map_CC_90.put(0x02, CC_90_AutomaticLockingBySpeed.Automatic_Locking_by_speed_enabled_default_OFF);
            map_CC_90.put(0x03, CC_90_AutomaticLockingBySpeed.Automatic_Locking_by_speed_enabled_default_ON);
            getValueData.put(CC_90_AutomaticLockingBySpeed.class, map_CC_90);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_91 = new HashMap<>();
    public enum CC_91_TwoStepUnlocking implements CarConfigEnumBase
    {
        Two_step_unlocking_default_OFF(0x01,"Two_step_unlocking_default_OFF"),
        Two_step_unlocking_default_ON(0x02,"Two_step_unlocking_default_ON");

        public final int value;
        public final String name;
        public static final int ParamNumber = 91;

        CC_91_TwoStepUnlocking(int value, String name){
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
            map_CC_91.put(0x01, CC_91_TwoStepUnlocking.Two_step_unlocking_default_OFF);
            map_CC_91.put(0x02, CC_91_TwoStepUnlocking.Two_step_unlocking_default_ON);
            getValueData.put(CC_91_TwoStepUnlocking.class, map_CC_91);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_94 = new HashMap<>();
    public enum CC_94_KeylessEntry implements CarConfigEnumBase
    {
        Without_Keyless_entry(0x01,"Without_Keyless_entry"),
        Passive_entry_Personal_Car_Communicator_incl_remote_tag(0x02,"Passive_entry_Personal_Car_Communicator_incl_remote_tag");

        public final int value;
        public final String name;
        public static final int ParamNumber = 94;

        CC_94_KeylessEntry(int value, String name){
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
            map_CC_94.put(0x01, CC_94_KeylessEntry.Without_Keyless_entry);
            map_CC_94.put(0x02, CC_94_KeylessEntry.Passive_entry_Personal_Car_Communicator_incl_remote_tag);
            getValueData.put(CC_94_KeylessEntry.class, map_CC_94);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_95 = new HashMap<>();
    public enum CC_95_VisibleLockingunlocking implements CarConfigEnumBase
    {
        Visible_locking_and_unlocking_feedback_default_OFF(0x01,"Visible_locking_and_unlocking_feedback_default_OFF"),
        Visible_locking_and_unlocking_feedback_default_ON(0x02,"Visible_locking_and_unlocking_feedback_default_ON");

        public final int value;
        public final String name;
        public static final int ParamNumber = 95;

        CC_95_VisibleLockingunlocking(int value, String name){
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
            map_CC_95.put(0x01, CC_95_VisibleLockingunlocking.Visible_locking_and_unlocking_feedback_default_OFF);
            map_CC_95.put(0x02, CC_95_VisibleLockingunlocking.Visible_locking_and_unlocking_feedback_default_ON);
            getValueData.put(CC_95_VisibleLockingunlocking.class, map_CC_95);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_96 = new HashMap<>();
    public enum CC_96_AudibleLockingFeedback implements CarConfigEnumBase
    {
        Audible_locking_feedback_disabled(0x01,"Audible_locking_feedback_disabled"),
        Audible_locking_feedback_enabled_default_OFF(0x02,"Audible_locking_feedback_enabled_default_OFF"),
        Audible_locking_feedback_enabled_default_ON(0x03,"Audible_locking_feedback_enabled_default_ON");

        public final int value;
        public final String name;
        public static final int ParamNumber = 96;

        CC_96_AudibleLockingFeedback(int value, String name){
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
            map_CC_96.put(0x01, CC_96_AudibleLockingFeedback.Audible_locking_feedback_disabled);
            map_CC_96.put(0x02, CC_96_AudibleLockingFeedback.Audible_locking_feedback_enabled_default_OFF);
            map_CC_96.put(0x03, CC_96_AudibleLockingFeedback.Audible_locking_feedback_enabled_default_ON);
            getValueData.put(CC_96_AudibleLockingFeedback.class, map_CC_96);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_106 = new HashMap<>();
    public enum CC_106_HeadlightsType implements CarConfigEnumBase
    {
        Halogen_lights_without_adjuster(0x01,"Halogen_lights_without_adjuster"),
        Halogen_lights_with_adjuster(0x02,"Halogen_lights_with_adjuster"),
        Xenon_Automatic_Bending_Light_type_A(0x03,"Xenon_Automatic_Bending_Light_type_A"),
        Xenon_Automatic_Bending_Light_type_B(0x04,"Xenon_Automatic_Bending_Light_type_B"),
        LED_With_Bending_Light_type_A_Top_Range(0x05,"LED_With_Bending_Light_type_A_Top_Range"),
        LED_With_Bending_Light_type_B_Top_Range(0x06,"LED_With_Bending_Light_type_B_Top_Range"),
        LED_Without_Bending_Light_type_A_Mid_Range(0x07,"LED_Without_Bending_Light_type_A_Mid_Range"),
        LED_Without_Bending_Light_type_B_Mid_Range(0x08,"LED_Without_Bending_Light_type_B_Mid_Range"),
        LED_type_A_with_HCM_US_CND_Mid_Range(0x09,"LED_type_A_with_HCM_US_CND_Mid_Range"),
        LED_type_B_with_HCM_ROW_Mid_Range(0x0A,"LED_type_B_with_HCM_ROW_Mid_Range"),
        LED_type_A_with_HCM_US_CND_High_Range(0x0B,"LED_type_A_with_HCM_US_CND_High_Range"),
        LED_type_B_with_HCM_ROW_High_Range(0x0C,"LED_type_B_with_HCM_ROW_High_Range"),
        HALO_B_Halogen_lights_with_adjuster_LED_Supplement_Lamp_for_DRL_Pos_TI(0x0D,"HALO_B_Halogen_lights_with_adjuster_LED_Supplement_Lamp_for_DRL_Pos_TI"),
        LED_Type_B_with_HCM_LED_Supplement_Lamp_for_DRL_Pos_TI(0x0E,"LED_Type_B_with_HCM_LED_Supplement_Lamp_for_DRL_Pos_TI");

        public final int value;
        public final String name;
        public static final int ParamNumber = 106;

        CC_106_HeadlightsType(int value, String name){
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
            map_CC_106.put(0x01, CC_106_HeadlightsType.Halogen_lights_without_adjuster);
            map_CC_106.put(0x02, CC_106_HeadlightsType.Halogen_lights_with_adjuster);
            map_CC_106.put(0x03, CC_106_HeadlightsType.Xenon_Automatic_Bending_Light_type_A);
            map_CC_106.put(0x04, CC_106_HeadlightsType.Xenon_Automatic_Bending_Light_type_B);
            map_CC_106.put(0x05, CC_106_HeadlightsType.LED_With_Bending_Light_type_A_Top_Range);
            map_CC_106.put(0x06, CC_106_HeadlightsType.LED_With_Bending_Light_type_B_Top_Range);
            map_CC_106.put(0x07, CC_106_HeadlightsType.LED_Without_Bending_Light_type_A_Mid_Range);
            map_CC_106.put(0x08, CC_106_HeadlightsType.LED_Without_Bending_Light_type_B_Mid_Range);
            map_CC_106.put(0x09, CC_106_HeadlightsType.LED_type_A_with_HCM_US_CND_Mid_Range);
            map_CC_106.put(0x0A, CC_106_HeadlightsType.LED_type_B_with_HCM_ROW_Mid_Range);
            map_CC_106.put(0x0B, CC_106_HeadlightsType.LED_type_A_with_HCM_US_CND_High_Range);
            map_CC_106.put(0x0C, CC_106_HeadlightsType.LED_type_B_with_HCM_ROW_High_Range);
            map_CC_106.put(0x0D, CC_106_HeadlightsType.HALO_B_Halogen_lights_with_adjuster_LED_Supplement_Lamp_for_DRL_Pos_TI);
            map_CC_106.put(0x0E, CC_106_HeadlightsType.LED_Type_B_with_HCM_LED_Supplement_Lamp_for_DRL_Pos_TI);
            getValueData.put(CC_106_HeadlightsType.class, map_CC_106);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_107 = new HashMap<>();
    public enum CC_107_ExteriorLightLogic implements CarConfigEnumBase
    {
        DDRL_not_together_with_position_light(0x01,"DDRL_not_together_with_position_light"),
        Without_DDRL(0x02,"Without_DDRL"),
        DDRL_in_pos_0_user_configurable(0x03,"DDRL_in_pos_0_user_configurable"),
        DDRL_in_pos_Auto_user_configurable(0x04,"DDRL_in_pos_Auto_user_configurable"),
        DDRL_in_all_positions(0x05,"DDRL_in_all_positions"),
        Without_DDRL_with_position_light_after_market(0x06,"Without_DDRL_with_position_light_after_market");

        public final int value;
        public final String name;
        public static final int ParamNumber = 107;

        CC_107_ExteriorLightLogic(int value, String name){
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
            map_CC_107.put(0x01, CC_107_ExteriorLightLogic.DDRL_not_together_with_position_light);
            map_CC_107.put(0x02, CC_107_ExteriorLightLogic.Without_DDRL);
            map_CC_107.put(0x03, CC_107_ExteriorLightLogic.DDRL_in_pos_0_user_configurable);
            map_CC_107.put(0x04, CC_107_ExteriorLightLogic.DDRL_in_pos_Auto_user_configurable);
            map_CC_107.put(0x05, CC_107_ExteriorLightLogic.DDRL_in_all_positions);
            map_CC_107.put(0x06, CC_107_ExteriorLightLogic.Without_DDRL_with_position_light_after_market);
            getValueData.put(CC_107_ExteriorLightLogic.class, map_CC_107);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_109 = new HashMap<>();
    public enum CC_109_ActiveHighBeam implements CarConfigEnumBase
    {
        Without_AHB(0x01,"Without_AHB"),
        With_AHB1_On_Off_Low_Beam_in_Village(0x02,"With_AHB1_On_Off_Low_Beam_in_Village"),
        With_AHB1_On_Off_High_Beam_in_Small_Village(0x03,"With_AHB1_On_Off_High_Beam_in_Small_Village"),
        With_AHB2_Adaptive_Low_Beam_in_Village(0x04,"With_AHB2_Adaptive_Low_Beam_in_Village"),
        With_AHB2_Adaptive_High_Beam_in_Small_Village(0x05,"With_AHB2_Adaptive_High_Beam_in_Small_Village");

        public final int value;
        public final String name;
        public static final int ParamNumber = 109;

        CC_109_ActiveHighBeam(int value, String name){
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
            map_CC_109.put(0x01, CC_109_ActiveHighBeam.Without_AHB);
            map_CC_109.put(0x02, CC_109_ActiveHighBeam.With_AHB1_On_Off_Low_Beam_in_Village);
            map_CC_109.put(0x03, CC_109_ActiveHighBeam.With_AHB1_On_Off_High_Beam_in_Small_Village);
            map_CC_109.put(0x04, CC_109_ActiveHighBeam.With_AHB2_Adaptive_Low_Beam_in_Village);
            map_CC_109.put(0x05, CC_109_ActiveHighBeam.With_AHB2_Adaptive_High_Beam_in_Small_Village);
            getValueData.put(CC_109_ActiveHighBeam.class, map_CC_109);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_111 = new HashMap<>();
    public enum CC_111_TouristLight implements CarConfigEnumBase
    {
        Without_configurable_Tourist_Light(0x01,"Without_configurable_Tourist_Light"),
        Left_asymmetry_with_configurable_Tourist_Light(0x02,"Left_asymmetry_with_configurable_Tourist_Light"),
        Right_asymmetry_with_configurable_Tourist_Light(0x03,"Right_asymmetry_with_configurable_Tourist_Light");

        public final int value;
        public final String name;
        public static final int ParamNumber = 111;

        CC_111_TouristLight(int value, String name){
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
            map_CC_111.put(0x01, CC_111_TouristLight.Without_configurable_Tourist_Light);
            map_CC_111.put(0x02, CC_111_TouristLight.Left_asymmetry_with_configurable_Tourist_Light);
            map_CC_111.put(0x03, CC_111_TouristLight.Right_asymmetry_with_configurable_Tourist_Light);
            getValueData.put(CC_111_TouristLight.class, map_CC_111);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_112 = new HashMap<>();
    public enum CC_112_CorneringLight implements CarConfigEnumBase
    {
        Without_cornering_light(0x01,"Without_cornering_light"),
        With_cornering_light(0x02,"With_cornering_light");

        public final int value;
        public final String name;
        public static final int ParamNumber = 112;

        CC_112_CorneringLight(int value, String name){
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
            map_CC_112.put(0x01, CC_112_CorneringLight.Without_cornering_light);
            map_CC_112.put(0x02, CC_112_CorneringLight.With_cornering_light);
            getValueData.put(CC_112_CorneringLight.class, map_CC_112);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_119 = new HashMap<>();
    public enum CC_119_AuxiliaryLights implements CarConfigEnumBase
    {
        Without_auxiliary_lights(0x01,"Without_auxiliary_lights"),
        With_Auxiliary_lights(0x02,"With_Auxiliary_lights");

        public final int value;
        public final String name;
        public static final int ParamNumber = 119;

        CC_119_AuxiliaryLights(int value, String name){
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
            map_CC_119.put(0x01, CC_119_AuxiliaryLights.Without_auxiliary_lights);
            map_CC_119.put(0x02, CC_119_AuxiliaryLights.With_Auxiliary_lights);
            getValueData.put(CC_119_AuxiliaryLights.class, map_CC_119);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_122 = new HashMap<>();
    public enum CC_122_HeatedWindscreen implements CarConfigEnumBase
    {
        No_Heated_Frontscreen(0x01,"No_Heated_Frontscreen"),
        Heated_Frontscreen(0x02,"Heated_Frontscreen");

        public final int value;
        public final String name;
        public static final int ParamNumber = 122;

        CC_122_HeatedWindscreen(int value, String name){
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
            map_CC_122.put(0x01, CC_122_HeatedWindscreen.No_Heated_Frontscreen);
            map_CC_122.put(0x02, CC_122_HeatedWindscreen.Heated_Frontscreen);
            getValueData.put(CC_122_HeatedWindscreen.class, map_CC_122);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_124 = new HashMap<>();
    public enum CC_124_RearWindowWiper implements CarConfigEnumBase
    {
        No_rear_window_wiper(0x01,"No_rear_window_wiper"),
        Rear_Window_wiper_with_washer(0x02,"Rear_Window_wiper_with_washer");

        public final int value;
        public final String name;
        public static final int ParamNumber = 124;

        CC_124_RearWindowWiper(int value, String name){
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
            map_CC_124.put(0x01, CC_124_RearWindowWiper.No_rear_window_wiper);
            map_CC_124.put(0x02, CC_124_RearWindowWiper.Rear_Window_wiper_with_washer);
            getValueData.put(CC_124_RearWindowWiper.class, map_CC_124);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_132 = new HashMap<>();
    public enum CC_132_CutoffSwitchPassengerAirbag implements CarConfigEnumBase
    {
        Without_Cut_Off_Switch_Passenger_Airbag(0x01,"Without_Cut_Off_Switch_Passenger_Airbag"),
        Cut_Off_Switch_Passenger_AirBag(0x02,"Cut_Off_Switch_Passenger_AirBag");

        public final int value;
        public final String name;
        public static final int ParamNumber = 132;

        CC_132_CutoffSwitchPassengerAirbag(int value, String name){
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
            map_CC_132.put(0x01, CC_132_CutoffSwitchPassengerAirbag.Without_Cut_Off_Switch_Passenger_Airbag);
            map_CC_132.put(0x02, CC_132_CutoffSwitchPassengerAirbag.Cut_Off_Switch_Passenger_AirBag);
            getValueData.put(CC_132_CutoffSwitchPassengerAirbag.class, map_CC_132);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_141 = new HashMap<>();
    public enum CC_141_NightVision implements CarConfigEnumBase
    {
        Without_Night_Vision(0x01,"Without_Night_Vision"),
        With_Night_Vision(0x02,"With_Night_Vision");

        public final int value;
        public final String name;
        public static final int ParamNumber = 141;

        CC_141_NightVision(int value, String name){
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
            map_CC_141.put(0x01, CC_141_NightVision.Without_Night_Vision);
            map_CC_141.put(0x02, CC_141_NightVision.With_Night_Vision);
            getValueData.put(CC_141_NightVision.class, map_CC_141);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_142 = new HashMap<>();
    public enum CC_142_ParkingAssistance implements CarConfigEnumBase
    {
        Without_parking_assistance(0x01,"Without_parking_assistance"),
        Parking_assistance_rear(0x02,"Parking_assistance_rear"),
        Parking_assistance_front_and_rear(0x03,"Parking_assistance_front_and_rear"),
        Parking_assistance_Semi_Automatic_Parking(0x04,"Parking_assistance_Semi_Automatic_Parking"),
        Parking_assistance_Full_Automatic_Parking(0x05,"Parking_assistance_Full_Automatic_Parking"),
        Parking_Assistance_SAP_(0x06,"Parking_Assistance_SAP_");

        public final int value;
        public final String name;
        public static final int ParamNumber = 142;

        CC_142_ParkingAssistance(int value, String name){
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
            map_CC_142.put(0x01, CC_142_ParkingAssistance.Without_parking_assistance);
            map_CC_142.put(0x02, CC_142_ParkingAssistance.Parking_assistance_rear);
            map_CC_142.put(0x03, CC_142_ParkingAssistance.Parking_assistance_front_and_rear);
            map_CC_142.put(0x04, CC_142_ParkingAssistance.Parking_assistance_Semi_Automatic_Parking);
            map_CC_142.put(0x05, CC_142_ParkingAssistance.Parking_assistance_Full_Automatic_Parking);
            map_CC_142.put(0x06, CC_142_ParkingAssistance.Parking_Assistance_SAP_);
            getValueData.put(CC_142_ParkingAssistance.class, map_CC_142);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_143 = new HashMap<>();
    public enum CC_143_DriverImpairmentMonitor implements CarConfigEnumBase
    {
        Without_Driver_Impairment_Monitor(0x01,"Without_Driver_Impairment_Monitor"),
        With_Driver_Impairment_Monitor(0x02,"With_Driver_Impairment_Monitor");

        public final int value;
        public final String name;
        public static final int ParamNumber = 143;

        CC_143_DriverImpairmentMonitor(int value, String name){
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
            map_CC_143.put(0x01, CC_143_DriverImpairmentMonitor.Without_Driver_Impairment_Monitor);
            map_CC_143.put(0x02, CC_143_DriverImpairmentMonitor.With_Driver_Impairment_Monitor);
            getValueData.put(CC_143_DriverImpairmentMonitor.class, map_CC_143);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_144 = new HashMap<>();
    public enum CC_144_DistanceAlert implements CarConfigEnumBase
    {
        Without_Distance_Alert(0x01,"Without_Distance_Alert"),
        With_Distance_Alert(0x02,"With_Distance_Alert");

        public final int value;
        public final String name;
        public static final int ParamNumber = 144;

        CC_144_DistanceAlert(int value, String name){
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
            map_CC_144.put(0x01, CC_144_DistanceAlert.Without_Distance_Alert);
            map_CC_144.put(0x02, CC_144_DistanceAlert.With_Distance_Alert);
            getValueData.put(CC_144_DistanceAlert.class, map_CC_144);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_145 = new HashMap<>();
    public enum CC_145_TrafficJamAssist implements CarConfigEnumBase
    {
        Without_Traffic_Jam_Assist(0x01,"Without_Traffic_Jam_Assist"),
        With_Traffic_Jam_Assist_Level_1(0x02,"With_Traffic_Jam_Assist_Level_1"),
        With_Traffic_Jam_Assist_Level_2(0x03,"With_Traffic_Jam_Assist_Level_2"),
        With_Traffic_Jam_Assist_Level_3(0x04,"With_Traffic_Jam_Assist_Level_3"),
        With_Traffic_Jam_Assist_Level_4(0x05,"With_Traffic_Jam_Assist_Level_4");

        public final int value;
        public final String name;
        public static final int ParamNumber = 145;

        CC_145_TrafficJamAssist(int value, String name){
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
            map_CC_145.put(0x01, CC_145_TrafficJamAssist.Without_Traffic_Jam_Assist);
            map_CC_145.put(0x02, CC_145_TrafficJamAssist.With_Traffic_Jam_Assist_Level_1);
            map_CC_145.put(0x03, CC_145_TrafficJamAssist.With_Traffic_Jam_Assist_Level_2);
            map_CC_145.put(0x04, CC_145_TrafficJamAssist.With_Traffic_Jam_Assist_Level_3);
            map_CC_145.put(0x05, CC_145_TrafficJamAssist.With_Traffic_Jam_Assist_Level_4);
            getValueData.put(CC_145_TrafficJamAssist.class, map_CC_145);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_146 = new HashMap<>();
    public enum CC_146_CollisionMitigationSupportFront implements CarConfigEnumBase
    {
        Without_Collision_Mitigation_Support_Front(0x01,"Without_Collision_Mitigation_Support_Front"),
        With_Collision_Mitigation_Support_Front_ver_1(0x02,"With_Collision_Mitigation_Support_Front_ver_1"),
        With_Collision_Mitigation_Support_Front_ver_2(0x03,"With_Collision_Mitigation_Support_Front_ver_2"),
        With_Collision_Mitigation_Support_Front_Argus(0x04,"With_Collision_Mitigation_Support_Front_Argus");

        public final int value;
        public final String name;
        public static final int ParamNumber = 146;

        CC_146_CollisionMitigationSupportFront(int value, String name){
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
            map_CC_146.put(0x01, CC_146_CollisionMitigationSupportFront.Without_Collision_Mitigation_Support_Front);
            map_CC_146.put(0x02, CC_146_CollisionMitigationSupportFront.With_Collision_Mitigation_Support_Front_ver_1);
            map_CC_146.put(0x03, CC_146_CollisionMitigationSupportFront.With_Collision_Mitigation_Support_Front_ver_2);
            map_CC_146.put(0x04, CC_146_CollisionMitigationSupportFront.With_Collision_Mitigation_Support_Front_Argus);
            getValueData.put(CC_146_CollisionMitigationSupportFront.class, map_CC_146);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_147 = new HashMap<>();
    public enum CC_147_RoadFrictionInformation implements CarConfigEnumBase
    {
        Without_Road_friction_indication(0x01,"Without_Road_friction_indication"),
        Road_friction_indication(0x02,"Road_friction_indication");

        public final int value;
        public final String name;
        public static final int ParamNumber = 147;

        CC_147_RoadFrictionInformation(int value, String name){
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
            map_CC_147.put(0x01, CC_147_RoadFrictionInformation.Without_Road_friction_indication);
            map_CC_147.put(0x02, CC_147_RoadFrictionInformation.Road_friction_indication);
            getValueData.put(CC_147_RoadFrictionInformation.class, map_CC_147);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_148 = new HashMap<>();
    public enum CC_148_CurveSpeedSupport implements CarConfigEnumBase
    {
        Without_Curve_Speed_Warning_without_Curve_Speed_Assist(0x01,"Without_Curve_Speed_Warning_without_Curve_Speed_Assist"),
        With_Curve_Speed_Warning_without_Curve_Speed_Assist(0x02,"With_Curve_Speed_Warning_without_Curve_Speed_Assist"),
        Without_Curve_Speed_Warning_with_Curve_Speed_Assist(0x03,"Without_Curve_Speed_Warning_with_Curve_Speed_Assist"),
        With_Curve_Speed_Warning_with_Curve_Speed_Assist(0x04,"With_Curve_Speed_Warning_with_Curve_Speed_Assist");

        public final int value;
        public final String name;
        public static final int ParamNumber = 148;

        CC_148_CurveSpeedSupport(int value, String name){
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
            map_CC_148.put(0x01, CC_148_CurveSpeedSupport.Without_Curve_Speed_Warning_without_Curve_Speed_Assist);
            map_CC_148.put(0x02, CC_148_CurveSpeedSupport.With_Curve_Speed_Warning_without_Curve_Speed_Assist);
            map_CC_148.put(0x03, CC_148_CurveSpeedSupport.Without_Curve_Speed_Warning_with_Curve_Speed_Assist);
            map_CC_148.put(0x04, CC_148_CurveSpeedSupport.With_Curve_Speed_Warning_with_Curve_Speed_Assist);
            getValueData.put(CC_148_CurveSpeedSupport.class, map_CC_148);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_149 = new HashMap<>();
    public enum CC_149_RoadSignInformation implements CarConfigEnumBase
    {
        Without_TSI_Traffic_Sign_Information_SLA_Speed_limit_Adaption_(0x01,"Without_TSI_Traffic_Sign_Information_SLA_Speed_limit_Adaption_"),
        SWEDEN_TSI_without_SLA(0x02,"SWEDEN_TSI_without_SLA"),
        SWEDEN_TSI_with_SLA(0x03,"SWEDEN_TSI_with_SLA"),
        IRELAND_TSI_without_SLA(0x04,"IRELAND_TSI_without_SLA"),
        IRELAND_TSI_with_SLA(0x05,"IRELAND_TSI_with_SLA"),
        ENGLAND_TSI_without_SLA(0x06,"ENGLAND_TSI_without_SLA"),
        ENGLAND_TSI_with_SLA(0x07,"ENGLAND_TSI_with_SLA"),
        MID_EU_TSI_without_SLA(0x08,"MID_EU_TSI_without_SLA"),
        MID_EU_TSI_with_SLA(0x09,"MID_EU_TSI_with_SLA"),
        EAST_EUROPE_TSI_without_SLA(0x0A,"EAST_EUROPE_TSI_without_SLA"),
        EAST_EUROPE_TSI_with_SLA(0x0B,"EAST_EUROPE_TSI_with_SLA"),
        SOUTH_EU_TSI_without_SLA(0x0C,"SOUTH_EU_TSI_without_SLA"),
        SOUTH_EU_TSI_with_SLA(0x0D,"SOUTH_EU_TSI_with_SLA"),
        USA_TSI_without_SLA(0x0E,"USA_TSI_without_SLA"),
        USA_TSI_with_SLA(0x0F,"USA_TSI_with_SLA"),
        MEXICO_TSI_without_SLA(0x10,"MEXICO_TSI_without_SLA"),
        MEXICO_TSI_with_SLA(0x11,"MEXICO_TSI_with_SLA"),
        CANADA_TSI_without_SLA(0x12,"CANADA_TSI_without_SLA"),
        CANADA_TSI_with_SLA(0x13,"CANADA_TSI_with_SLA"),
        SINGAPORE_TSI_without_SLA(0x14,"SINGAPORE_TSI_without_SLA"),
        SINGAPORE_TSI_with_SLA(0x15,"SINGAPORE_TSI_with_SLA"),
        KOREA_TSI_without_SLA(0x16,"KOREA_TSI_without_SLA"),
        KOREA_TSI_with_SLA(0x17,"KOREA_TSI_with_SLA"),
        LATIN_AMERICA_TSI_without_SLA(0x18,"LATIN_AMERICA_TSI_without_SLA"),
        LATIN_AMERICA_TSI_with_SLA(0x19,"LATIN_AMERICA_TSI_with_SLA"),
        ARABIA_TSI_without_SLA(0x1A,"ARABIA_TSI_without_SLA"),
        ARABIA_TSI_with_SLA(0x1B,"ARABIA_TSI_with_SLA"),
        HONGKONG_TSI_without_SLA(0x1C,"HONGKONG_TSI_without_SLA"),
        HONGKONG_TSI_with_SLA(0x1D,"HONGKONG_TSI_with_SLA"),
        TAIWAN_TSI_without_SLA(0x1E,"TAIWAN_TSI_without_SLA"),
        TAIWAN_TSI_with_SLA(0x1F,"TAIWAN_TSI_with_SLA"),
        AUSTRALIA_TSI_without_SLA(0x20,"AUSTRALIA_TSI_without_SLA"),
        AUSTRALIA_TSI_with_SLA(0x21,"AUSTRALIA_TSI_with_SLA"),
        SOUTH_AFRICA_TSI_without_SLA(0x22,"SOUTH_AFRICA_TSI_without_SLA"),
        SOUTH_AFRICA_TSI_with_SLA(0x23,"SOUTH_AFRICA_TSI_with_SLA"),
        TURKEY_TSI_without_SLA(0x24,"TURKEY_TSI_without_SLA"),
        TURKEY_TSI_with_SLA(0x25,"TURKEY_TSI_with_SLA"),
        JAPAN_TSI_without_SLA(0x26,"JAPAN_TSI_without_SLA"),
        JAPAN_TSI_with_SLA(0x27,"JAPAN_TSI_with_SLA"),
        CHINA_TSI_without_SLA(0x28,"CHINA_TSI_without_SLA"),
        CHINA_TSI_with_SLA(0x29,"CHINA_TSI_with_SLA"),
        INDIA_TSI_without_SLA(0x2A,"INDIA_TSI_without_SLA"),
        INDIA_TSI_with_SLA(0x2B,"INDIA_TSI_with_SLA"),
        BRAZIL_TSI_without_SLA(0x2C,"BRAZIL_TSI_without_SLA"),
        BRAZIL_TSI_with_SLA(0x2D,"BRAZIL_TSI_with_SLA"),
        THAILAND_TSI_without_SLA(0x2E,"THAILAND_TSI_without_SLA"),
        THAILAND_TSI_with_SLA(0x2F,"THAILAND_TSI_with_SLA"),
        MALAYSIA_TSI_without_SLA(0x30,"MALAYSIA_TSI_without_SLA"),
        MALAYSIA_TSI_with_SLA(0x31,"MALAYSIA_TSI_with_SLA"),
        OTHERS_TSI_without_SLA(0x32,"OTHERS_TSI_without_SLA"),
        OTHERS_TSI_with_SLA(0x33,"OTHERS_TSI_with_SLA"),
        Not_allowed_varaint(0xF0,"Not_allowed_varaint");

        public final int value;
        public final String name;
        public static final int ParamNumber = 149;

        CC_149_RoadSignInformation(int value, String name){
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
            map_CC_149.put(0x01, CC_149_RoadSignInformation.Without_TSI_Traffic_Sign_Information_SLA_Speed_limit_Adaption_);
            map_CC_149.put(0x02, CC_149_RoadSignInformation.SWEDEN_TSI_without_SLA);
            map_CC_149.put(0x03, CC_149_RoadSignInformation.SWEDEN_TSI_with_SLA);
            map_CC_149.put(0x04, CC_149_RoadSignInformation.IRELAND_TSI_without_SLA);
            map_CC_149.put(0x05, CC_149_RoadSignInformation.IRELAND_TSI_with_SLA);
            map_CC_149.put(0x06, CC_149_RoadSignInformation.ENGLAND_TSI_without_SLA);
            map_CC_149.put(0x07, CC_149_RoadSignInformation.ENGLAND_TSI_with_SLA);
            map_CC_149.put(0x08, CC_149_RoadSignInformation.MID_EU_TSI_without_SLA);
            map_CC_149.put(0x09, CC_149_RoadSignInformation.MID_EU_TSI_with_SLA);
            map_CC_149.put(0x0A, CC_149_RoadSignInformation.EAST_EUROPE_TSI_without_SLA);
            map_CC_149.put(0x0B, CC_149_RoadSignInformation.EAST_EUROPE_TSI_with_SLA);
            map_CC_149.put(0x0C, CC_149_RoadSignInformation.SOUTH_EU_TSI_without_SLA);
            map_CC_149.put(0x0D, CC_149_RoadSignInformation.SOUTH_EU_TSI_with_SLA);
            map_CC_149.put(0x0E, CC_149_RoadSignInformation.USA_TSI_without_SLA);
            map_CC_149.put(0x0F, CC_149_RoadSignInformation.USA_TSI_with_SLA);
            map_CC_149.put(0x10, CC_149_RoadSignInformation.MEXICO_TSI_without_SLA);
            map_CC_149.put(0x11, CC_149_RoadSignInformation.MEXICO_TSI_with_SLA);
            map_CC_149.put(0x12, CC_149_RoadSignInformation.CANADA_TSI_without_SLA);
            map_CC_149.put(0x13, CC_149_RoadSignInformation.CANADA_TSI_with_SLA);
            map_CC_149.put(0x14, CC_149_RoadSignInformation.SINGAPORE_TSI_without_SLA);
            map_CC_149.put(0x15, CC_149_RoadSignInformation.SINGAPORE_TSI_with_SLA);
            map_CC_149.put(0x16, CC_149_RoadSignInformation.KOREA_TSI_without_SLA);
            map_CC_149.put(0x17, CC_149_RoadSignInformation.KOREA_TSI_with_SLA);
            map_CC_149.put(0x18, CC_149_RoadSignInformation.LATIN_AMERICA_TSI_without_SLA);
            map_CC_149.put(0x19, CC_149_RoadSignInformation.LATIN_AMERICA_TSI_with_SLA);
            map_CC_149.put(0x1A, CC_149_RoadSignInformation.ARABIA_TSI_without_SLA);
            map_CC_149.put(0x1B, CC_149_RoadSignInformation.ARABIA_TSI_with_SLA);
            map_CC_149.put(0x1C, CC_149_RoadSignInformation.HONGKONG_TSI_without_SLA);
            map_CC_149.put(0x1D, CC_149_RoadSignInformation.HONGKONG_TSI_with_SLA);
            map_CC_149.put(0x1E, CC_149_RoadSignInformation.TAIWAN_TSI_without_SLA);
            map_CC_149.put(0x1F, CC_149_RoadSignInformation.TAIWAN_TSI_with_SLA);
            map_CC_149.put(0x20, CC_149_RoadSignInformation.AUSTRALIA_TSI_without_SLA);
            map_CC_149.put(0x21, CC_149_RoadSignInformation.AUSTRALIA_TSI_with_SLA);
            map_CC_149.put(0x22, CC_149_RoadSignInformation.SOUTH_AFRICA_TSI_without_SLA);
            map_CC_149.put(0x23, CC_149_RoadSignInformation.SOUTH_AFRICA_TSI_with_SLA);
            map_CC_149.put(0x24, CC_149_RoadSignInformation.TURKEY_TSI_without_SLA);
            map_CC_149.put(0x25, CC_149_RoadSignInformation.TURKEY_TSI_with_SLA);
            map_CC_149.put(0x26, CC_149_RoadSignInformation.JAPAN_TSI_without_SLA);
            map_CC_149.put(0x27, CC_149_RoadSignInformation.JAPAN_TSI_with_SLA);
            map_CC_149.put(0x28, CC_149_RoadSignInformation.CHINA_TSI_without_SLA);
            map_CC_149.put(0x29, CC_149_RoadSignInformation.CHINA_TSI_with_SLA);
            map_CC_149.put(0x2A, CC_149_RoadSignInformation.INDIA_TSI_without_SLA);
            map_CC_149.put(0x2B, CC_149_RoadSignInformation.INDIA_TSI_with_SLA);
            map_CC_149.put(0x2C, CC_149_RoadSignInformation.BRAZIL_TSI_without_SLA);
            map_CC_149.put(0x2D, CC_149_RoadSignInformation.BRAZIL_TSI_with_SLA);
            map_CC_149.put(0x2E, CC_149_RoadSignInformation.THAILAND_TSI_without_SLA);
            map_CC_149.put(0x2F, CC_149_RoadSignInformation.THAILAND_TSI_with_SLA);
            map_CC_149.put(0x30, CC_149_RoadSignInformation.MALAYSIA_TSI_without_SLA);
            map_CC_149.put(0x31, CC_149_RoadSignInformation.MALAYSIA_TSI_with_SLA);
            map_CC_149.put(0x32, CC_149_RoadSignInformation.OTHERS_TSI_without_SLA);
            map_CC_149.put(0x33, CC_149_RoadSignInformation.OTHERS_TSI_with_SLA);
            map_CC_149.put(0xF0, CC_149_RoadSignInformation.Not_allowed_varaint);
            getValueData.put(CC_149_RoadSignInformation.class, map_CC_149);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_150 = new HashMap<>();
    public enum CC_150_LaneKeepingAid implements CarConfigEnumBase
    {
        Without_Lane_Keeping_Aid_Lane_Departure_Warning(0x01,"Without_Lane_Keeping_Aid_Lane_Departure_Warning"),
        Lane_Keeping_Aid(0x02,"Lane_Keeping_Aid"),
        Lane_Departure_Warning(0x05,"Lane_Departure_Warning");

        public final int value;
        public final String name;
        public static final int ParamNumber = 150;

        CC_150_LaneKeepingAid(int value, String name){
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
            map_CC_150.put(0x01, CC_150_LaneKeepingAid.Without_Lane_Keeping_Aid_Lane_Departure_Warning);
            map_CC_150.put(0x02, CC_150_LaneKeepingAid.Lane_Keeping_Aid);
            map_CC_150.put(0x05, CC_150_LaneKeepingAid.Lane_Departure_Warning);
            getValueData.put(CC_150_LaneKeepingAid.class, map_CC_150);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_151 = new HashMap<>();
    public enum CC_151_ElectronicHorizon implements CarConfigEnumBase
    {
        Without_Electronic_Horizon(0x01,"Without_Electronic_Horizon"),
        With_Electronic_Horizon(0x02,"With_Electronic_Horizon");

        public final int value;
        public final String name;
        public static final int ParamNumber = 151;

        CC_151_ElectronicHorizon(int value, String name){
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
            map_CC_151.put(0x01, CC_151_ElectronicHorizon.Without_Electronic_Horizon);
            map_CC_151.put(0x02, CC_151_ElectronicHorizon.With_Electronic_Horizon);
            getValueData.put(CC_151_ElectronicHorizon.class, map_CC_151);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_152 = new HashMap<>();
    public enum CC_152_BlindSpotInformationSystem implements CarConfigEnumBase
    {
        Without_Blind_spot_Information_System(0x01,"Without_Blind_spot_Information_System"),
        Blind_spot_Information_System_with_CTA(0x02,"Blind_spot_Information_System_with_CTA"),
        Without_Blind_spot_Information_System_sensors_fitted(0x03,"Without_Blind_spot_Information_System_sensors_fitted"),
        BLIS_System_with_audio_and_CTA_for_Geely(0x04,"BLIS_System_with_audio_and_CTA_for_Geely");

        public final int value;
        public final String name;
        public static final int ParamNumber = 152;

        CC_152_BlindSpotInformationSystem(int value, String name){
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
            map_CC_152.put(0x01, CC_152_BlindSpotInformationSystem.Without_Blind_spot_Information_System);
            map_CC_152.put(0x02, CC_152_BlindSpotInformationSystem.Blind_spot_Information_System_with_CTA);
            map_CC_152.put(0x03, CC_152_BlindSpotInformationSystem.Without_Blind_spot_Information_System_sensors_fitted);
            map_CC_152.put(0x04, CC_152_BlindSpotInformationSystem.BLIS_System_with_audio_and_CTA_for_Geely);
            getValueData.put(CC_152_BlindSpotInformationSystem.class, map_CC_152);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_153 = new HashMap<>();
    public enum CC_153_CollisionMitigWarnRear implements CarConfigEnumBase
    {
        Without_Rear_Collision_Warning_and_Mitigation(0x01,"Without_Rear_Collision_Warning_and_Mitigation"),
        With_Rear_Collision_Warning_and_Mitigation(0x02,"With_Rear_Collision_Warning_and_Mitigation"),
        With_Rear_Collision_Mitigation(0x03,"With_Rear_Collision_Mitigation"),
        With_Rear_Collision_Warning(0x04,"With_Rear_Collision_Warning");

        public final int value;
        public final String name;
        public static final int ParamNumber = 153;

        CC_153_CollisionMitigWarnRear(int value, String name){
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
            map_CC_153.put(0x01, CC_153_CollisionMitigWarnRear.Without_Rear_Collision_Warning_and_Mitigation);
            map_CC_153.put(0x02, CC_153_CollisionMitigWarnRear.With_Rear_Collision_Warning_and_Mitigation);
            map_CC_153.put(0x03, CC_153_CollisionMitigWarnRear.With_Rear_Collision_Mitigation);
            map_CC_153.put(0x04, CC_153_CollisionMitigWarnRear.With_Rear_Collision_Warning);
            getValueData.put(CC_153_CollisionMitigWarnRear.class, map_CC_153);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_154 = new HashMap<>();
    public enum CC_154_ParkingCamera implements CarConfigEnumBase
    {
        Without_Parking_Camera(0x01,"Without_Parking_Camera"),
        Rear_Parking_Camera(0x02,"Rear_Parking_Camera"),
        Parking_camera_360(0x03,"Parking_camera_360"),
        Front_Parking_Camera_accessory_(0x04,"Front_Parking_Camera_accessory_"),
        Rear_Parking_Camera_always_activated_during_reverse(0x05,"Rear_Parking_Camera_always_activated_during_reverse"),
        Parking_Camera_360_always_activated_during_reverse(0x06,"Parking_Camera_360_always_activated_during_reverse"),
        Parking_Camera_preparation_front_rear(0x07,"Parking_Camera_preparation_front_rear"),
        Rear_Parking_Camera_GEN_2(0x08,"Rear_Parking_Camera_GEN_2"),
        Rear_Parking_Camera_GEN_2_always_activated_during_reverse(0x09,"Rear_Parking_Camera_GEN_2_always_activated_during_reverse"),
        Parking_Camera_360_Japan(0x0A,"Parking_Camera_360_Japan"),
        Parking_camera_360_GEN_2(0x0B,"Parking_camera_360_GEN_2"),
        Parking_Camera_360_GEN_2_always_activated_during(0x0C,"Parking_Camera_360_GEN_2_always_activated_during");

        public final int value;
        public final String name;
        public static final int ParamNumber = 154;

        CC_154_ParkingCamera(int value, String name){
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
            map_CC_154.put(0x01, CC_154_ParkingCamera.Without_Parking_Camera);
            map_CC_154.put(0x02, CC_154_ParkingCamera.Rear_Parking_Camera);
            map_CC_154.put(0x03, CC_154_ParkingCamera.Parking_camera_360);
            map_CC_154.put(0x04, CC_154_ParkingCamera.Front_Parking_Camera_accessory_);
            map_CC_154.put(0x05, CC_154_ParkingCamera.Rear_Parking_Camera_always_activated_during_reverse);
            map_CC_154.put(0x06, CC_154_ParkingCamera.Parking_Camera_360_always_activated_during_reverse);
            map_CC_154.put(0x07, CC_154_ParkingCamera.Parking_Camera_preparation_front_rear);
            map_CC_154.put(0x08, CC_154_ParkingCamera.Rear_Parking_Camera_GEN_2);
            map_CC_154.put(0x09, CC_154_ParkingCamera.Rear_Parking_Camera_GEN_2_always_activated_during_reverse);
            map_CC_154.put(0x0A, CC_154_ParkingCamera.Parking_Camera_360_Japan);
            map_CC_154.put(0x0B, CC_154_ParkingCamera.Parking_camera_360_GEN_2);
            map_CC_154.put(0x0C, CC_154_ParkingCamera.Parking_Camera_360_GEN_2_always_activated_during);
            getValueData.put(CC_154_ParkingCamera.class, map_CC_154);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_156 = new HashMap<>();
    public enum CC_156_RearWiewMirrorsouter implements CarConfigEnumBase
    {
        Without_retractable_mirrors(0x01,"Without_retractable_mirrors"),
        Retractable_mirrors(0x02,"Retractable_mirrors"),
        Without_retractable_mirrors_and_without_electric_defroster(0x80,"Without_retractable_mirrors_and_without_electric_defroster");

        public final int value;
        public final String name;
        public static final int ParamNumber = 156;

        CC_156_RearWiewMirrorsouter(int value, String name){
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
            map_CC_156.put(0x01, CC_156_RearWiewMirrorsouter.Without_retractable_mirrors);
            map_CC_156.put(0x02, CC_156_RearWiewMirrorsouter.Retractable_mirrors);
            map_CC_156.put(0x80, CC_156_RearWiewMirrorsouter.Without_retractable_mirrors_and_without_electric_defroster);
            getValueData.put(CC_156_RearWiewMirrorsouter.class, map_CC_156);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_157 = new HashMap<>();
    public enum CC_157_CombinedInstrument implements CarConfigEnumBase
    {
        Drivers_Information_Module_with_8_inch_screen(0x01,"Drivers_Information_Module_with_8_inch_screen"),
        Drivers_Information_Module_with_12_3_inch_screen(0x02,"Drivers_Information_Module_with_12_3_inch_screen"),
        Drivers_Information_Module_with_4_2_inch_screen_KMH(0x03,"Drivers_Information_Module_with_4_2_inch_screen_KMH"),
        Drivers_Information_Module_with_7_inch_screen_Non_VCC(0x80,"Drivers_Information_Module_with_7_inch_screen_Non_VCC"),
        Drivers_Information_Module_with_10_inch_screen_Non_VCC(0x81,"Drivers_Information_Module_with_10_inch_screen_Non_VCC");

        public final int value;
        public final String name;
        public static final int ParamNumber = 157;

        CC_157_CombinedInstrument(int value, String name){
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
            map_CC_157.put(0x01, CC_157_CombinedInstrument.Drivers_Information_Module_with_8_inch_screen);
            map_CC_157.put(0x02, CC_157_CombinedInstrument.Drivers_Information_Module_with_12_3_inch_screen);
            map_CC_157.put(0x03, CC_157_CombinedInstrument.Drivers_Information_Module_with_4_2_inch_screen_KMH);
            map_CC_157.put(0x80, CC_157_CombinedInstrument.Drivers_Information_Module_with_7_inch_screen_Non_VCC);
            map_CC_157.put(0x81, CC_157_CombinedInstrument.Drivers_Information_Module_with_10_inch_screen_Non_VCC);
            getValueData.put(CC_157_CombinedInstrument.class, map_CC_157);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_158 = new HashMap<>();
    public enum CC_158_HeadUpDisplay implements CarConfigEnumBase
    {
        With_Head_Up_Display_FCW(0x01,"With_Head_Up_Display_FCW"),
        With_Head_Up_Display_graphic(0x02,"With_Head_Up_Display_graphic"),
        Without_Head_Up_Display(0x03,"Without_Head_Up_Display"),
        With_Head_Up_Display_graphic_2(0x04,"With_Head_Up_Display_graphic_2"),
        With_Head_Up_Display_graphic_3(0x05,"With_Head_Up_Display_graphic_3"),
        Combiner_Head_Up_Display_graphic(0x06,"Combiner_Head_Up_Display_graphic");

        public final int value;
        public final String name;
        public static final int ParamNumber = 158;

        CC_158_HeadUpDisplay(int value, String name){
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
            map_CC_158.put(0x01, CC_158_HeadUpDisplay.With_Head_Up_Display_FCW);
            map_CC_158.put(0x02, CC_158_HeadUpDisplay.With_Head_Up_Display_graphic);
            map_CC_158.put(0x03, CC_158_HeadUpDisplay.Without_Head_Up_Display);
            map_CC_158.put(0x04, CC_158_HeadUpDisplay.With_Head_Up_Display_graphic_2);
            map_CC_158.put(0x05, CC_158_HeadUpDisplay.With_Head_Up_Display_graphic_3);
            map_CC_158.put(0x06, CC_158_HeadUpDisplay.Combiner_Head_Up_Display_graphic);
            getValueData.put(CC_158_HeadUpDisplay.class, map_CC_158);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_161 = new HashMap<>();
    public enum CC_161_UnitSettings implements CarConfigEnumBase
    {
        Temp_C_Clock_24_Distance_KM_Fuel_Liter_KM(0x01,"Temp_C_Clock_24_Distance_KM_Fuel_Liter_KM"),
        Temp_C_Clock_24_Distance_KM_Fuel_KM_Liter(0x02,"Temp_C_Clock_24_Distance_KM_Fuel_KM_Liter"),
        Temp_C_Clock_24_Distance_Km_Fuel_MPG_US_(0x03,"Temp_C_Clock_24_Distance_Km_Fuel_MPG_US_"),
        Temp_C_Clock_24_Distance_Km_Fuel_MPG_UK_(0x04,"Temp_C_Clock_24_Distance_Km_Fuel_MPG_UK_"),
        Temp_C_Clock_24_Distance_Miles_Fuel_L_Km(0x05,"Temp_C_Clock_24_Distance_Miles_Fuel_L_Km"),
        Temp_C_Clock_24_Distance_Miles_Fuel_Km_L(0x06,"Temp_C_Clock_24_Distance_Miles_Fuel_Km_L"),
        Temp_C_Clock_24_Distance_Miles_Fuel_MPG_US_(0x07,"Temp_C_Clock_24_Distance_Miles_Fuel_MPG_US_"),
        Temp_C_Clock_24_Distance_Miles_Fuel_UK_Gallon(0x08,"Temp_C_Clock_24_Distance_Miles_Fuel_UK_Gallon"),
        Temp_C_Clock_12_Distance_KM_Fuel_Liter_KM(0x09,"Temp_C_Clock_12_Distance_KM_Fuel_Liter_KM"),
        Temp_C_Clock_12_Distance_Km_Fuel_Km_L(0x0A,"Temp_C_Clock_12_Distance_Km_Fuel_Km_L"),
        Temp_C_Clock_12_Distance_Km_Fuel_MPG_US_(0x0B,"Temp_C_Clock_12_Distance_Km_Fuel_MPG_US_"),
        Temp_C_Clock_12_Distance_Km_Fuel_MPG_UK_(0x0C,"Temp_C_Clock_12_Distance_Km_Fuel_MPG_UK_"),
        Temp_C_Clock_12_Distance_Miles_Fuel_L_Km(0x0D,"Temp_C_Clock_12_Distance_Miles_Fuel_L_Km"),
        Temp_C_Clock_12_Distance_Miles_Fuel_Km_L(0x0E,"Temp_C_Clock_12_Distance_Miles_Fuel_Km_L"),
        Temp_C_Clock_12_Distance_Miles_Fuel_MPG_US_(0x0F,"Temp_C_Clock_12_Distance_Miles_Fuel_MPG_US_"),
        Temp_C_Clock_12_Distance_Miles_Fuel_MPG_UK_(0x10,"Temp_C_Clock_12_Distance_Miles_Fuel_MPG_UK_"),
        Temp_F_Clock_24_Distance_Km_Fuel_L_Km(0x11,"Temp_F_Clock_24_Distance_Km_Fuel_L_Km"),
        Temp_F_Clock_24_Distance_Km_Fuel_Km_L(0x12,"Temp_F_Clock_24_Distance_Km_Fuel_Km_L"),
        Temp_F_Clock_24_Distance_Km_Fuel_MPG_US_(0x13,"Temp_F_Clock_24_Distance_Km_Fuel_MPG_US_"),
        Temp_F_Clock_24_Distance_Km_Fuel_MPG_UK_(0x14,"Temp_F_Clock_24_Distance_Km_Fuel_MPG_UK_"),
        Temp_F_Clock_24_Distance_Miles_Fuel_L_Km(0x15,"Temp_F_Clock_24_Distance_Miles_Fuel_L_Km"),
        Temp_F_Clock_24_Distance_Miles_Fuel_Km_L(0x16,"Temp_F_Clock_24_Distance_Miles_Fuel_Km_L"),
        Temp_F_Clock_24_Distance_Miles_Fuel_MPG_US_(0x17,"Temp_F_Clock_24_Distance_Miles_Fuel_MPG_US_"),
        Temp_F_Clock_24_Distance_Miles_Fuel_MPG_UK_(0x18,"Temp_F_Clock_24_Distance_Miles_Fuel_MPG_UK_"),
        Temp_F_Clock_12_Distance_Km_Fuel_L_Km(0x19,"Temp_F_Clock_12_Distance_Km_Fuel_L_Km"),
        Temp_F_Clock_12_Distance_Km_Fuel_Km_L(0x1A,"Temp_F_Clock_12_Distance_Km_Fuel_Km_L"),
        Temp_F_Clock_12_Distance_Km_Fuel_MPG_US_(0x1B,"Temp_F_Clock_12_Distance_Km_Fuel_MPG_US_"),
        Temp_F_Clock_12_Distance_Km_Fuel_MPG_UK_(0x1C,"Temp_F_Clock_12_Distance_Km_Fuel_MPG_UK_"),
        Temp_F_Clock_12_Distance_Miles_Fuel_L_Km(0x1D,"Temp_F_Clock_12_Distance_Miles_Fuel_L_Km"),
        Temp_F_Clock_12_Distance_Miles_Fuel_Km_L(0x1E,"Temp_F_Clock_12_Distance_Miles_Fuel_Km_L"),
        Temp_F_Clock_12_Distance_Miles_Fuel_US_Gallon(0x1F,"Temp_F_Clock_12_Distance_Miles_Fuel_US_Gallon"),
        Temp_F_Clock_12_Distance_Miles_Fuel_MPG_UK_(0x20,"Temp_F_Clock_12_Distance_Miles_Fuel_MPG_UK_");

        public final int value;
        public final String name;
        public static final int ParamNumber = 161;

        CC_161_UnitSettings(int value, String name){
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
            map_CC_161.put(0x01, CC_161_UnitSettings.Temp_C_Clock_24_Distance_KM_Fuel_Liter_KM);
            map_CC_161.put(0x02, CC_161_UnitSettings.Temp_C_Clock_24_Distance_KM_Fuel_KM_Liter);
            map_CC_161.put(0x03, CC_161_UnitSettings.Temp_C_Clock_24_Distance_Km_Fuel_MPG_US_);
            map_CC_161.put(0x04, CC_161_UnitSettings.Temp_C_Clock_24_Distance_Km_Fuel_MPG_UK_);
            map_CC_161.put(0x05, CC_161_UnitSettings.Temp_C_Clock_24_Distance_Miles_Fuel_L_Km);
            map_CC_161.put(0x06, CC_161_UnitSettings.Temp_C_Clock_24_Distance_Miles_Fuel_Km_L);
            map_CC_161.put(0x07, CC_161_UnitSettings.Temp_C_Clock_24_Distance_Miles_Fuel_MPG_US_);
            map_CC_161.put(0x08, CC_161_UnitSettings.Temp_C_Clock_24_Distance_Miles_Fuel_UK_Gallon);
            map_CC_161.put(0x09, CC_161_UnitSettings.Temp_C_Clock_12_Distance_KM_Fuel_Liter_KM);
            map_CC_161.put(0x0A, CC_161_UnitSettings.Temp_C_Clock_12_Distance_Km_Fuel_Km_L);
            map_CC_161.put(0x0B, CC_161_UnitSettings.Temp_C_Clock_12_Distance_Km_Fuel_MPG_US_);
            map_CC_161.put(0x0C, CC_161_UnitSettings.Temp_C_Clock_12_Distance_Km_Fuel_MPG_UK_);
            map_CC_161.put(0x0D, CC_161_UnitSettings.Temp_C_Clock_12_Distance_Miles_Fuel_L_Km);
            map_CC_161.put(0x0E, CC_161_UnitSettings.Temp_C_Clock_12_Distance_Miles_Fuel_Km_L);
            map_CC_161.put(0x0F, CC_161_UnitSettings.Temp_C_Clock_12_Distance_Miles_Fuel_MPG_US_);
            map_CC_161.put(0x10, CC_161_UnitSettings.Temp_C_Clock_12_Distance_Miles_Fuel_MPG_UK_);
            map_CC_161.put(0x11, CC_161_UnitSettings.Temp_F_Clock_24_Distance_Km_Fuel_L_Km);
            map_CC_161.put(0x12, CC_161_UnitSettings.Temp_F_Clock_24_Distance_Km_Fuel_Km_L);
            map_CC_161.put(0x13, CC_161_UnitSettings.Temp_F_Clock_24_Distance_Km_Fuel_MPG_US_);
            map_CC_161.put(0x14, CC_161_UnitSettings.Temp_F_Clock_24_Distance_Km_Fuel_MPG_UK_);
            map_CC_161.put(0x15, CC_161_UnitSettings.Temp_F_Clock_24_Distance_Miles_Fuel_L_Km);
            map_CC_161.put(0x16, CC_161_UnitSettings.Temp_F_Clock_24_Distance_Miles_Fuel_Km_L);
            map_CC_161.put(0x17, CC_161_UnitSettings.Temp_F_Clock_24_Distance_Miles_Fuel_MPG_US_);
            map_CC_161.put(0x18, CC_161_UnitSettings.Temp_F_Clock_24_Distance_Miles_Fuel_MPG_UK_);
            map_CC_161.put(0x19, CC_161_UnitSettings.Temp_F_Clock_12_Distance_Km_Fuel_L_Km);
            map_CC_161.put(0x1A, CC_161_UnitSettings.Temp_F_Clock_12_Distance_Km_Fuel_Km_L);
            map_CC_161.put(0x1B, CC_161_UnitSettings.Temp_F_Clock_12_Distance_Km_Fuel_MPG_US_);
            map_CC_161.put(0x1C, CC_161_UnitSettings.Temp_F_Clock_12_Distance_Km_Fuel_MPG_UK_);
            map_CC_161.put(0x1D, CC_161_UnitSettings.Temp_F_Clock_12_Distance_Miles_Fuel_L_Km);
            map_CC_161.put(0x1E, CC_161_UnitSettings.Temp_F_Clock_12_Distance_Miles_Fuel_Km_L);
            map_CC_161.put(0x1F, CC_161_UnitSettings.Temp_F_Clock_12_Distance_Miles_Fuel_US_Gallon);
            map_CC_161.put(0x20, CC_161_UnitSettings.Temp_F_Clock_12_Distance_Miles_Fuel_MPG_UK_);
            getValueData.put(CC_161_UnitSettings.class, map_CC_161);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_167 = new HashMap<>();
    public enum CC_167_TripComputerAndStatistics implements CarConfigEnumBase
    {
        Without_trip_comp_and_trip_stat(0x01,"Without_trip_comp_and_trip_stat"),
        With_trip_comp_and_without_trip_stat(0x02,"With_trip_comp_and_without_trip_stat"),
        Without_trip_comp_and_with_trip_stat(0x03,"Without_trip_comp_and_with_trip_stat"),
        With_trip_comp_and_with_trip_stat(0x04,"With_trip_comp_and_with_trip_stat");

        public final int value;
        public final String name;
        public static final int ParamNumber = 167;

        CC_167_TripComputerAndStatistics(int value, String name){
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
            map_CC_167.put(0x01, CC_167_TripComputerAndStatistics.Without_trip_comp_and_trip_stat);
            map_CC_167.put(0x02, CC_167_TripComputerAndStatistics.With_trip_comp_and_without_trip_stat);
            map_CC_167.put(0x03, CC_167_TripComputerAndStatistics.Without_trip_comp_and_with_trip_stat);
            map_CC_167.put(0x04, CC_167_TripComputerAndStatistics.With_trip_comp_and_with_trip_stat);
            getValueData.put(CC_167_TripComputerAndStatistics.class, map_CC_167);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_168 = new HashMap<>();
    public enum CC_168_ScreenThemesskins implements CarConfigEnumBase
    {
        Screen_Skins_Standard_1_(0x01,"Screen_Skins_Standard_1_"),
        Screen_Skins_R_Design_(0x02,"Screen_Skins_R_Design_"),
        Screen_Skins_Inscription_(0x03,"Screen_Skins_Inscription_"),
        Screen_Skins_VOR_(0x04,"Screen_Skins_VOR_"),
        Screen_Skins_PS14_(0x05,"Screen_Skins_PS14_"),
        Screen_Skins_Excellence_(0x06,"Screen_Skins_Excellence_");

        public final int value;
        public final String name;
        public static final int ParamNumber = 168;

        CC_168_ScreenThemesskins(int value, String name){
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
            map_CC_168.put(0x01, CC_168_ScreenThemesskins.Screen_Skins_Standard_1_);
            map_CC_168.put(0x02, CC_168_ScreenThemesskins.Screen_Skins_R_Design_);
            map_CC_168.put(0x03, CC_168_ScreenThemesskins.Screen_Skins_Inscription_);
            map_CC_168.put(0x04, CC_168_ScreenThemesskins.Screen_Skins_VOR_);
            map_CC_168.put(0x05, CC_168_ScreenThemesskins.Screen_Skins_PS14_);
            map_CC_168.put(0x06, CC_168_ScreenThemesskins.Screen_Skins_Excellence_);
            getValueData.put(CC_168_ScreenThemesskins.class, map_CC_168);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_170 = new HashMap<>();
    public enum CC_170_HvacRear implements CarConfigEnumBase
    {
        Without_rear_VAC(0x01,"Without_rear_VAC"),
        With_rear_VAC_without_PTCs(0x02,"With_rear_VAC_without_PTCs"),
        With_rear_VAC_with_PTCs(0x03,"With_rear_VAC_with_PTCs");

        public final int value;
        public final String name;
        public static final int ParamNumber = 170;

        CC_170_HvacRear(int value, String name){
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
            map_CC_170.put(0x01, CC_170_HvacRear.Without_rear_VAC);
            map_CC_170.put(0x02, CC_170_HvacRear.With_rear_VAC_without_PTCs);
            map_CC_170.put(0x03, CC_170_HvacRear.With_rear_VAC_with_PTCs);
            getValueData.put(CC_170_HvacRear.class, map_CC_170);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_171 = new HashMap<>();
    public enum CC_171_AdditionalHeater implements CarConfigEnumBase
    {
        Without_Additional_Heater(0x01,"Without_Additional_Heater"),
        With_Additional_Heater(0x02,"With_Additional_Heater");

        public final int value;
        public final String name;
        public static final int ParamNumber = 171;

        CC_171_AdditionalHeater(int value, String name){
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
            map_CC_171.put(0x01, CC_171_AdditionalHeater.Without_Additional_Heater);
            map_CC_171.put(0x02, CC_171_AdditionalHeater.With_Additional_Heater);
            getValueData.put(CC_171_AdditionalHeater.class, map_CC_171);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_174 = new HashMap<>();
    public enum CC_174_AirQualitySystem implements CarConfigEnumBase
    {
        No_Air_Quality_System(0x01,"No_Air_Quality_System"),
        Air_Quality_System(0x02,"Air_Quality_System");

        public final int value;
        public final String name;
        public static final int ParamNumber = 174;

        CC_174_AirQualitySystem(int value, String name){
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
            map_CC_174.put(0x01, CC_174_AirQualitySystem.No_Air_Quality_System);
            map_CC_174.put(0x02, CC_174_AirQualitySystem.Air_Quality_System);
            getValueData.put(CC_174_AirQualitySystem.class, map_CC_174);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_175 = new HashMap<>();
    public enum CC_175_HvacVariants implements CarConfigEnumBase
    {
        Electronic_Climate_Control_2_zone(0x01,"Electronic_Climate_Control_2_zone"),
        Electronic_Climate_Control_4_zone(0x02,"Electronic_Climate_Control_4_zone"),
        HVAC_Small_1_zone(0x03,"HVAC_Small_1_zone"),
        HVAC_Small_2_zone(0x04,"HVAC_Small_2_zone"),
        HVAC_Small_3_zone(0x05,"HVAC_Small_3_zone"),
        Manual_AC(0x06,"Manual_AC");

        public final int value;
        public final String name;
        public static final int ParamNumber = 175;

        CC_175_HvacVariants(int value, String name){
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
            map_CC_175.put(0x01, CC_175_HvacVariants.Electronic_Climate_Control_2_zone);
            map_CC_175.put(0x02, CC_175_HvacVariants.Electronic_Climate_Control_4_zone);
            map_CC_175.put(0x03, CC_175_HvacVariants.HVAC_Small_1_zone);
            map_CC_175.put(0x04, CC_175_HvacVariants.HVAC_Small_2_zone);
            map_CC_175.put(0x05, CC_175_HvacVariants.HVAC_Small_3_zone);
            map_CC_175.put(0x06, CC_175_HvacVariants.Manual_AC);
            getValueData.put(CC_175_HvacVariants.class, map_CC_175);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_176 = new HashMap<>();
    public enum CC_176_RearViewMirrorType implements CarConfigEnumBase
    {
        Manual_dimming(0x01,"Manual_dimming"),
        Automatic_dimming_inner(0x02,"Automatic_dimming_inner"),
        Automatic_dimming_inner_outer(0x03,"Automatic_dimming_inner_outer");

        public final int value;
        public final String name;
        public static final int ParamNumber = 176;

        CC_176_RearViewMirrorType(int value, String name){
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
            map_CC_176.put(0x01, CC_176_RearViewMirrorType.Manual_dimming);
            map_CC_176.put(0x02, CC_176_RearViewMirrorType.Automatic_dimming_inner);
            map_CC_176.put(0x03, CC_176_RearViewMirrorType.Automatic_dimming_inner_outer);
            getValueData.put(CC_176_RearViewMirrorType.class, map_CC_176);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_177 = new HashMap<>();
    public enum CC_177_RoofHatch implements CarConfigEnumBase
    {
        No_roof_hatch_panorama_roof(0x01,"No_roof_hatch_panorama_roof"),
        Openable_panorama_roof(0x02,"Openable_panorama_roof"),
        Hatch(0x03,"Hatch"),
        Electrochromic_Panorama_Roof(0x04,"Electrochromic_Panorama_Roof"),
        Panorama_roof_fixed_(0x05,"Panorama_roof_fixed_");

        public final int value;
        public final String name;
        public static final int ParamNumber = 177;

        CC_177_RoofHatch(int value, String name){
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
            map_CC_177.put(0x01, CC_177_RoofHatch.No_roof_hatch_panorama_roof);
            map_CC_177.put(0x02, CC_177_RoofHatch.Openable_panorama_roof);
            map_CC_177.put(0x03, CC_177_RoofHatch.Hatch);
            map_CC_177.put(0x04, CC_177_RoofHatch.Electrochromic_Panorama_Roof);
            map_CC_177.put(0x05, CC_177_RoofHatch.Panorama_roof_fixed_);
            getValueData.put(CC_177_RoofHatch.class, map_CC_177);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_179 = new HashMap<>();
    public enum CC_179_FrontSeatVentilation implements CarConfigEnumBase
    {
        Without_ventilated_seats(0x01,"Without_ventilated_seats"),
        Ventilated_seats(0x02,"Ventilated_seats"),
        Ventilated_seat_driver_seat(0x03,"Ventilated_seat_driver_seat");

        public final int value;
        public final String name;
        public static final int ParamNumber = 179;

        CC_179_FrontSeatVentilation(int value, String name){
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
            map_CC_179.put(0x01, CC_179_FrontSeatVentilation.Without_ventilated_seats);
            map_CC_179.put(0x02, CC_179_FrontSeatVentilation.Ventilated_seats);
            map_CC_179.put(0x03, CC_179_FrontSeatVentilation.Ventilated_seat_driver_seat);
            getValueData.put(CC_179_FrontSeatVentilation.class, map_CC_179);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_180 = new HashMap<>();
    public enum CC_180_HeatedFrontSeats implements CarConfigEnumBase
    {
        Without_heated_front_seats(0x01,"Without_heated_front_seats"),
        Heated_front_seats(0x02,"Heated_front_seats");

        public final int value;
        public final String name;
        public static final int ParamNumber = 180;

        CC_180_HeatedFrontSeats(int value, String name){
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
            map_CC_180.put(0x01, CC_180_HeatedFrontSeats.Without_heated_front_seats);
            map_CC_180.put(0x02, CC_180_HeatedFrontSeats.Heated_front_seats);
            getValueData.put(CC_180_HeatedFrontSeats.class, map_CC_180);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_181 = new HashMap<>();
    public enum CC_181_HeatedRearSeat implements CarConfigEnumBase
    {
        Without_heated_rear_seat(0x01,"Without_heated_rear_seat"),
        Heated_rear_seat(0x02,"Heated_rear_seat");

        public final int value;
        public final String name;
        public static final int ParamNumber = 181;

        CC_181_HeatedRearSeat(int value, String name){
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
            map_CC_181.put(0x01, CC_181_HeatedRearSeat.Without_heated_rear_seat);
            map_CC_181.put(0x02, CC_181_HeatedRearSeat.Heated_rear_seat);
            getValueData.put(CC_181_HeatedRearSeat.class, map_CC_181);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_182 = new HashMap<>();
    public enum CC_182_ParkingClimateLevel implements CarConfigEnumBase
    {
        No_parking_climate(0x01,"No_parking_climate"),
        Afterrun_parking_climate(0x02,"Afterrun_parking_climate"),
        Heating_parking_climate(0x03,"Heating_parking_climate"),
        Full_parking_climate(0x04,"Full_parking_climate");

        public final int value;
        public final String name;
        public static final int ParamNumber = 182;

        CC_182_ParkingClimateLevel(int value, String name){
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
            map_CC_182.put(0x01, CC_182_ParkingClimateLevel.No_parking_climate);
            map_CC_182.put(0x02, CC_182_ParkingClimateLevel.Afterrun_parking_climate);
            map_CC_182.put(0x03, CC_182_ParkingClimateLevel.Heating_parking_climate);
            map_CC_182.put(0x04, CC_182_ParkingClimateLevel.Full_parking_climate);
            getValueData.put(CC_182_ParkingClimateLevel.class, map_CC_182);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_183 = new HashMap<>();
    public enum CC_183_InteriorIllumination implements CarConfigEnumBase
    {
        Interior_lighting_base_version(0x01,"Interior_lighting_base_version"),
        Interior_lighting_high_version(0x02,"Interior_lighting_high_version"),
        Interior_lighting_mid_version(0x03,"Interior_lighting_mid_version"),
        Interior_lighting_base_version_not_for_Volvo(0x11,"Interior_lighting_base_version_not_for_Volvo"),
        Interior_lighting_mid_version_not_for_Volvo(0x12,"Interior_lighting_mid_version_not_for_Volvo"),
        Interior_lighting_high_version_not_for_Volvo(0x13,"Interior_lighting_high_version_not_for_Volvo");

        public final int value;
        public final String name;
        public static final int ParamNumber = 183;

        CC_183_InteriorIllumination(int value, String name){
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
            map_CC_183.put(0x01, CC_183_InteriorIllumination.Interior_lighting_base_version);
            map_CC_183.put(0x02, CC_183_InteriorIllumination.Interior_lighting_high_version);
            map_CC_183.put(0x03, CC_183_InteriorIllumination.Interior_lighting_mid_version);
            map_CC_183.put(0x11, CC_183_InteriorIllumination.Interior_lighting_base_version_not_for_Volvo);
            map_CC_183.put(0x12, CC_183_InteriorIllumination.Interior_lighting_mid_version_not_for_Volvo);
            map_CC_183.put(0x13, CC_183_InteriorIllumination.Interior_lighting_high_version_not_for_Volvo);
            getValueData.put(CC_183_InteriorIllumination.class, map_CC_183);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_184 = new HashMap<>();
    public enum CC_184_PowerSeats implements CarConfigEnumBase
    {
        No_Memory_Seats(0x01,"No_Memory_Seats"),
        Driver_Memory_seat_and_Mechanical_passenger_seat(0x02,"Driver_Memory_seat_and_Mechanical_passenger_seat"),
        Driver_Memory_seat_and_Passenger_seat_without_memory(0x03,"Driver_Memory_seat_and_Passenger_seat_without_memory"),
        Driver_and_Passenger_Memory_Seats(0x04,"Driver_and_Passenger_Memory_Seats");

        public final int value;
        public final String name;
        public static final int ParamNumber = 184;

        CC_184_PowerSeats(int value, String name){
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
            map_CC_184.put(0x01, CC_184_PowerSeats.No_Memory_Seats);
            map_CC_184.put(0x02, CC_184_PowerSeats.Driver_Memory_seat_and_Mechanical_passenger_seat);
            map_CC_184.put(0x03, CC_184_PowerSeats.Driver_Memory_seat_and_Passenger_seat_without_memory);
            map_CC_184.put(0x04, CC_184_PowerSeats.Driver_and_Passenger_Memory_Seats);
            getValueData.put(CC_184_PowerSeats.class, map_CC_184);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_185 = new HashMap<>();
    public enum CC_185_PowerFoldRise3dRow implements CarConfigEnumBase
    {
        No_fold_rise_3_d_row_rear_seat(0x01,"No_fold_rise_3_d_row_rear_seat"),
        Mecanical_fold_rise_3_d_row_rear_seat(0x02,"Mecanical_fold_rise_3_d_row_rear_seat"),
        Power_fold_rise_3_d_row_rear_seat(0x03,"Power_fold_rise_3_d_row_rear_seat");

        public final int value;
        public final String name;
        public static final int ParamNumber = 185;

        CC_185_PowerFoldRise3dRow(int value, String name){
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
            map_CC_185.put(0x01, CC_185_PowerFoldRise3dRow.No_fold_rise_3_d_row_rear_seat);
            map_CC_185.put(0x02, CC_185_PowerFoldRise3dRow.Mecanical_fold_rise_3_d_row_rear_seat);
            map_CC_185.put(0x03, CC_185_PowerFoldRise3dRow.Power_fold_rise_3_d_row_rear_seat);
            getValueData.put(CC_185_PowerFoldRise3dRow.class, map_CC_185);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_186 = new HashMap<>();
    public enum CC_186_HeatedSteeringWheel implements CarConfigEnumBase
    {
        Without_heated_steering_wheel(0x01,"Without_heated_steering_wheel"),
        Heated_steering_wheel(0x02,"Heated_steering_wheel");

        public final int value;
        public final String name;
        public static final int ParamNumber = 186;

        CC_186_HeatedSteeringWheel(int value, String name){
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
            map_CC_186.put(0x01, CC_186_HeatedSteeringWheel.Without_heated_steering_wheel);
            map_CC_186.put(0x02, CC_186_HeatedSteeringWheel.Heated_steering_wheel);
            getValueData.put(CC_186_HeatedSteeringWheel.class, map_CC_186);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_187 = new HashMap<>();
    public enum CC_187_Headrests2ndRowFolding implements CarConfigEnumBase
    {
        Manually_foldable_rear_headrests(0x01,"Manually_foldable_rear_headrests"),
        El_folding_rear_headrests(0x02,"El_folding_rear_headrests"),
        Fixed_rear_headrests(0x03,"Fixed_rear_headrests"),
        Rear_headsrests_Comfort(0x04,"Rear_headsrests_Comfort");

        public final int value;
        public final String name;
        public static final int ParamNumber = 187;

        CC_187_Headrests2ndRowFolding(int value, String name){
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
            map_CC_187.put(0x01, CC_187_Headrests2ndRowFolding.Manually_foldable_rear_headrests);
            map_CC_187.put(0x02, CC_187_Headrests2ndRowFolding.El_folding_rear_headrests);
            map_CC_187.put(0x03, CC_187_Headrests2ndRowFolding.Fixed_rear_headrests);
            map_CC_187.put(0x04, CC_187_Headrests2ndRowFolding.Rear_headsrests_Comfort);
            getValueData.put(CC_187_Headrests2ndRowFolding.class, map_CC_187);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_189 = new HashMap<>();
    public enum CC_189_RearSeatVentilation implements CarConfigEnumBase
    {
        Without_rear_seat_ventilation(0x01,"Without_rear_seat_ventilation"),
        With_rear_seat_ventilation(0x02,"With_rear_seat_ventilation");

        public final int value;
        public final String name;
        public static final int ParamNumber = 189;

        CC_189_RearSeatVentilation(int value, String name){
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
            map_CC_189.put(0x01, CC_189_RearSeatVentilation.Without_rear_seat_ventilation);
            map_CC_189.put(0x02, CC_189_RearSeatVentilation.With_rear_seat_ventilation);
            getValueData.put(CC_189_RearSeatVentilation.class, map_CC_189);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_190 = new HashMap<>();
    public enum CC_190_GlobalNavigationSatSystemReceiver implements CarConfigEnumBase
    {
        Without_Global_Navigation_Satellite_System_receiver(0x01,"Without_Global_Navigation_Satellite_System_receiver"),
        With_Global_Navigation_Satellite_System_receiver(0x02,"With_Global_Navigation_Satellite_System_receiver");

        public final int value;
        public final String name;
        public static final int ParamNumber = 190;

        CC_190_GlobalNavigationSatSystemReceiver(int value, String name){
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
            map_CC_190.put(0x01, CC_190_GlobalNavigationSatSystemReceiver.Without_Global_Navigation_Satellite_System_receiver);
            map_CC_190.put(0x02, CC_190_GlobalNavigationSatSystemReceiver.With_Global_Navigation_Satellite_System_receiver);
            getValueData.put(CC_190_GlobalNavigationSatSystemReceiver.class, map_CC_190);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_191 = new HashMap<>();
    public enum CC_191_TelematicModule implements CarConfigEnumBase
    {
        Without_Telematic_module(0x01,"Without_Telematic_module"),
        With_Telematic_module(0x02,"With_Telematic_module"),
        With_Telematic_module_Brazil(0x03,"With_Telematic_module_Brazil"),
        With_Telematic_module_USA_CDN(0x04,"With_Telematic_module_USA_CDN"),
        With_Telematic_module_China(0x05,"With_Telematic_module_China"),
        With_Telematic_module_ROW_LTE(0x06,"With_Telematic_module_ROW_LTE"),
        With_Telematic_module_Russia(0x07,"With_Telematic_module_Russia"),
        With_Telematic_module_only_eCall_Volvo_Emergency_Call_(0x08,"With_Telematic_module_only_eCall_Volvo_Emergency_Call_");

        public final int value;
        public final String name;
        public static final int ParamNumber = 191;

        CC_191_TelematicModule(int value, String name){
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
            map_CC_191.put(0x01, CC_191_TelematicModule.Without_Telematic_module);
            map_CC_191.put(0x02, CC_191_TelematicModule.With_Telematic_module);
            map_CC_191.put(0x03, CC_191_TelematicModule.With_Telematic_module_Brazil);
            map_CC_191.put(0x04, CC_191_TelematicModule.With_Telematic_module_USA_CDN);
            map_CC_191.put(0x05, CC_191_TelematicModule.With_Telematic_module_China);
            map_CC_191.put(0x06, CC_191_TelematicModule.With_Telematic_module_ROW_LTE);
            map_CC_191.put(0x07, CC_191_TelematicModule.With_Telematic_module_Russia);
            map_CC_191.put(0x08, CC_191_TelematicModule.With_Telematic_module_only_eCall_Volvo_Emergency_Call_);
            getValueData.put(CC_191_TelematicModule.class, map_CC_191);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_192 = new HashMap<>();
    public enum CC_192_IhuLevel implements CarConfigEnumBase
    {
        IHU_Mid_Base_Performance(0x01,"IHU_Mid_Base_Performance"),
        IHU_high(0x02,"IHU_high"),
        IHU_Mid_High_Performance(0x03,"IHU_Mid_High_Performance");

        public final int value;
        public final String name;
        public static final int ParamNumber = 192;

        CC_192_IhuLevel(int value, String name){
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
            map_CC_192.put(0x01, CC_192_IhuLevel.IHU_Mid_Base_Performance);
            map_CC_192.put(0x02, CC_192_IhuLevel.IHU_high);
            map_CC_192.put(0x03, CC_192_IhuLevel.IHU_Mid_High_Performance);
            getValueData.put(CC_192_IhuLevel.class, map_CC_192);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_193 = new HashMap<>();
    public enum CC_193_DigitalRadioReceiver implements CarConfigEnumBase
    {
        Without_digital_radio_system(0x01,"Without_digital_radio_system"),
        DAB_radio_system(0x02,"DAB_radio_system"),
        HD_SDARS_radio_system_US(0x03,"HD_SDARS_radio_system_US"),
        Not_in_use(0x04,"Not_in_use"),
        HD_SDARS_radio_system_Can(0x05,"HD_SDARS_radio_system_Can");

        public final int value;
        public final String name;
        public static final int ParamNumber = 193;

        CC_193_DigitalRadioReceiver(int value, String name){
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
            map_CC_193.put(0x01, CC_193_DigitalRadioReceiver.Without_digital_radio_system);
            map_CC_193.put(0x02, CC_193_DigitalRadioReceiver.DAB_radio_system);
            map_CC_193.put(0x03, CC_193_DigitalRadioReceiver.HD_SDARS_radio_system_US);
            map_CC_193.put(0x04, CC_193_DigitalRadioReceiver.Not_in_use);
            map_CC_193.put(0x05, CC_193_DigitalRadioReceiver.HD_SDARS_radio_system_Can);
            getValueData.put(CC_193_DigitalRadioReceiver.class, map_CC_193);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_194 = new HashMap<>();
    public enum CC_194_RearSeatEntertainment implements CarConfigEnumBase
    {
        Without_Rear_Seat_Entertainment(0x01,"Without_Rear_Seat_Entertainment"),
        Rear_Seat_Entertainment(0x02,"Rear_Seat_Entertainment");

        public final int value;
        public final String name;
        public static final int ParamNumber = 194;

        CC_194_RearSeatEntertainment(int value, String name){
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
            map_CC_194.put(0x01, CC_194_RearSeatEntertainment.Without_Rear_Seat_Entertainment);
            map_CC_194.put(0x02, CC_194_RearSeatEntertainment.Rear_Seat_Entertainment);
            getValueData.put(CC_194_RearSeatEntertainment.class, map_CC_194);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_195 = new HashMap<>();
    public enum CC_195_RegionIfDvd implements CarConfigEnumBase
    {
        DVD_region_0(0x01,"DVD_region_0"),
        DVD_region_1(0x02,"DVD_region_1"),
        DVD_region_2(0x03,"DVD_region_2"),
        DVD_region_3(0x04,"DVD_region_3"),
        DVD_region_4(0x05,"DVD_region_4"),
        DVD_region_5(0x06,"DVD_region_5"),
        DVD_region_6(0x07,"DVD_region_6"),
        DVD_region_7(0x08,"DVD_region_7"),
        DVD_region_8(0x09,"DVD_region_8"),
        DVD_region_9(0x0A,"DVD_region_9"),
        DVD_region_10(0x0B,"DVD_region_10");

        public final int value;
        public final String name;
        public static final int ParamNumber = 195;

        CC_195_RegionIfDvd(int value, String name){
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
            map_CC_195.put(0x01, CC_195_RegionIfDvd.DVD_region_0);
            map_CC_195.put(0x02, CC_195_RegionIfDvd.DVD_region_1);
            map_CC_195.put(0x03, CC_195_RegionIfDvd.DVD_region_2);
            map_CC_195.put(0x04, CC_195_RegionIfDvd.DVD_region_3);
            map_CC_195.put(0x05, CC_195_RegionIfDvd.DVD_region_4);
            map_CC_195.put(0x06, CC_195_RegionIfDvd.DVD_region_5);
            map_CC_195.put(0x07, CC_195_RegionIfDvd.DVD_region_6);
            map_CC_195.put(0x08, CC_195_RegionIfDvd.DVD_region_7);
            map_CC_195.put(0x09, CC_195_RegionIfDvd.DVD_region_8);
            map_CC_195.put(0x0A, CC_195_RegionIfDvd.DVD_region_9);
            map_CC_195.put(0x0B, CC_195_RegionIfDvd.DVD_region_10);
            getValueData.put(CC_195_RegionIfDvd.class, map_CC_195);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_196 = new HashMap<>();
    public enum CC_196_WifiFrequencyBand implements CarConfigEnumBase
    {
        Two_4_GHz(0x01,"Two_4_GHz"),
        Five_GHz(0x02,"Five_GHz"),
        Two_4_GHz_and_5_GHz(0x03,"Two_4_GHz_and_5_GHz");

        public final int value;
        public final String name;
        public static final int ParamNumber = 196;

        CC_196_WifiFrequencyBand(int value, String name){
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
            map_CC_196.put(0x01, CC_196_WifiFrequencyBand.Two_4_GHz);
            map_CC_196.put(0x02, CC_196_WifiFrequencyBand.Five_GHz);
            map_CC_196.put(0x03, CC_196_WifiFrequencyBand.Two_4_GHz_and_5_GHz);
            getValueData.put(CC_196_WifiFrequencyBand.class, map_CC_196);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_197 = new HashMap<>();
    public enum CC_197_SystemLanguage implements CarConfigEnumBase
    {
        Arabic(0x01,"Arabic"),
        Chinese_Simp_Man(0x02,"Chinese_Simp_Man"),
        Chinese_Trad_Can(0x03,"Chinese_Trad_Can"),
        Chinese_Trad_Man(0x04,"Chinese_Trad_Man"),
        Czech(0x05,"Czech"),
        Danish(0x06,"Danish"),
        Dutch(0x07,"Dutch"),
        Australien_English(0x08,"Australien_English"),
        English(0x09,"English"),
        American_English(0x0A,"American_English"),
        Finnish(0x0B,"Finnish"),
        Flemmish(0x0C,"Flemmish"),
        Canadian_French(0x0D,"Canadian_French"),
        French(0x0E,"French"),
        German(0x0F,"German"),
        Greek(0x10,"Greek"),
        Hungarian(0x11,"Hungarian"),
        Italian(0x12,"Italian"),
        Japanese(0x13,"Japanese"),
        Korean_Han_Gul_(0x14,"Korean_Han_Gul_"),
        Norwegian(0x15,"Norwegian"),
        Polish(0x16,"Polish"),
        Brazilian_Portuguese(0x17,"Brazilian_Portuguese"),
        Portuguese(0x18,"Portuguese"),
        Russian(0x19,"Russian"),
        Spanish(0x1A,"Spanish"),
        American_Spanish(0x1B,"American_Spanish"),
        Swedish(0x1C,"Swedish"),
        Thai(0x1D,"Thai"),
        Turkish(0x1E,"Turkish"),
        Bulgarian(0x1F,"Bulgarian"),
        Estonian(0x20,"Estonian"),
        Latvian(0x21,"Latvian"),
        Lithuanian(0x22,"Lithuanian"),
        Romanian(0x23,"Romanian"),
        Slovak(0x24,"Slovak"),
        Slovene(0x25,"Slovene");

        public final int value;
        public final String name;
        public static final int ParamNumber = 197;

        CC_197_SystemLanguage(int value, String name){
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
            map_CC_197.put(0x01, CC_197_SystemLanguage.Arabic);
            map_CC_197.put(0x02, CC_197_SystemLanguage.Chinese_Simp_Man);
            map_CC_197.put(0x03, CC_197_SystemLanguage.Chinese_Trad_Can);
            map_CC_197.put(0x04, CC_197_SystemLanguage.Chinese_Trad_Man);
            map_CC_197.put(0x05, CC_197_SystemLanguage.Czech);
            map_CC_197.put(0x06, CC_197_SystemLanguage.Danish);
            map_CC_197.put(0x07, CC_197_SystemLanguage.Dutch);
            map_CC_197.put(0x08, CC_197_SystemLanguage.Australien_English);
            map_CC_197.put(0x09, CC_197_SystemLanguage.English);
            map_CC_197.put(0x0A, CC_197_SystemLanguage.American_English);
            map_CC_197.put(0x0B, CC_197_SystemLanguage.Finnish);
            map_CC_197.put(0x0C, CC_197_SystemLanguage.Flemmish);
            map_CC_197.put(0x0D, CC_197_SystemLanguage.Canadian_French);
            map_CC_197.put(0x0E, CC_197_SystemLanguage.French);
            map_CC_197.put(0x0F, CC_197_SystemLanguage.German);
            map_CC_197.put(0x10, CC_197_SystemLanguage.Greek);
            map_CC_197.put(0x11, CC_197_SystemLanguage.Hungarian);
            map_CC_197.put(0x12, CC_197_SystemLanguage.Italian);
            map_CC_197.put(0x13, CC_197_SystemLanguage.Japanese);
            map_CC_197.put(0x14, CC_197_SystemLanguage.Korean_Han_Gul_);
            map_CC_197.put(0x15, CC_197_SystemLanguage.Norwegian);
            map_CC_197.put(0x16, CC_197_SystemLanguage.Polish);
            map_CC_197.put(0x17, CC_197_SystemLanguage.Brazilian_Portuguese);
            map_CC_197.put(0x18, CC_197_SystemLanguage.Portuguese);
            map_CC_197.put(0x19, CC_197_SystemLanguage.Russian);
            map_CC_197.put(0x1A, CC_197_SystemLanguage.Spanish);
            map_CC_197.put(0x1B, CC_197_SystemLanguage.American_Spanish);
            map_CC_197.put(0x1C, CC_197_SystemLanguage.Swedish);
            map_CC_197.put(0x1D, CC_197_SystemLanguage.Thai);
            map_CC_197.put(0x1E, CC_197_SystemLanguage.Turkish);
            map_CC_197.put(0x1F, CC_197_SystemLanguage.Bulgarian);
            map_CC_197.put(0x20, CC_197_SystemLanguage.Estonian);
            map_CC_197.put(0x21, CC_197_SystemLanguage.Latvian);
            map_CC_197.put(0x22, CC_197_SystemLanguage.Lithuanian);
            map_CC_197.put(0x23, CC_197_SystemLanguage.Romanian);
            map_CC_197.put(0x24, CC_197_SystemLanguage.Slovak);
            map_CC_197.put(0x25, CC_197_SystemLanguage.Slovene);
            getValueData.put(CC_197_SystemLanguage.class, map_CC_197);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_198 = new HashMap<>();
    public enum CC_198_Speech implements CarConfigEnumBase
    {
        No_speech(0x01,"No_speech"),
        Speech(0x02,"Speech");

        public final int value;
        public final String name;
        public static final int ParamNumber = 198;

        CC_198_Speech(int value, String name){
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
            map_CC_198.put(0x01, CC_198_Speech.No_speech);
            map_CC_198.put(0x02, CC_198_Speech.Speech);
            getValueData.put(CC_198_Speech.class, map_CC_198);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_199 = new HashMap<>();
    public enum CC_199_SpeechLanguage implements CarConfigEnumBase
    {
        No_microphone_mounted_in_car(0x01,"No_microphone_mounted_in_car"),
        One_handsfree_microphone_is_available(0x02,"One_handsfree_microphone_is_available"),
        Chinese_Trad_Can(0x03,"Chinese_Trad_Can"),
        Danish(0x04,"Danish"),
        Dutch(0x05,"Dutch"),
        Australien_English(0x06,"Australien_English"),
        English(0x07,"English"),
        American_English(0x08,"American_English"),
        Finnish(0x09,"Finnish"),
        Flemmish(0x0A,"Flemmish"),
        French_Canadian(0x0B,"French_Canadian"),
        French(0x0C,"French"),
        German(0x0D,"German"),
        Italian(0x0E,"Italian"),
        Japanese(0x0F,"Japanese"),
        Korean_Han_Gul_(0x10,"Korean_Han_Gul_"),
        Brazilian_Portuguese(0x11,"Brazilian_Portuguese"),
        Portuguese(0x12,"Portuguese"),
        Russian(0x13,"Russian"),
        Spanish(0x14,"Spanish"),
        American_Spanish(0x15,"American_Spanish"),
        Swedish(0x16,"Swedish");

        public final int value;
        public final String name;
        public static final int ParamNumber = 199;

        CC_199_SpeechLanguage(int value, String name){
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
            map_CC_199.put(0x01, CC_199_SpeechLanguage.No_microphone_mounted_in_car);
            map_CC_199.put(0x02, CC_199_SpeechLanguage.One_handsfree_microphone_is_available);
            map_CC_199.put(0x03, CC_199_SpeechLanguage.Chinese_Trad_Can);
            map_CC_199.put(0x04, CC_199_SpeechLanguage.Danish);
            map_CC_199.put(0x05, CC_199_SpeechLanguage.Dutch);
            map_CC_199.put(0x06, CC_199_SpeechLanguage.Australien_English);
            map_CC_199.put(0x07, CC_199_SpeechLanguage.English);
            map_CC_199.put(0x08, CC_199_SpeechLanguage.American_English);
            map_CC_199.put(0x09, CC_199_SpeechLanguage.Finnish);
            map_CC_199.put(0x0A, CC_199_SpeechLanguage.Flemmish);
            map_CC_199.put(0x0B, CC_199_SpeechLanguage.French_Canadian);
            map_CC_199.put(0x0C, CC_199_SpeechLanguage.French);
            map_CC_199.put(0x0D, CC_199_SpeechLanguage.German);
            map_CC_199.put(0x0E, CC_199_SpeechLanguage.Italian);
            map_CC_199.put(0x0F, CC_199_SpeechLanguage.Japanese);
            map_CC_199.put(0x10, CC_199_SpeechLanguage.Korean_Han_Gul_);
            map_CC_199.put(0x11, CC_199_SpeechLanguage.Brazilian_Portuguese);
            map_CC_199.put(0x12, CC_199_SpeechLanguage.Portuguese);
            map_CC_199.put(0x13, CC_199_SpeechLanguage.Russian);
            map_CC_199.put(0x14, CC_199_SpeechLanguage.Spanish);
            map_CC_199.put(0x15, CC_199_SpeechLanguage.American_Spanish);
            map_CC_199.put(0x16, CC_199_SpeechLanguage.Swedish);
            getValueData.put(CC_199_SpeechLanguage.class, map_CC_199);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_200 = new HashMap<>();
    public enum CC_200_WifiFunctionalityControl implements CarConfigEnumBase
    {
        N_STA_N_AP_No_Wi_Fi_Station_No_Wi_Fi_AP(0x01,"N_STA_N_AP_No_Wi_Fi_Station_No_Wi_Fi_AP"),
        STA_N_AP_Wi_Fi_Station_No_Wi_Fi_AP(0x02,"STA_N_AP_Wi_Fi_Station_No_Wi_Fi_AP"),
        N_STA_AP_No_Wi_Fi_Station_Wi_Fi_AP(0x03,"N_STA_AP_No_Wi_Fi_Station_Wi_Fi_AP"),
        STA_AP_Wi_Fi_Station_Wi_Fi_AP(0x04,"STA_AP_Wi_Fi_Station_Wi_Fi_AP");

        public final int value;
        public final String name;
        public static final int ParamNumber = 200;

        CC_200_WifiFunctionalityControl(int value, String name){
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
            map_CC_200.put(0x01, CC_200_WifiFunctionalityControl.N_STA_N_AP_No_Wi_Fi_Station_No_Wi_Fi_AP);
            map_CC_200.put(0x02, CC_200_WifiFunctionalityControl.STA_N_AP_Wi_Fi_Station_No_Wi_Fi_AP);
            map_CC_200.put(0x03, CC_200_WifiFunctionalityControl.N_STA_AP_No_Wi_Fi_Station_Wi_Fi_AP);
            map_CC_200.put(0x04, CC_200_WifiFunctionalityControl.STA_AP_Wi_Fi_Station_Wi_Fi_AP);
            getValueData.put(CC_200_WifiFunctionalityControl.class, map_CC_200);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_201 = new HashMap<>();
    public enum CC_201_Connectivity implements CarConfigEnumBase
    {
        No_Connectivity(0x01,"No_Connectivity"),
        Connectivity(0x02,"Connectivity");

        public final int value;
        public final String name;
        public static final int ParamNumber = 201;

        CC_201_Connectivity(int value, String name){
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
            map_CC_201.put(0x01, CC_201_Connectivity.No_Connectivity);
            map_CC_201.put(0x02, CC_201_Connectivity.Connectivity);
            getValueData.put(CC_201_Connectivity.class, map_CC_201);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_202 = new HashMap<>();
    public enum CC_202_OwnersManual implements CarConfigEnumBase
    {
        Without_digital_owners_manual(0x01,"Without_digital_owners_manual"),
        Digital_owners_manual(0x02,"Digital_owners_manual"),
        Canadian_French(0x03,"Canadian_French"),
        Chinese_simplified(0x04,"Chinese_simplified"),
        Chinese_traditional(0x05,"Chinese_traditional"),
        Czech(0x06,"Czech"),
        Danish(0x07,"Danish"),
        Dutch(0x08,"Dutch"),
        English_International(0x09,"English_International"),
        English_UK(0x0A,"English_UK"),
        Estonian(0x0B,"Estonian"),
        Finnish(0x0C,"Finnish"),
        French(0x0D,"French"),
        German(0x0E,"German"),
        Italian(0x0F,"Italian"),
        Japanese(0x10,"Japanese"),
        Latvian(0x11,"Latvian"),
        Lithuanian(0x12,"Lithuanian"),
        Norska(0x13,"Norska"),
        Polish(0x14,"Polish"),
        Portuguese(0x15,"Portuguese"),
        Russian(0x16,"Russian"),
        Spanish(0x17,"Spanish"),
        Swedish(0x18,"Swedish"),
        Thai(0x19,"Thai"),
        Turkish(0x1A,"Turkish"),
        Ukrainian(0x1B,"Ukrainian"),
        Us_English(0x1C,"Us_English"),
        Greek(0x1D,"Greek"),
        Hungarian(0x1E,"Hungarian");

        public final int value;
        public final String name;
        public static final int ParamNumber = 202;

        CC_202_OwnersManual(int value, String name){
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
            map_CC_202.put(0x01, CC_202_OwnersManual.Without_digital_owners_manual);
            map_CC_202.put(0x02, CC_202_OwnersManual.Digital_owners_manual);
            map_CC_202.put(0x03, CC_202_OwnersManual.Canadian_French);
            map_CC_202.put(0x04, CC_202_OwnersManual.Chinese_simplified);
            map_CC_202.put(0x05, CC_202_OwnersManual.Chinese_traditional);
            map_CC_202.put(0x06, CC_202_OwnersManual.Czech);
            map_CC_202.put(0x07, CC_202_OwnersManual.Danish);
            map_CC_202.put(0x08, CC_202_OwnersManual.Dutch);
            map_CC_202.put(0x09, CC_202_OwnersManual.English_International);
            map_CC_202.put(0x0A, CC_202_OwnersManual.English_UK);
            map_CC_202.put(0x0B, CC_202_OwnersManual.Estonian);
            map_CC_202.put(0x0C, CC_202_OwnersManual.Finnish);
            map_CC_202.put(0x0D, CC_202_OwnersManual.French);
            map_CC_202.put(0x0E, CC_202_OwnersManual.German);
            map_CC_202.put(0x0F, CC_202_OwnersManual.Italian);
            map_CC_202.put(0x10, CC_202_OwnersManual.Japanese);
            map_CC_202.put(0x11, CC_202_OwnersManual.Latvian);
            map_CC_202.put(0x12, CC_202_OwnersManual.Lithuanian);
            map_CC_202.put(0x13, CC_202_OwnersManual.Norska);
            map_CC_202.put(0x14, CC_202_OwnersManual.Polish);
            map_CC_202.put(0x15, CC_202_OwnersManual.Portuguese);
            map_CC_202.put(0x16, CC_202_OwnersManual.Russian);
            map_CC_202.put(0x17, CC_202_OwnersManual.Spanish);
            map_CC_202.put(0x18, CC_202_OwnersManual.Swedish);
            map_CC_202.put(0x19, CC_202_OwnersManual.Thai);
            map_CC_202.put(0x1A, CC_202_OwnersManual.Turkish);
            map_CC_202.put(0x1B, CC_202_OwnersManual.Ukrainian);
            map_CC_202.put(0x1C, CC_202_OwnersManual.Us_English);
            map_CC_202.put(0x1D, CC_202_OwnersManual.Greek);
            map_CC_202.put(0x1E, CC_202_OwnersManual.Hungarian);
            getValueData.put(CC_202_OwnersManual.class, map_CC_202);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_203 = new HashMap<>();
    public enum CC_203_Messaging implements CarConfigEnumBase
    {
        Messaging_disabled(0x01,"Messaging_disabled"),
        Messaging_enabled(0x02,"Messaging_enabled");

        public final int value;
        public final String name;
        public static final int ParamNumber = 203;

        CC_203_Messaging(int value, String name){
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
            map_CC_203.put(0x01, CC_203_Messaging.Messaging_disabled);
            map_CC_203.put(0x02, CC_203_Messaging.Messaging_enabled);
            getValueData.put(CC_203_Messaging.class, map_CC_203);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_204 = new HashMap<>();
    public enum CC_204_TvTuner implements CarConfigEnumBase
    {
        Without_TV_tuner(0x01,"Without_TV_tuner"),
        With_DVB_T_tuner_8Mhz(0x02,"With_DVB_T_tuner_8Mhz"),
        With_ISDB_T_tuner(0x03,"With_ISDB_T_tuner"),
        With_DTMB_T_tuner(0x04,"With_DTMB_T_tuner"),
        With_SBTVD_tuner(0x05,"With_SBTVD_tuner"),
        With_T_DMB_tuner(0x06,"With_T_DMB_tuner"),
        With_DVB_T_tuner_6Mhz(0x07,"With_DVB_T_tuner_6Mhz");

        public final int value;
        public final String name;
        public static final int ParamNumber = 204;

        CC_204_TvTuner(int value, String name){
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
            map_CC_204.put(0x01, CC_204_TvTuner.Without_TV_tuner);
            map_CC_204.put(0x02, CC_204_TvTuner.With_DVB_T_tuner_8Mhz);
            map_CC_204.put(0x03, CC_204_TvTuner.With_ISDB_T_tuner);
            map_CC_204.put(0x04, CC_204_TvTuner.With_DTMB_T_tuner);
            map_CC_204.put(0x05, CC_204_TvTuner.With_SBTVD_tuner);
            map_CC_204.put(0x06, CC_204_TvTuner.With_T_DMB_tuner);
            map_CC_204.put(0x07, CC_204_TvTuner.With_DVB_T_tuner_6Mhz);
            getValueData.put(CC_204_TvTuner.class, map_CC_204);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_205 = new HashMap<>();
    public enum CC_205_DsrcUnit implements CarConfigEnumBase
    {
        Without_DSRC_unit(0x01,"Without_DSRC_unit"),
        DSRC_unit(0x02,"DSRC_unit");

        public final int value;
        public final String name;
        public static final int ParamNumber = 205;

        CC_205_DsrcUnit(int value, String name){
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
            map_CC_205.put(0x01, CC_205_DsrcUnit.Without_DSRC_unit);
            map_CC_205.put(0x02, CC_205_DsrcUnit.DSRC_unit);
            getValueData.put(CC_205_DsrcUnit.class, map_CC_205);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_206 = new HashMap<>();
    public enum CC_206_DualMediaVicsAntenna implements CarConfigEnumBase
    {
        Without_VICS(0x01,"Without_VICS"),
        With_VICS(0x02,"With_VICS");

        public final int value;
        public final String name;
        public static final int ParamNumber = 206;

        CC_206_DualMediaVicsAntenna(int value, String name){
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
            map_CC_206.put(0x01, CC_206_DualMediaVicsAntenna.Without_VICS);
            map_CC_206.put(0x02, CC_206_DualMediaVicsAntenna.With_VICS);
            getValueData.put(CC_206_DualMediaVicsAntenna.class, map_CC_206);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_207 = new HashMap<>();
    public enum CC_207_VideoAllowed implements CarConfigEnumBase
    {
        Never(0x01,"Never"),
        Always_All_speeds_(0x02,"Always_All_speeds_"),
        Speed_dependent(0x03,"Speed_dependent");

        public final int value;
        public final String name;
        public static final int ParamNumber = 207;

        CC_207_VideoAllowed(int value, String name){
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
            map_CC_207.put(0x01, CC_207_VideoAllowed.Never);
            map_CC_207.put(0x02, CC_207_VideoAllowed.Always_All_speeds_);
            map_CC_207.put(0x03, CC_207_VideoAllowed.Speed_dependent);
            getValueData.put(CC_207_VideoAllowed.class, map_CC_207);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_209 = new HashMap<>();
    public enum CC_209_RoadAndTrafficInfoRti implements CarConfigEnumBase
    {
        Without_RTI_map(0x01,"Without_RTI_map"),
        Map_region_for_Europe(0x02,"Map_region_for_Europe"),
        Map_region_for_North_America(0x03,"Map_region_for_North_America"),
        Map_region_for_South_America(0x04,"Map_region_for_South_America"),
        Map_region_for_Africa_and_Middle_East(0x05,"Map_region_for_Africa_and_Middle_East"),
        Map_region_for_South_East_Asia(0x06,"Map_region_for_South_East_Asia"),
        Map_region_for_Pacific_Australia_New_Zeeland_(0x07,"Map_region_for_Pacific_Australia_New_Zeeland_"),
        Map_region_for_Russia(0x08,"Map_region_for_Russia"),
        Map_region_for_Korea(0x09,"Map_region_for_Korea"),
        Map_region_for_China(0x0A,"Map_region_for_China"),
        Map_region_for_Taiwan(0x0B,"Map_region_for_Taiwan"),
        Map_region_for_Japan(0x0C,"Map_region_for_Japan"),
        Map_region_for_India(0x0D,"Map_region_for_India"),
        Map_region_for_Israel(0x0E,"Map_region_for_Israel"),
        Map_region_for_Turkey(0x0F,"Map_region_for_Turkey");

        public final int value;
        public final String name;
        public static final int ParamNumber = 209;

        CC_209_RoadAndTrafficInfoRti(int value, String name){
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
            map_CC_209.put(0x01, CC_209_RoadAndTrafficInfoRti.Without_RTI_map);
            map_CC_209.put(0x02, CC_209_RoadAndTrafficInfoRti.Map_region_for_Europe);
            map_CC_209.put(0x03, CC_209_RoadAndTrafficInfoRti.Map_region_for_North_America);
            map_CC_209.put(0x04, CC_209_RoadAndTrafficInfoRti.Map_region_for_South_America);
            map_CC_209.put(0x05, CC_209_RoadAndTrafficInfoRti.Map_region_for_Africa_and_Middle_East);
            map_CC_209.put(0x06, CC_209_RoadAndTrafficInfoRti.Map_region_for_South_East_Asia);
            map_CC_209.put(0x07, CC_209_RoadAndTrafficInfoRti.Map_region_for_Pacific_Australia_New_Zeeland_);
            map_CC_209.put(0x08, CC_209_RoadAndTrafficInfoRti.Map_region_for_Russia);
            map_CC_209.put(0x09, CC_209_RoadAndTrafficInfoRti.Map_region_for_Korea);
            map_CC_209.put(0x0A, CC_209_RoadAndTrafficInfoRti.Map_region_for_China);
            map_CC_209.put(0x0B, CC_209_RoadAndTrafficInfoRti.Map_region_for_Taiwan);
            map_CC_209.put(0x0C, CC_209_RoadAndTrafficInfoRti.Map_region_for_Japan);
            map_CC_209.put(0x0D, CC_209_RoadAndTrafficInfoRti.Map_region_for_India);
            map_CC_209.put(0x0E, CC_209_RoadAndTrafficInfoRti.Map_region_for_Israel);
            map_CC_209.put(0x0F, CC_209_RoadAndTrafficInfoRti.Map_region_for_Turkey);
            getValueData.put(CC_209_RoadAndTrafficInfoRti.class, map_CC_209);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_210 = new HashMap<>();
    public enum CC_210_Subwoofer implements CarConfigEnumBase
    {
        Without_subwoofer(0x01,"Without_subwoofer"),
        Subwoofer(0x02,"Subwoofer");

        public final int value;
        public final String name;
        public static final int ParamNumber = 210;

        CC_210_Subwoofer(int value, String name){
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
            map_CC_210.put(0x01, CC_210_Subwoofer.Without_subwoofer);
            map_CC_210.put(0x02, CC_210_Subwoofer.Subwoofer);
            getValueData.put(CC_210_Subwoofer.class, map_CC_210);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_211 = new HashMap<>();
    public enum CC_211_TrailerModuleAftermarket implements CarConfigEnumBase
    {
        Without_TRailer_Module(0x01,"Without_TRailer_Module"),
        TRailer_Module_13_pos_socket_Fixed_towbar(0x02,"TRailer_Module_13_pos_socket_Fixed_towbar"),
        TRailer_Module_4_7_pos_socket_Fixed_towbar(0x03,"TRailer_Module_4_7_pos_socket_Fixed_towbar"),
        TRailer_Module_13_pos_socket_Semi_el_retractable_towbar(0x04,"TRailer_Module_13_pos_socket_Semi_el_retractable_towbar"),
        TRailer_Module_4_7_pos_socket_Semi_el_retractable_towbar(0x05,"TRailer_Module_4_7_pos_socket_Semi_el_retractable_towbar"),
        TRailer_Module_Preparation(0x06,"TRailer_Module_Preparation");

        public final int value;
        public final String name;
        public static final int ParamNumber = 211;

        CC_211_TrailerModuleAftermarket(int value, String name){
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
            map_CC_211.put(0x01, CC_211_TrailerModuleAftermarket.Without_TRailer_Module);
            map_CC_211.put(0x02, CC_211_TrailerModuleAftermarket.TRailer_Module_13_pos_socket_Fixed_towbar);
            map_CC_211.put(0x03, CC_211_TrailerModuleAftermarket.TRailer_Module_4_7_pos_socket_Fixed_towbar);
            map_CC_211.put(0x04, CC_211_TrailerModuleAftermarket.TRailer_Module_13_pos_socket_Semi_el_retractable_towbar);
            map_CC_211.put(0x05, CC_211_TrailerModuleAftermarket.TRailer_Module_4_7_pos_socket_Semi_el_retractable_towbar);
            map_CC_211.put(0x06, CC_211_TrailerModuleAftermarket.TRailer_Module_Preparation);
            getValueData.put(CC_211_TrailerModuleAftermarket.class, map_CC_211);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_212 = new HashMap<>();
    public enum CC_212_Alcolock implements CarConfigEnumBase
    {
        Without_Alcolock(0x01,"Without_Alcolock"),
        Alcolock_with_bypass(0x02,"Alcolock_with_bypass"),
        Alcolock_without_bypass(0x03,"Alcolock_without_bypass");

        public final int value;
        public final String name;
        public static final int ParamNumber = 212;

        CC_212_Alcolock(int value, String name){
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
            map_CC_212.put(0x01, CC_212_Alcolock.Without_Alcolock);
            map_CC_212.put(0x02, CC_212_Alcolock.Alcolock_with_bypass);
            map_CC_212.put(0x03, CC_212_Alcolock.Alcolock_without_bypass);
            getValueData.put(CC_212_Alcolock.class, map_CC_212);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_213 = new HashMap<>();
    public enum CC_213_RunningboardAccessories implements CarConfigEnumBase
    {
        Without_Runningboard(0x01,"Without_Runningboard"),
        Fixed_Runningboard(0x02,"Fixed_Runningboard"),
        Retractable_Runningboard(0x03,"Retractable_Runningboard"),
        Retractable_Runningboard_Special(0x04,"Retractable_Runningboard_Special");

        public final int value;
        public final String name;
        public static final int ParamNumber = 213;

        CC_213_RunningboardAccessories(int value, String name){
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
            map_CC_213.put(0x01, CC_213_RunningboardAccessories.Without_Runningboard);
            map_CC_213.put(0x02, CC_213_RunningboardAccessories.Fixed_Runningboard);
            map_CC_213.put(0x03, CC_213_RunningboardAccessories.Retractable_Runningboard);
            map_CC_213.put(0x04, CC_213_RunningboardAccessories.Retractable_Runningboard_Special);
            getValueData.put(CC_213_RunningboardAccessories.class, map_CC_213);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_214 = new HashMap<>();
    public enum CC_214_RestrictedKeyFunction implements CarConfigEnumBase
    {
        Without_restricted_key(0x01,"Without_restricted_key"),
        Restricted_key(0x02,"Restricted_key");

        public final int value;
        public final String name;
        public static final int ParamNumber = 214;

        CC_214_RestrictedKeyFunction(int value, String name){
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
            map_CC_214.put(0x01, CC_214_RestrictedKeyFunction.Without_restricted_key);
            map_CC_214.put(0x02, CC_214_RestrictedKeyFunction.Restricted_key);
            getValueData.put(CC_214_RestrictedKeyFunction.class, map_CC_214);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_216 = new HashMap<>();
    public enum CC_216_AccessoryGatewayUnit implements CarConfigEnumBase
    {
        Without_AGU(0x01,"Without_AGU"),
        With_AGU(0x02,"With_AGU"),
        With_Traffic_Incident_Recorder(0x03,"With_Traffic_Incident_Recorder"),
        With_CAM(0x04,"With_CAM"),
        With_TIR_CAM(0x05,"With_TIR_CAM"),
        With_TIR_FS(0x06,"With_TIR_FS"),
        With_VC_FS(0x07,"With_VC_FS"),
        With_TIR_VC_FS(0x08,"With_TIR_VC_FS");

        public final int value;
        public final String name;
        public static final int ParamNumber = 216;

        CC_216_AccessoryGatewayUnit(int value, String name){
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
            map_CC_216.put(0x01, CC_216_AccessoryGatewayUnit.Without_AGU);
            map_CC_216.put(0x02, CC_216_AccessoryGatewayUnit.With_AGU);
            map_CC_216.put(0x03, CC_216_AccessoryGatewayUnit.With_Traffic_Incident_Recorder);
            map_CC_216.put(0x04, CC_216_AccessoryGatewayUnit.With_CAM);
            map_CC_216.put(0x05, CC_216_AccessoryGatewayUnit.With_TIR_CAM);
            map_CC_216.put(0x06, CC_216_AccessoryGatewayUnit.With_TIR_FS);
            map_CC_216.put(0x07, CC_216_AccessoryGatewayUnit.With_VC_FS);
            map_CC_216.put(0x08, CC_216_AccessoryGatewayUnit.With_TIR_VC_FS);
            getValueData.put(CC_216_AccessoryGatewayUnit.class, map_CC_216);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_218 = new HashMap<>();
    public enum CC_218_MarketTelephoneNetwork implements CarConfigEnumBase
    {
        Telephone_system_GSM(0x01,"Telephone_system_GSM"),
        Telephone_system_CDMA2000(0x02,"Telephone_system_CDMA2000"),
        Telephone_system_TD_SCDMA(0x03,"Telephone_system_TD_SCDMA"),
        Telephone_system_OTHER_1_(0x04,"Telephone_system_OTHER_1_"),
        Telephone_system_OTHER_2_(0x05,"Telephone_system_OTHER_2_");

        public final int value;
        public final String name;
        public static final int ParamNumber = 218;

        CC_218_MarketTelephoneNetwork(int value, String name){
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
            map_CC_218.put(0x01, CC_218_MarketTelephoneNetwork.Telephone_system_GSM);
            map_CC_218.put(0x02, CC_218_MarketTelephoneNetwork.Telephone_system_CDMA2000);
            map_CC_218.put(0x03, CC_218_MarketTelephoneNetwork.Telephone_system_TD_SCDMA);
            map_CC_218.put(0x04, CC_218_MarketTelephoneNetwork.Telephone_system_OTHER_1_);
            map_CC_218.put(0x05, CC_218_MarketTelephoneNetwork.Telephone_system_OTHER_2_);
            getValueData.put(CC_218_MarketTelephoneNetwork.class, map_CC_218);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_219 = new HashMap<>();
    public enum CC_219_MicrophoneSetup implements CarConfigEnumBase
    {
        No_microphone_mounted_in_car(0x01,"No_microphone_mounted_in_car"),
        Two_digital_handsfree_microphones_are_available(0x02,"Two_digital_handsfree_microphones_are_available"),
        Four_digital_handsfree_microphones_are_available(0x03,"Four_digital_handsfree_microphones_are_available"),
        Three_handsfree_microphones(0x04,"Three_handsfree_microphones"),
        Four_digital_handsfree_microphones_and_two_ANC_microphones_are_available(0x05,"Four_digital_handsfree_microphones_and_two_ANC_microphones_are_available"),
        Three_handsfree_microphones_and_two_ANC_microphone_are_available(0x06,"Three_handsfree_microphones_and_two_ANC_microphone_are_available");

        public final int value;
        public final String name;
        public static final int ParamNumber = 219;

        CC_219_MicrophoneSetup(int value, String name){
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
            map_CC_219.put(0x01, CC_219_MicrophoneSetup.No_microphone_mounted_in_car);
            map_CC_219.put(0x02, CC_219_MicrophoneSetup.Two_digital_handsfree_microphones_are_available);
            map_CC_219.put(0x03, CC_219_MicrophoneSetup.Four_digital_handsfree_microphones_are_available);
            map_CC_219.put(0x04, CC_219_MicrophoneSetup.Three_handsfree_microphones);
            map_CC_219.put(0x05, CC_219_MicrophoneSetup.Four_digital_handsfree_microphones_and_two_ANC_microphones_are_available);
            map_CC_219.put(0x06, CC_219_MicrophoneSetup.Three_handsfree_microphones_and_two_ANC_microphone_are_available);
            getValueData.put(CC_219_MicrophoneSetup.class, map_CC_219);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_220 = new HashMap<>();
    public enum CC_220_LanguageRegion implements CarConfigEnumBase
    {
        Language_Region_for_World(0x01,"Language_Region_for_World"),
        Language_Region_for_Europe(0x02,"Language_Region_for_Europe"),
        Language_Region_for_North_America(0x03,"Language_Region_for_North_America"),
        Language_Region_for_South_America(0x04,"Language_Region_for_South_America"),
        Language_Region_for_Middle_East_Africa(0x05,"Language_Region_for_Middle_East_Africa"),
        Language_Region_for_South_East_Asia(0x06,"Language_Region_for_South_East_Asia"),
        Language_Region_for_Pacific(0x07,"Language_Region_for_Pacific"),
        Language_Region_for_Russia(0x08,"Language_Region_for_Russia"),
        Language_Region_for_Korea(0x09,"Language_Region_for_Korea"),
        Language_Region_for_China(0x0A,"Language_Region_for_China"),
        Language_Region_for_Taiwan(0x0B,"Language_Region_for_Taiwan"),
        Language_Region_for_Japan(0x0C,"Language_Region_for_Japan"),
        Language_Region_for_India(0x0D,"Language_Region_for_India"),
        Language_Region_for_Israel(0x0E,"Language_Region_for_Israel"),
        Language_Region_for_Turkey(0x0F,"Language_Region_for_Turkey");

        public final int value;
        public final String name;
        public static final int ParamNumber = 220;

        CC_220_LanguageRegion(int value, String name){
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
            map_CC_220.put(0x01, CC_220_LanguageRegion.Language_Region_for_World);
            map_CC_220.put(0x02, CC_220_LanguageRegion.Language_Region_for_Europe);
            map_CC_220.put(0x03, CC_220_LanguageRegion.Language_Region_for_North_America);
            map_CC_220.put(0x04, CC_220_LanguageRegion.Language_Region_for_South_America);
            map_CC_220.put(0x05, CC_220_LanguageRegion.Language_Region_for_Middle_East_Africa);
            map_CC_220.put(0x06, CC_220_LanguageRegion.Language_Region_for_South_East_Asia);
            map_CC_220.put(0x07, CC_220_LanguageRegion.Language_Region_for_Pacific);
            map_CC_220.put(0x08, CC_220_LanguageRegion.Language_Region_for_Russia);
            map_CC_220.put(0x09, CC_220_LanguageRegion.Language_Region_for_Korea);
            map_CC_220.put(0x0A, CC_220_LanguageRegion.Language_Region_for_China);
            map_CC_220.put(0x0B, CC_220_LanguageRegion.Language_Region_for_Taiwan);
            map_CC_220.put(0x0C, CC_220_LanguageRegion.Language_Region_for_Japan);
            map_CC_220.put(0x0D, CC_220_LanguageRegion.Language_Region_for_India);
            map_CC_220.put(0x0E, CC_220_LanguageRegion.Language_Region_for_Israel);
            map_CC_220.put(0x0F, CC_220_LanguageRegion.Language_Region_for_Turkey);
            getValueData.put(CC_220_LanguageRegion.class, map_CC_220);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_221 = new HashMap<>();
    public enum CC_221_ConnectedServiceBookingIfConnectivity implements CarConfigEnumBase
    {
        Without_Connected_Service_Booking(0x01,"Without_Connected_Service_Booking"),
        Connected_Service_Booking(0x02,"Connected_Service_Booking");

        public final int value;
        public final String name;
        public static final int ParamNumber = 221;

        CC_221_ConnectedServiceBookingIfConnectivity(int value, String name){
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
            map_CC_221.put(0x01, CC_221_ConnectedServiceBookingIfConnectivity.Without_Connected_Service_Booking);
            map_CC_221.put(0x02, CC_221_ConnectedServiceBookingIfConnectivity.Connected_Service_Booking);
            getValueData.put(CC_221_ConnectedServiceBookingIfConnectivity.class, map_CC_221);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_222 = new HashMap<>();
    public enum CC_222_Mediaplayer implements CarConfigEnumBase
    {
        Without_media_player(0x01,"Without_media_player"),
        Media_player(0x02,"Media_player"),
        Media_player_BD(0x03,"Media_player_BD");

        public final int value;
        public final String name;
        public static final int ParamNumber = 222;

        CC_222_Mediaplayer(int value, String name){
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
            map_CC_222.put(0x01, CC_222_Mediaplayer.Without_media_player);
            map_CC_222.put(0x02, CC_222_Mediaplayer.Media_player);
            map_CC_222.put(0x03, CC_222_Mediaplayer.Media_player_BD);
            getValueData.put(CC_222_Mediaplayer.class, map_CC_222);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_223 = new HashMap<>();
    public enum CC_223_NavigationSystemAndInternetMap implements CarConfigEnumBase
    {
        Without_Connected_Navigation_enabled_without_Internet_map_support(0x01,"Without_Connected_Navigation_enabled_without_Internet_map_support"),
        Without_Connected_Navigation_enabled_with_Internet_map_support(0x02,"Without_Connected_Navigation_enabled_with_Internet_map_support"),
        With_Connected_Navigation_enabled_without_Internet_map_support(0x03,"With_Connected_Navigation_enabled_without_Internet_map_support"),
        With_Connected_Navigation_enabled_with_Internet_map_support(0x04,"With_Connected_Navigation_enabled_with_Internet_map_support");

        public final int value;
        public final String name;
        public static final int ParamNumber = 223;

        CC_223_NavigationSystemAndInternetMap(int value, String name){
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
            map_CC_223.put(0x01, CC_223_NavigationSystemAndInternetMap.Without_Connected_Navigation_enabled_without_Internet_map_support);
            map_CC_223.put(0x02, CC_223_NavigationSystemAndInternetMap.Without_Connected_Navigation_enabled_with_Internet_map_support);
            map_CC_223.put(0x03, CC_223_NavigationSystemAndInternetMap.With_Connected_Navigation_enabled_without_Internet_map_support);
            map_CC_223.put(0x04, CC_223_NavigationSystemAndInternetMap.With_Connected_Navigation_enabled_with_Internet_map_support);
            getValueData.put(CC_223_NavigationSystemAndInternetMap.class, map_CC_223);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_224 = new HashMap<>();
    public enum CC_224_DeviceMirroring implements CarConfigEnumBase
    {
        Without_device_mirroring(0x01,"Without_device_mirroring"),
        With_device_mirroring_ACP(0x02,"With_device_mirroring_ACP"),
        With_device_mirroring_AA(0x03,"With_device_mirroring_AA"),
        With_device_mirroring_ACP_AA(0x04,"With_device_mirroring_ACP_AA"),
        With_device_mirroring_BCL(0x05,"With_device_mirroring_BCL"),
        With_device_mirroring_ACP_BCL(0x06,"With_device_mirroring_ACP_BCL"),
        With_device_mirroring_AA_BCL(0x07,"With_device_mirroring_AA_BCL"),
        With_device_mirroring_APC_AA_BCL(0x08,"With_device_mirroring_APC_AA_BCL");

        public final int value;
        public final String name;
        public static final int ParamNumber = 224;

        CC_224_DeviceMirroring(int value, String name){
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
            map_CC_224.put(0x01, CC_224_DeviceMirroring.Without_device_mirroring);
            map_CC_224.put(0x02, CC_224_DeviceMirroring.With_device_mirroring_ACP);
            map_CC_224.put(0x03, CC_224_DeviceMirroring.With_device_mirroring_AA);
            map_CC_224.put(0x04, CC_224_DeviceMirroring.With_device_mirroring_ACP_AA);
            map_CC_224.put(0x05, CC_224_DeviceMirroring.With_device_mirroring_BCL);
            map_CC_224.put(0x06, CC_224_DeviceMirroring.With_device_mirroring_ACP_BCL);
            map_CC_224.put(0x07, CC_224_DeviceMirroring.With_device_mirroring_AA_BCL);
            map_CC_224.put(0x08, CC_224_DeviceMirroring.With_device_mirroring_APC_AA_BCL);
            getValueData.put(CC_224_DeviceMirroring.class, map_CC_224);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_239 = new HashMap<>();
    public enum CC_239_MassageFrontSeat implements CarConfigEnumBase
    {
        Without_Massage_front_seat(0x01,"Without_Massage_front_seat"),
        Massage_front_seat(0x02,"Massage_front_seat");

        public final int value;
        public final String name;
        public static final int ParamNumber = 239;

        CC_239_MassageFrontSeat(int value, String name){
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
            map_CC_239.put(0x01, CC_239_MassageFrontSeat.Without_Massage_front_seat);
            map_CC_239.put(0x02, CC_239_MassageFrontSeat.Massage_front_seat);
            getValueData.put(CC_239_MassageFrontSeat.class, map_CC_239);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_240 = new HashMap<>();
    public enum CC_240_LumbarSupport implements CarConfigEnumBase
    {
        Fixed_lumbar_support(0x01,"Fixed_lumbar_support"),
        Four_way_lumbar_support(0x02,"Four_way_lumbar_support"),
        Two_way_lumbar_support(0x03,"Two_way_lumbar_support");

        public final int value;
        public final String name;
        public static final int ParamNumber = 240;

        CC_240_LumbarSupport(int value, String name){
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
            map_CC_240.put(0x01, CC_240_LumbarSupport.Fixed_lumbar_support);
            map_CC_240.put(0x02, CC_240_LumbarSupport.Four_way_lumbar_support);
            map_CC_240.put(0x03, CC_240_LumbarSupport.Two_way_lumbar_support);
            getValueData.put(CC_240_LumbarSupport.class, map_CC_240);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_241 = new HashMap<>();
    public enum CC_241_CushionExtension implements CarConfigEnumBase
    {
        Without_Cushion_Extension(0x01,"Without_Cushion_Extension"),
        With_Cushion_Extension_Driver(0x02,"With_Cushion_Extension_Driver"),
        With_Cushion_Extension_Driver_Passenger(0x03,"With_Cushion_Extension_Driver_Passenger");

        public final int value;
        public final String name;
        public static final int ParamNumber = 241;

        CC_241_CushionExtension(int value, String name){
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
            map_CC_241.put(0x01, CC_241_CushionExtension.Without_Cushion_Extension);
            map_CC_241.put(0x02, CC_241_CushionExtension.With_Cushion_Extension_Driver);
            map_CC_241.put(0x03, CC_241_CushionExtension.With_Cushion_Extension_Driver_Passenger);
            getValueData.put(CC_241_CushionExtension.class, map_CC_241);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_242 = new HashMap<>();
    public enum CC_242_HeadrestsFront implements CarConfigEnumBase
    {
        Two_way_headsrests_Front_manual(0x01,"Two_way_headsrests_Front_manual"),
        Two_way_headsrests_Front_Power(0x02,"Two_way_headsrests_Front_Power"),
        Headsrests_Front_Fixed(0x03,"Headsrests_Front_Fixed");

        public final int value;
        public final String name;
        public static final int ParamNumber = 242;

        CC_242_HeadrestsFront(int value, String name){
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
            map_CC_242.put(0x01, CC_242_HeadrestsFront.Two_way_headsrests_Front_manual);
            map_CC_242.put(0x02, CC_242_HeadrestsFront.Two_way_headsrests_Front_Power);
            map_CC_242.put(0x03, CC_242_HeadrestsFront.Headsrests_Front_Fixed);
            getValueData.put(CC_242_HeadrestsFront.class, map_CC_242);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_243 = new HashMap<>();
    public enum CC_243_PowerAdjustableSideSupport implements CarConfigEnumBase
    {
        Without_power_adjustable_side_support(0x01,"Without_power_adjustable_side_support"),
        Power_adjustable_side_support(0x02,"Power_adjustable_side_support");

        public final int value;
        public final String name;
        public static final int ParamNumber = 243;

        CC_243_PowerAdjustableSideSupport(int value, String name){
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
            map_CC_243.put(0x01, CC_243_PowerAdjustableSideSupport.Without_power_adjustable_side_support);
            map_CC_243.put(0x02, CC_243_PowerAdjustableSideSupport.Power_adjustable_side_support);
            getValueData.put(CC_243_PowerAdjustableSideSupport.class, map_CC_243);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_244 = new HashMap<>();
    public enum CC_244_RemoteControlOfPassengerSeatFunctions implements CarConfigEnumBase
    {
        Without_Remote_control_of_Passengers_Seat_Functions(0x01,"Without_Remote_control_of_Passengers_Seat_Functions"),
        Remote_control_of_Passengers_Seat_Functions(0x02,"Remote_control_of_Passengers_Seat_Functions");

        public final int value;
        public final String name;
        public static final int ParamNumber = 244;

        CC_244_RemoteControlOfPassengerSeatFunctions(int value, String name){
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
            map_CC_244.put(0x01, CC_244_RemoteControlOfPassengerSeatFunctions.Without_Remote_control_of_Passengers_Seat_Functions);
            map_CC_244.put(0x02, CC_244_RemoteControlOfPassengerSeatFunctions.Remote_control_of_Passengers_Seat_Functions);
            getValueData.put(CC_244_RemoteControlOfPassengerSeatFunctions.class, map_CC_244);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_245 = new HashMap<>();
    public enum CC_245_EasyIngressEgress implements CarConfigEnumBase
    {
        Without_Easy_Ingress_Egress(0x01,"Without_Easy_Ingress_Egress"),
        With_Easy_Ingress_Egress_Down(0x02,"With_Easy_Ingress_Egress_Down"),
        With_Easy_Ingress_Egress_Retracting(0x03,"With_Easy_Ingress_Egress_Retracting");

        public final int value;
        public final String name;
        public static final int ParamNumber = 245;

        CC_245_EasyIngressEgress(int value, String name){
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
            map_CC_245.put(0x01, CC_245_EasyIngressEgress.Without_Easy_Ingress_Egress);
            map_CC_245.put(0x02, CC_245_EasyIngressEgress.With_Easy_Ingress_Egress_Down);
            map_CC_245.put(0x03, CC_245_EasyIngressEgress.With_Easy_Ingress_Egress_Retracting);
            getValueData.put(CC_245_EasyIngressEgress.class, map_CC_245);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_248 = new HashMap<>();
    public enum CC_248_IaqsSmokeSensor implements CarConfigEnumBase
    {
        Without_Smoke_Sensor(0x01,"Without_Smoke_Sensor"),
        With_Smoke_Sensor(0x02,"With_Smoke_Sensor");

        public final int value;
        public final String name;
        public static final int ParamNumber = 248;

        CC_248_IaqsSmokeSensor(int value, String name){
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
            map_CC_248.put(0x01, CC_248_IaqsSmokeSensor.Without_Smoke_Sensor);
            map_CC_248.put(0x02, CC_248_IaqsSmokeSensor.With_Smoke_Sensor);
            getValueData.put(CC_248_IaqsSmokeSensor.class, map_CC_248);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_256 = new HashMap<>();
    public enum CC_256_BendingLight implements CarConfigEnumBase
    {
        Without_Bending_Light(0x01,"Without_Bending_Light"),
        With_Bending_Light(0x02,"With_Bending_Light"),
        With_Bending_Light_in_High_and_Low_Beam(0x03,"With_Bending_Light_in_High_and_Low_Beam");

        public final int value;
        public final String name;
        public static final int ParamNumber = 256;

        CC_256_BendingLight(int value, String name){
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
            map_CC_256.put(0x01, CC_256_BendingLight.Without_Bending_Light);
            map_CC_256.put(0x02, CC_256_BendingLight.With_Bending_Light);
            map_CC_256.put(0x03, CC_256_BendingLight.With_Bending_Light_in_High_and_Low_Beam);
            getValueData.put(CC_256_BendingLight.class, map_CC_256);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_267 = new HashMap<>();
    public enum CC_267_IndirectTpmsSystem implements CarConfigEnumBase
    {
        Without_Indirect_Tyre_Pressure_Monitoring_System(0x01,"Without_Indirect_Tyre_Pressure_Monitoring_System"),
        Indirect_TPMS_Basic_Algorithm(0x02,"Indirect_TPMS_Basic_Algorithm"),
        Indirect_TPMS_US_Algorithm(0x03,"Indirect_TPMS_US_Algorithm"),
        Indirect_TPMS_EU_Algorithm(0x04,"Indirect_TPMS_EU_Algorithm");

        public final int value;
        public final String name;
        public static final int ParamNumber = 267;

        CC_267_IndirectTpmsSystem(int value, String name){
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
            map_CC_267.put(0x01, CC_267_IndirectTpmsSystem.Without_Indirect_Tyre_Pressure_Monitoring_System);
            map_CC_267.put(0x02, CC_267_IndirectTpmsSystem.Indirect_TPMS_Basic_Algorithm);
            map_CC_267.put(0x03, CC_267_IndirectTpmsSystem.Indirect_TPMS_US_Algorithm);
            map_CC_267.put(0x04, CC_267_IndirectTpmsSystem.Indirect_TPMS_EU_Algorithm);
            getValueData.put(CC_267_IndirectTpmsSystem.class, map_CC_267);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_305 = new HashMap<>();
    public enum CC_305_ParkingAssistanceSensorGeneration implements CarConfigEnumBase
    {
        GEN5(0x01,"GEN5"),
        GEN6(0x02,"GEN6");

        public final int value;
        public final String name;
        public static final int ParamNumber = 305;

        CC_305_ParkingAssistanceSensorGeneration(int value, String name){
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
            map_CC_305.put(0x01, CC_305_ParkingAssistanceSensorGeneration.GEN5);
            map_CC_305.put(0x02, CC_305_ParkingAssistanceSensorGeneration.GEN6);
            getValueData.put(CC_305_ParkingAssistanceSensorGeneration.class, map_CC_305);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_316 = new HashMap<>();
    public enum CC_316_EmergencyLaneKeepingAid implements CarConfigEnumBase
    {
        Without_Emergency_Lane_Keeping_Aid_eLKA_(0x01,"Without_Emergency_Lane_Keeping_Aid_eLKA_"),
        eLKA_level_1(0x02,"eLKA_level_1"),
        eLKA_level_2(0x03,"eLKA_level_2"),
        eLKA_level_3(0x04,"eLKA_level_3");

        public final int value;
        public final String name;
        public static final int ParamNumber = 316;

        CC_316_EmergencyLaneKeepingAid(int value, String name){
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
            map_CC_316.put(0x01, CC_316_EmergencyLaneKeepingAid.Without_Emergency_Lane_Keeping_Aid_eLKA_);
            map_CC_316.put(0x02, CC_316_EmergencyLaneKeepingAid.eLKA_level_1);
            map_CC_316.put(0x03, CC_316_EmergencyLaneKeepingAid.eLKA_level_2);
            map_CC_316.put(0x04, CC_316_EmergencyLaneKeepingAid.eLKA_level_3);
            getValueData.put(CC_316_EmergencyLaneKeepingAid.class, map_CC_316);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_337 = new HashMap<>();
    public enum CC_337_CenterConsoleSwitchModuleccsm implements CarConfigEnumBase
    {
        Without_LIN_communication(0x01,"Without_LIN_communication"),
        With_Dummy_button(0x02,"With_Dummy_button"),
        With_Drive_Mode_button(0x03,"With_Drive_Mode_button");

        public final int value;
        public final String name;
        public static final int ParamNumber = 337;

        CC_337_CenterConsoleSwitchModuleccsm(int value, String name){
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
            map_CC_337.put(0x01, CC_337_CenterConsoleSwitchModuleccsm.Without_LIN_communication);
            map_CC_337.put(0x02, CC_337_CenterConsoleSwitchModuleccsm.With_Dummy_button);
            map_CC_337.put(0x03, CC_337_CenterConsoleSwitchModuleccsm.With_Drive_Mode_button);
            getValueData.put(CC_337_CenterConsoleSwitchModuleccsm.class, map_CC_337);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_338 = new HashMap<>();
    public enum CC_338_Pre_clean implements CarConfigEnumBase
    {
        without_Pre_Clean(0x01,"without_Pre_Clean"),
        with_Pre_Clean(0x02,"with_Pre_Clean");

        public final int value;
        public final String name;
        public static final int ParamNumber = 338;

        CC_338_Pre_clean(int value, String name){
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
            map_CC_338.put(0x01, CC_338_Pre_clean.without_Pre_Clean);
            map_CC_338.put(0x02, CC_338_Pre_clean.with_Pre_Clean);
            getValueData.put(CC_338_Pre_clean.class, map_CC_338);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_346 = new HashMap<>();
    public enum CC_346_SunCurtainRearWindow implements CarConfigEnumBase
    {
        Without_Electric_Rear_Sun_Curtain(0x01,"Without_Electric_Rear_Sun_Curtain"),
        With_Electric_Rear_Sun_Curtain(0x02,"With_Electric_Rear_Sun_Curtain");

        public final int value;
        public final String name;
        public static final int ParamNumber = 346;

        CC_346_SunCurtainRearWindow(int value, String name){
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
            map_CC_346.put(0x01, CC_346_SunCurtainRearWindow.Without_Electric_Rear_Sun_Curtain);
            map_CC_346.put(0x02, CC_346_SunCurtainRearWindow.With_Electric_Rear_Sun_Curtain);
            getValueData.put(CC_346_SunCurtainRearWindow.class, map_CC_346);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_347 = new HashMap<>();
    public enum CC_347_HeaterType implements CarConfigEnumBase
    {
        Without_heater(0x01,"Without_heater"),
        FOH_Fuel_Operated_Heater(0x02,"FOH_Fuel_Operated_Heater"),
        HVCH_High_Voltage_Coolant_heater(0x03,"HVCH_High_Voltage_Coolant_heater"),
        EPH_Electrical_Parking_Heater_accessory(0x04,"EPH_Electrical_Parking_Heater_accessory");

        public final int value;
        public final String name;
        public static final int ParamNumber = 347;

        CC_347_HeaterType(int value, String name){
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
            map_CC_347.put(0x01, CC_347_HeaterType.Without_heater);
            map_CC_347.put(0x02, CC_347_HeaterType.FOH_Fuel_Operated_Heater);
            map_CC_347.put(0x03, CC_347_HeaterType.HVCH_High_Voltage_Coolant_heater);
            map_CC_347.put(0x04, CC_347_HeaterType.EPH_Electrical_Parking_Heater_accessory);
            getValueData.put(CC_347_HeaterType.class, map_CC_347);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_349 = new HashMap<>();
    public enum CC_349_IonicInternalAirCleaner implements CarConfigEnumBase
    {
        Without_Ionic_internal_air_cleaner(0x01,"Without_Ionic_internal_air_cleaner"),
        With_Ionic_internal_air_cleaner(0x02,"With_Ionic_internal_air_cleaner");

        public final int value;
        public final String name;
        public static final int ParamNumber = 349;

        CC_349_IonicInternalAirCleaner(int value, String name){
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
            map_CC_349.put(0x01, CC_349_IonicInternalAirCleaner.Without_Ionic_internal_air_cleaner);
            map_CC_349.put(0x02, CC_349_IonicInternalAirCleaner.With_Ionic_internal_air_cleaner);
            getValueData.put(CC_349_IonicInternalAirCleaner.class, map_CC_349);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_355 = new HashMap<>();
    public enum CC_355_PtcInFrontHvac implements CarConfigEnumBase
    {
        Without_PTC(0x01,"Without_PTC"),
        With_PTC_1000W(0x02,"With_PTC_1000W");

        public final int value;
        public final String name;
        public static final int ParamNumber = 355;

        CC_355_PtcInFrontHvac(int value, String name){
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
            map_CC_355.put(0x01, CC_355_PtcInFrontHvac.Without_PTC);
            map_CC_355.put(0x02, CC_355_PtcInFrontHvac.With_PTC_1000W);
            getValueData.put(CC_355_PtcInFrontHvac.class, map_CC_355);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_358 = new HashMap<>();
    public enum CC_358_ActiveNoiseControl implements CarConfigEnumBase
    {
        Without_Active_Noise_Control(0x01,"Without_Active_Noise_Control"),
        Active_Noise_Control(0x02,"Active_Noise_Control"),
        Active_Noise_Control_EOE_Engine_Order_Enhancement_(0x03,"Active_Noise_Control_EOE_Engine_Order_Enhancement_"),
        Active_Noise_Control_EOR_Engine_Order_Reduction_(0x04,"Active_Noise_Control_EOR_Engine_Order_Reduction_");

        public final int value;
        public final String name;
        public static final int ParamNumber = 358;

        CC_358_ActiveNoiseControl(int value, String name){
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
            map_CC_358.put(0x01, CC_358_ActiveNoiseControl.Without_Active_Noise_Control);
            map_CC_358.put(0x02, CC_358_ActiveNoiseControl.Active_Noise_Control);
            map_CC_358.put(0x03, CC_358_ActiveNoiseControl.Active_Noise_Control_EOE_Engine_Order_Enhancement_);
            map_CC_358.put(0x04, CC_358_ActiveNoiseControl.Active_Noise_Control_EOR_Engine_Order_Reduction_);
            getValueData.put(CC_358_ActiveNoiseControl.class, map_CC_358);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_359 = new HashMap<>();
    public enum CC_359_ExteriorSoundHmiOption implements CarConfigEnumBase
    {
        Exterior_Sound_HMI_Not_Available(0x01,"Exterior_Sound_HMI_Not_Available"),
        Exterior_Sound_HMI_Available(0x02,"Exterior_Sound_HMI_Available");

        public final int value;
        public final String name;
        public static final int ParamNumber = 359;

        CC_359_ExteriorSoundHmiOption(int value, String name){
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
            map_CC_359.put(0x01, CC_359_ExteriorSoundHmiOption.Exterior_Sound_HMI_Not_Available);
            map_CC_359.put(0x02, CC_359_ExteriorSoundHmiOption.Exterior_Sound_HMI_Available);
            getValueData.put(CC_359_ExteriorSoundHmiOption.class, map_CC_359);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_360 = new HashMap<>();
    public enum CC_360_FrequencyBandStepAmfmHdRadio implements CarConfigEnumBase
    {
        Frequency_band_and_step_for_EU(0x01,"Frequency_band_and_step_for_EU"),
        Frequency_band_and_step_for_EU_without_RDS(0x02,"Frequency_band_and_step_for_EU_without_RDS"),
        Frequency_band_and_step_for_USA(0x03,"Frequency_band_and_step_for_USA"),
        Frequency_band_and_step_for_Japan(0x04,"Frequency_band_and_step_for_Japan"),
        Frequency_band_and_step_for_China(0x05,"Frequency_band_and_step_for_China"),
        Frequency_band_and_step_for_Australia(0x06,"Frequency_band_and_step_for_Australia"),
        Frequency_band_and_step_for_Gulf(0x07,"Frequency_band_and_step_for_Gulf"),
        Frequency_band_and_step_for_Thailand(0x08,"Frequency_band_and_step_for_Thailand");

        public final int value;
        public final String name;
        public static final int ParamNumber = 360;

        CC_360_FrequencyBandStepAmfmHdRadio(int value, String name){
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
            map_CC_360.put(0x01, CC_360_FrequencyBandStepAmfmHdRadio.Frequency_band_and_step_for_EU);
            map_CC_360.put(0x02, CC_360_FrequencyBandStepAmfmHdRadio.Frequency_band_and_step_for_EU_without_RDS);
            map_CC_360.put(0x03, CC_360_FrequencyBandStepAmfmHdRadio.Frequency_band_and_step_for_USA);
            map_CC_360.put(0x04, CC_360_FrequencyBandStepAmfmHdRadio.Frequency_band_and_step_for_Japan);
            map_CC_360.put(0x05, CC_360_FrequencyBandStepAmfmHdRadio.Frequency_band_and_step_for_China);
            map_CC_360.put(0x06, CC_360_FrequencyBandStepAmfmHdRadio.Frequency_band_and_step_for_Australia);
            map_CC_360.put(0x07, CC_360_FrequencyBandStepAmfmHdRadio.Frequency_band_and_step_for_Gulf);
            map_CC_360.put(0x08, CC_360_FrequencyBandStepAmfmHdRadio.Frequency_band_and_step_for_Thailand);
            getValueData.put(CC_360_FrequencyBandStepAmfmHdRadio.class, map_CC_360);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_361 = new HashMap<>();
    public enum CC_361_SimCardForConnectivity implements CarConfigEnumBase
    {
        Without_SIM(0x01,"Without_SIM"),
        only_through_Personal_SIM(0x02,"only_through_Personal_SIM"),
        only_through_Telematic_SIM(0x03,"only_through_Telematic_SIM"),
        T_SIM_and_P_SIM_Accessory_(0x04,"T_SIM_and_P_SIM_Accessory_"),
        Volvo_SIM_Accessory_(0x05,"Volvo_SIM_Accessory_");

        public final int value;
        public final String name;
        public static final int ParamNumber = 361;

        CC_361_SimCardForConnectivity(int value, String name){
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
            map_CC_361.put(0x01, CC_361_SimCardForConnectivity.Without_SIM);
            map_CC_361.put(0x02, CC_361_SimCardForConnectivity.only_through_Personal_SIM);
            map_CC_361.put(0x03, CC_361_SimCardForConnectivity.only_through_Telematic_SIM);
            map_CC_361.put(0x04, CC_361_SimCardForConnectivity.T_SIM_and_P_SIM_Accessory_);
            map_CC_361.put(0x05, CC_361_SimCardForConnectivity.Volvo_SIM_Accessory_);
            getValueData.put(CC_361_SimCardForConnectivity.class, map_CC_361);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_362 = new HashMap<>();
    public enum CC_362_UsbConnection implements CarConfigEnumBase
    {
        Without_USB_connection(0x01,"Without_USB_connection"),
        With_1_USB_connecion(0x02,"With_1_USB_connecion"),
        With_USB_hub(0x03,"With_USB_hub"),
        With_1_USB_connecion_type_C(0x04,"With_1_USB_connecion_type_C"),
        With_USB_hub_Type_C(0x05,"With_USB_hub_Type_C"),
        With_2_USB_connections(0x80,"With_2_USB_connections");

        public final int value;
        public final String name;
        public static final int ParamNumber = 362;

        CC_362_UsbConnection(int value, String name){
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
            map_CC_362.put(0x01, CC_362_UsbConnection.Without_USB_connection);
            map_CC_362.put(0x02, CC_362_UsbConnection.With_1_USB_connecion);
            map_CC_362.put(0x03, CC_362_UsbConnection.With_USB_hub);
            map_CC_362.put(0x04, CC_362_UsbConnection.With_1_USB_connecion_type_C);
            map_CC_362.put(0x05, CC_362_UsbConnection.With_USB_hub_Type_C);
            map_CC_362.put(0x80, CC_362_UsbConnection.With_2_USB_connections);
            getValueData.put(CC_362_UsbConnection.class, map_CC_362);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_363 = new HashMap<>();
    public enum CC_363_AudioAuxiliaryInput implements CarConfigEnumBase
    {
        Without_Auxiliary_Audio_Input(0x01,"Without_Auxiliary_Audio_Input"),
        With_Auxiliary_Audio_Input(0x02,"With_Auxiliary_Audio_Input");

        public final int value;
        public final String name;
        public static final int ParamNumber = 363;

        CC_363_AudioAuxiliaryInput(int value, String name){
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
            map_CC_363.put(0x01, CC_363_AudioAuxiliaryInput.Without_Auxiliary_Audio_Input);
            map_CC_363.put(0x02, CC_363_AudioAuxiliaryInput.With_Auxiliary_Audio_Input);
            getValueData.put(CC_363_AudioAuxiliaryInput.class, map_CC_363);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_365 = new HashMap<>();
    public enum CC_365_WifiWorkshopModeFunctionalControl implements CarConfigEnumBase
    {
        WLAN_Workshop_Mode_Disabled(0x01,"WLAN_Workshop_Mode_Disabled"),
        WLAN_Workshop_Mode_Enabled(0x02,"WLAN_Workshop_Mode_Enabled");

        public final int value;
        public final String name;
        public static final int ParamNumber = 365;

        CC_365_WifiWorkshopModeFunctionalControl(int value, String name){
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
            map_CC_365.put(0x01, CC_365_WifiWorkshopModeFunctionalControl.WLAN_Workshop_Mode_Disabled);
            map_CC_365.put(0x02, CC_365_WifiWorkshopModeFunctionalControl.WLAN_Workshop_Mode_Enabled);
            getValueData.put(CC_365_WifiWorkshopModeFunctionalControl.class, map_CC_365);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_366 = new HashMap<>();
    public enum CC_366_WirelessPhoneCharger implements CarConfigEnumBase
    {
        No_inductive_charging_for_smartphone(0x01,"No_inductive_charging_for_smartphone"),
        Inductive_charging_for_smartphone(0x02,"Inductive_charging_for_smartphone"),
        Inductive_charging_for_smartphone_NFC(0x03,"Inductive_charging_for_smartphone_NFC");

        public final int value;
        public final String name;
        public static final int ParamNumber = 366;

        CC_366_WirelessPhoneCharger(int value, String name){
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
            map_CC_366.put(0x01, CC_366_WirelessPhoneCharger.No_inductive_charging_for_smartphone);
            map_CC_366.put(0x02, CC_366_WirelessPhoneCharger.Inductive_charging_for_smartphone);
            map_CC_366.put(0x03, CC_366_WirelessPhoneCharger.Inductive_charging_for_smartphone_NFC);
            getValueData.put(CC_366_WirelessPhoneCharger.class, map_CC_366);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_368 = new HashMap<>();
    public enum CC_368_EngineFamilyType implements CarConfigEnumBase
    {
        No_ANC(0x01,"No_ANC"),
        VEP_HP(0x02,"VEP_HP"),
        VEP_MP(0x03,"VEP_MP"),
        VEP_HP_PHEV_Hybrid_T8_(0x04,"VEP_HP_PHEV_Hybrid_T8_"),
        VED_HP(0x05,"VED_HP"),
        VED_MP(0x06,"VED_MP"),
        GEP3_HP(0x07,"GEP3_HP"),
        GEP3_MP(0x08,"GEP3_MP"),
        GEP3_HEP7_Hybrid_(0x09,"GEP3_HEP7_Hybrid_"),
        BEV(0x10,"BEV");

        public final int value;
        public final String name;
        public static final int ParamNumber = 368;

        CC_368_EngineFamilyType(int value, String name){
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
            map_CC_368.put(0x01, CC_368_EngineFamilyType.No_ANC);
            map_CC_368.put(0x02, CC_368_EngineFamilyType.VEP_HP);
            map_CC_368.put(0x03, CC_368_EngineFamilyType.VEP_MP);
            map_CC_368.put(0x04, CC_368_EngineFamilyType.VEP_HP_PHEV_Hybrid_T8_);
            map_CC_368.put(0x05, CC_368_EngineFamilyType.VED_HP);
            map_CC_368.put(0x06, CC_368_EngineFamilyType.VED_MP);
            map_CC_368.put(0x07, CC_368_EngineFamilyType.GEP3_HP);
            map_CC_368.put(0x08, CC_368_EngineFamilyType.GEP3_MP);
            map_CC_368.put(0x09, CC_368_EngineFamilyType.GEP3_HEP7_Hybrid_);
            map_CC_368.put(0x10, CC_368_EngineFamilyType.BEV);
            getValueData.put(CC_368_EngineFamilyType.class, map_CC_368);
    }

    private static HashMap<Integer, CarConfigEnumBase> map_CC_401 = new HashMap<>();
    public enum CC_401_RainSensorFunction implements CarConfigEnumBase
    {
        No_Rain_Sensor_Function(0x01,"No_Rain_Sensor_Function"),
        Rain_Sensor_Function_Activated_By_Button(0x02,"Rain_Sensor_Function_Activated_By_Button"),
        Rain_Sensor_Function_Activated_By_Interval_Position(0x03,"Rain_Sensor_Function_Activated_By_Interval_Position");

        public final int value;
        public final String name;
        public static final int ParamNumber = 401;

        CC_401_RainSensorFunction(int value, String name){
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
            map_CC_401.put(0x01, CC_401_RainSensorFunction.No_Rain_Sensor_Function);
            map_CC_401.put(0x02, CC_401_RainSensorFunction.Rain_Sensor_Function_Activated_By_Button);
            map_CC_401.put(0x03, CC_401_RainSensorFunction.Rain_Sensor_Function_Activated_By_Interval_Position);
            getValueData.put(CC_401_RainSensorFunction.class, map_CC_401);
    }

}