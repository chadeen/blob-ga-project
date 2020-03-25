#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

const string W_NAME = "Main";
constexpr unsigned int W_WIDTH = 800;
constexpr unsigned int W_HEIGHT = 600;

struct Ball2D {
	int x;
	int y;
	int radius;
};

vector<Ball2D> balls(0);
Mat wMat(W_HEIGHT, W_WIDTH, CV_8UC3);
bool mousePressed = false;
int mouseX = 0, mouseY = 0, radius = 20;

void mouseCallback(int event, int x, int y, int flags, void *userdata = 0) {
	if(event == EVENT_FLAG_LBUTTON) {
		mouseX = x;
		mouseY = y;
		cout << "(" << x << ", " << y << ")" << endl;
		mousePressed = true;
	}
}

void draw() {
	wMat = Mat::zeros(W_HEIGHT, W_WIDTH, CV_8UC3);
	for(Ball2D ball : balls) {
		circle(wMat, Point(ball.x, ball.y), ball.radius, Scalar(0, 0, 255));
	}
	imshow(W_NAME, wMat);
}

int main(int argc, char *argv[]) {
	namedWindow(W_NAME);
	resizeWindow(W_NAME, W_WIDTH, W_HEIGHT);
	setMouseCallback(W_NAME, mouseCallback);

	int64 t1, t2;
	int key = -1;
	bool done = false;
	t1 = getTickCount();
	while(!done) {
		if(getWindowProperty(W_NAME, WND_PROP_AUTOSIZE) == -1) {
			done = true;
			break;
		}

		if((mousePressed == true) || ((key = waitKey(10)) != -1)) {
			if(mousePressed == true) {
				balls.push_back({mouseX, mouseY, radius});
				mousePressed = false;
			}
			if(key != -1) {
				switch(key) {
					case 27:
					case 'q':
						done = true; break;
					case 'f':
						cout << "You're paying respects...RIP" << endl; break;
					case 32 :
						balls.clear(); break;
					default:
						break;
				}
				key = -1;
			}
		}

		t2 = getTickCount();
		for(Ball2D & ball : balls) {
			ball.y = (ball.y + (t2 - t1)/100000) % W_HEIGHT;
		}

		draw();
	}
	destroyAllWindows();
	cout << "Bye !" << endl;
	return 0;
}