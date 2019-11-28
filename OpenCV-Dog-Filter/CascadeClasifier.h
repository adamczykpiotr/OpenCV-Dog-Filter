#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

class CascadeClasifier {
	cv::Mat * p_frame;
	cv::Mat * p_filter;

	cv::CascadeClassifier m_face;
	cv::CascadeClassifier m_eyes;

	class Config {
		public:
			const char* faceClassifierPath = "xml/haarcascade_frontalface_alt.xml";
			const char* eyeClassifierPath = "xml/haarcascade_eye_tree_eyeglasses.xml";
			const char* filterPath = "snapchat.png";
			bool drawElipsis = false;
			float minFaceSize = 0.05; //face size has to be at lest 10% of the frame
			
			float minEyesSize = 0.01;
			float maxEyesSize = 0.9;
	};

	class Helpers {
		public:
			float calculateAngleRadians(const cv::Point& first, const cv::Point& second);
			float calculateAngleDegrees(const cv::Point& first, const cv::Point& second);
			void drawTransparency(cv::Mat * frame, cv::Mat * transp, int xPos, int yPos);
	} helpers;

public:
	
	Config defaults;

	void init();

	void setFrame(cv::Mat * frame);

	void setFilter(cv::Mat* filter);
	void setFilter(const char * path);

	void setEyesClassifier(cv::CascadeClassifier * classifier);
	void setEyesClassifier(const char* path);

	void setFaceClassifier(cv::CascadeClassifier* classifier);
	void setFaceClassifier(const char* path);

	void detect();
	//void display();
};

