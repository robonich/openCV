#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main( int argc, char** argv )
{
  Mat rgb;

  rgb = imread( "/usr/local/share/OpenCV/samples/c/fruits.jpg", /* 3-channel color image */ 1 );
  namedWindow( "Display Image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO );
  imshow( "Display Image", rgb );

  waitKey(0);

  return 0;
}
