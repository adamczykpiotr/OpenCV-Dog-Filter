# OpenCV-Dog-Filter
Filter only             |  Filter with ellipse
:-------------------------:|:-------------------------:
![](https://github.com/adamczykpiotr/OpenCV-Dog-Filter/raw/master/me.png)  |  ![](https://github.com/adamczykpiotr/OpenCV-Dog-Filter/raw/master/me_ellipsis.png)




Simple OpenCV app written in C++ that applies dog filter over each face found in a frame. [Cascade Classifier](https://docs.opencv.org/4.1.2/db/d28/tutorial_cascade_classifier.html) is used in order to detect faces and eyes.

## Usage:
```
OpenCV-Dog-Filter.exe -1 /footage.mp4 1
```
* First argument: Webcam ID (default 0) or -1 (photo/video) (mandatory)
* Second argument: Video / image path (ignored in webcam mode) (can be omitted)
* Third argument: 1 (show ellipsis around each face and eye) or 0 (defalut; do not) (can be omitted)

[Download link](https://github.com/adamczykpiotr/OpenCV-Dog-Filter/releases/tag/v1.0)

Help:
```
OpenCV-Dog-Filter.exe --help
```

If you do not have Visual Studio Code installed it is possible that you'll encounter missing _vcruntime140_1.dll_  file, to resolve that issue just install _Visual C++ Redistributable for Visual Studio 2015_

## Compilation:
Visual Studio 2019 was used with OpenCV 4.1.1. It is possible (and highly recommended) to use newer versions of OpenCV but that requires changing every occurance of 'opencv_world411.lib' with applicable filename ex. 'opencv_world412.lib' for version 4.1.2. 'C:\opencv' path was used for OpenCV libraries. 

Once you finish debugging remember to switch to _Release_ mode, it is **several times faster**  than debug.

## Thoughts:
Thanks to OpenCV implementation app scales nearly perfectly with additional threads (up to around 8 _t_). Using my AMD Ryzen 3900X (12c24t) does not yeld in higher overall performance (average time per loop iteration is nearly the same as 6c12t equivalent processor)\
\
![CPU usage](https://github.com/adamczykpiotr/OpenCV-Dog-Filter/raw/master/cpu.png)
