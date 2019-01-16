#include "Listener.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <time.h>
using namespace std;
using namespace Leap;

const float MinX = -150;
const float MaxX = 150;
const float MinY = 300;
const float MaxY = 100;
const int WIDTH = 1200;
const int HEIGHT = 313;
int counter = 0;
clock_t mytime;

const int LeftHandIndex = 0;
const int RightHandIndex = 1;

enum {
	Thumb = 0,
	IndexFinger = 1,
	MiddleFinger = 2,
	RingFinger = 3,
	LittleFinger = 4
};

const char ListenerMode[10][20] = {
	"Undetected!",
	"Typing",
	"Selecting",
	"Waiting"
};

void MyListener::processLeftHand(const Hand& hand) {
	this->leftHandMode = 0;
	for (int i = 1;i <= 4;i++) {
		if (checkFingerAngle(LeftHandIndex, i) <= 0.2)
			this->leftHandMode++;
		else break;
	}
}

void MyListener::processRightHand(const Hand& hand) {
	this->processBones(hand, RightHandIndex);
	// 没有找到三个指头中的任何一个

	Finger indexFinger = hand.finger(IndexFinger);
	// 计算食指长度
	float length = 0;
	for (int i = 0;i < 4;i++)
		length += bones[RightHandIndex][IndexFinger][i].distanceTo(bones[RightHandIndex][IndexFinger][i + 1]);
	// 以自动机的形式实现状态转移判定。
	if ((typeMode == Undetected || typeMode == Typing) &&
		bones[RightHandIndex][Thumb][4].distanceTo(bones[RightHandIndex][MiddleFinger][4]) > length * 0.4) {
		typeMode = Selecting;
	}
	if (typeMode == Selecting &&
		bones[RightHandIndex][Thumb][4].distanceTo(bones[RightHandIndex][IndexFinger][4]) < length / 4 &&
		bones[RightHandIndex][Thumb][4].distanceTo(bones[RightHandIndex][MiddleFinger][4]) < length / 2) {
		typeMode = Waiting;
	}
	if (typeMode == Waiting &&
		bones[RightHandIndex][Thumb][4].distanceTo(bones[RightHandIndex][IndexFinger][4]) > length * 0.45) {
		typeMode = Typing;
	}

}

inline int slice(int x, int l, int r) {
	if (x < l) return l;
	if (x > r) return r;
	return x;
}

void MyListener::onFrame(const Controller& controller) {

	const Frame frame = controller.frame();
	HandList hands = frame.hands();
	memset(this->findBones, 0, sizeof(this->findBones));
	memset(this->bones, 0, sizeof(this->bones));
	this->preMode = this->typeMode;
	int hasRightHand = 0;
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;
		if (hand.isLeft()) processLeftHand(hand);
		else {
			processRightHand(hand);
			hasRightHand = 1;
		}
	}
	if (hasRightHand == 0) {
		this->typeMode = Undetected;
	}

	if (preMode != typeMode) {
		cout << "[preMode != typeMode]" << ListenerMode[this->typeMode] << endl;
		//if(this->typeMode>=1)
		board.setGesture(this->typeMode - 1);
	}
	if (this->typeMode == Typing && this->preMode == Waiting) {
		cout << "Position: " << this->bones[1][1][4] << endl;
	}
	if (this->typeMode != Undetected) {
		double x = (this->bones[1][1][4].x - MinX) / (MaxX - MinX) * WIDTH;
		double y = (this->bones[1][1][4].y - MinY) / (MaxY - MinY) * HEIGHT;
		x = slice(x, 0, WIDTH);
		y = slice(y, 0, HEIGHT);
		board.setPosXY((int)x, (int)y);
		if (this->typeMode == Typing) {
			this->lock();
			if (this->preMode != Typing) this->points.clear();
			this->points.push_back(Point(x, y));
			this->unlock();
		}
	}
	// Log The PathData
	if (this->typeMode == Selecting && this->preMode == Typing) {
		dump();
	}
}

void MyListener::lock() {
	this->locker.lock();
}
void MyListener::unlock() {
	this->locker.unlock();
}

