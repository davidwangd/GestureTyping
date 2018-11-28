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
	Leap::Vector bones[2][5][5];	// ����/�������ָͷ���ĸ��ؽ�λ�ü�¼
	// 0: �ƹ� -> 4ĩ��
	int findBones[2][5];			// ����/�������ָͷ��û�з���
	enum TypeMode{
		Undetected = 0,				// δ��⵽����
		Typing,						// ��ǰ���ڽ��д���(ʳָ���)
		Selecting,					// ��ǰ���ڽ���ѡ��(��ָ�ſ�)
		Waiting 					// ��ǰ���ڵȴ���ʼ����(��ȭ״̬)
	} typeMode, preMode;
	keyboard board;
};

#endif