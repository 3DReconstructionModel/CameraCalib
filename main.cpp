#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
    int num_imgs = 14;
    string prepath = "/home/pelayo/Documentos/Images/Calib/im";
    string extension = ".jpg";
    vector < Mat > vector_im, corners_im, gray_im;
    Mat im,gray;

    float measure = 22.5;
    int board_w=7;
    int board_h=9;
    Size patternsize(board_w,board_h);
    vector < Point2f > corners;
    vector< vector< Point2f> > image_points;
    vector< vector< Point3f> > object_points;

    Size imageSize;
   for(int i = 1; i <= num_imgs; i++){
        stringstream ss;
        ss << i;
        string total = prepath +ss.str()+ extension;
        im = imread(total, CV_LOAD_IMAGE_COLOR);
        gray = imread(total, CV_LOAD_IMAGE_GRAYSCALE);
        //cvtColor(im, gray, CV_RGB2GRAY);
        if(!im.data){
            cout << "Error loading image" << endl;
            return 1;
        }
        vector_im.push_back(im);
        gray_im.push_back(gray);
    }
   imageSize = Size(vector_im[0].cols, vector_im[0].rows);
   for(int k = 0; k < num_imgs; k++){
       //Calculate the corners
       bool patternfound = false;
       patternfound =  findChessboardCorners(gray_im[k],patternsize,corners,CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
                                                  + CALIB_CB_FAST_CHECK);
       if(patternfound){
           //corner point refine
              cornerSubPix(gray_im[k], corners, Size(11,11), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));
              //draw corner
              drawChessboardCorners(vector_im[k], patternsize, corners, patternfound);
              if(corners.size() == board_w*board_h)
              {
               vector< Point2f> v_tImgPT;
               vector< Point3f> v_tObjPT;
               //save 2d coordenate and world coordinate
               for(int j=0; j< corners.size(); ++j)
               {
                Point2f tImgPT;
                Point3f tObjPT;

                tImgPT.x = corners[j].x;
                tImgPT.y = corners[j].y;

                tObjPT.x = j%board_w*measure;
                tObjPT.y = j/board_w*measure;
                tObjPT.z = 0;

                v_tImgPT.push_back(tImgPT);
                v_tObjPT.push_back(tObjPT);
               }
               image_points.push_back(v_tImgPT);
               object_points.push_back(v_tObjPT);
              }

       }
    }

   //calibration part

      Mat K;
      Mat D;
      vector< Mat > rvecs, tvecs;
      int flag = 0;
      flag |= CV_CALIB_FIX_K4;
      flag |= CV_CALIB_FIX_K5;
      calibrateCamera(object_points, image_points, vector_im[0].size(), K, D, rvecs, tvecs, flag);

  //Write the necessary parameters to a file
    ofstream myfile;
      myfile.open ("parameters.txt");
      myfile << "Camera Matrix:\n " << K << "\n"<<"Distortion Matrix: \n" << D<< "\n" << "Rotation vectors: \n";
                for(int u = 0; u < rvecs.size(); u++){
                    stringstream dd;
                    dd << (u + 1);
                    myfile<< "Image"+ dd.str()+ "\t" << rvecs[u] << endl;
                }
      myfile << "Translation vectors:" << "\n";
      for(int f = 0; f < tvecs.size(); f++){
          stringstream dd;
          dd << (f + 1);
          myfile<< "Image"+ dd.str()+ "\t" << rvecs[f] << endl;
      }
      myfile.close();

    return 0;
}
