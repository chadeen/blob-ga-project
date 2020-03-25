#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <c2ga/Mvec.hpp>

using namespace std;
using namespace cv;
using namespace c2ga;

const string W_NAME = "Main";
constexpr unsigned int W_WIDTH = 800;
constexpr unsigned int W_HEIGHT = 600;

bool mousePressed = false;
int mouseX = 0, mouseY = 0, radius = 20;
int obj_ID = 1;

Mat wMat(W_HEIGHT, W_WIDTH, CV_8UC3);

template<typename T>
Mvec<T> point(const T &x, const T &y) {
	Mvec<T> mv;
	mv[E1] = x;
    mv[E2] = y;
    mv[Ei] = 0.5 * (x*x + y*y);
    mv[E0] = 1.0;
    return mv;
}

template<typename T>
Mvec<T> point(const Mvec<T> &vec){
    return point(vec[E1], vec[E2]);
}

template<typename T>
bool intersect(const Mvec<T> & mv1, const Mvec<T> & mv2) {
	Mvec<double> tmp = (mv1.dual() ^ mv2.dual());
	double norm = tmp.dual().norm();
	cout << norm * norm << endl;
	return (norm * norm >= 0);
}

struct Ball2D {
	int x;
	int y;
	int radius;
	int id;
};

vector<Ball2D> balls(0);
vector<Mvec<double>> objects(0);

bool detect_collisions(Ball2D ball) {
	Mvec<double> mv = objects[ball.id];
	for(int i = 0; i < (int)objects.size(); i++) {
		if(i != ball.id) {
			if(intersect(mv, objects[i])) {
				return true;
			}
		}
	}
	return false;
}

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
	for(Ball2D & ball : balls) {
		circle(wMat, Point(ball.x, ball.y), ball.radius, Scalar(0, 0, 255), FILLED);
	}
	imshow(W_NAME, wMat);
}

int main(int argc, char *argv[]) {
	namedWindow(W_NAME);
	resizeWindow(W_NAME, W_WIDTH, W_HEIGHT);
	setMouseCallback(W_NAME, mouseCallback);

	Mvec<double> ground = point(0., (double)W_HEIGHT) ^ point((double)W_WIDTH, (double)W_HEIGHT);
	objects.push_back(ground);

	int key = -1;
	bool done = false;
	while(!done) {
		if(getWindowProperty(W_NAME, WND_PROP_AUTOSIZE) == -1) {
			done = true;
			break;
		}

		if((mousePressed == true) || ((key = waitKey(10)) != -1)) {
			if(mousePressed == true) {
				balls.push_back({mouseX, mouseY, radius, obj_ID++});
				objects.push_back(
					point(mouseX, mouseY + radius) ^
					point(mouseX, mouseY - radius) ^
					point(mouseX + radius, mouseY)
				);
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
						balls.clear(); objects.clear(); objects.push_back(ground); obj_ID = 1; 
						break;
					default:
						break;
				}
				key = -1;
			}
		}

		for(Ball2D & ball : balls) {
			if(ball.y + ball.radius < W_HEIGHT) {
				ball.y++;
			}
			//ball.y = (ball.y + 1) % W_HEIGHT;
			
			/*
			if(!detect_collisions(ball)) {
				ball.y++;
			}*/
		}

		draw();
	}
	destroyAllWindows();
	cout << "Bye !" << endl;
	return 0;
}