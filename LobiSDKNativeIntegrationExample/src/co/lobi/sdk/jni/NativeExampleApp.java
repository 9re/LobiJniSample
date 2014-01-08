package co.lobi.sdk.jni;

import com.kayac.nakamap.sdk.Nakamap;

import android.app.Application;

public class NativeExampleApp extends Application {
    static {
        System.loadLibrary("lobi");
    }
    @Override
    public void onCreate() {
        super.onCreate();
        
        Nakamap.setup(this, "d534747a58dba2003deb451bc49fdce211eed4bb", "native user");
    }
}
