#include "StudentUndo.h"
#include <iostream>
#include <string>

using namespace std;

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	if (!undo.empty()) {
		ActionObject obj = undo.top();
		Action topAction = obj.action;
		int topRow = obj.row;
		int topCol = obj.col;

		if (topAction == action && topRow == row) {
			if (topAction == Undo::Action::DELETE) {
				if (topCol == col) {
					undo.top().str += ch;
					undo.top().col = col;
					return;
				}
				else if (topCol == col + 1) {
					string s = ch + obj.str;
					undo.top().str = s;
					undo.top().col = col;
					return;
				}
			}

			if (topAction == Undo::Action::INSERT) {
				if (topCol == col - 1) {
					undo.top().str += ch;
					undo.top().col = col;
					return;
				}
			}
		}
	}
	ActionObject newAction;
	newAction.action = action;
	newAction.row = row;
	newAction.col = col;
	newAction.str = "";
	newAction.str += ch;

	undo.push(newAction);
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
	if (undo.empty())
		return Action::ERROR;
	
	ActionObject toUndo = undo.top();
	undo.pop();
	Action action = toUndo.action;
	Action toReturn;

	switch (action) {
		case Action::INSERT:
			toReturn = Action::DELETE;
			break;
		case Action::DELETE:
			toReturn = Action::INSERT;
			break;
		case Action::SPLIT:
			toReturn = Action::JOIN;
			break;
		default: //Action::JOIN
			toReturn = Action::SPLIT;
			break;
	}

	switch (toReturn) {
		case Action::INSERT:
		case Action::DELETE:
		case Action::JOIN:
		{
			row = toUndo.row;
			col = toUndo.col;
			break;
		}
		case Action::SPLIT:
		{
			row = toUndo.row;
			col = toUndo.col;
			break;
		}
	
		default:
			break;
	}

	row = toUndo.row;
	col = toUndo.col;

	if (toReturn == Action::DELETE)
		count = toUndo.str.length();
	else
		count = 1;

	if (toReturn == Action::INSERT)
		text = toUndo.str;
	else
		text = "";

	return toReturn;

}

void StudentUndo::clear() {
	while (!undo.empty())
		undo.pop();
}
