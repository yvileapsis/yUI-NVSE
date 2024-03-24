#pragma once

class GridArray {
public:
	virtual		 ~GridArray();
	virtual void Init();
	virtual void DetachAll();
	virtual void KillAll();
	virtual void SetCenter(UInt32 aiX, UInt32 aiY);
	virtual void Shift(int aiCols, int aiRows);
	virtual void Detach(UInt32 aiX, UInt32 aiY);
	virtual void ClearItem(UInt32 aiX, UInt32 aiY);
	virtual void MoveItem(UInt32 aiX1, UInt32 aiY1, UInt32 aiX2, UInt32 aiY2);
	virtual void SwapItem(UInt32 aiX1, UInt32 aiY1, UInt32 aiX2, UInt32 aiY2);

	SInt32 iWorldX;
	SInt32 iWorldY;
};