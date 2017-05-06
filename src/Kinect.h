#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#ifndef CLASSES_Kinect_H_
#define CLASSES_Kinect_H_

class Kinect {
public:
	Kinect();
	virtual ~Kinect();
	bool isKinectOK();
	Mat getPCMFrame();
	Mat getRGBFrame();
	Mat getDepthFrame();
	Mat getRGBFrame(Mat);
	Mat getDepthFrame(int);
	Mat getDepthFrame(Mat,float);
	Mat getDistMap();
	Mat getDistRGBFrame(Mat,float);
	Mat getNearestObject(Mat,Mat,int,int);

private:
	VideoCapture kinect;
};

#endif /* SRC_KINECT_H_ */
