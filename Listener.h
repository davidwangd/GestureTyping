#ifndef LISTENER_H
#define LISTENER_H
#include <Leap.h>
#include <vector>
#include <mutex>
#include "Point.h"
#include "keyboardUI/keyboardUI.h"

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
	/**
	 * 返回 nSamplePoints 个 路径上的Sample点
	 * 函数要保证线程安全，不锁死的返回
	 *  onFrame中 我们返回上一次onFrame计算的结果.
	 *  onFrame外 我们返回当前的结果.
	 *  要保证运行效率==
	 */
	std::vector<Point> getSamplePoints(int nSamplePoints = 50) ;
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
	int leftHandMode;
	keyboard board;
	int frameCounter;
	std::vector<Point> points;
	std::mutex locker;
	void lock();
	void unlock();
	/*
	 * 检测手指的弯曲程度， 参数为手的编号和手指的编号
	 * 返回一个 0 ~ 1 之间的实数表示手指弯曲程度， 0为完全伸直， 1 为完全折叠弯曲
	 */
	double checkFingerAngle(int HandIndex, int FingerIndex) const;
};

#endif