
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


## New project creation notes

   #### Appcompat
Android project requires appcompat v7. This needs to be added to a new project as library or possibly as source.
For billing to work

   #### Billing
Google sample of play store billing has to be added to the project as source. These include files from trivialdrivesample.util such as IabHelper.java, Inventory.java and Purchase.java. From vending.billing the lInAppBillingService.aidl file must also be added
https://github.com/googlesamples/android-play-billing/tree/master/TrivialDrive/app/src/main
