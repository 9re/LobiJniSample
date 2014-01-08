package co.lobi.sdk.jni;

import org.json.JSONObject;

import android.app.NativeActivity;
import android.os.Handler;
import android.widget.Toast;

import com.kayac.nakamap.sdk.Nakamap;

public class CustomNativeActivity extends NativeActivity {
    private Handler mHandler = new Handler();
    native void onResult(int code, String json, long callback, long data);
    public void joinGroupWithExternalID(final String externalID, final String groupName, final long callback, final long data) {
        Nakamap.joinGroupWithExternalID(externalID, groupName, new Nakamap.NakamapApiCallback() {
            @Override
            public void onResult(int code, JSONObject jsonObject) {
                CustomNativeActivity.this.onResult(code, jsonObject != null ? jsonObject.toString() : null, callback, data);
            }
        });
    }
    
    public boolean isSignedIn() {
        return Nakamap.isSignedIn();
    }
    
    public void signupWithBaseName(final long callback, final long data) {
        Nakamap.signupWithBaseName(Nakamap.sharedClient().getNewAccountBaseName(), new Nakamap.NakamapApiCallback() {
            @Override
            public void onResult(int code, JSONObject jsonObject) {
                CustomNativeActivity.this.onResult(code, jsonObject != null ? jsonObject.toString() : null, callback, data);
            }
        });
    }
    
    public void showChatView() {
        Nakamap.showChatView();
    }
    
    public void showToast(final String message) {
        mHandler.post(new Runnable(){
            @Override
            public void run() {
                Toast
                    .makeText(CustomNativeActivity.this, message, Toast.LENGTH_SHORT)
                    .show();
            }
        });
    }
}
