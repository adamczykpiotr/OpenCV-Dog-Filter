#include "CascadeClasifier.h"

void CascadeClasifier::init() {
	setEyesClassifier(defaults.eyeClassifierPath);
	setFaceClassifier(defaults.faceClassifierPath);
	setFilter(defaults.filterPath);
}

void CascadeClasifier::setFrame(cv::Mat* frame) {
	p_frame = frame;
}

void CascadeClasifier::setFilter(cv::Mat* filter) {
	p_filter = filter;
}

void CascadeClasifier::setFilter(const char* path) {
	p_filter = new cv::Mat();
	*p_filter = cv::imread(path, cv::IMREAD_UNCHANGED);
}

void CascadeClasifier::setEyesClassifier(cv::CascadeClassifier* classifier) {
	m_eyes = *classifier;
}

void CascadeClasifier::setEyesClassifier(const char* path) {
	if (!m_eyes.load(path)) {
		throw new std::exception("Invalid classifier path");
	}
}

void CascadeClasifier::setFaceClassifier(cv::CascadeClassifier* classifier) {
	m_face = *classifier;
}

void CascadeClasifier::setFaceClassifier(const char* path) {
	if (!m_face.load(path)) {
		throw new std::exception("Invalid classifier path");
	}
}

void CascadeClasifier::detect() {

	cv::Mat frame_gray;
	cvtColor(*p_frame, frame_gray, cv::COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//detect all faces
	std::vector<cv::Rect> faces;

	cv::Size minFaceSize = frame_gray.size();
	minFaceSize.height *= defaults.minFaceSize;
	minFaceSize.height *= defaults.minFaceSize;

	m_face.detectMultiScale(frame_gray, faces, 1.2, 3, 0); //increase size in every step by 20%
	for (size_t i = 0; i < faces.size(); i++) {
		cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);

		cv::Mat filter(*p_filter);
		resize(filter, filter, cv::Size(faces[i].width, faces[i].height));

		int posX = faces[i].x;
		int posY = faces[i].y - filter.rows * 0.25; //25% to place nose and ears perfectly

		int centerX = posX + faces[i].width / 2;
		int centerY = posY + faces[i].height / 2;

		if (defaults.drawElipsis) {
			int centerYelipsis = faces[i].y + faces[i].height / 2;
			ellipse(*p_frame, {centerX, centerYelipsis}, cv::Size(faces[i].width/2, faces[i].height/2),0, 0, 360, cv::Scalar(255, 0, 255), 4);
		}

		cv::Mat faceROI = frame_gray(faces[i]);

		//detect all eyes in face
		std::vector<cv::Rect> eyes;
/*
		cv::Size minEyesSize = faceROI.size();
		minEyesSize.height *= defaults.minEyesSize;
		minEyesSize.height *= defaults.minEyesSize;

		cv::Size maxEyesSize = faceROI.size();
		maxEyesSize.height *= defaults.maxEyesSize;
		maxEyesSize.height *= defaults.maxEyesSize;
		*/
		m_eyes.detectMultiScale(faceROI, eyes, 1.05, 3, 0); //increase size in every step by 5% to improve detection
		float angle = 0;
		float sign = 1.;
		if (eyes.size() == 2) {
			cv::Point left_eye(faces[i].x + eyes[0].x + eyes[0].width / 2, faces[i].y + eyes[0].y + eyes[0].height / 2);
			cv::Point right_eye(faces[i].x + eyes[1].x + eyes[1].width / 2, faces[i].y + eyes[1].y + eyes[1].height / 2);

			int left_radius = cvRound((eyes[0].width + eyes[0].height) * 0.25);
			int right_radius = cvRound((eyes[1].width + eyes[1].height) * 0.25);

			if (defaults.drawElipsis) {
				circle(*p_frame, left_eye, left_radius, cv::Scalar(255, 0, 0), 4);
				circle(*p_frame, right_eye, right_radius, cv::Scalar(255, 0, 0), 4);
			}	

			angle = this->helpers.calculateAngleDegrees(left_eye, right_eye);

			if (left_eye.y > right_eye.x) {
				sign = -1.;
			}
		}

		if (angle > 0.02) {
			cv::Point2f pc(filter.cols / 2., filter.rows / 2.);
			double finalAngle = sign * angle;
			cv::Mat r = cv::getRotationMatrix2D(pc, finalAngle, 1.0);
			cv::warpAffine(filter, filter, r, filter.size());
		}

		this->helpers.drawTransparency(p_frame, &filter, posX, posY);

	}
	
	cv::imshow("Output", *p_frame);
}

float CascadeClasifier::Helpers::calculateAngleRadians(const cv::Point& first, const cv::Point& second) {
	double cosAngle = first.dot(second) / (cv::norm(first) * cv::norm(second));
	if (cosAngle > 1.0)
		return 0.0;
	else if (cosAngle < -1.0)
		return CV_PI;
	return std::acos(cosAngle);
}

float CascadeClasifier::Helpers::calculateAngleDegrees(const cv::Point& first, const cv::Point& second) {
	return this->calculateAngleRadians(first, second) * 180.0 / CV_PI;
}

void CascadeClasifier::Helpers::drawTransparency(cv::Mat* frame, cv::Mat* transp, int xPos, int yPos) {
	cv::Mat mask;
	std::vector<cv::Mat> layers;

	split(*transp, layers); //seperate channels
	cv::Mat rgb[3] = { layers[0],layers[1],layers[2] };
	mask = layers[3]; //png's alpha channel used as mask
	merge(rgb, 3, *transp);  //put together the RGB channels, now transp insn't transparent 
	try {
		(*transp).copyTo((*frame).rowRange(yPos, yPos + (*transp).rows).colRange(xPos, xPos + (*transp).cols), mask);
	} catch (std::exception e) {
		std::cout << e.what();
	}
}