class Investment {
public:
	Investment(string name, int price) :m_name(name), m_price(price) {}
	string name() const {
		return m_name;
	}
	int purchasePrice() const {
		return m_price;
	}
	virtual bool fungible() const {
		return false;
	}

	virtual string description() const = 0;

	virtual ~Investment() {

	}

private:
	string m_name;
	bool m_fungible;
	int m_price;
};

class Painting : public Investment {
public:
	Painting(string name, int price) : Investment(name, price) {}

	virtual string description() const {
		return "painting";
	}

	virtual ~Painting() {
		cout << "Destroying " << Investment::name() << ", a painting" << endl;
	}
};

class Stock : public Investment {
public:
	Stock(string name, int price, string ticker) : Investment(name, price), ticker(ticker) {}

	virtual string description() const {
		return "stock trading as " + ticker;
	}

	virtual bool fungible() const {
		return true;
	}

	virtual ~Stock() {
		cout << "Destroying " << Investment::name() << ", a stock holding" << endl;
	}

private:
	string ticker;
};

class House : public Investment {
public:
	House(string name, int price) : Investment(name, price) {}
	
	virtual string description() const {
		return "house";
	}

	virtual ~House() {
		cout << "Destroying the house " << Investment::name() << endl;
	}
};