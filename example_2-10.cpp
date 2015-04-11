//変換されたものが表示できない。

/*!
  \example example_2-10.cpp 動画の変換

  \author Satofumi KAMIMURA

  $Id$
*/

#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;


int main(int argc, char *argv[])
{
    if (argc <= 2) {
        cout << "usage: " << argv[0] << " <input movie> <output movie>" << endl;
        return 1;
    }

    const char* input_movie_file = argv[1];
    const char* output_movie_file = argv[2];
    CvCapture* capture = cvCreateFileCapture(input_movie_file);
    if (!capture) {
        cout << "cvCreateFileCapture(" << input_movie_file << "): fail."
             << endl;
        return 1;
    }

    IplImage* bgr_frame = cvQueryFrame(capture);
    double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    CvSize size =
        cvSize(cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
               cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
    CvVideoWriter* writer =
        cvCreateVideoWriter(output_movie_file,
                            CV_FOURCC('M', 'J', 'P', 'G'), fps, size);
    IplImage* logpolar_frame = cvCreateImage(size, IPL_DEPTH_8U, 3);
//画像を対数極座標（Log-Polar）空間に再マッピングする(人の網膜のモデル化）
/*

void cvLogPolar( const CvArr* src, CvArr* dst,
                 CvPoint2D32f center, double M,
                 int flags=CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS );

src
    入力画像． 
dst
    出力画像． 
center
    出力精度が最大となるような変換の中心座標． 
M
    スケーリング係数の大きさ．以下を参照． 
flags
    補間方法（CvResizeを参照）と，以下に示すオプションフラグの組み合わせ．

        CV_WARP_FILL_OUTLIERS - 出力画像の全ピクセルの値を埋める．対応ピクセルが入力画像外であるようなピクセルである場合は， 0がセットされる．
        CV_WARP_INVERSE_MAP - このフラグは map_matrixが出力画像から入力画像への逆変換のための行列であることを意味するので，直接ピクセル補間に用いることができる．これがセットされていない場合，この関数は map_matrix を使って逆変換を計算する． 
*/
    while ((bgr_frame = cvQueryFrame(capture)) != NULL) {
        cvLogPolar(bgr_frame, logpolar_frame,
                   cvPoint2D32f(bgr_frame->width / 2,
                                bgr_frame->height / 2),
                   40, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
        cvWriteFrame(writer, logpolar_frame);
    }

  
    cvReleaseVideoWriter(&writer);
    cvReleaseImage(&logpolar_frame);
    cvReleaseCapture(&capture);

    return 0;
}
