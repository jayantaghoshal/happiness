/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.mediactrl.service.player;

import android.content.Context;
import android.media.AudioAttributes;
import android.media.AudioFocusRequest;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Handler;
import android.util.Log;

import com.google.android.exoplayer2.C;
import com.google.android.exoplayer2.DefaultLoadControl;
import com.google.android.exoplayer2.DefaultRenderersFactory;
import com.google.android.exoplayer2.ExoPlaybackException;
import com.google.android.exoplayer2.ExoPlayerFactory;
import com.google.android.exoplayer2.Format;
import com.google.android.exoplayer2.LoadControl;
import com.google.android.exoplayer2.PlaybackParameters;
import com.google.android.exoplayer2.Player;
import com.google.android.exoplayer2.RenderersFactory;
import com.google.android.exoplayer2.SimpleExoPlayer;
import com.google.android.exoplayer2.Timeline;
import com.google.android.exoplayer2.source.ExtractorMediaSource;
import com.google.android.exoplayer2.source.MediaSource;
import com.google.android.exoplayer2.source.MediaSourceEventListener;
import com.google.android.exoplayer2.source.TrackGroupArray;
import com.google.android.exoplayer2.trackselection.AdaptiveTrackSelection;
import com.google.android.exoplayer2.trackselection.DefaultTrackSelector;
import com.google.android.exoplayer2.trackselection.TrackSelection;
import com.google.android.exoplayer2.trackselection.TrackSelectionArray;
import com.google.android.exoplayer2.upstream.DataSource;
import com.google.android.exoplayer2.upstream.DataSpec;
import com.google.android.exoplayer2.upstream.DefaultAllocator;
import com.google.android.exoplayer2.upstream.DefaultBandwidthMeter;
import com.google.android.exoplayer2.upstream.DefaultHttpDataSourceFactory;
import com.google.android.exoplayer2.upstream.UdpDataSource;

import com.google.android.exoplayer2.util.EventLogger;

import java.io.IOException;

import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyStatus;

public class MediaPlayer {

    private static final String TAG = "RemoteCtrl.MediaCtrl.Service.Player.MediaPlayer";
    private static final String url = "http://198.19.102.162:8080/";

    private SimpleExoPlayer player = null;
    private Context context;
    private MediaPlayerCallback mediaPlayerCallback;
    private boolean isPlaying = false;
    private boolean isPaused = false;

    private AudioManager mAudioManager;
    private AudioFocusRequest mFocusRequest;
    private boolean mAudioFocusGranted = false;


    public MediaPlayer(Context context, MediaPlayerCallback mediaPlayerCallback) {
        this.context = context;
        this.mediaPlayerCallback = mediaPlayerCallback;
        initializePlayer();
    }

    private void initializePlayer() {
        Log.v(TAG, "Initializing MediaPlayer");
        DefaultAllocator defaultAllocator = new DefaultAllocator(true, C.DEFAULT_BUFFER_SEGMENT_SIZE);
        DefaultBandwidthMeter bandwidthMeter = new DefaultBandwidthMeter();
        TrackSelection.Factory adaptiveTrackSelectionFactory =
                new AdaptiveTrackSelection.Factory(bandwidthMeter);
        DefaultTrackSelector trackSelector = new DefaultTrackSelector(adaptiveTrackSelectionFactory);
        RenderersFactory renderersFactory = new DefaultRenderersFactory(context);
        LoadControl loadControl = new DefaultLoadControl(defaultAllocator, 150, 300, 10, 10, C.DEFAULT_AUDIO_BUFFER_SIZE, true);
        player = ExoPlayerFactory.newSimpleInstance(renderersFactory, trackSelector, loadControl);

        player.addListener(playerEventListener);
        player.setPlayWhenReady(false);
    }

    private void prepareMediaPlayerHttp() {
        Log.v(TAG, "Preparing HTTP source");
        DefaultHttpDataSourceFactory dataSourceFactory = new DefaultHttpDataSourceFactory("RemoteCtrl.MediaPlayer");

        MediaSource mediaSource = new ExtractorMediaSource.Factory(dataSourceFactory)
                .createMediaSource(Uri.parse(url), new Handler(), mediaSourceEventListener);
        player.prepare(mediaSource);
    }

    public boolean play() {
        if (isPaused) {
            player.setPlayWhenReady(true);
            return true;
        }

        requestAudioFocus();
        if (!isPlayerReady() || !mAudioFocusGranted) {
            return false;
        }
        player.setPlayWhenReady(true);
        prepareMediaPlayerHttp();
        return true;
    }

    public boolean stop() {
        if (!isPlayerReady()) {
            return false;
        }
        player.stop();
        player.setPlayWhenReady(false);
        releaseAudioFocus();
        return true;
    }

    public boolean pause() {
        if (!isPlayerReady() || !isPlaying) {
            return false;
        }
        player.setPlayWhenReady(false);
        return true;
    }

    public boolean next() {
        boolean resultStop;
        boolean resultPlay;
        resultStop = stop();
        resultPlay = play();
        return (resultStop && resultPlay);
    }

