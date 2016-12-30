package com.sometrik.framework;

import java.util.ArrayList;

import com.android.vending.billing.IInAppBillingService;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;

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

	public Bundle getItemList(ArrayList<String> productIDList, String packageName) {
		//Create a query with Product IDs
		Bundle querySkus = new Bundle();
		querySkus.putStringArrayList("ITEM_ID_LIST", productIDList);
		try {
			//Retrieve details with query. 3 is the in-app Billing version.
			return mService.getSkuDetails(3, packageName, "inapp", querySkus);
		} catch (RemoteException e) {
			System.out.println("Error retrieving product list information");
			//TODO
			e.printStackTrace();
		}
		return null;
	}

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
