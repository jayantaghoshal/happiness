package com.volvocars.audioanalyzer;

import static java.lang.Math.abs;

import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.app.Activity;
import be.tarsos.dsp.AudioDispatcher;
import be.tarsos.dsp.AudioEvent;
import be.tarsos.dsp.AudioProcessor;
import be.tarsos.dsp.io.android.AudioDispatcherFactory;
import be.tarsos.dsp.pitch.PitchDetectionHandler;
import be.tarsos.dsp.pitch.PitchDetectionResult;
import be.tarsos.dsp.pitch.PitchProcessor;

public class MainActivity extends Activity {
    private final int freq = 200;
    private boolean compareFreqs = false;
    double maxfreq = 0.0;

    AudioDispatcher dispatcher;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        dispatcher = AudioDispatcherFactory.fromDefaultMicrophone(22050, 1024, 0);

        PitchDetectionHandler pdh = new PitchDetectionHandler() {
            @Override
            public void handlePitch(PitchDetectionResult result, AudioEvent e) {
                final float pitchInHz = result.getPitch();
                runOnUiThread(new Runnable() {
                                  @Override
                                  public void run() {
                                      // TODO Somehow make this an arbitrary frequency check
                                      compareFreqs = abs(pitchInHz - freq) < 5.0;
                                      TextView text = (TextView) findViewById(R.id.textView);

                                      TextView text2 = (TextView) findViewById(R.id.textView2);
                                      text2.setText("frequency:  " + pitchInHz);
                                      if (pitchInHz >= maxfreq) {

                                          text.setText("Maximum freq" +Math.round(maxfreq));
                                          maxfreq = pitchInHz;
                                          Log.i("maxfrequency", String.valueOf(maxfreq));
                                      }

                                  }
                              }
                );
            }
        };
        AudioProcessor p = new PitchProcessor(PitchProcessor.PitchEstimationAlgorithm.FFT_YIN, 22050, 1024, pdh);
        dispatcher.addAudioProcessor(p);

        new Thread(dispatcher, "Audio Dispatcher").start();

    }

    public boolean getCompareFreqs() { return compareFreqs; }

    public void playSound(int freq) {
        TonePlayer player = new TonePlayer(freq);
        player.execute();
    }
}

class TonePlayer extends AsyncTask {

    int frequency = 0;

    TonePlayer(int freq) { frequency = freq; }

    @Override
    protected Object doInBackground(Object... params) {
        ToneGen generator = new ToneGen(frequency);

        generator.startSound();
        try {
            Log.v("test", "Playing freq " + 200);
            Thread.sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        return null;
    }
}


