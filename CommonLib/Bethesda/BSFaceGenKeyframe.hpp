#pragma once

class BSFaceGenKeyframe {
public:
	BSFaceGenKeyframe();
	virtual ~BSFaceGenKeyframe();
	virtual void Func0001();
	virtual void Func0002();
	virtual void Func0003();
	virtual void Func0004();
	virtual void Func0005();
	virtual void Func0006();
	virtual void Func0007();
	virtual void Func0008();
	virtual void Func0009();
	virtual void Func000A();
	virtual void Differs();
	virtual void Func000C();
	virtual void Func000D();
	virtual void ResetExpressionValues();
	virtual void Func000F();
	virtual void Func0010();
	virtual void Func0011();
	virtual void Func0012();
	virtual void Func0013();
	virtual void Func0014();

	int unk04;
	float unk08;
};

ASSERT_SIZE(BSFaceGenKeyframe, 0xC)