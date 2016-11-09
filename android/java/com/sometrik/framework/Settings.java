package com.sometrik.framework;

import java.lang.reflect.Array;
import java.util.ArrayList;

import com.example.machine.R;

import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.EditTextPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceCategory;
import android.preference.PreferenceFragment;
import android.preference.PreferenceScreen;
import android.preference.SwitchPreference;

public class Settings extends PreferenceFragment {

  private String dimension;
  private String arvo;
  private int numero;
  ArrayList<MenuItem> menuList;

  SharedPreferences prefs;
  SharedPreferences.Editor editor;

  PreferenceScreen screen;
  Context context;

  public Settings(Context context) {
    this.context = context;
    menuList = new ArrayList<>();
  }

  @Override
  public void onActivityCreated(Bundle savedInstanceState) {
    super.onActivityCreated(savedInstanceState);

    getView().setBackgroundColor(Color.BLACK);
    getView().setClickable(true);
  }

  @Override
  public void onResume() {
    super.onResume();
    context = this.getActivity();
    create();
  }

  public Settings() {
    create();
  }

  public void setContext(Context context) {
    this.context = context;
  }

  public void create() {

    System.out.println("Settings onCreate called");

    // prefs = context.getSharedPreferences("com.example.Work",
    // Context.MODE_PRIVATE);
    // editor = prefs.edit();

//    addPreferencesFromResource(R.layout.settings2);

    PreferenceCategory facebookCategory = new PreferenceCategory(this.getActivity());
    facebookCategory.setTitle("Facebook");

    PreferenceCategory twitterCategory = new PreferenceCategory(this.getActivity());
    twitterCategory.setTitle("Twitter");

    PreferenceCategory instagramCategory = new PreferenceCategory(this.getActivity());
    instagramCategory.setTitle("Instagram");

    PreferenceCategory settingsCategory = new PreferenceCategory(this.getActivity());
    settingsCategory.setTitle("Settings");

    screen = this.getPreferenceScreen();

    screen.addPreference(facebookCategory);
    screen.addPreference(twitterCategory);
    screen.addPreference(instagramCategory);
    screen.addPreference(settingsCategory);

    for (int i = 0; i < menuList.size(); i++) {

      // Possible preference types:
      // CheckBoxPreference, EditTextPreference, ListPreference,
      // MultiSelectListPreference, PreferenceCategory, SwitchPreference
      switch (menuList.get(i).getMode()) {

      case 0:
	Preference pref = new Preference(this.getActivity());
	pref.setTitle(menuList.get(i).getName());

	if (menuList.get(i).getMedia().equals("nothing")) {
	  settingsCategory.addPreference(pref);
	} else if (menuList.get(i).getMedia().equals("facebook")) {
	  pref.setIcon(menuList.get(i).getIcon());
	  facebookCategory.addPreference(pref);
	} else if (menuList.get(i).getMedia().equals("twitter")) {
	  pref.setIcon(menuList.get(i).getIcon());
	  twitterCategory.addPreference(pref);
	} else if (menuList.get(i).getMedia().equals("instagram")) {
	  pref.setIcon(menuList.get(i).getIcon());
	  instagramCategory.addPreference(pref);
	}

	break;

      case 1:

	CheckBoxPreference checkBoxPreference = new CheckBoxPreference(this.getActivity());
	checkBoxPreference.setTitle(menuList.get(i).getName());

	screen.addPreference(checkBoxPreference);

	break;

      case 2:

	EditTextPreference edit = new EditTextPreference(this.getActivity());
	edit.setTitle(menuList.get(i).getName());

	screen.addPreference(edit);

	break;

      case 3:

	SwitchPreference switchPref = new SwitchPreference(this.getActivity());
	switchPref.setTitle(menuList.get(i).getName());

	screen.addPreference(switchPref);

	break;
      }

    }

    if (facebookCategory.getPreferenceCount() == 0) {
      System.out.println("facebook tyhjä");
      screen.removePreference(facebookCategory);
    }
    if (twitterCategory.getPreferenceCount() == 0) {
      System.out.println("twitter tyhjä");
      screen.removePreference(twitterCategory);
    }
    if (instagramCategory.getPreferenceCount() == 0) {
      System.out.println("instagram tyhjä");
      screen.removePreference(instagramCategory);
    }

  }

  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

  }

  public void createMenuItem(int mode, int id, String name) {
    menuList.add(new MenuItem(mode, id, name));
  }

  public void createMenuItem(String media, int id, String name) {
    System.out.println("social creation " + media + " " + name);
    System.out.println("getActivity " + context);
    menuList.add(new MenuItem(context, id, name, media));
  }

  public static class MyPreferenceFragment extends PreferenceFragment {
    @Override
    public void onCreate(final Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);

      System.out.println("Settings fragment created");
//      addPreferencesFromResource(R.layout.settings2);

      PreferenceScreen screen = this.getPreferenceScreen();

      EditTextPreference edit = new EditTextPreference(this.getActivity());
      edit.setTitle("Enter your username");

      CheckBoxPreference checkBoxPreference = new CheckBoxPreference(this.getActivity());
      checkBoxPreference.setTitle("Something checkbox");

      screen.addPreference(edit);
      screen.addPreference(checkBoxPreference);

    }
  }

}