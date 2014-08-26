package net.kuznetsovs.tc;

public class TCC {

    static {
        System.loadLibrary("zslib");
        System.loadLibrary("tcc");
    }

    public static native void test();

}
