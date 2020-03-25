//
// Created by jang on 2020-03-23.
//

#include "FaceDetector.h"
#include <iostream>
#include <opencv2/core.hpp>

#include "mtcnn.h"
#include <android/log.h>
MTCNN* gmtcnn = nullptr;

extern "C" {
JNIEXPORT jobject JNICALL Java_com_syj_faceremover_FaceRemover_DectectFace
        (JNIEnv *env, jobject thiz, jlong img) {

    if (gmtcnn == nullptr)
        return nullptr;

    cv::Mat& image =*(cv::Mat*)(img);
    ncnn::Mat input;
    if(image.channels() == 4) {
        input = ncnn::Mat::from_pixels(image.data, ncnn::Mat::PIXEL_RGBA2RGB, image.cols,
                                            image.rows);
    }else{
        input = ncnn::Mat::from_pixels(image.data, ncnn::Mat::PIXEL_BGR2RGB, image.cols,
                                            image.rows);
    }
    std::vector<Bbox> outputs;
    gmtcnn->detect(input,outputs);
    if (outputs.size() == 0 ) {
        __android_log_print(ANDROID_LOG_VERBOSE, "outputsize", "is zero");
        return nullptr;
    }

    jclass infoClazz = env->FindClass("com/syj/faceremover/FaceRemover$FaceInformation");
    if(infoClazz == 0) {return nullptr;};

    jclass clazz = env->FindClass("com/syj/faceremover/FaceRemover$FaceResults");
    if (clazz == 0) {
        std::cout << "FindClass error !!!\n";
        return nullptr;
    }


    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    if (constructor == 0) {
        std::cout << "GetMethodID error !!!\n";
        return nullptr;
    }

    jmethodID constructorinfoInfo = env->GetMethodID(infoClazz, "<init>", "()V");
    if (constructorinfoInfo == 0) {
        std::cout << "GetMethodID error !!!\n";
        return nullptr;
    }


    // Create an instance of clazz
    jobject obj = env->NewObject(clazz, constructor);
    jfieldID resultsID = env->GetFieldID(clazz, "results",
                                         "[Lcom/syj/faceremover/FaceRemover$FaceInformation;");

    jobjectArray objArray = env->NewObjectArray(outputs.size(),infoClazz, nullptr);

    int index = 0;
    for(auto& box :outputs) {
//        if( box.score < 0.4 )
//            continue;
        jobject faceInfoObj = env->NewObject(infoClazz,constructorinfoInfo);


        jfieldID xId = env->GetFieldID(infoClazz, "x","I");
        jfieldID yId = env->GetFieldID(infoClazz, "y","I");
        jfieldID wId = env->GetFieldID(infoClazz, "w","I");
        jfieldID hId = env->GetFieldID(infoClazz, "h","I");

        env->SetIntField(faceInfoObj, xId,(box.x1 + box.x2)/2);
        env->SetIntField(faceInfoObj, yId,(box.y1 + box.y2)/2);
        env->SetIntField(faceInfoObj, wId,(box.x2 - box.x1)/2);
        env->SetIntField(faceInfoObj, hId,abs(box.y2 - box.y1)/2);
        env->SetObjectArrayElement(objArray,index++,faceInfoObj);
    }

    env->SetObjectField(obj,resultsID,objArray);

    return obj;
}


JNIEXPORT jobject JNICALL Java_com_syj_faceremover_FaceRemover_DectectFace2
        (JNIEnv *env, jobject thiz, jbyteArray image_, jint width,jint height,jint channel) {

    if (gmtcnn == nullptr)
        return nullptr;


    int tImageDateLen = env->GetArrayLength(image_);
    jbyte *imageDate = env->GetByteArrayElements(image_, NULL);
    unsigned char *faceImageCharDate = (unsigned char*)imageDate;
    ncnn::Mat ncnn_img;
    if(channel==3) {
        ncnn_img = ncnn::Mat::from_pixels(faceImageCharDate, ncnn::Mat::PIXEL_BGR2RGB,
                                          width, height);
    }else{
        ncnn_img = ncnn::Mat::from_pixels(faceImageCharDate, ncnn::Mat::PIXEL_RGBA2RGB, width, height);
    }


    std::vector<Bbox> outputs;
    gmtcnn->detect(ncnn_img,outputs);
    if (outputs.size() == 0 ) {
        __android_log_print(ANDROID_LOG_VERBOSE, "outputsize", "is zero");
        return nullptr;
    }

    jclass infoClazz = env->FindClass("com/syj/faceremover/FaceRemover$FaceInformation");
    if(infoClazz == 0) {return nullptr;};

    jclass clazz = env->FindClass("com/syj/faceremover/FaceRemover$FaceResults");
    if (clazz == 0) {
        std::cout << "FindClass error !!!\n";
        return nullptr;
    }


    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
    if (constructor == 0) {
        std::cout << "GetMethodID error !!!\n";
        return nullptr;
    }

    jmethodID constructorinfoInfo = env->GetMethodID(infoClazz, "<init>", "()V");
    if (constructorinfoInfo == 0) {
        std::cout << "GetMethodID error !!!\n";
        return nullptr;
    }


    // Create an instance of clazz
    jobject obj = env->NewObject(clazz, constructor);
    jfieldID resultsID = env->GetFieldID(clazz, "results",
                                         "[Lcom/syj/faceremover/FaceRemover$FaceInformation;");

    jobjectArray objArray = env->NewObjectArray(outputs.size(),infoClazz, nullptr);

    int index = 0;
    for(auto& box :outputs) {
        if( box.score < 0.4 )
            continue;
        jobject faceInfoObj = env->NewObject(infoClazz,constructorinfoInfo);


        jfieldID xId = env->GetFieldID(infoClazz, "x","I");
        jfieldID yId = env->GetFieldID(infoClazz, "y","I");
        jfieldID wId = env->GetFieldID(infoClazz, "w","I");
        jfieldID hId = env->GetFieldID(infoClazz, "h","I");

        env->SetIntField(faceInfoObj, xId,(box.x1 + box.x2)/2);
        env->SetIntField(faceInfoObj, yId,(box.y1 + box.y2)/2);
        env->SetIntField(faceInfoObj, wId,(box.x2 - box.x1)/2);
        env->SetIntField(faceInfoObj, hId,(box.y1 - box.y1)/2);
        env->SetObjectArrayElement(objArray,index++,faceInfoObj);
    }

    env->SetObjectField(obj,resultsID,objArray);

    return obj;
}

JNIEXPORT jboolean JNICALL Java_com_syj_faceremover_FaceRemover_EngingInit(JNIEnv *env, jobject thiz)
{
    using namespace std;

    jboolean tRet = false;

    if (gmtcnn != nullptr)
        return true;

    gmtcnn = new MTCNN();
//    gmtcnn->SetMinFace(40);

    return tRet;
}
}