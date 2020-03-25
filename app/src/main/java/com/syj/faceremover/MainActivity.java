package com.syj.faceremover;

import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.media.FaceDetector;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.opencv.android.OpenCVLoader;

import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");

        if(!OpenCVLoader.initDebug()) {
            Log.d("opencv","fucking opencv");
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        Button btn = findViewById(R.id.btnLoadImg);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try{
                    Intent photoPickerIntent = new Intent(Intent.ACTION_PICK);
                    photoPickerIntent.setType("image/*");
                    startActivityForResult(photoPickerIntent, 100);
                }catch(Exception exp){
                    Log.i("Error",exp.toString());
                }
            }
        });
    }

    private Rect makeRect(FaceRemover.FaceInformation info){
        Rect rect = new Rect(info.x - info.w/2 ,info.y - info.h/2,info.x + info.w/2 , info.y + info.h/2);
        return rect;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == 100 && resultCode == RESULT_OK && null != data) {
            try {
                final Uri imageUri = data.getData();
                final InputStream imageStream = getContentResolver().openInputStream(imageUri);
                Bitmap selectedImage = BitmapFactory.decodeStream(imageStream);

                List<FaceRemover.FaceInformation> falist = FaceRemover.FindFace(selectedImage);
                Bitmap mutableBitmap = selectedImage.copy(Bitmap.Config.ARGB_8888, true);
                Canvas canvas = new Canvas(mutableBitmap);
                Paint paint = new Paint();
                paint.setColor(Color.RED);
//                paint.setStrokeWidth(4);

                for(FaceRemover.FaceInformation info : falist) {
                    Rect rect = makeRect(info);
                    canvas.drawRect(rect,paint);

                }


//                FaceRemover.blur(selectedImage);
                ImageView imView = findViewById(R.id.imageView);
                imView.setImageBitmap(mutableBitmap);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }

        }

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
