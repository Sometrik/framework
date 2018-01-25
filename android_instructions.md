
Android Instructions
=======

## Updating Manifest

 #### Release
AndroidManifest.xml holds information about the release.
For a new version of the application to be uploaded on Google Play it has to
have a new version code. Google Play Console will not accept duplicate apk versions.
update android:versionCode and android:versionName.
 
 #### Permissions
 Permissions can be added inside the manifest tag with <uses-permission android:name="android.permission.SOMETHING" />.
 normal permission can be found here https://developer.android.com/guide/topics/permissions/normal-permissions.html

## Exporting .apk file

  #### Export wizard
File -> Export->Android->Export Android Application->Select project
  
  #### Keystore
Android keystore can be used to authenticate owner of the application.
Use this to sign your .apk file Either create a new keystore or use the one in c:\\chaff_release


## Uploading .apk file

Go to Google Play Console and select the application and go to releases.
on releases you can publish a new version of Beta or release full product.
