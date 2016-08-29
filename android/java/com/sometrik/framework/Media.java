package com.sometrik.framework;

import java.io.FileDescriptor;
import java.io.IOException;

import android.content.Context;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.audiofx.PresetReverb;

public class Media {

	String name;
	Context context;
	MediaPlayer mp;
	int soundId;

	public Media(Context context, String name) {

		System.out.println("Sound constructor called");

		this.name = name;
		this.context = context;

		soundId = context.getResources().getIdentifier(name, "raw", context.getPackageName());
		// final MediaPlayer mp = MediaPlayer.create(context, R.raw.test);

		System.out.println("playSound soudnId: " + soundId);
		mp = MediaPlayer.create(context, soundId);

		mp.setOnCompletionListener(new OnCompletionListener() {

			@Override
			public void onCompletion(MediaPlayer mp) {

				mp.release();
			}
		});
	}

	public void play() {
		mp.start();
	}

	public void reset() throws IllegalStateException, IOException {
		mp.reset();
		// mp.setDataSource(MEDIA_PATH);
		mp.prepare();
	}

	public void stop() throws IllegalStateException, IOException {

		if (mp.isPlaying()) {
			mp.stop();
			mp = MediaPlayer.create(context, soundId);
		}
	}

	public void pause() {
		if (mp.isPlaying())
			mp.pause();
	}

	public void loop() {
		mp.setLooping(true);
		mp.start();

	}
	
	public void loop(int times) {
		mp.setLooping(true);
		mp.start();

		
	}
	
	
	public void addEffect(){
		
		
		//testailua
		PresetReverb mReverb = new PresetReverb(0,mp.getAudioSessionId());//<<<<<<<<<<<<<
		mReverb.setPreset(PresetReverb.PRESET_SMALLROOM);
		mReverb.setEnabled(true);
		mp.setAuxEffectSendLevel(1.0f);
		
		
	}

}