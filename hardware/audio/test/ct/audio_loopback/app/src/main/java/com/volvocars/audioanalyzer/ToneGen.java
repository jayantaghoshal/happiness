package com.volvocars.audioanalyzer;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

public class ToneGen {
    private final int duration = 2; // seconds
    private final int sampleRate = 8000;
    private final int numSamples = duration * sampleRate;
    private final double samples[] = new double[numSamples];
    private final byte generatedSnd[] = new byte[2 * numSamples];
    Handler handler = new Handler(Looper.getMainLooper());
    private double freqOfTone; // hz

    public ToneGen(int frequency) {
        freqOfTone = frequency;
        Log.e("generating tone", String.valueOf(frequency));
    }

    void genTone() {
        // fill out the array
        for (int i = 0; i < numSamples; ++i) {
            samples[i] = Math.sin(2 * Math.PI * i / (sampleRate / freqOfTone));
        }

        // convert to 16 bit pcm sound array
        // assumes the sample buffer is normalised.
        int idx = 0;
        for (final double dVal : samples) {
            // scale to maximum amplitude
            final short val = (short) ((dVal * 32767));
            // in 16 bit wav PCM, first byte is the low order byte
            generatedSnd[idx++] = (byte) (val & 0x00ff);
            generatedSnd[idx++] = (byte) ((val & 0xff00) >>> 8);

        }
    }

    private void playSound() {
        final AudioTrack audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,
                sampleRate, AudioFormat.CHANNEL_OUT_MONO,
                AudioFormat.ENCODING_PCM_16BIT, generatedSnd.length,
                AudioTrack.MODE_STATIC);
        audioTrack.write(generatedSnd, 0, generatedSnd.length);
        audioTrack.play();
    }

    public void startSound() {
        final Thread thread = new Thread(new Runnable() {
            public void run() {
                genTone();
                handler.post(new Runnable() {
                                 public void run() {
                                     playSound();
                                 }
                             }
                );
            }
        });
        thread.start();
    }
}
