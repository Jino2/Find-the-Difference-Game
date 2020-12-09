#include<opencv2/opencv.hpp>
#include<iostream>
#include<string.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <windows.h>
#include <io.h>	
#include <process.h>
#include <time.h>
#pragma warning(disable : 4996) 



#define MAIN_WIDTH 600
#define MAIN_HEIGHT 800
#define LITTLE_WIDTH 50
#define LITTLE_HEIGHT 50
#define MODULE_ROW 8
#define MODULE_COL 6
#define MAX_LITTLE 22
#define MAX_BACKGROUND 5


#define minHard 60
#define minNormal 40
#define minEasy 20
using namespace std;
using namespace cv;

// structures
typedef struct module {
    int width;
    int height;
    int x;
    int y;
    bool filled; // check little image is  filled in this module
} Module;

typedef struct imgInfo {
    int x;
    int y;
} ImgInfo;

// Global vars,,
const string PATH = "D:\\images\\";
const string BACKGROUND_PATH = PATH + "BACKGROUND\\";
const string LITTLE_PATH = PATH + "LITTLE\\";


Mat origin_image, added_image;
Mat main_image;
Mat combine_image;
Mat answer_image;
int little_num;
Mat little_images[10];
Module modules[MODULE_ROW][MODULE_COL];
ImgInfo infos[10];

int remainTime;
int countItem;

//function
Mat puangimg();
Mat whiteimg();
void gamebigimg(Mat* a);
Mat imgconq(Mat a, Mat b);
void initImage(int a);
void combine(int a);
void randLittleInfo(int a);
void makeline(Mat image);
void check_event(int event, int x, int y, int flags, void* userdata);
void game_end();


unsigned _stdcall Thread_Time(void* arg) {

    char strTime[13];

    while (true) {
        game_end();
        sprintf(strTime, "Time : %02dsec", remainTime);
        namedWindow("statuspage", 1);
        Mat timerImg = imread(PATH + "whiteall.jpg", 1);
        putText(timerImg, strTime, Point(10, 30), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2, 4);
        putText(timerImg, to_string(countItem), Point(10, 60), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2, 4);

        imshow("statuspage", timerImg);
        waitKey(800);
        Sleep(200);
        destroyWindow("statuspage");
        if (remainTime > 0) remainTime--;
        else break;

    }

    return 0;
}

void game_end() {
    // 승리
    if (remainTime > 0) {
        if (countItem == 0) {
            destroyWindow("statuspage");
            namedWindow("statuspage2", 1);
            Mat timerImg = imread(PATH + "whiteall.jpg", 1);
            putText(timerImg, "You Win!", Point(10, 30), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2, 4);
            //putText(timerImg, to_string(countItem), Point(10, 60), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2, 4);
            imshow("statuspage2", timerImg);
            waitKey(0);
            destroyAllWindows();
        }
    }
    // 패배
    else {
        destroyWindow("statuspage");
        namedWindow("statuspage1", 1);
        Mat timerImg = imread(PATH + "whiteall.jpg", 1);
        putText(timerImg, "TimeOver!", Point(10, 30), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2, 4);
        putText(timerImg, to_string(countItem), Point(10, 60), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2, 4);
        imshow("statuspage1", timerImg);
        waitKey(0);

        destroyAllWindows();
    }
}


int main() {
    Mat puang;
    //Mat img[3];
    //gamebigimg(img);
    Mat white = whiteimg();
    //Mat whiteall = imread(PATH + "whiteall.jpg", 1);
    int time;
    //img[0] = imread(PATH + "1.jpg", 1);
    //img[1] = imread(PATH + "2.jpg", 1);
    //img[2] = imread(PATH + "3.jpg", 1);
    puang = puangimg();
    namedWindow("startpage", 1);
    imshow("startpage", puang);
    while (1) {
        int key = waitKey();
        if (key != NULL) {
            imshow("startpage", white);
            break;
        }
    }
    int check = 1;
    while (1) {
        int key = waitKey();
        if (key == '1') {
            destroyAllWindows();
            little_num = 5;
            initImage(little_num);
            randLittleInfo(little_num);
            combine(little_num);
            origin_image = imgconq(main_image, combine_image);
            remainTime = 60;
            countItem = 5;
            break;
        }
        else if (key == '2') {
            destroyAllWindows();
            little_num = 10;
            initImage(little_num);
            randLittleInfo(little_num);
            combine(little_num);
            origin_image = imgconq(main_image, combine_image);
            remainTime = 40;
            countItem = 10;
            break;
        }
        else if (key == '3') {
            destroyAllWindows();
            little_num = 10;
            initImage(little_num);
            randLittleInfo(little_num);
            combine(little_num);
            origin_image = imgconq(main_image, combine_image);
            remainTime = 20;
            countItem = 10;
            break;
        }
        else if (key != NULL) {
            if (check < 3) {
                putText(white, "please follow messages", Point(100, 160 + check * 20), 1, 1, Scalar(0, 0, 0));
                imshow("startpage", white);
                check++;
            }
            else if (check == 3) {
                putText(white, "one more error -> program end", Point(100, 160 + check * 20), 1, 1, Scalar(0, 0, 0));
                imshow("startpage", white);
                check++;
            }
            else {
                destroyAllWindows();
                return 0;
            }
        }
    }
    namedWindow("Imagepage", 1);
    //whiteall에다가 timer 넣기
    _beginthreadex(NULL, 0, Thread_Time, 0, 0, NULL);
    imshow("Imagepage", origin_image);
    setMouseCallback("Imagepage", check_event);
    waitKey(0);
}

