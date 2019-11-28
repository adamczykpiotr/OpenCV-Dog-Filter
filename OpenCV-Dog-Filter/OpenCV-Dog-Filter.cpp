#include "CascadeClasifier.h"
#include "Timer.h"
#include <iostream>
#include <vector>
#include <fstream>

int main(int argc, const char** argv) {

#ifdef DEBUG
	const int argCount = 4;
	const char* argValues[argCount];
	argValues[0] = argv[0];
	argValues[1] = "0";
	argValues[2] = "ignore";
	argValues[3] = "1";

	argv = argValues;
	argc = argCount;

#endif // DEBUG

	if (argc > 1 && strcmp(argv[1], "--help") == 0 || argc == 1) {
		std::cout << R"(
	This is snapchat-like dog filter. Parameters:
	1. Video or single image (-1) / Camera (ID of selected camera input, default = 0)
	2. Path to file (only if in Video / photo mode, otherwise ignored)
	3. Show elipsis around each face and eye (1 = true, 0 = false)	
)";
		return 0;
	}
	
	auto cc = CascadeClasifier();
	cc.init();

	cv::VideoCapture capture;

	int argv1 = strtol(argv[1], NULL, 10);

	if (argv1 == -1 && argc > 2 && strlen(argv[2]) > 0) {
		std::cout << "Path: " << argv[2] << "\n";
		capture = cv::VideoCapture(argv[2]);
	} else {
		capture.open(argv1);
	}

	if (!capture.isOpened()) {
		std::cout << "Error opening video stream. Exiting...\n";
		return -1;
	} 	else {
		std::cout << "Opening stream with " << capture.get(cv::CAP_PROP_FPS) << " fps.\n";
	}

	if (argc > 3 && strcmp(argv[3], "1") == 0) {
		cc.defaults.drawElipsis = true;
	}

	cv::Mat frame;

	while (capture.read(frame)) {
		Timer timer("Loop iteration"); //measure time

		if (frame.empty()) {
			std::cout << "End of stream.\n";
			break;
		}

		cc.setFrame(&frame);
		cc.detect();

		if (cv::waitKey(1) == static_cast<unsigned short>('q')) {
			break;
		}
	}

	std::cin.get();

	return 0;
}

