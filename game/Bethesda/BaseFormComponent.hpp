#pragma once

// 0x4
class BaseFormComponent 
{
public:
	BaseFormComponent();
	~BaseFormComponent();

	virtual void	InitializeDataComponent();
	virtual void	ClearDataComponent();
	virtual void	CopyComponent(BaseFormComponent* apComponent);
	virtual bool	CompareComponent(BaseFormComponent* apSource);
};
static_assert(sizeof(BaseFormComponent) == 0x4);