Mat puangimg() {
    Mat img;

    img = imread(PATH + "puang.jpg", 1);
    putText(img, "press any button", Point(30, 250), 1, 1, Scalar(0, 0, 0));
    return img;
}

Mat whiteimg() {
    Mat img;
    string white = "white.jpg";
    img = imread(PATH + white, 1);
    //img = imread(PATH + "white.jpg", 1);
    putText(img, "press keyboard", Point(100, 100), 1, 1, Scalar(0, 0, 0));
    putText(img, "easy : 1", Point(100, 120), 1, 1, Scalar(0, 0, 0));
    putText(img, "normal : 2", Point(100, 140), 1, 1, Scalar(0, 0, 0));
    putText(img, "hard : 3", Point(100, 160), 1, 1, Scalar(0, 0, 0));
    return img;
}

Mat imgconq(Mat a, Mat b) {
    Mat img(a.rows, a.cols * 2, a.type());
    a.copyTo(img(Rect(0, 0, a.cols, a.rows)));
    b.copyTo(img(Rect(a.cols, 0, a.cols, a.rows)));
    return img;
}

void initImage(int littlenum) {
    /*
    * load original images
    * main image, little images
    * and preprocessing with them
    */
    srand((unsigned int)time(NULL));
    string index = to_string(rand() % MAX_BACKGROUND);
    string jpg_name = BACKGROUND_PATH + index + ".jpg";
    string png_name = BACKGROUND_PATH + index + ".png";
    Mat temp;
    if (_access(jpg_name.c_str(), 0) == 0)
    {
        temp = imread(jpg_name, IMREAD_COLOR);
        resize(temp, main_image, Size(MAIN_WIDTH, MAIN_HEIGHT), 0, 0);

    }
    else if (_access(png_name.c_str(), 0) == 0)
    {
        temp = imread(png_name, IMREAD_COLOR);
        resize(temp, main_image, Size(MAIN_WIDTH, MAIN_HEIGHT), 0, 0);

    }
    else {
        //nofile
        return;
    }
    int indexes[10] = { 0, };
    //little_images = (Mat*)malloc(sizeof(Mat) * LITTLE_NUM);
    for (int i = 0; i < littlenum; i++)
    {
        int t = rand() % MAX_LITTLE;
        bool pass = false;
        for (int j = 0; j < i; j++) {
            if (indexes[j] == t) {
                pass = true;
                break;
            }
        }
        if (pass) {
            i--;
            continue;
        }
        indexes[i] = t;
        printf("indexes: %d\n", t);

        index = to_string(t);
        string jpg_name = LITTLE_PATH + index + ".jpg";
        string png_name = LITTLE_PATH + index + ".png";

        if (_access(jpg_name.c_str(), 0) == 0)
        {
            temp = imread(jpg_name, IMREAD_COLOR);
            resize(temp, *(little_images + i), Size(LITTLE_WIDTH, LITTLE_HEIGHT), 0, 0);

        }
        else if (_access(png_name.c_str(), 0) == 0)
        {
            temp = imread(png_name, IMREAD_COLOR);
            resize(temp, *(little_images + i), Size(LITTLE_WIDTH, LITTLE_HEIGHT), 0, 0);

        }
        else {
            //nofile
            return;
        }
    }
}
void combine(int littlenum) {
    /*
    * randomly place N processed little image to each module
    */
    combine_image = main_image.clone();

    for (int i = 0; i < littlenum; i++) {
        Mat grayimage, mask, mask_inv, ROI, img_bg, img_fg, result;
        cvtColor(little_images[i], grayimage, COLOR_BGR2GRAY);
        threshold(grayimage, mask, 245, 255, THRESH_BINARY);
        bitwise_not(mask, mask_inv);
        cout << "y,x: " << infos[i].y << " " << infos[i].x << endl;
        ROI = combine_image(Rect(infos[i].x, infos[i].y, LITTLE_WIDTH, LITTLE_HEIGHT));
        cout << "ROI shape: " << ROI.size() << endl;
        bitwise_and(ROI, ROI, img_bg, mask = mask);
        bitwise_and(little_images[i], little_images[i], img_fg, mask = mask_inv);
        add(img_bg, img_fg, ROI);
        GaussianBlur(ROI, ROI, Size(5, 5), 0, 0);
    }
}
void randLittleInfo(int littlenum) {
    srand((unsigned int)time(NULL));
    /*
    * divide main image to L x M modules
    * randomly pick N modules from all modules
    */
    //cout << ceil(MAIN_HEIGHT / MODULE_ROW) << endl;
    for (int y = 0; y < MODULE_ROW; y++) {
        for (int x = 0; x < MODULE_COL; x++) {
            modules[y][x].filled = false;
            if (x == MODULE_COL - 1) {
                modules[y][x].width = MAIN_WIDTH - ceil(MAIN_WIDTH / MODULE_COL) * x;
            }
            else {
                modules[y][x].width = ceil(MAIN_WIDTH / MODULE_COL);

            }
            modules[y][x].x = ceil(MAIN_WIDTH / MODULE_COL) * x;
            if (y == MODULE_ROW - 1) {
                modules[y][x].height = MAIN_HEIGHT - ceil(MAIN_HEIGHT / MODULE_ROW) * y;
            }
            else {
                modules[y][x].height = ceil(MAIN_HEIGHT / MODULE_ROW);
            }
            modules[y][x].y = ceil(MAIN_HEIGHT / MODULE_ROW) * y;
            //cout.width(10);
            //cout << modules[y][x].width << " " << modules[y][x].height;
        }
        //cout << endl;
    }

    //randomly pick N modules
    int indexes[10];
    int i = 0;
    while (i < littlenum) {
        int temp = rand() % (MODULE_ROW * MODULE_COL);
        if (i == 0) {
            indexes[i++] = temp;
        }
        else {
            for (int j = 0; j < i; j++) {
                if (indexes[j] == temp) break;
                if (j == i - 1) indexes[i++] = temp;
            }
        }

    }
    // set info
    for (int i = 0; i < littlenum; i++) {
        int row = indexes[i] / MODULE_COL;
        int col = indexes[i] % MODULE_COL;
        cout << "index: " << indexes[i] << " row: " << row << " col: " << col << endl;
        modules[row][col].filled = true;
        infos[i].x = (rand() % (modules[row][col].width - (LITTLE_WIDTH - 1)))
            + modules[row][col].x;
        infos[i].y = (rand() % (modules[row][col].height - (LITTLE_HEIGHT - 1)))
            + modules[row][col].y;
    }

}
void makeline(Mat image) {
    uchar* img_data = image.data;
    int height = image.rows;
    int width = image.cols;
    int line = width / 2;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            if (w == line) {
                img_data[h * width * 3 + w * 3] = 0;
                img_data[h * width * 3 + w * 3 + 1] = 0;
                img_data[h * width * 3 + w * 3 + 2] = 0;
            }
        }
    }

}
void check_event(int event, int x, int y, int flags, void* userdata) {
    int module_width = MAIN_WIDTH / MODULE_COL;
    int module_height = MAIN_HEIGHT / MODULE_ROW;
    int mouse_col;
    int check;
    if (x < origin_image.cols / 2) {
        mouse_col = x / module_width;
        check = 1;
    }
    else {
        mouse_col = (x - origin_image.cols / 2) / module_width;
        check = 2;
    }
    int mouse_row = y / module_height;
    if (event == EVENT_LBUTTONDOWN) {
        if (modules[mouse_row][mouse_col].filled) {
            circle(origin_image, Point(x, y), 20, Scalar(0, 0, 255), 2, 8, 0);
            if (check == 1) circle(origin_image, Point(x + origin_image.cols / 2, y), 20, Scalar(0, 0, 255), 2, 8, 0);
            else circle(origin_image, Point(x - origin_image.cols / 2, y), 20, Scalar(0, 0, 255), 2, 8, 0);
            modules[mouse_row][mouse_col].filled = false;
            countItem--;
            printf("ok");
        }
        else {//틀렸을 때
            remainTime = remainTime - 5;
        }
        imshow("Imagepage", origin_image);
        waitKey(0);
        printf("%d %d\n", mouse_row, mouse_col);
    }
}