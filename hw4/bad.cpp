void removeBad(list<Movie*>& li)
{
	for (list<Movie*>::iterator p = li.begin(); p != li.end();) {
		if ((*p)->rating() < 55) {
			Movie* toDelete = *p;
			p = li.erase(p);
			delete toDelete;

		}
		else {
			p++;
		}
	}
}