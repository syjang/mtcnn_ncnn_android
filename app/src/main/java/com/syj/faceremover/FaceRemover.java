package com.syj.faceremover;

import android.graphics.Bitmap;
import android.media.FaceDetector;
import android.os.Environment;
import android.util.Log;

import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import java.io.File;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import javax.xml.namespace.NamespaceContext;

public class FaceRemover {

    public static class FaceResults {
        FaceInformation results[];
    }

    public static class FaceInformation {
        public int x;
        public int y;
        public int w;
        public int h;
    }

    public static void blur(Bitmap bitmap) {
        Mat img = new Mat();
        Mat dst = new Mat();
        Utils.bitmapToMat(bitmap, img);


        Imgproc.blur(img, dst, new Size(img.cols() / 20, img.rows() / 20));
        Utils.matToBitmap(dst, bitmap);
    }

    public static List<FaceInformation> FindFace(Bitmap bitmap) {


        List<FaceInformation> faceList = new ArrayList<FaceInformation>();
        Mat img = new Mat();
        Utils.bitmapToMat(bitmap, img);
        FaceResults rets = FaceRemover.DectectFace(img.getNativeObjAddr());
//        int bytes = bitmap.getByteCount();
//        ByteBuffer buffer = ByteBuffer.allocate(bytes); // Create a new buffer
//        bitmap.copyPixelsToBuffer(buffer); // Move the byte data to the buffer
//        byte[] temp = buffer.array(); // Get the underlying array containing the
//
//        FaceResults rets = FaceRemover.DectectFace2(temp,bitmap.getWidth(),bitmap.getHeight(),4);
        if( rets != null) {
            faceList = Arrays.asList(rets.results);
        }
        return faceList;
    }


    static {
        if (EngingInit() == false) {
            Log.d("ncnn", "Fucking ncnn");
        }
    }

    public native static FaceResults DectectFace(long img);
    public native static FaceResults DectectFace2(byte[] imageDate, int imageWidth , int imageHeight, int imageChannel);

    public native static boolean EngingInit();
}
