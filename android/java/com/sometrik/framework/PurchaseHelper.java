package com.sometrik.framework;

import java.util.ArrayList;

import org.json.JSONException;
import org.json.JSONObject;

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
	
	public PurchaseHelper() { }

	public ArrayList<String> getItemList(ArrayList<String> productIDList, String packageName) {
		//Create a query with Product IDs
		Bundle querySkus = new Bundle();
		querySkus.putStringArrayList("ITEM_ID_LIST", productIDList);
		try {
			//Retrieve details with query. 3 is the in-app Billing version.
			Bundle skuDetails =  mService.getSkuDetails(3, packageName, "inapp", querySkus);
			
			int response = skuDetails.getInt("RESPONSE_CODE");
			if (response == 0) {
				return skuDetails.getStringArrayList("DETAILS_LIST");
			}
		} catch (RemoteException e) {
			System.out.println("Error retrieving product list information");
			//TODO
			e.printStackTrace();
		}
		return null;
	}

	//Meant to be used with responseList returned from getItemList
	public String getProductId(String response) {
		JSONObject object;
		try {
			object = new JSONObject(response);
			return object.getString("productId");
		} catch (JSONException e) {
			System.out.println("Error retrieving product ID from response");
			e.printStackTrace();
		}
		return "";
	}
	
	//Meant to be used with responseList returned from getItemList
	public String getProductPrice(String response) {
		JSONObject object;
		try {
			object = new JSONObject(response);
			return object.getString("price");
		} catch (JSONException e) {
			System.out.println("Error retrieving product ID from response");
			e.printStackTrace();
		}
		return "";
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
