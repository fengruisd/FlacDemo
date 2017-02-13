package com.fr.flac;

/**
 * Created by fengruifr on 17/2/9.
 *
 */
public class FrFlac {

    static {
        System.loadLibrary("flac");
    }


    public native void initEncoder(String file);

    public native void encode(short[] data, int size);

    public native void finishEncode();

    public native void initDecoder(String file);
}