void MyListener::dump() {
	/*
	ofstream fout("log.txt");
	auto list = this->getSamplePoints();
	printf("%d\n", list.size());
	for (Point x : list) {
	fout << x.x << " " << x.y << endl;
	}
	*/
}

vector<Point> MyListener::getSamplePoints(int nSamplePoints) {
	this->lock();
	vector<Point> cur = this->points;
	this->unlock();

	// 样本过于少不能进行处理
	if (cur.size() <= nSamplePoints / 5)
	{
		//printf("[getSamplePoints] cur.size() %d\n",cur.size());
		return cur;
	}
	vector<Point> ret;
	// 去除距离不正常过的噪声点
	for (int i = 0;i + 2 < cur.size() && cur.size() > nSamplePoints; i++) {
		double d1 = cur[i].distanceToSqr(cur[i + 1]);
		double d2 = cur[i + 1].distanceToSqr(cur[i + 2]);
		double pd = cur[i].distanceToSqr(cur[i + 2]);
		// 杂声点，删除
		if (d1 > pd * 2 && d2 > pd * 2) {
			cur.erase(cur.begin() + i + 1);
		}
	}
	if (cur.size() <= nSamplePoints / 5) return cur;

	// 按照距离进行切片处理
	vector<double> dis = vector<double>(cur.size(), 0);
	for (int i = 1;i < cur.size();i++) {
		dis[i] = dis[i - 1] + cur[i].distanceTo(cur[i - 1]);
	}
	double slicelen = dis[cur.size() - 1] / (nSamplePoints + 1);

	//printf("GetSamples(%d) => %lf\n", nSamplePoints, slicelen);

	double now = 0;
	for (int i = 0;i + 1 < cur.size();) {
		// 将在cur[i]与cur[i+1]中
		// 采用线性插值的方法计算中点
		if (dis[i] <= now && dis[i + 1] >= now) {
			// 在一定范围内不值得插值
			if (dis[i + 1] - dis[i] < 1e-3) {
				ret.push_back((cur[i] + cur[i + 1])*0.5);
			}
			else {
				double f = (now - dis[i]) / (dis[i + 1] - dis[i]);
				ret.push_back(cur[i] * f + cur[i + 1] * (1 - f));
			}
			if (ret.size() == nSamplePoints - 1) break;
			now += slicelen;
		}
		if (dis[i + 1] <= now) i++;
	}
	ret.push_back(cur[cur.size() - 1]);
	return ret;
}

int MyListener::getState()
{
	return this->typeMode - 1;
}

void MyListener::processBones(const Hand &hand, int t) {
	FingerList fingers = hand.fingers();
	for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
		const Finger finger = *fl;
		findBones[t][finger.type()] = 1;
		for (int b = 0; b < 4;b++) {
			Bone::Type boneType = static_cast<Bone::Type>(b);
			Bone bone = finger.bone(boneType);
			this->bones[t][finger.type()][b] = bone.prevJoint();
			if (b == 3) {
				this->bones[t][finger.type()][4] = bone.nextJoint();
			}
		}
	}
}

double MyListener::checkFingerAngle(int HandIndex, int FingerIndex) const {
	// 手指的中间三个骨节
	Vector a = bones[HandIndex][FingerIndex][1];
	Vector b = bones[HandIndex][FingerIndex][2];
	Vector c = bones[HandIndex][FingerIndex][3];
	// 计算夹角的cos
	double cos = (a - b).dot(b - c) / b.distanceTo(a) / b.distanceTo(c);
	// 将弯曲程度化到0~1
	return acos(cos) / acos(-1);
}

// 其他监听函数
// 可以忽略
void MyListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void MyListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	std::cout << "Disconnected" << std::endl;
}

void MyListener::onExit(const Controller& controller) {
	std::cout << "Exited" << std::endl;
}

void MyListener::onFocusGained(const Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void MyListener::onFocusLost(const Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

void MyListener::onDeviceChange(const Controller& controller) {
	std::cout << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void MyListener::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
}

void MyListener::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}

void MyListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
}