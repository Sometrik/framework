package com.sometrik.framework;

import android.content.Context;
import android.media.AudioManager;
import android.media.SoundPool;


//Loadaus vie aikaa
public class Sound {

	private String name;
	private Context context;
	private SoundPool sp;
	private int id;

	public Sound(Context context, String name) {

		this.context = context;
		this.name = name;

		// Tarvitsee toisen tyylin uusille versioille (kait)
		sp = new SoundPool(4, AudioManager.STREAM_MUSIC, 100);
		id = sp.load(context, context.getResources().getIdentifier(name, "raw", context.getPackageName()), 1);

	}
	
	public void loadSound(String name){
		
		
	}

	public void play() {
		sp.play(id, 1f, 1f, 1, 0, 1f);
	}

}
