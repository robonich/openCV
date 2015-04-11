PREFIX = /usr/local

CXXFLAGS = -g -O0 -Wall -Werror $(INCLUDES)
INCLUDES = -I$(PREFIX)/include/opencv -I$(PREFIX)/include/opencv2
#リンカーの場所を示す。-L
LDFLAGS = -L$(PREFIX)/lib
#リンカーに含まれる名前の一部を指定して探してくれる。-l
LDLIBS = -lopencv_highgui -lopencv_core -lopencv_calib3d -lopencv_contrib -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videostab


TARGET : \
        example_2-1 \
        example_2-2 \
        example_2-3 \
        example_2-4 \
	nwe \
        example_2-5 \
        example_2-6 \
        example_2-9 \
        example_2-10 \
	binarization \

all :
	$(TARGET)

clean : 
	$(RM) $(TARGET) *.o

depend :
	 makedepend -Y -- $(INCLUDES) -- $(wildcard *.h *.c)
