//============================================================================
// Name        : RTMNIST.cpp
// Author      : Dourado
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Kinect.h"
#include "CaffeClassifier.h"
#include <list>

using namespace std;
using namespace cv;


Rect reshapeRect(Mat,Rect);


int main() {

	Mat rgb,hsv,hsvm,frame,threshOut;
	Kinect kinect;
	list<pair<Mat,Rect>> x;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	CaffeClassifier caffe;
	list<pair<Mat,Rect>> lMat;

	while(1){
		rgb = kinect.getRGBFrame();
		cvtColor(rgb,frame,CV_BGR2GRAY);
		cvtColor(rgb,hsv,CV_BGR2HSV);

		Mat1b m1,m2,hm;
		inRange(hsv,Scalar(0,160,100),Scalar(10,255,255),m1);
		inRange(hsv,Scalar(160,160,100),Scalar(180,255,255),m2);

		hm = m1 | m2;

		bitwise_not(hm,hm);

		Mat sEl = getStructuringElement(MORPH_RECT,Size(17,17));
		morphologyEx(hm,hm,MORPH_OPEN,sEl);
		Canny(hm,threshOut,100,255,3);
		findContours(threshOut,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);


		if(contours.size() > 0){
			lMat.clear();

			vector<Rect> bonds(contours.size());

			for(unsigned int i=0;i<contours.size();i=hierarchy[i][0]){
				bonds[i] = boundingRect(contours[i]);
			}

			bool sub;

			for(unsigned int i=0;i<contours.size();i=hierarchy[i][0]){
				Rect bond = boundingRect(contours[i]);

				sub = false;
				for(unsigned int j=0;j<contours.size();j=hierarchy[j][0])
					if(i!=j && bonds[j].contains(Point(bond.x,bond.y)))
						sub=true;

				if(!sub){
					bond = reshapeRect(frame,bond);
					lMat.push_front(pair<Mat,Rect>(frame(bond),bond));
				}
			}

			if(lMat.size() > 0)
				for(auto l : lMat){
					Rect b = l.second;
					rectangle(rgb,b.tl(),b.br(),Scalar(0,255,255),3,8,0);
					Mat cmat = l.first.clone();
					threshold(cmat,cmat,90,255,CV_THRESH_BINARY);
					vector<Prediction> pred = caffe.Classify(cmat);
					//cout << pred[0].first << " " << pred[0].second << endl;
					putText(rgb,pred[0].first,Point(b.x,b.y-5),CV_FONT_HERSHEY_PLAIN,2,Scalar(0,255,0),2);
				}
		}

		imshow("Numeros",rgb);
		waitKey(1);
	}

	return 0;
}

Rect reshapeRect(Mat m, Rect r){
	int px = 50;
	int mw = m.cols;
	int mh = m.rows;


	r.x = (r.x - px <= 0) ? 0 : r.x - px;
	r.width = (r.x + r.width + px >= mw) ? r.width : r.width + px;
	r.width = (r.x + r.width + px >= mw) ? r.width : r.width + px;
	r.y = (r.y - px <= 0) ? 0 : r.y - px;
	r.height = (r.y + r.height + px >= mh) ? r.height : r.height + px;
	r.height = (r.y + r.height + px >= mh) ? r.height : r.height + px;

	return r;

}




/*					bool asize = false;
					float wmul = 0.6f;
					float hmul = 0.6f;
					while(!asize){
						Size dsize(bond.width*wmul,bond.height*hmul);
						bond += dsize;
						bond -= Point(dsize.width/2,dsize.height/2);
						if(bond.x < 0 || bond.x+bond.width > frame.cols || bond.y < 0 || bond.y+bond.height > frame.rows){
							wmul -= 0.1f;
							hmul -= 0.1f;
						}
						else
							asize = true;
					}*/
