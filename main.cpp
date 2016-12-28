#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
    int num_imgs = 14;
    string prepath = "/home/pelayo/Documentos/Images/Calib/im";
    string extension = ".jpg";
    vector < Mat > vector_im, corners_im;
    Mat im;
    Size patternsize(7,9);
    int board_height = 205;
    int board_width = 160;
    float square_size = 22.75;

    vector < Point2f > corners;
    vector< vector< Point3f > > object_points;
    vector< vector< Point2f > > image_points;
   for(int i = 1; i <= num_imgs; i++){
        stringstream ss;
        ss << i;
        string total = prepath +ss.str()+ extension;
        im = imread(total, CV_LOAD_IMAGE_GRAYSCALE);
        if(!im.data){
            cout << "Error loading image" << endl;
            return 1;
        }
        vector_im.push_back(im);
    }
   for(int k = 0; k < num_imgs; k++){
       //Calculate the corners
       bool patternfound = false;
       patternfound =  findChessboardCorners(vector_im[k],patternsize,corners,CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
                                                  + CALIB_CB_FAST_CHECK);
       if(patternfound)
         cornerSubPix(vector_im[k], corners, Size(11, 11), Size(-1, -1),
           TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
         drawChessboardCorners(vector_im[k], patternsize, Mat(corners), patternfound);

    }

    namedWindow("hue",CV_WINDOW_AUTOSIZE);
    imshow("hue",vector_im[0]);
    waitKey(0);
    return 0;
}
