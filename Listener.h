#ifndef LISTENER_H
#define LISTENER_H
#include <Leap.h>
#include "keyboardUI\keyboardUI.h"

class MyListener : public Leap::Listener {
public:
	virtual void onInit(const Leap::Controller&);
	virtual void onConnect(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onExit(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller&);
	virtual void onFocusGained(const Leap::Controller&);
	virtual void onFocusLost(const Leap::Controller&);
	virtual void onDeviceChange(const Leap::Controller&);
	virtual void onServiceConnect(const Leap::Controller&);
	virtual void onServiceDisconnect(const Leap::Controller&);
private:
	void processLeftHand(const Leap::Hand& hand);
	void processRightHand(const Leap::Hand& hand);
	void processBones(const Leap::Hand &hand, int t);
	Leap::Vector bones[2][5][5];	// 左手/右手五个指头的四个关节位置记录
	// 0: 掌骨 -> 4末节
	int findBones[2][5];			// 左手/右手五个指头有没有发现
	enum TypeMode{
		Undetected = 0,				// 未检测到右手
		Typing,						// 当前正在进行打字(食指伸出)
		Selecting,					// 当前正在进行选词(五指张开)
		Waiting 					// 当前正在等待开始打字(握拳状态)
	} typeMode, preMode;
	keyboard board;
};

#endif