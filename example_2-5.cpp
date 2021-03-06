/*!
  \brief example_2-5.cpp 画像サイズの変更

  \author Satofumi KAMIMURA

  $Id$
*/

#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;


IplImage* do_pyr_down(IplImage* input)
{
#if 0
  assert(((input->width % 2) == 0) && ((input->height % 2) == 0));
#endif

  IplImage* output = cvCreateImage(cvSize(input->width / 2,//サイズだけ小さくする
					  input->height / 2),
				   input->depth,//深さ変わらず
				   input->nChannels);//色のチャネルかな？も変わらず　
  cvPyrDown(input, output);//縮小する
  return output;
}



int main(int argc, char *argv[])
{
  if (argc <= 1) {
    cout << "usage: " << argv[0] << " <image file>" << endl;
    return 1;
  }

  const char* image_file = argv[1];
  IplImage* original = cvLoadImage(image_file);
  IplImage* converted = do_pyr_down(original);

  const char* input_name = "Example 5 in";
  const char* output_name = "Example 5 out"; 
  cvNamedWindow(input_name);
  cvNamedWindow(output_name);
  cvShowImage(input_name, original);
  cvShowImage(output_name, converted);

  // キーの入力待ち
  cvWaitKey(0);
  cvDestroyWindow(input_name);
  cvDestroyWindow(output_name);

  return 0;
}
