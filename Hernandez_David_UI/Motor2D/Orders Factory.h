#ifndef _ORDERS_FACTORY_H_
#define _ORDERS_FACTORY_H_

#define NULL 0
class UIButton;
class UIButton2;
class UIRect;
class UIImage;

class Order
{
public:
	Order(){ button = NULL; }

	virtual void Function(){}

	//Setters & Getters
	void SetButton(UIButton2& nButt){ button = &nButt; }

	const UIButton2* getButton() const { return button; }

private:
	UIButton2* button;


};

struct Gen_probe : public Order
{
	Gen_probe() :Order(){}
	void Function();
	
};

struct Attack : public Order
{
	Attack() :Order(){}
	void Function();
	
};
#endif // !_ORDERS_FACTORY_H_
