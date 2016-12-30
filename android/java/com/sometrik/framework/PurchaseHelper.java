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
	protected PurchaseHelper() { }
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

	public void disconnect(FrameWork framework) {
		if (mService != null) {
			framework.unbindService(mServiceConn);
		}
	}

	public void createConnection(FrameWork framework) {
		Intent serviceIntent = new Intent("com.android.vending.billing.InAppBillingService.BIND");
		serviceIntent.setPackage("com.android.vending");
		framework.bindService(serviceIntent, mServiceConn, Context.BIND_AUTO_CREATE);
	}

}
