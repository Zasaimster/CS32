#include <iostream>
#include "Sequence.h"


using namespace std;

Sequence::Sequence() {
	head = nullptr;
};

Sequence::Sequence(const Sequence& src) {
	head = nullptr;
	Node* n = src.head;
	
	for (int i = 0; i < src.size(); i++) {
		insert(i, n->value);
		n = n->next;
	}
	
}

Sequence& Sequence::operator= (const Sequence& src) {
	if (this == &src) return *this;

	Node* p = head;
	while (p != nullptr) {
		Node* node = p->next;
		delete p;
		p = node;
	}
	
	head = nullptr;
	Node* n = src.head;
	for (int i = 0; i < src.size(); i++) {
		insert(i, n->value);
		n = n->next;
	}

	return *this;
}

Sequence::~Sequence() {
	Node* p = head;
	while (p != nullptr) {
		Node* node = p->next;
		delete p;
		p = node;
	}
}

bool Sequence::empty() const {
	if (head == nullptr) return true;

	return false;
};

int Sequence::size() const {
	Node* p = head;
	int count = 0;
	while (p != nullptr) {
		count++;
		p = p->next;
	}

	return count;
};

int Sequence::insert(int pos, const ItemType& value) {
	if (!(0 <= pos && pos <= size())) return -1;

	if (pos == 0) {
		Node* toBeAdded = new Node;
		toBeAdded->value = value;
		toBeAdded->next = head;
		toBeAdded->prev = nullptr;
		if (size() != 0) {
			head->prev = toBeAdded;
		}
		head = toBeAdded;

		return pos;
	}

	if (pos == size()) {
		Node* p = head;

		while (p->next != nullptr) {
			p = p->next;
		}
		Node* toBeAdded = new Node;
		toBeAdded->value = value;
		p->next = toBeAdded;
		toBeAdded->next = nullptr;
		toBeAdded->prev = p;

		return pos;
	}

	Node* p = head;
	int count = 0;
	while (count != pos - 1) {
		count++;
		p = p->next;
	}

	Node* toBeAdded = new Node;
	Node* newNext = p->next;

	toBeAdded->value = value;
	toBeAdded->prev = p;
	p->next = toBeAdded;
	newNext->prev = toBeAdded;
	toBeAdded->next = newNext;


	return pos;
};

int Sequence::insert(const ItemType& value) {

	if (size() == 0) {
		insert(0, value);
		return 0;
	}

	int count = 0;
	Node* p = head;
	while (p != nullptr && value > p->value) {
		p = p->next;
		count++;
	}

	insert(count, value);

	return count;

};

bool Sequence::erase(int pos) {
	if (!(0 <= pos && pos < size())) return false;

	if (pos == 0) {
		Node* toBeDeleted = head;
		head = toBeDeleted->next;
		if (size() != 0) {
			head->prev = nullptr;
		}

		delete toBeDeleted;
	}
	else if (pos == size() - 1) {
		Node* p = head;
		while (p->next->next != nullptr) {
			p = p->next;
		}

		Node* toBeDeleted = p->next;
		p->next = nullptr;
		delete toBeDeleted;

	}
	else {
		Node* p = head;
		int count = 0;
		while (count != pos - 1) {
			p = p->next;
			count++;
		}
		Node* toBeDeleted = p->next;
		Node* newNext = p->next->next;
		p->next = newNext;
		newNext->prev = p;

		delete toBeDeleted;
	}

	return true;
};

int Sequence::remove(const ItemType& value) {
	int index = 0;
	int countOfRemoved = 0;
	Node* p = head;

	while (p != nullptr) {
		if (p->value == value) {
			p = p->next;
			erase(index);
			countOfRemoved++;
		}
		else {
			index++;
			p = p->next;
		}
	}

	return countOfRemoved;
};

bool Sequence::get(int pos, ItemType& value) const {
	if (!(0 <= pos && pos < size())) return false;

	int index = 0;
	Node* p = head;
	while (index != pos) {
		p = p->next;
		index++;
	}
	value = p->value;

	return true;
};

bool Sequence::set(int pos, const ItemType& value) {
	if (!(0 <= pos && pos < size())) return false;

	int index = 0;
	Node* p = head;
	while (index != pos) {
		p = p->next;
		index++;
	}
	p->value = value;

	return true;
};

int Sequence::find(const ItemType& value) const {
	int index = 0;
	Node* p = head;

	while (p != nullptr && p->value != value) {
		index++;
		p = p->next;
	}

	if (p == nullptr)
		return -1;
	else
		return index;
};

void Sequence::swap(Sequence& other) {
	Node* tempHead = head;
	head = other.head;
	other.head = tempHead;
};

int subsequence(const Sequence& seq1, const Sequence& seq2) {
	if (seq1.size() < seq2.size()) return -1;

	ItemType startOfSeq2;
	seq2.get(0, startOfSeq2);

	for (int i = 0; i <= seq1.size() - seq2.size(); i++) {
		ItemType temp;
		seq1.get(i, temp);
		if (temp == startOfSeq2) {
			ItemType seq2Temp;
			int count = 1;
			for (int j = 1; j < seq2.size(); j++) {
				seq1.get(i + j, temp);
				seq2.get(j, seq2Temp);
				if (temp == seq2Temp)
					count++;
				else
					break;
			}
			if (count == seq2.size()) {
				return i;
			}

		}

	}

	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
	int minLength = min(seq1.size(), seq2.size());
	int maxLength = max(seq1.size(), seq2.size());
	int resultIndex = result.size();

	for (int i = 0; i < minLength; i++) {
		ItemType tempVal;
		seq1.get(i, tempVal);
		result.insert(resultIndex, tempVal);

		seq2.get(i, tempVal);
		result.insert(resultIndex + 1, tempVal);

		resultIndex += 2;
	}

	for (int i = minLength; i < maxLength; i++) {
		ItemType tempVal;
		if (seq1.size() > seq2.size()) {
			seq1.get(i, tempVal);
		}
		else {
			seq2.get(i, tempVal);
		}
		result.insert(resultIndex, tempVal);

		resultIndex++;
	}
}
