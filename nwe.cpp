//--------------------------------------------------------------- 
//【関数名　】：cv_ColorExtraction 
//【処理概要】：色抽出 
//【引数　　】：src_img        = 入力画像(8bit3ch) 
//　　　　　　：dst_img        = 出力画像(8bit3ch) 
//　　　　　　：code        = 色空間の指定（CV_BGR2HSV,CV_BGR2Labなど）
//　　　　　　：ch1_lower    = ch1のしきい値(小)
//　　　　　　：ch1_upper    = ch1のしきい値(大)
//　　　　　　：ch2_lower    = ch2のしきい値(小)
//　　　　　　：ch2_upper    = ch2のしきい値(大)
//　　　　　　：ch3_lower    = ch3のしきい値(小)
//　　　　　　：ch3_upper    = ch3のしきい値(大)
//【戻り値　】：なし 
//【備考　　】：lower <= upperの場合、lower以上upper以下の範囲を抽出、
//　　　　　　：lower >  upperの場合、upper以下lower以上の範囲を抽出します。
//--------------------------------------------------------------- 

#include <opencv2/core/core.hpp>

#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#include <opencv2/opencv.hpp>

#include<cv.h>

using namespace std;

void cvColorExtraction(IplImage* src_img, IplImage* dst_img,
		       int code,
		       int ch1_lower, int ch1_upper,
		       int ch2_lower, int ch2_upper,
		       int ch3_lower, int ch3_upper
		       ){

  int i, k;

  IplImage *Color_img;
  IplImage *ch1_img, *ch2_img, *ch3_img;
  IplImage *Mask_img;

  int lower[3];
  int upper[3];
  int val[3];

  CvMat *lut;

  //codeに基づいたカラー変換
  Color_img = cvCreateImage(cvGetSize(src_img), src_img->depth, src_img->nChannels);
  cvCvtColor(src_img, Color_img, code);

  //3ChのLUT作成
  lut = cvCreateMat(256, 1, CV_8UC3);

  lower[0] = ch1_lower;
  lower[1] = ch2_lower;
  lower[2] = ch3_lower;

  upper[0] = ch1_upper;
  upper[1] = ch2_upper;
  upper[2] = ch3_upper;

  for (i = 0; i < 256; i++){
    for (k = 0; k < 3; k++){
      if (lower[k] <= upper[k]){
	if ((lower[k] <= i) && (i <= upper[k])){
	  val[k] = 255;
	}
	else{
	  val[k] = 0;
	}
      }
      else{
	if ((i <= upper[k]) || (lower[k] <= i)){
	  val[k] = 255;
	}
	else{
	  val[k] = 0;
	}
      }
    }
    //LUTの設定
    cvSet1D(lut, i, cvScalar(val[0], val[1], val[2]));
  }

  //3ChごとのLUT変換（各チャンネルごとに２値化処理）
  cvLUT(Color_img, Color_img, lut);
  cvReleaseMat(&lut);

  //各チャンネルごとのIplImageを確保する
  ch1_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
  ch2_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
  ch3_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);

  //チャンネルごとに二値化された画像をそれぞれのチャンネルに分解する
  cvSplit(Color_img, ch1_img, ch2_img, ch3_img, NULL);

  //3Ch全てのANDを取り、マスク画像を作成する。
  Mask_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
  cvAnd(ch1_img, ch2_img, Mask_img);
  cvAnd(Mask_img, ch3_img, Mask_img);

  //入力画像(src_img)のマスク領域を出力画像(dst_img)へコピーする
  cvZero(dst_img);
  cvCopy(src_img, dst_img, Mask_img);

  //解放
  cvReleaseImage(&Color_img);
  cvReleaseImage(&ch1_img);
  cvReleaseImage(&ch2_img);
  cvReleaseImage(&ch3_img);
  cvReleaseImage(&Mask_img);

}

int main(int argc, char** argv){

  cvNamedWindow("Example2-in", CV_WINDOW_AUTOSIZE);
  cvNamedWindow("Example2-out", CV_WINDOW_AUTOSIZE);
  IplImage* src_img = cvLoadImage("/usr/local/share/OpenCV/samples/c/fruits.jpg");
  cvShowImage("Example2-in", src_img);

  IplImage* dst_img = cvCreateImage(cvGetSize(src_img), src_img->depth, src_img->nChannels);
  cvColorExtraction(src_img, dst_img, CV_BGR2HSV, 30, 80, 80, 255, 0, 255);
  cvShowImage("Example2-out", dst_img);

  cvWaitKey(0);
  cvDestroyWindow("Example2-in");
  cvDestroyWindow("Example2-out");
  return 0;
}
