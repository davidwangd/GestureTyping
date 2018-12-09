#include "Listener.h"
#include <iostream>
#include <cstring>
using namespace std;
using namespace Leap;

const float MinX = -120;
const float MaxX = 120;
const float MinY = 250;
const float MaxY = 120;
const int WIDTH = 1200;
const int HEIGHT = 313;

const char ListenerMode[10][20] = {
	"Undetected!",
	"Typing",
	"Selecting",
	"Waiting"
};

void MyListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
}

void MyListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void MyListener::processLeftHand(const Hand& hand) {
	
}

void MyListener::processRightHand(const Hand& hand) {
	this->processBones(hand, 1);
	// 没有找到三个指头中的任何一个
	if (findBones[1] == 0 || findBones[2] == 0 || findBones[0] == 0) {
		typeMode = Undetected;
		return;
	}
	Finger indexFinger = hand.finger(1);
	// 拇指和食指之间的距离小于一半食指的长度=>握拳
	float length = 0;
	for (int i = 0;i < 4;i++)
		length += bones[1][1][i].distanceTo(bones[1][1][i + 1]);
	//cout << bones[1][0][4] << " " << bones[1][1][4] << " " << bones[1][2][4] << " " << length << endl;
	if (bones[1][0][4].distanceTo(bones[1][1][4]) < length / 3) {
		typeMode = Waiting;
	}
	else if (bones[1][0][4].distanceTo(bones[1][2][4]) < length / 4 && bones[1][0][4].distanceTo(bones[1][1][4]) > length / 4) {
		typeMode = Typing;
	}
	else if (bones[1][0][4].distanceTo(bones[1][2][4]) > length * 0.4 && bones[1][0][4].distanceTo(bones[1][1][4]) > length / 4){
		typeMode = Selecting;
	}
}

void MyListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	std::cout << "Disconnected" << std::endl;
}

void MyListener::onExit(const Controller& controller) {
	std::cout << "Exited" << std::endl;
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

	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;
		if (hand.isLeft()) processLeftHand(hand);
		else processRightHand(hand);
	}
	if (preMode != typeMode) {
		cout << ListenerMode[this->typeMode] << endl;
		board.setGesture(this->typeMode - 1);
	}
	if (this->typeMode == Typing && this->preMode == Waiting) {
		cout << "Position: " << this->bones[1][1][4] << endl;
	}

	if (this->typeMode != Undetected) {
		int x = (this->bones[1][1][4].x - MinX) / (MaxX - MinX) * WIDTH;
		int y = (this->bones[1][1][4].y - MinY) / (MaxY - MinY) * HEIGHT;
		x = slice(x, 0, WIDTH);
		y = slice(y, 0, HEIGHT);
		board.setPosXY(x, y);
	}

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