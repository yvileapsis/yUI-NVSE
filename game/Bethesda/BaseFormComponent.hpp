#pragma once

class BaseFormComponent {
public:
	BaseFormComponent() {};
	~BaseFormComponent() {};

	virtual void	InitializeDataComponent();
	virtual void	ClearDataComponent();
	virtual void	CopyComponent(BaseFormComponent* apComponent);
	virtual bool	CompareComponent(BaseFormComponent* apSource);
};

ASSERT_SIZE(BaseFormComponent, 0x4);