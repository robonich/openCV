//参考にしたサイト
//http://carnation.is.konan-u.ac.jp/seminar/colorextraction.htm 特定の色のマスク作成
//http://qiita.com/crakaC/items/65fab9d0b0ac29e68ab6 マスクに色をつける

#include <iostream>
#include <opencv2/opencv.hpp>

#define  LOW_HUE        0              //hueの下限
#define  UP_HUE         15             //hueの上限
#define  LOW_SATURATION 60              //saturation（彩度）の下限
#define  LOW_VALUE      80              //value（明度）の下限

int main(int argc, char **argv)

{
  std::string window_capture = "capture"; // キャプチャした画像用
  cv::namedWindow(window_capture, CV_WINDOW_AUTOSIZE);

  std::string window_output = "output"; // 結果表示用
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

  cv::Mat hsv, frame, hue, hue1, hue2, saturation, value, hue_saturation, mask;          //変数確保

  int low_hue, up_hue, low_saturation, low_value;
  
  cv::createTrackbar("low_hue", window_output, &low_hue, 255);
  cv::createTrackbar("up_hue", window_output, &up_hue, 255);
  cv::createTrackbar("low_saturation", window_output, &low_saturation, 255);
  cv::createTrackbar("low_value", window_output, &low_value, 255);

  while (1) {

    cap >> frame;                                       //画像取り込み

    cv::cvtColor(frame, hsv, CV_BGR2HSV);               // RGB（BGR）画像をHSV画像に変換する（frame → hsv）

    //赤い領域を取得

    // hsvをチャンネル毎にsinglechannelsというMat型の配列に分解して格納する。その結果、singlechannels[0]がHue, singlechannels[1]がSaturation, singlechannels[2]がValueの情報を持つ。

    std::vector<cv::Mat> singlechannels;//Matクラスのベクトルとしてsinglechannelsを定義

    cv::split(hsv, singlechannels);//hsvをsinglechannelsに分解([0]:h, [1]:s,[2]:v)

    //それぞれのチャンネルことに閾値を設定して二値化
    cv::threshold(singlechannels[0], hue1, low_hue, 255, CV_THRESH_BINARY);                 // singlechannels[0]をLOW_HUEを閾値処理して、LOW_HUE以上の部分が255,それ以下の部分が0になるように、hue1に格納する。
    cv::threshold(singlechannels[0], hue2, up_hue, 255, CV_THRESH_BINARY_INV);              // singlechannels[0]をUP_HUEを閾値処理して、UP_HUE以上の部分が0,それ以下の部分が255になるように、hue2に格納する。
    cv::threshold(singlechannels[1], saturation, low_saturation, 255, CV_THRESH_BINARY);    //彩度LOW_SATURATION以上
    cv::threshold(singlechannels[2], value, low_value, 255, CV_THRESH_BINARY);              //明度LOW_VALUE以上

    //条件を満たした領域をoutに設定
    cv::bitwise_and(hue1, hue2, hue);                                                       // hue1とhue2のbitごとのandをとる→hue
    cv::bitwise_and(hue, saturation, hue_saturation);                                       // hueとsaturationのbitごとのandをとる→hue_saturation
    cv::bitwise_and(hue_saturation, value, mask);                              // hue_saturationとvalueのbitごとのandをとる→マスクを作成

    //マスクともとの画像を重ねる
    cv::Mat maskedImage;
    frame.copyTo(maskedImage, mask);

    // 画像を表示する
    cv::imshow(window_capture, mask);
    cv::imshow(window_output, maskedImage)
      ;
    int key = cv::waitKey(5);
    if (key == 'q') {
      break;
    }
  }
  return 0;
}
