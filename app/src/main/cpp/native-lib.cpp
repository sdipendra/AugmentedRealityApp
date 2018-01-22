#include <jni.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/aruco.hpp>

#include <android/log.h>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_illuminati_augmentedreality_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

int transformMat(cv::Mat input, cv::Mat& output);

extern "C"
JNIEXPORT jint JNICALL
Java_com_illuminati_augmentedreality_MainActivity_transformImage(
        JNIEnv* env,
        jobject /* this */,
        jlong addrInputImage,
        jlong addrOutputImage) {

    //__android_log_write(ANDROID_LOG_ERROR, "Call", "transformImage called");//Or ANDROID_LOG_INFO, ...

    cv::Mat& matInputImage = *(cv::Mat*)addrInputImage;
    cv::Mat& matOutputImage = *(cv::Mat*)addrOutputImage;

    int cppReturn;
    jint javaReturn;

    cppReturn = transformMat(matInputImage, matOutputImage);
    javaReturn = (jint)cppReturn;
    return javaReturn;
}

// todo: Fix the orientation of the displayed image and allow it to go full screen
// Do all image transformation changes here only
int transformMat(cv::Mat input, cv::Mat& output) {
    cv::cvtColor(input, output, CV_BGR2GRAY);
    // output = input.clone();

    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f> > corners;
    cv::aruco::detectMarkers(output, dictionary, corners, ids);
    // if at least one marker detected
    if (ids.size() > 0)
        cv::aruco::drawDetectedMarkers(output, corners, ids);
    return 0;
}
