package com.sometrik.framework;

import com.android.vending.billing.IInAppBillingService;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;

public class PurchaseHelper {
	
	IInAppBillingService mService;
	private static PurchaseHelper instance = null;

	public static PurchaseHelper getInstance() {
		if (instance == null) {
			instance = new PurchaseHelper();
		}
		return instance;
	}

	ServiceConnection mServiceConn = new ServiceConnection() {
		@Override
		public void onServiceDisconnected(ComponentName name) {
			mService = null;
		}

		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			mService = IInAppBillingService.Stub.asInterface(service);
		}
	};

	protected PurchaseHelper() {
	}

	public void createConnection(Activity activity) {
		Intent serviceIntent = new Intent("com.android.vending.billing.InAppBillingService.BIND");
		serviceIntent.setPackage("com.android.vending");
		activity.bindService(serviceIntent, mServiceConn, Context.BIND_AUTO_CREATE);
	}

}
