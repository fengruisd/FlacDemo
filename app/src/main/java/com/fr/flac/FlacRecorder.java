package com.fr.flac;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Handler;
import android.os.HandlerThread;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by fengruifr on 17/2/9.
 *
 */
public class FlacRecorder {

    private String flacFile;
    private boolean isRecording;
    private int bufferSize;
    private AudioRecord audioRecord;
    private short[] audioData;

    private FrFlac frFlac;

    private RecordThread recordThread;
    private WriteThread writeThread;

    public FlacRecorder(String flacFile) {
        this.flacFile = flacFile;
        this.isRecording = false;
        this.frFlac = new FrFlac();
    }

    public boolean isRecording() {
        return isRecording;
    }

    private void initAudioRecord() {
        bufferSize = AudioRecord.getMinBufferSize(44100,
                AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);

        if(bufferSize % 200 != 0) {
            bufferSize = ((bufferSize / 200) + 1) * 200;
        }

        audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC,
                44100, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT,
                bufferSize);

        audioData = new short[bufferSize];
    }

    public void start() {
        if(isRecording) {
            return;
        }
        File file = new File(flacFile);
        if(file.exists()) {
            file.delete();
        }
        try {
            file.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
        isRecording = true;
        initAudioRecord();
        writeThread = new WriteThread(frFlac);
        writeThread.start();
        audioRecord.setRecordPositionUpdateListener(writeThread, new Handler(writeThread.getLooper()));
        audioRecord.setPositionNotificationPeriod(100);
        audioRecord.startRecording();
        recordThread = new RecordThread();
        recordThread.start();
    }

    public void stop() {
        recordThread.needStop();
    }

    class RecordThread extends Thread {

        @Override
        public void run() {
            android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);
            while (isRecording) {
                int readSize = audioRecord.read(audioData, 0, bufferSize);
                if (readSize > 0) {
                    writeThread.enqueuData(audioData, readSize);
                }
            }

            audioRecord.stop();
            audioRecord.release();
            audioRecord = null;

            writeThread.needStop();
        }

        public void needStop() {
            isRecording = false;
        }
    }

    class WriteThread extends HandlerThread implements AudioRecord.OnRecordPositionUpdateListener {

        private List<AudioData> audioDataList;
        private FrFlac frFlac;
        private boolean needStop;

        public WriteThread(FrFlac frFlac) {
            super("FlacWriter");
            this.frFlac = frFlac;
            this.needStop = false;
            this.audioDataList = new ArrayList<AudioData>();
            frFlac.initEncoder(flacFile);
        }

        public void enqueuData(short[] audioData, int readSize) {
            this.audioDataList.add(new AudioData(audioData, readSize));
        }

        public void needStop() {
            needStop = true;

        }

        @Override
        public void onMarkerReached(AudioRecord recorder) {

        }

        @Override
        public void onPeriodicNotification(AudioRecord recorder) {
            if(!needStop) {
                writeData();
            } else {
                while (audioDataList.size() > 0) {
                    writeData();
                }
                frFlac.finishEncode();
                getLooper().quit();
            }
        }

        private void writeData() {
            if(audioDataList.size() > 0) {
                AudioData audioData = audioDataList.remove(0);
                frFlac.encode(audioData.data, audioData.size);
            }
        }
    }

    class AudioData {
        public short[] data;
        public int size;

        public AudioData(short[] data, int size) {
            this.data = data;
            this.size = size;
        }
    }
}
