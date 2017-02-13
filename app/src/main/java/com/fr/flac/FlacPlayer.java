package com.fr.flac;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;

import java.io.File;

/**
 * Created by fengruifr on 17/2/11.
 *
 */
public class FlacPlayer {

    private String flacFile;
    private boolean isPlaying;

    private int bufferSize;

    private AudioTrack audioTrack;

    public FlacPlayer(String file) {
        this.flacFile = file;
        initAudioTrack();
    }

    private void initAudioTrack() {
        bufferSize = AudioRecord.getMinBufferSize(44100,
                AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);

        if(bufferSize % 200 != 0) {
            bufferSize = ((bufferSize / 200) + 1) * 200;
        }

        audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,
                44100, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT,
                bufferSize, AudioTrack.MODE_STREAM);
    }

    public void start() {
        if(isPlaying || !new File(flacFile).exists()) {
            return;
        }

        isPlaying = true;
    }
}
