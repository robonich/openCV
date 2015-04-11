#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>



int main(int argc, char **argv)

{

  char window_capture[] = "capture"; // キャプチャした画像用

  cv::namedWindow(window_capture, CV_WINDOW_AUTOSIZE);

  char window_output[] = "output"; // 結果表示用

  cv::namedWindow(window_output, CV_WINDOW_AUTOSIZE);

  //カメラのオープン

  cv::VideoCapture cap;

  cap.open(0);

  //エラー処理

  if (!cap.isOpened()) {

    std::cout << "cannot find a camera." << std::endl;

    cv::waitKey(5000);

    return -1;

  }

  int lower[3];
  int upper[3];
  
  cv::Mat lut = cv::Mat(256, 1, CV_8UC3);
  
  cv::Mat frame, dst;

  const int ch1Lower = 150, ch2Lower = 100, ch3Lower = 70;
  const int ch1Upper = 180, ch2Upper = 255, ch3Upper = 255;

  lower[0] = ch1Lower;
  lower[1] = ch2Lower;
  lower[2] = ch3Lower;
    
  upper[0] = ch1Upper;
  upper[1] = ch2Upper;
  upper[2] = ch3Upper;
   
  
  while (1) {

    cap >> frame;                                       //画像取り込み
    
    cv::Mat colorImage, colorImage2;
    cv::cvtColor(frame, colorImage, CV_RGB2HSV);
 
    for (int i = 0; i < 256; i++){
      for (int k = 0; k < 3; k++){
	if (lower[k] <= upper[k]){
	  if ((lower[k] <= i) && (i <= upper[k])){
	    lut.data[i*lut.step+k] = 255;
	  }else{
	    lut.data[i*lut.step+k] = 0;
	  }
	}else{
	  if ((i <= upper[k]) || (lower[k] <= i)){
	    lut.data[i*lut.step+k] = 255;
	  }else{
	    lut.data[i*lut.step+k] = 0;
	  }
	}
      }
    }
    
    //LUTを使用して二値化
    cv::LUT(colorImage, lut, colorImage2);
    
    //Channel毎に分解
    std::vector<cv::Mat> planes;
    cv::split(colorImage2, planes);
    
    //マスクを作成
    cv::Mat maskImage, maskImage2;
    cv::bitwise_and(planes[0], planes[1], maskImage);
    cv::bitwise_and(maskImage, planes[2], maskImage2);
    
    //出力
    cv::Mat maskedImage;
    frame.copyTo(maskedImage, planes[1]);
    dst = maskedImage;
 

    // 画像を表示する

    cv::imshow(window_capture, frame);

    cv::imshow(window_output, dst);

    int key = cv::waitKey(5);

    if (key == 'q') {

      break;

    }

  }

  return 0;

}