    public boolean previous() {
        return next();
    }

    public boolean isPlayerReady() {
        if (player != null) {
            return true;
        }
        return false;
    }

    private Player.EventListener playerEventListener = new Player.EventListener() {
        @Override
        public void onTimelineChanged(Timeline timeline, Object o, int i) {

        }

        @Override
        public void onTracksChanged(TrackGroupArray trackGroupArray, TrackSelectionArray trackSelectionArray) {

        }

        @Override
        public void onLoadingChanged(boolean isLoading) {
            Log.v(TAG, "loading [" + isLoading + "]");

        }

        @Override
        public void onPlayerStateChanged(boolean playWhenReady, int state) {
            Log.v(TAG, "state [" + playWhenReady + ", "
                    + getStateString(state) + "]");

            if (playWhenReady && state == Player.STATE_READY) {
                mediaPlayerCallback.PlaybackStatusChanged(MediaPlayerService.PLAY_PLAYING,
                        RemoteCtrlHalPropertyStatus.AVAILABLE);
                isPlaying = true;
                isPaused = false;
            }
            if (!playWhenReady) {
                switch (state) {
                    case Player.STATE_IDLE:
                        mediaPlayerCallback.PlaybackStatusChanged(MediaPlayerService.STOP_STOPPED,
                                RemoteCtrlHalPropertyStatus.AVAILABLE);
                        isPlaying = false;
                        isPaused = false;
                    	break;
                    case Player.STATE_READY:
                        mediaPlayerCallback.PlaybackStatusChanged(MediaPlayerService.PAUSE_PAUSED,
                                RemoteCtrlHalPropertyStatus.AVAILABLE);
                        isPlaying = false;
                        isPaused = true;
                        break;
                    default:
                        break;
                }
            }
        }

        @Override
        public void onRepeatModeChanged(int i) {

        }

        @Override
        public void onShuffleModeEnabledChanged(boolean b) {

        }

        @Override
        public void onPlayerError(ExoPlaybackException e) {
            Log.e(TAG, "playerFailed ", e);
            mediaPlayerCallback.PlaybackStatusChanged(MediaPlayerService.STOP_STOPPED,
                    RemoteCtrlHalPropertyStatus.SYSTEM_ERROR);
            isPlaying = false;
        }

        @Override
        public void onPositionDiscontinuity(int i) {

        }

        @Override
        public void onPlaybackParametersChanged(PlaybackParameters playbackParameters) {

        }

        @Override
        public void onSeekProcessed() {

        }
    };

    private MediaSourceEventListener mediaSourceEventListener = new MediaSourceEventListener() {
        @Override
        public void onLoadStarted(DataSpec dataSpec, int i, int i1, Format format, int i2, Object o, long l, long l1, long l2) {

        }

        @Override
        public void onLoadCompleted(DataSpec dataSpec, int i, int i1, Format format, int i2, Object o, long l, long l1, long l2, long l3, long l4) {

        }

        @Override
        public void onLoadCanceled(DataSpec dataSpec, int i, int i1, Format format, int i2, Object o, long l, long l1, long l2, long l3, long l4) {

        }

        @Override
        public void onLoadError(DataSpec dataSpec, int i, int i1, Format format, int i2, Object o, long l, long l1, long l2, long l3, long l4, IOException e, boolean b) {
            Log.e(TAG, "onLoadError ", e);
        }

        @Override
        public void onUpstreamDiscarded(int i, long l, long l1) {

        }

        @Override
        public void onDownstreamFormatChanged(int i, Format format, int i1, Object o, long l) {

        }
    };


    private static String getStateString(int state) {
        switch (state) {
            case Player.STATE_BUFFERING:
                return "BUFFERING";
            case Player.STATE_ENDED:
                return "ENDED";
            case Player.STATE_IDLE:
                return "IDLE";
            case Player.STATE_READY:
                return "READY";
            default:
                return "UNKNOWN";
        }
    }

    private void requestAudioFocus() {
        mAudioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        AudioAttributes mPlaybackAttributes = new AudioAttributes.Builder()
                .setUsage(AudioAttributes.USAGE_MEDIA)
                .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
                .build();
        mFocusRequest = new AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN_TRANSIENT)
                .setAudioAttributes(mPlaybackAttributes)
                .setAcceptsDelayedFocusGain(false)
                .build();
        int res = mAudioManager.requestAudioFocus(mFocusRequest);

        mAudioFocusGranted = false;
        if (res == AudioManager.AUDIOFOCUS_REQUEST_GRANTED) {
            mAudioFocusGranted = true;
        }
        Log.v(TAG, "Audio focus requested.Result :" + res);
    }

    private void releaseAudioFocus() {
        if (mAudioFocusGranted) {
            int res = mAudioManager.abandonAudioFocusRequest(mFocusRequest);
            Log.v(TAG, "Releasing audio focus : " + res);
            return;
        }
        Log.v(TAG, "No audio focus to be released");
    }

}
