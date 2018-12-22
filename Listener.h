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
	 * ���� nSamplePoints �� ·���ϵ�Sample��
	 * ����Ҫ��֤�̰߳�ȫ���������ķ���
	 *  onFrame�� ���Ƿ�����һ��onFrame����Ľ��.
	 *  onFrame�� ���Ƿ��ص�ǰ�Ľ��.
	 *  Ҫ��֤����Ч��==
	 */
	std::vector<Point> getSamplePoints(int nSamplePoints = 50) ;
private:
	void processLeftHand(const Leap::Hand& hand);
	void processRightHand(const Leap::Hand& hand);
	void processBones(const Leap::Hand &hand, int t);
	Leap::Vector bones[2][5][5];	// ����/�������ָͷ���ĸ��ؽ�λ�ü�¼
	// 0: �ƹ� -> 4ĩ��
	int findBones[2][5];			// ����/�������ָͷ��û�з���
	enum TypeMode{
		Undetected = 0,				// δ��⵽����
		Typing,						// ��ǰ���ڽ��д���(ʳָ���)
		Selecting,					// ��ǰ���ڽ���ѡ��(��ָ�ſ�)
		Waiting 					// ��ǰ���ڵȴ���ʼ����(��ȭ״̬)
	} typeMode, preMode;
	int leftHandMode;
	keyboard board;
	int frameCounter;
	std::vector<Point> points;
	std::mutex locker;
	void lock();
	void unlock();
	/*
	 * �����ָ�������̶ȣ� ����Ϊ�ֵı�ź���ָ�ı��
	 * ����һ�� 0 ~ 1 ֮���ʵ����ʾ��ָ�����̶ȣ� 0Ϊ��ȫ��ֱ�� 1 Ϊ��ȫ�۵�����
	 */
	double checkFingerAngle(int HandIndex, int FingerIndex) const;
};

#endif